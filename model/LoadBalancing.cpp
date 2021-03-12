#include "LoadBalancing.h"

#include <iostream>

int lba::roundrobin(std::vector<ServiceNode> nodeList) { return -1; }

int lba::testLBA(std::vector<ServiceNode> nodeList) {
  if (nodeList.size() > 0) {
    for (ServiceNode node : nodeList) {
      std::cout << node << std::endl;
    }
    return nodeList[0].getId();
  }

  return -1;
}