#include "Node.h"

ServiceNode::ServiceNode(int id) : id{id}, util{0}, maxQueueSz{0} {}

ServiceNode::ServiceNode(int id, size_t maxQueueSz)
    : id{id}, util{0}, maxQueueSz{maxQueueSz} {}

void ServiceNode::updateUtil(int nJobs, double departure,
                             double avgServiceTime) {
  util = ((double)nJobs / departure) * avgServiceTime;
}

bool ServiceNode::enterQueue(int job) {
  if (q.size() < maxQueueSz) {
    q.push(static_cast<double>(job));
    return true;
  }

  return false;
}

int ServiceNode::getId() const { return id; }

double ServiceNode::getUtil() const { return util; }

std::ostream& operator<<(std::ostream& out, const ServiceNode& node) {
  out << "ID: " << node.getId() << ", util: " << node.getUtil();

  return out;
}