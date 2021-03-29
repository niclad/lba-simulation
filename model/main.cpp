#include <functional>
#include <iostream>
#include <vector>

#include "Job.h"
#include "LoadBalancing.h"
#include "Node.h"
#include "rvgs.h"

const int DAY_SEC{24 * 60 * 60};   // seconds in a day
const int NOON_TIME{DAY_SEC / 2};  // time of day for noon

void test_lba(std::function<int(std::vector<ServiceNode>)> lba,
              std::vector<ServiceNode> nodes, int num_iters = 25);
double getArrival();

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
}

// tests a load balancing algorithm with 'nodes', 'num_iter' times
void test_lba(std::function<int(std::vector<ServiceNode>)> lba, 
        std::vector<ServiceNode> nodes, int num_iters = 25) {
    for (int ii = 0; ii < num_iters; ii++) {
        std::cout << "Job index: " << ii << " | "
            << "Chosen node: " << lba(nodes) << std::endl;
    }
}