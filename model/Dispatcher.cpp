#include "Dispatcher.h"

Dispatcher::Dispatcher(int (*func)(std::vector<ServiceNode>), std::vector<ServiceNode> nodes)
    : lbFunc{func}, nodeList{nodes} {}

int Dispatcher::dispatchJob() { return lbFunc(nodeList); }