#ifndef LOAD_BALANCING_ALGO_H
#define LOAD_BALANCING_ALGO_H

#include <vector>
#include <functional>

#include "Node.h"

namespace lba {
/**
 * @brief Round-robin load-balancing algorithm
 * 
 * NOTE: will need to add a job argument
 * 
 * @param nodeList The list of available Service Nodes to choose from
 * @return int The Service Node chosen
 */
int roundrobin(std::vector<ServiceNode> nodeList);

/**
 * @brief Random load-balancing algorithm
 *
 * NOTE: might need to add a job argument
 * @param nodeList the list of available Service Nodes to choose from
 * @return int the chosen service node
 */
int random(std::vector<ServiceNode> NodeList);

/**
 * @brief Utilization based load-balancing algorithm
 *
 * NOTE: might need a job argument
 *
 * @param nodeList the list of available service nodes to choose from
 * @return int the chosen service node
 */
int utilizationbased(std::vector<ServiceNode> nodeList);

/**
 * @brief Least Connections load-balancing algorithms
 *
 * NOTE: might need to add a job argument
 * NOTE: maybe change this name?
 *
 * @param nodeList the list of available service nodes to choose from
 * @return int the chosen service node
 */
int leastconnections(std::vector<ServiceNode> nodeList);

/**
 * @brief Function to test the currenct implementation of the system
 * 
 * @param nodeList 
 * @return int 
 */
int testLBA(std::vector<ServiceNode> nodeList);

// place holders for other algorithms
int a(std::vector<ServiceNode> nodeList);
int b(std::vector<ServiceNode> nodeList);
int c(std::vector<ServiceNode> nodeList);

// converts specifer string to lba
std::function<int(std::vector<ServiceNode>)> getLba(std::string name);
}  // namespace lba


#endif
