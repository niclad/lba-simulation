#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <vector>

#include "Node.h"

/* NOTE: "int job" is a place holder until a job can be better defined
 */

class Dispatcher {
public:
  /**
   * @brief Construct a new Dispatcher object
   * 
   * @param func The load-balancing function to use for the dispatching
   * @param nodes The list of the nodes that are in the model
   */
  Dispatcher(int (*func)(int), std::vector<ServiceNode> nodes);

  /**
   * @brief Dispatch a job to a node based on the lbFunc.
   * 
   * @param job The job to dispatch.
   * @return int The node where the job was sent.
   */
  int dispatchJob(int job);

private:
  // pointer to a defined load-balancing function
  int (*lbFunc)(int);

  // list holding all the service nodes in the model
  std::vector<ServiceNode> nodeList;
};

#endif