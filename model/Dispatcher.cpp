#include "Dispatcher.h"

Dispatcher::Dispatcher(void (*func)(int job), std::vector<ServiceNode> nodes) 
  : lbFunc{func}, nodeList{nodes} 
{

}

Dispatcher::~Dispatcher() {
 
}

void
Dispatcher::dispatchJob(int job) {
  lbFunc(job);
}