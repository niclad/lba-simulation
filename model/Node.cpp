#include "Node.h"
#include "Job.h"

ServiceNode::ServiceNode(int id)
    : id{id}, util{0}, maxQueueSz{0}, avgST{0}, numJobsProcessed{0} {}

ServiceNode::ServiceNode(int id, size_t maxQueueSz)
    : id{id}, util{0}, maxQueueSz{maxQueueSz}, avgST{0}, numJobsProcessed{0} {}

void ServiceNode::updateUtil(double lastDeparture) {
  util = ((double)numJobsProcessed / lastDeparture) * avgST;
}

// FIXME: abstract to entering the node, then determine if there's queue space 
// if the server is IN USE
bool ServiceNode::enterQueue(Job job) {
  if (jobQueue.size() < maxQueueSz) {
    ++numJobsProcessed; // this Job can be processed

    jobQueue.push(job);

    // update the running average service time
    updateAvgST(job.getServiceTime());

    // update the running average of the utilization
    updateUtil(job.calcDeparture());

    return true;
  }

  return false;
}

int ServiceNode::getId() const { return id; }

double ServiceNode::getUtil() const { return util; }

int ServiceNode::getQueueLength() const { return jobQueue.size(); }

double ServiceNode::updateAvgST(double lastST) {
  size_t currQSz{jobQueue.size()};
  avgST = avgST + ((1 / currQSz) * (lastST - avgST));

  return avgST;
}

std::ostream& operator<<(std::ostream& out, const ServiceNode& node) {
  out << "ID: " << node.getId() << ", util: " << node.getUtil();

  return out;
}
