#ifndef LOAD_BALANCING_ALGO_H
#define LOAD_BALANCING_ALGO_H

#include <vector>

#include "Node.h"

namespace lba {
/**
 * @brief Round-robin load-balancing algorithm
 * 
 * @param nodeList The list of available Service Nodes to choose from
 * @return int The Service Node chosen
 */
int roundrobin(std::vector<ServiceNode> nodeList);

// place holders for other algorithms
int a(std::vector<ServiceNode> nodeList);
int b(std::vector<ServiceNode> nodeList);
int c(std::vector<ServiceNode> nodeList);
}  // namespace lba

#endif