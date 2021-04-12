#include <fstream>
#include <functional>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"
#include "rvgs.h"

// Global variables
const int DAY_SEC{24 * 60 * 60};   // seconds in a day
const int NOON_TIME{DAY_SEC / 2};  // time of day for noon
const int HOUR_SEC{DAY_SEC / 24};
const double START{0.0};                 // start time for the simulation
const double END{(double)DAY_SEC * 30};  // end time for the simulation
enum class Model { mqms, sqms };
std::string alg{""};

// Type definition aliases
typedef int node_idx;
typedef std::function<node_idx(std::vector<ServiceNode>)> lba_alg;
typedef std::vector<ServiceNode> node_list;

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
node_list buildNodeList(int nNodes, int qSz);
node_idx dispatcher(node_list nodes, lba_alg lba);
void log_sim(std::string alg, int nNodes, int qSize, int nJobs, node_list nodes);

/* TO-DO:
 * Implement the function declartions below this list....
 */
void sqmsSimulation(int nNodes, lba_alg lba, int nJobs);
void mqmsSimulation(int nNodes, lba_alg lba, int qSz, int nJobs);
void accumStats(node_list nodes, int nJobs, Model modelName,
                std::string funcName);
void serverDistribution(int nNodes, int nJobs);

int main(int argc, char* argv[]) {
  // get command line arguments
  if (argc != 5) {
    std::cout << "Usage: " << argv[0] << " ";
    std::cout << "<nNodes> <lba_alg> <qSize> <nJobs>" << std::endl;
    return 1;
  }
  int nNodes{atoi(argv[1])};
  lba_alg lba{lba::getLba(argv[2])};
  if (!lba) {
    std::cerr << "Invalid load balancing algorithm: " 
            << argv[2] << std::endl;
    return 1;
  }
  alg = argv[2];
  int qSize{atoi(argv[3])};
  int nJobs{atoi(argv[4])};
  std::cout << "Running simulation with: "
      << nNodes << " Nodes, "
      << argv[2] << " Algorithm, "
      << qSize << " Queue length, "
      << nJobs << " Jobs." << std::endl;
  serverDistribution(100, 10000);
  // std::vector<ServiceNode> nodes = {};
  // for (int ii = 0; ii < 10; ii++) {
  //   nodes.push_back(ServiceNode(ii));
  // }

  // // generic LBA and ServiceNode test
  // lba::testLBA(nodes);

  // // test with round robin LBA
  // std::cout << "+-------Round Robin--------+" << std::endl;
  // test_lba(lba::roundrobin, nodes);

  // // test with random LBA
  // std::cout << "+-------Random--------+" << std::endl;
  // test_lba(lba::random, nodes, 50);
  

  // testing sqms simulation
  mqmsSimulation(nNodes, lba, qSize, nJobs);
}

// get a service time for a job
// NOTE: This makes no sense
//     EDIT: I think it makes sense now
double getArrival() {
  static double prevArr{START};     // the previous arrival time
  double st{Uniform(0, HOUR_SEC)};  // choose an arrival time
  prevArr += st;                    // update the the

  return prevArr;
}

/**
 * @brief Build a list of service nodes
 *
 * @param nNodes The number of nodes to include in the model
 * @param qSz The queue size of the nodes
 * @return std::vector<ServiceNode>
 */
node_list buildNodeList(int nNodes, int qSz) {
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
int dispatcher(node_list nodes, lba_alg lba) {
  int nodeIdx{-1};       // -1 as no node will have this index
  nodeIdx = lba(nodes);  // pick a node using the LBA

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
 * @return void Writes/ appends to a csv file to log info and utilization results
 */
void log_sim(std::string alg, int nNodes, int qSize, int nJobs, node_list nodes)
{
    std:: ofstream logfile;
    logfile.open("simlog.csv", std::ios::app);
    logfile << alg <<","<< nJobs <<","<< nNodes <<","<< qSize;
    for(int i=0; i<nNodes; i++)
    {
        logfile<<","<< nodes[i].getUtil();
    }
    logfile<<",\n";
    logfile.close();
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
void mqmsSimulation(int nNodes, lba_alg lba, int qSize, int nJobs) {
  /* TO-DO:
   * Actually implement code.
   * Generate nodes list
   * Run simulation to n jobs
   */

  // build node list
  node_list nodes{buildNodeList(nNodes, qSize)};

  // run for the number of jobs
  for (int ii = 0; ii < nJobs; ii++) {
    // get the next jobs arrival
    Job job{getArrival()};

    // determine receiving server based on lba
    int receiver{dispatcher(nodes, lba)};
    std::cout << "Node " << receiver << " selected for job" << std::endl;

    // attempt to enter the job into the node
    if (nodes[receiver].enterNode(job)) {
      // node added successfully
      //std::cout << "Job successfully added" << std::endl;
    } else {
      // node unable to be added, this is where different rejection
      // techiniques could be used
      //std::cout << "Job unsuccessfully added" << std::endl;
    }
  }

  // get simulation results
  for (auto node : nodes) {
    std::cout << "Utilization: " << node.getUtil() << "   Average Service Time: " 
      << node.calcAvgSt() << std::endl;
  }
  log_sim(alg, nNodes, qSize, nJobs, nodes);
}

/**
 * @brief Run a multi-queue, multi-server simulation
 *
 * The simulation will generate it's own list of nodes and use the LBA to send
 * nJobs to the nodes in the model.
 *
 * @param nNodes The number of nodes to use in the simulation
 * @param lba The node balancing
 * @param nJobs The number of jobs to "process" in the simulation
 */
void sqmsSimulation(int nNodes, lba_alg lba, int nJobs) {
  /* TO-DO:
   * Actually implement code.
   * Generate nodes list
   * Run simulation to n jobs
   */

  // build node list
  node_list nodes{buildNodeList(nNodes, 0)};

  // run for the number of jobs
  for (int ii = 0; ii < nJobs; ii++) {
    
  }

  // get simulation results
  for (auto node : nodes) {
    std::cout << "Utilization: " << node.getUtil() << "   Average Service Time: " 
      << node.calcAvgSt() << std::endl;
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
  node_list nodes{buildNodeList(nNodes, 0)}; // queue size doesn't matter

  // list of available load-balancing algorithms
  std::vector<lba_alg> funcs = {
    lba::roundrobin, 
    lba::random, 
    lba::utilizationbased,
    lba::leastconnections
  };

  std::ofstream lba_dat("lba-data.csv");

  for (lba_alg alg : funcs) {
    for (int i = 0; i < nJobs-1; i++) {
      lba_dat << alg(nodes) << ",";
    }
    lba_dat << alg(nodes) << std::endl;
  }

  lba_dat.close();

  std::cout << "> ... done" << std::endl;
}

void accumStats(node_list nodes, int nJobs, Model modelName, std::string funcName) {
  std::string model = (modelName == Model::mqms) ? "mqms" : "sqms";
  std::ofstream data(model + funcName + ".csv");

  // write the headers
  data << "sid,avg_x,avg_s,n_jobs" << std::endl;

  // will need to get n_jobs
  int nodeId{0};
  for (ServiceNode node : nodes) {
    data << nodeId++ << ","
         << node.getUtil() << ","
         << node.getAvgSt() << ","
         << node.getNumProcJobs() << std::endl;
  }

  data.close();
}
