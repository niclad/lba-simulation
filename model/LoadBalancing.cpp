#include "LoadBalancing.h"

#include <iostream>

#include "rvgs.h"

int lba::roundrobin(std::vector<ServiceNode> nodeList, double _ct) {
  // static counter of jobs
  static int index{0};

  int server = index;

  // update index accounting for node_size
  index++;
  index %= nodeList.size();

  return server;
}

int lba::random(std::vector<ServiceNode> nodeList, double _ct) {
  // return a random server index
  return Equilikely(0, nodeList.size() - 1);
}

/**
 * These two algorithms have an issue
 * These algorithms assume that reading the queue length or
 * Utilization retrieves the Nodes "current" status
 * but since it is possible for jobs to have not entered these
 * nodes for a long time, (maybe they are already full) they
 * don'd update their utilization or queue length so they might
 * never be picked again
 */

int lba::utilizationbased(std::vector<ServiceNode> nodeList, double currT) {
  // NOTE: this should still work for both sqms and mqms
  int least_utilized{0};

  // find the index with the least utilization
  for (size_t ii = 0; ii < nodeList.size(); ii++) {
    // process queue
    nodeList[ii].processQueue(currT);

    // update utilization if applicable
    // NOTE: why is this here? could calcUtil() in Node be used here instead?
    if (nodeList[ii].getNumProcJobs() > 0) {
      nodeList[ii].updateUtil(currT);  // maybe only calc util then update later to prevent doubling
    }

    // check if this is less utilized
    if (nodeList[least_utilized].getUtil() > nodeList[ii].getUtil()) {
      least_utilized = ii;
    }
  }
  return least_utilized;
}

int lba::leastconnections(std::vector<ServiceNode> nodeList, double currT) {
  // NOTE: Not sure how to rework for sqms. Perhaps a condition on the model
  // type could work. Perhaps average number of jobs processed by this node?
  int least_connections{0};

  // find the index with the least number of jobs
  for (size_t ii = 0; ii < nodeList.size(); ii++) {
    nodeList[ii].processQueue(currT);

    // use the average queue lengths of nodes to determine best node
    // for a job
    if (nodeList[least_connections].calcAvgQueue() >
        nodeList[ii].calcAvgQueue()) {
      least_connections = ii;
    }
  }

  return least_connections;
}

int lba::testLBA(std::vector<ServiceNode> nodeList, double currT) {
  if (nodeList.size() > 0) {
    for (ServiceNode node : nodeList) {
      std::cout << node << std::endl;
    }
    return nodeList[0].getId();
  }

  return -1;
}
