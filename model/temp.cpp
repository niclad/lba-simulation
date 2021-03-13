#include <iostream>
#include <vector>
#include <functional>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"

// tests a load balancing algorithm with 'nodes', 'num_iter' times
void test_lba(std::function<int(std::vector<ServiceNode>)> lba, 
        std::vector<ServiceNode> nodes, int num_iters = 25) {
    for (int ii = 0; ii < num_iters; ii++) {
        std::cout << "Job index: " << ii << " | "
            << "Chosen node: " << lba(nodes) << std::endl;
    }
}

int main() {
  std::vector<ServiceNode> nodes = {};
  for (int i = 0; i < 10; i++) {
    nodes.push_back(ServiceNode(i));
  }

  lba::testLBA(nodes);

  std::cout << "Hello, world" << std::endl;

  // testing with round robin
  std::cout << "+-------Round Robin--------+" << std::endl;
  test_lba(lba::roundrobin, nodes);

  // testing with random
  std::cout << "+-------Random--------+" << std::endl;
  test_lba(lba::random, nodes, 50);
  
}
