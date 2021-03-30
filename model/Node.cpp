#include "Node.h"

#include "Job.h"

ServiceNode::ServiceNode(int id)
    : id{id},
      util{0},
      maxQueueSz{0},
      avgST{0},
      numJobsProcessed{0},
      lastDeparture{0.0} {}

ServiceNode::ServiceNode(int id, size_t maxQueueSz)
    : id{id},
      util{0},
      maxQueueSz{maxQueueSz},
      avgST{0},
      numJobsProcessed{0},
      lastDeparture{0.0} {}

void ServiceNode::updateUtil(double lastDeparture) {
  util = ((double)numJobsProcessed / lastDeparture) * avgST;
}

bool ServiceNode::enterQueue(Job job) {
  if (jobQueue.size() < maxQueueSz) {
    ++numJobsProcessed;  // this Job can be processed

    jobQueue.push(job);

    // MOVE THESE TO PROCESSJOBS
    // update the running average service time
    updateAvgST(job.getServiceTime());

    // update the running average of the utilization
    updateUtil(job.calcDeparture());

    return true;
  }

  return false;
}

bool ServiceNode::enterNode(Job job) {
  if (job.getArrival() < lastDeparture) {
    bool isInQueue{enterQueue(job)};

    return isInQueue;
  }

  // update the last Job's departure time
  lastDeparture = job.calcDeparture();

  // return the result of this expression
  // If work-conserve, then the server can start work immidiately.
  return (jobQueue.size() == 0);
}

void ServiceNode::processQueue() {
  if (jobQueue.size() > 0) {
    // 1. pop first element
    // 2. calculate the service begin time
    //    A. if begin time <= lastDeparture
    //       I. update the lastDeparture
    //      II. 
    // PROBLEM: need to figure out how the simulation will run
    // cant pop an element and put it back. Also won't run in real-time.
    // This means, that the queue might not be empty when a job is trying to be
    // added to the ServiceNode.
    double beginTime{};
    // process queue
  }
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
