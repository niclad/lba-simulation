#include <functional>
#include <iostream>
#include <vector>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"
#include "rvgs.h"

// Global variables
const int DAY_SEC{24 * 60 * 60};         // seconds in a day
const int NOON_TIME{DAY_SEC / 2};        // time of day for noon
const int HOUR_SEC{DAY_SEC / 24};
const double START{0.0};                 // start time for the simulation
const double END{(double)DAY_SEC * 30};  // end time for the simulation

// Type definition aliases
typedef int node_idx;
typedef std::function<node_idx(std::vector<ServiceNode>)> lba_alg;

// tests a load balancing algorithm with 'nodes', 'num_iter' times
void test_lba(std::function<int(std::vector<ServiceNode>)> lba,
              std::vector<ServiceNode> nodes, int num_iters = 25) {
  for (int ii = 0; ii < num_iters; ii++) {
    std::cout << "Job index: " << ii << " | "
              << "Chosen node: " << lba(nodes) << std::endl;
  }
}

double getArrival();
std::vector<ServiceNode> buildNodeList(int nNodes, int qSz);
node_idx dispatcher(std::vector<ServiceNode> nodes, lba_alg lba);

/* TO-DO:
 * Implement the function declartions below this list....
 */
void sqmsSimulation(int nNodes, lba_alg lba, int nJobs);
void mqmsSimulation(int nNodes, lba_alg lba, int qSz, int nJobs);

int main() {
  std::vector<ServiceNode> nodes = {};
  for (int ii = 0; ii < 10; ii++) {
    nodes.push_back(ServiceNode(ii));
  }

  // generic LBA and ServiceNode test
  lba::testLBA(nodes);

  // test with round robin LBA
  std::cout << "+-------Round Robin--------+" << std::endl;
  test_lba(lba::roundrobin, nodes);

  // test with random LBA
  std::cout << "+-------Random--------+" << std::endl;
  test_lba(lba::random, nodes, 50);

  // testing sqms simulation
  sqmsSimulation(3 /*nodes*/, lba::roundrobin, 50000/*jobs*/);
}

// get a service time for a job
// NOTE: This makes no sense
//     EDIT: I think it makes sense now
double getArrival() {
  static double prevArr{START};          // the previous arrival time
  double st{Uniform(0, NOON_TIME)};  // choose an arrival time
  prevArr += st;                         // update the the

  return prevArr;
}

/**
 * @brief Build a list of service nodes
 *
 * @param nNodes The number of nodes to include in the model
 * @param qSz The queue size of the nodes
 * @return std::vector<ServiceNode>
 */
std::vector<ServiceNode> buildNodeList(int nNodes, int qSz) {
  std::vector<ServiceNode> tempList;

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
int dispatcher(std::vector<ServiceNode> nodes,
        std::function<int(std::vector<ServiceNode>)> lba) {
  int nodeIdx{-1};       // -1 as no node will have this index
  nodeIdx = lba(nodes);  // pick a node using the LBA

  return nodeIdx;
}

/**
 * @brief Run a single-queue, multi-server simulation
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
  std::vector<ServiceNode> nodes{buildNodeList(nNodes, 5/*arbitrary value*/)};

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
      std::cout << "Job successfully added" << std::endl;
    }
    else {
      // node unable to be added, this is where different rejection
      // techiniques could be used
      std::cout << "Job unsuccessfully added" << std::endl;
    }
  }
}
