#include <iostream>
#include <vector>

#include "Dispatcher.h"
#include "LoadBalancing.h"
#include "Node.h"

int main() {
  std::vector<ServiceNode> nodes = {};
  for (int i = 0; i < 10; i++) {
    nodes.push_back(ServiceNode(i));
  }
  Dispatcher test(&lba::testLBA, nodes);

  test.dispatchJob();

  std::cout << "Hello, world" << std::endl;
}