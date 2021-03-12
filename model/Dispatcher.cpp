#include "Dispatcher.h"

Dispatcher::Dispatcher(int (*func)(int), std::vector<ServiceNode> nodes)
    : lbFunc{func}, nodeList{nodes} {}

int Dispatcher::dispatchJob(int job) { return lbFunc(job); }