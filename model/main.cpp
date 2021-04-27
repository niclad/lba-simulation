#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"
#include "rngs.h"
#include "rvgs.h"

// Type definition aliases
typedef int node_idx;
typedef std::function<node_idx(std::vector<ServiceNode>, Job)> lba_func;
typedef std::vector<ServiceNode> node_list;
typedef int lba_alg;

// struct to collect all node stats in one
typedef struct NodeStats {
  int nNodes;
  double avgUtil;
  double varUtil;
  double avgQueue;
  double varQueue;
  double avgJobs;
  double varJobs;
  double avgDelay;
  double varDelay;
  double avgWait;
  double varWait;
  double avgThruput;
  double varThruput;
  double avgSt;
  double reject;
} NodeStats;

// GLOBAL VARIABLES
const int DAY_SEC{24 * 60 * 60};   // seconds in a day
const int NOON_TIME{DAY_SEC / 2};  // time of day for noon
const int HOUR_SEC{DAY_SEC / 24};
const double START{0.0};                 // start time for the simulation
const double END{(double)DAY_SEC * 30};  // end time for the simulation
enum class Model { mqms, sqms };         // model enums
std::string alg{""};

double SERV_MEAN{0.0};
double IA_AVG{0.0};
static double prevArr{START};     // the previous arrival time


// NOTE: surely there must be a better way to deal with the below
const struct algs_t {
  const lba_alg rr{0};
  const lba_alg rand{1};
  const lba_alg util{2};
  const lba_alg cxns{3};
} Algs;

// this is a list that should be able to be indexed using the enumerator
const std::vector<lba_func> LBA_FUNCTIONS = {
    lba::roundrobin, lba::random, lba::utilizationbased, lba::leastconnections};
const std::vector<std::string> LBA_NAMES = {"roundrobin", "random", "utilbased",
                                            "leastcxns"};
// =========================== END GLOBAL VARIABLES ============================

lba_alg name_to_index(std::string name) {
  for (size_t ii = 0; ii < LBA_NAMES.size(); ii++) {
    if (name == LBA_NAMES[ii]) return ii;
  }
  return -1;
}

// tests a load balancing algorithm with 'nodes', 'num_iter' times
void test_lba(std::function<int(std::vector<ServiceNode>)> lba,
              std::vector<ServiceNode> nodes, int num_iters = 25) {
  for (int ii = 0; ii < num_iters; ii++) {
    std::cout << "Job index: " << ii << " | "
              << "Chosen node: " << lba(nodes) << std::endl;
  }
}

// Function declarations
double getArrival();
node_list buildNodeList(int nNodes, size_t qSz);
node_idx dispatcher(node_list nodes, lba_func lba);
void log_sim(lba_func lba, int nNodes, int qSize, int nJobs, node_list nodes);
void consistencyCheck(NodeStats stats);

/* TO-DO:
 * Implement the function declartions below this list....
 */
NodeStats sqmsSimulation(int nNodes, lba_alg lba, size_t qSize, int nJobs);
NodeStats mqmsSimulation(int nNodes, lba_alg lba, size_t qSize, int nJobs);
void accumStats(node_list nodes, int nJobs, Model modelName,
                std::string funcName);
void serverDistribution(int nNodes, int nJobs);
void log_sim(std::string alg, int nNodes, int qSize, int nJobs,
             node_list nodes);
void printStats(node_list nodes, int totalRejects, int nJobs);
void printAvgStats(NodeStats statistics, double ia, double st);
NodeStats avgStats(node_list nodes);

NodeStats sumStats(NodeStats src, NodeStats dst) {
  dst.avgDelay += src.avgDelay;
  dst.varDelay += src.varDelay;
  dst.avgJobs += src.avgJobs;
  dst.varJobs += src.varJobs;
  dst.avgQueue += src.avgQueue;
  dst.varQueue += src.varQueue;
  dst.avgThruput += src.avgThruput;
  dst.varThruput += src.varThruput;
  dst.avgUtil += src.avgUtil;
  dst.varUtil += src.varUtil;
  dst.avgWait += src.avgWait;
  dst.varWait += src.varWait;
  dst.nNodes += src.nNodes;
  dst.reject += src.reject;

  return dst;
}

