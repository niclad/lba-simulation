#include "LoadBalancing.h"
#include "rvgs.h"

#include <iostream>

int lba::roundrobin(std::vector<ServiceNode> nodeList) { 
    // static counter of jobs
    static int index{0};

    int server = index;

    // update index accounting for node_size
    index++;
    index %= nodeList.size();

    return server;
}

int lba::random(std::vector<ServiceNode> nodeList) {
    // return a random server index
    return Equilikely(0, nodeList.size()-1);
}

int lba::utilizationbased(std::vector<ServiceNode> nodeList) {
    int least_utilized{0};

    // find the index with the least utilization
    for (size_t ii = 0; ii < nodeList.size(); ii++) {
        if (nodeList[least_utilized].getUtil() >
                nodeList[ii].getUtil()) {
            least_utilized = ii;
        }
    }
    return least_utilized;
} 

int lba::leastconnections(std::vector<ServiceNode> nodeList) {
    int least_connections{0};

    // find the index with the least number of jobs
    for (size_t ii = 1; ii < nodeList.size(); ii++) {
        if (nodeList[least_connections].getQueueLength() >
                nodeList[ii].getQueueLength()) {
            least_connections = ii;
        }
    }

    return least_connections;
}

int lba::testLBA(std::vector<ServiceNode> nodeList) {
  if (nodeList.size() > 0) {
    for (ServiceNode node : nodeList) {
      std::cout << node << std::endl;
    }
    return nodeList[0].getId();
  }

  return -1;
}

int
lba::getLba(std::string name)
{
    if (name == "rand") {
        return 1;
    }
    else if (name == "util") {
        return 2;
    }
    else if (name == "least") {
        return 3;
    }
    else if (name == "round") {
        return 0;
    }
    else {
        return -1;
    }
}
