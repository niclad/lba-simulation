#include "LoadBalancing.h"
#include "rvgs.h"

#include <iostream>

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
    return Equilikely(0, nodeList.size()-1);
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
    int least_utilized{0};

    // find the index with the least utilization
    for (size_t ii = 0; ii < nodeList.size(); ii++) {
        nodeList[ii].processQueue(currT);
        nodeList[ii].updateUtil(currT);
        if (nodeList[least_utilized].getUtil() >
                nodeList[ii].getUtil()) {
            least_utilized = ii;
        }
    }
    return least_utilized;
} 

int lba::leastconnections(std::vector<ServiceNode> nodeList, double currT) {
    int least_connections{0};

    // find the index with the least number of jobs
    for (size_t ii = 0; ii < nodeList.size(); ii++) {
        nodeList[ii].processQueue(currT);
        if (nodeList[least_connections].getQueueLength() >
                nodeList[ii].getQueueLength()) {
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