NodeStats avgStatStats(NodeStats sts, int nStatSets) {
  sts.avgDelay /= nStatSets;
  sts.varDelay /= nStatSets;
  sts.avgJobs /= nStatSets;
  sts.varJobs /= nStatSets;
  sts.avgQueue /= nStatSets;
  sts.varQueue /= nStatSets;
  sts.avgThruput /= nStatSets;
  sts.varThruput /= nStatSets;
  sts.avgUtil /= nStatSets;
  sts.varUtil /= nStatSets;
  sts.avgWait /= nStatSets;
  sts.varWait /= nStatSets;
  sts.nNodes /= nStatSets;
  sts.reject /= nStatSets;

  return sts;
}

int main(int argc, char* argv[]) {
  // get command line arguments
  if (argc < 6) {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "<interarrival avg> <service mean> <lab_alg> <seed> <model>"
              << std::endl;
    return 1;
  }

  // set up the passed cli
  IA_AVG = std::strtod(argv[1], 0);
  SERV_MEAN = std::strtod(argv[2], 0);

  int nNodes{10};  // set the number of nodes

  // set the seed (check that seed was given)
  long int seed{argc < 6 ? 123456789 : atol(argv[4])};

  // pick the user's LBA
  lba_alg lbaChoice{name_to_index(argv[3])};
  if (lbaChoice < 0 || lbaChoice > (int)LBA_FUNCTIONS.size()) {
    std::cerr << "Invalid load balancing algorithm: " << argv[2] << std::endl;
    std::cerr << "Possible choices are: ";
    for (auto choice : LBA_NAMES) std::cout << choice << " ";
    std::cout << std::endl;
    return 1;
  }

  int qSize{100};
  int nJobs{10000};
  long int seeds[3] = {453324, 343252, 985043254};
  struct NodeStats stats = {0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  // test using a new seed each time
  for (int i = 0; i < 3; i++) {
    PutSeed(seeds[i]);  // seed the RNG

    // testing mqms simulation
    if (std::string(argv[5]) == "mqms") {
      NodeStats tempStats = mqmsSimulation(nNodes, lbaChoice, qSize, nJobs);
      stats = sumStats(tempStats, stats);
    }

    // testing sqms simulation
    if (std::string(argv[5]) == "sqms") {
      qSize *= nNodes;  // have same queue length
      NodeStats tempStats = sqmsSimulation(nNodes, lbaChoice, qSize, nJobs);
      stats = sumStats(tempStats, stats);
    }
    prevArr = START;
  }

  stats = avgStatStats(stats, 3);
  printAvgStats(stats, IA_AVG, SERV_MEAN);
}

// get a service time for a job
// ia == interarrival
double getArrival(double ia) {
  double st{Uniform(0, (ia * 2))};  // choose an arrival time
  prevArr += st;                    // update the the

  return prevArr;
}

double getService(double m) {
  // get an exponential random variate in seconds
  return Exponential(m);
}

/**
 * @brief Build a list of service nodes
 *
 * @param nNodes The number of nodes to include in the model
 * @param qSz The queue size of the nodes
 * @return std::vector<ServiceNode>
 */
node_list buildNodeList(int nNodes, size_t qSz) {
  node_list tempList;

  for (int id = 0; id < nNodes; id++) {
    tempList.push_back(ServiceNode(id, qSz));
  }

  return tempList;
}

/**
 * @brief Pick a service node for the next job to go to
 *
 * dispatcher will choose a node's index to send a job to. However, this will
 * not ignore nodes with a full queue. (I.e., if a job is sent to a full node,
 * that job won't be able to run unless the dispatcher picks a node with space.)
 *
 * @param nodes A list of usable service nodes (that may have full queues)
 * @param lba The load-balancing algorithm to use to choose a node
 * @return int The index of the node to send a job to
 */
int dispatcher(node_list nodes, lba_func alg, Job job) {
  int nodeIdx{-1};            // -1 as no node will have this index
  nodeIdx = alg(nodes, job);  // pick a node using the LBA

  return nodeIdx;
}

/**
 * @brief log information and utilization results of a simulation run
 *
 *  Call at end of mqmsSimulation and sqmsSimulation
 *
 *
 * @param lba The load-balancing algorithm to use to choose a node
 * @param nNodes The number of nodes to use in the simulation
 * @param qSize The number of jobs allowed in each server's queue
 * @param nJobs The number of jobs to "process" in the simulation
 * @param nodes node_list of nodes to get node utilizations.
 * @return void Writes/ appends to a csv file to log info and utilization
 * results
 */
void log_sim(std::string alg, int nNodes, int qSize, int nJobs,
             node_list nodes) {
  // not sure if this will work, if not can just do if or case/switches to get
  // name of alg std::string alg{std::to_string(lba)};
  std::ofstream logfile;
  logfile.open("simlog.csv", std::ios::app);
  logfile << alg << "," << nJobs << "," << nNodes << "," << qSize;
  for (int i = 0; i < nNodes; i++) {
    logfile << "," << nodes[i].getUtil();
  }
  logfile << ",\n";
  logfile.close();
}

NodeStats avgStats(node_list nodes) {
  struct NodeStats tempStats = {0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  tempStats.nNodes = nodes.size();

  // gather sums for all the results
  for (ServiceNode node : nodes) {
    double tempDelay{node.calcAvgDelay()};

    tempStats.avgUtil += node.getUtil();
    tempStats.avgQueue += node.calcAvgQueue();
    tempStats.avgJobs += node.getNumProcJobs();
    tempStats.avgDelay += tempDelay;
    tempStats.avgWait += node.calcAvgSt() + tempDelay;
    tempStats.avgThruput += node.getTput();
    tempStats.avgSt += node.calcAvgSt();
  }

  // find the averages of the results
  size_t numNodes{nodes.size()};
  tempStats.avgUtil /= numNodes;
  tempStats.avgQueue /= numNodes;
  tempStats.avgJobs /= numNodes;
  tempStats.avgDelay /= numNodes;
  tempStats.avgWait /= numNodes;
  tempStats.avgThruput /= numNodes;
  tempStats.avgSt /= numNodes;

  // compute the variances
  for (ServiceNode node : nodes) {
    double tempDelay{node.calcAvgDelay()};

    tempStats.varUtil += (node.getUtil() - tempStats.avgUtil) * (node.getUtil() - tempStats.avgUtil);
    tempStats.varQueue += (node.calcAvgQueue() - tempStats.avgQueue) * (node.calcAvgQueue() - tempStats.avgQueue);
    tempStats.varJobs += (node.getNumProcJobs() - tempStats.avgJobs) * (node.getNumProcJobs() - tempStats.avgJobs);
    tempStats.varDelay += (tempDelay - tempStats.avgDelay) * (tempDelay - tempStats.avgDelay);
    tempStats.varWait += ((node.calcAvgSt() + tempDelay) - tempStats.avgWait) * ((node.calcAvgSt() + tempDelay) - tempStats.avgWait);
    tempStats.varThruput += (node.getTput() - tempStats.avgThruput) * (node.getTput() - tempStats.avgThruput);    
  }

  tempStats.varUtil /= numNodes;
  tempStats.varQueue /= numNodes;
  tempStats.varJobs /= numNodes;
  tempStats.varDelay /= numNodes;
  tempStats.varWait /= numNodes;
  tempStats.varThruput /= numNodes;

  return tempStats;
}

/**
 * @brief Run a multi-queue, multi-server simulation
 *
 * The simulation will generate it's own list of nodes and use the LBA to send
 * nJobs to the nodes in the model.
 *
 * @param nNodes The number of nodes to use in the simulation
 * @param qSize The number of jobs allowed in each server's queue
 * @param lba The node balancing
 * @param nJobs The number of jobs to "process" in the simulation
 */
NodeStats mqmsSimulation(int nNodes, lba_alg lba, size_t qSize, int nJobs) {
  // select the algorithm besing used
  lba_func alg{LBA_FUNCTIONS[lba]};
  std::string funcName{LBA_NAMES[lba]};

  // build node list
  node_list nodes{buildNodeList(nNodes, qSize)};

  // track the total number of rejections
  int totalRejects{0};

  // run for the number of jobs
  for (int ii = 0; ii < nJobs; ii++) {
    // get the next jobs arrival
    Job job(getArrival(IA_AVG), getService(SERV_MEAN));

    // determine receiving server based on lba
    int receiver{dispatcher(nodes, alg, job)};
    // std::cout << "Node " << receiver << " selected for job" << std::endl;

    // attempt to enter the job into the node
    if (nodes[receiver].enterNode(job)) {
      // node added successfully
      // std::cout << "Job successfully added" << std::endl;
    } else {
      // node unable to be added, this is where different rejection
      // techiniques could be used
      // std::cout << "Job unsuccessfully added" << std::endl;

      ++totalRejects;
    }
  }

  double rejectRatio{static_cast<double>(totalRejects) / nJobs};
  NodeStats mqmsStats{avgStats(nodes)};
  mqmsStats.reject = rejectRatio;
  // consistencyCheck(mqmsStats);

  // std::cout << "--------------------------------------------------------" <<std::endl;
  // printStats(nodes, totalRejects, nJobs);


  return mqmsStats;
}

/**
 * @brief Run a single-queue, multi-server simulation
 *
 * The simulation will generate it's own list of nodes and use the LBA to send
 * nJobs to the nodes in the model.
 *
 * @param nNodes The number of nodes to use in the simulation
 * @param lba The node balancing
 * @param qSize The size of the dispatcher's queue
 * @param nJobs The number of jobs to "process" in the simulation
 */
NodeStats sqmsSimulation(int nNodes, lba_alg lba, size_t qSize, int nJobs) {
  // select the algorithm besing used
  lba_func alg{LBA_FUNCTIONS[lba]};
  std::string funcName{LBA_NAMES[lba]};

  // build node list
  node_list nodes{buildNodeList(nNodes, 0)};

  int totalRejects{0};        // the total number of rejections
  double totalDelay{0.0};     // total queue length
  double totalServ{0.0};      // total service times
  double lastDeparture{0.0};  // last job's departure
  double totalTime{0.0};

  // the dispatcher's queue
  std::queue<Job> jobQueue;

  // run for the number of jobs
  for (int ii = 0; ii < nJobs; ii++) {
    Job job(getArrival(IA_AVG), getService(SERV_MEAN));
    double currArr{job.getArrival()};

    // pick the service node to send the current job to
    int receiver{dispatcher(nodes, alg, job)};

    // if there's a non-empty queue, check to see if the first job can be sent
    if (jobQueue.size() > 0) {
      // send the job to the selected node
      double tempDep{nodes[receiver].getLD()};
      if (nodes[receiver].enterNode(job)) {
        double tempArr{jobQueue.front().getArrival()};

        if (tempDep > tempArr) {
          totalDelay += (tempDep - tempArr);    // add to the total delay in queue
        }
        totalServ += job.getServiceTime();  // add this st
        lastDeparture = job.calcDeparture();

        jobQueue.pop();  // remove the job from the queue as it can be serviced
      }
    }

    // check to make sure the job can be queued
    if (jobQueue.size() < qSize) {
      jobQueue.push(job);  // the job is able to enter the queue.
    } else {
      ++totalRejects;
    }
  }

  // calculate the reject ratio
  double rejectRatio{static_cast<double>(totalRejects) / nJobs};

  // get the system stats
  NodeStats sqmsStats{avgStats(nodes)};
  int processedJobs{nJobs - totalRejects};  // number of processed jobs.

  // calculate the delay and wait for sqms on outside queue
  sqmsStats.avgDelay = totalDelay / processedJobs;
  sqmsStats.avgWait = sqmsStats.avgDelay + (totalServ / processedJobs);
  sqmsStats.avgThruput = processedJobs / lastDeparture;
  sqmsStats.avgQueue = (processedJobs / lastDeparture) * sqmsStats.avgDelay;
  sqmsStats.reject = rejectRatio;
  // consistencyCheck(sqmsStats);

  return sqmsStats;
}

void printAvgStats(NodeStats statistics, double ia, double st) {
  // order: nNodes, util, q, jobs, delay, wait, reject
  std::cout << statistics.nNodes << "," << statistics.avgUtil << ","
            << statistics.varUtil << ","
            << statistics.avgQueue << "," << statistics.varQueue << ","
            << statistics.avgJobs << "," << statistics.varJobs << ","
            << statistics.avgDelay << "," << statistics.varDelay << ","
            << statistics.avgWait << "," << statistics.varWait << ","
            << statistics.avgThruput << "," << statistics.varThruput << ","
            << statistics.reject << "," << ia
            << "," << st << std::endl;
}

void printStats(node_list nodes, int totalRejects, int nJobs) {
  // calculate the fraction of rejected jobs
  double rejectRatio{(static_cast<double>(totalRejects) / nJobs) * 100};

  // print the reject amount
  std::cout << std::setprecision(5) << "Rejection amount: " << rejectRatio
            << "%" << std::endl;

  // print the node-wise data
  for (ServiceNode node : nodes) {
    std::cout << node << std::endl;
  }
}

/**
 * @brief Find the distribution of servers for a load-balancing algorithm
 *
 * @param nNodes The number of nodes for the alogrithm to choose from
 * @param nJobs The number of "jobs" to send to the nodes
 */
void serverDistribution(int nNodes, int nJobs) {
  std::cout << "> Testing node choice distribution..." << std::endl;
  node_list nodes{buildNodeList(nNodes, 0)};  // queue size doesn't matter

  // list of available load-balancing algorithms
  std::vector<lba_func> funcs = {lba::roundrobin, lba::random,
                                 lba::utilizationbased, lba::leastconnections};

  std::ofstream lba_dat("lba-data.csv");

  for (lba_func alg : funcs) {
    for (int i = 0; i < nJobs - 1; i++) {
      lba_dat << alg(nodes, Job(0, 0)) << ",";
    }
    lba_dat << alg(nodes, Job(0, 0)) << std::endl;
  }

  lba_dat.close();

  std::cout << "> ... done" << std::endl;
}

void accumStats(node_list nodes, int nJobs, Model modelName,
                std::string funcName) {
  std::string model = (modelName == Model::mqms) ? "mqms" : "sqms";
  std::ofstream data(model + "_" + funcName + ".csv");

  // write the headers
  data << "sid,avg_x,avg_s,avg_q,avg_d,n_jobs" << std::endl;

  // will need to get n_jobs
  int nodeId{0};
  for (ServiceNode node : nodes) {
    data << nodeId++ << ","                      // sid
         << node.getUtil() << ","                // avg_x
         << node.calcAvgSt() << ","              // avg_s
         << node.calcAvgQueue() << ","           // avg_q
         << node.calcAvgDelay() << ","           // avg_d
         << node.getNumProcJobs() << std::endl;  // n_jobs
  }

  data.close();
}

void consistencyCheck(NodeStats stats) {
  const double e = 1e-5;  // prob. shouldn't compare floats directly
  double expectedWait{stats.avgDelay + stats.avgSt};
  double distance{std::abs(expectedWait - stats.avgWait)};
  if (distance > e) {
    std::cout << "SIMULATION HAS INCONSISTENCIES" << std::endl;
    std::cout <<std::setprecision(9) << "avgWait: " << stats.avgWait << std::endl;
    std::cout << "avgDelay: " << stats.avgDelay << std::endl;
    std::cout << "avgService: " << stats.avgSt << std::endl;
    std::cout << "expected wait: " << expectedWait << std::endl;
  }
}
