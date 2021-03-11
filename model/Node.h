#ifndef MY_NODE_H
#define MY_NODE_H

#include <queue>

#include "SimServer.h"


class ServiceNode
{
public:
  ServiceNode();
private:
  SimServer server;
  std::queue<double> q;
  int maxQueueSz;
};

#endif