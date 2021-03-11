#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <vector>

#include "Node.h"

class Dispatcher {
public:
  Dispatcher(void (*func)(), std::vector<ServiceNode> nodes);
  ~Dispatcher();
  void dispatchJob(int job);

private:
  // pointer to a defined load-balancing function
  void (*lbFunc)(int job);

  // list holding all the service nodes in the model
  std::vector<ServiceNode> nodeList;
};

#endif