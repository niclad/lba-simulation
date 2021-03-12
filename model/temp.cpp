#include <iostream>
#include <vector>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"

int main() {
  std::vector<ServiceNode> nodes = {};
  for (int i = 0; i < 10; i++) {
    nodes.push_back(ServiceNode(i));
  }

  lba::testLBA(nodes);

  std::cout << "Hello, world" << std::endl;
}