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

void ServiceNode::updateUtil(double mostRecentDep) {
  util = ((double)numJobsProcessed / mostRecentDep) * avgST;
}

bool ServiceNode::enterQueue(Job job) {
  if (jobQueue.size() < maxQueueSz) {
    ++numJobsProcessed;  // this Job can be processed

    if (!jobQueue.empty()) {
      job.setDelay(jobQueue.back().calcDeparture());
    }

    jobQueue.push(job);

    // process the existing queue now that a new arrival has come
    // problem, how to handle case where queue is full
    // - can't add new job to queue so the processQueue() function won't
    //   be able to see the "current" time
    // processQueue(job.getArrival());

    return true;
  }

  return false;
}

bool ServiceNode::enterNode(Job job) {
  // MOVE THESE TO PROCESSJOBS
  // update the running average service time
  updateAvgST(job.getServiceTime());

  // update the running average of the utilization
  updateUtil(job.calcDeparture());

  double currArrival{job.getArrival()};
  if (maxQueueSz > 0) {
    processQueue(currArrival);
  }

  if (job.getArrival() < lastDeparture) {
    // the server is busy and there's no queue, so job can't wait here
    if (maxQueueSz == 0) {
      return false;
    }

    bool isInQueue{enterQueue(job)};
    return isInQueue;
  }

  // update the last Job's departure time
  lastDeparture = job.calcDeparture();

  // return the result of this expression
  // If work-conserve, then the server can start work immidiately.
  return (jobQueue.size() == 0);
}

void ServiceNode::processQueue(double currArrival) {
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

    // check the most recent arrival time. if it's less than the running job's
    // departure time, then update and process the server until a departure >
    // arrival is found.

    // process queue

    // get the most recent arrival time (back of queue)
    // double currentTime{jobQueue.back().getArrival()};
    // if the job in the server is still being processed
    if (currArrival > lastDeparture) {
      while (!jobQueue.empty()) {
        // if front of queue has already departed
        if (jobQueue.front().calcDeparture() < currArrival) {
          // insert calculations here maybe?

          // remove job from queue
          jobQueue.pop();
        } else {
          break;
        }
      }

      // update job in servers departure time
      if (!jobQueue.empty()) {
        // first job in what's left is being processed in server
        lastDeparture = jobQueue.front().calcDeparture();
        jobQueue.pop();
      } else {
        // completed all jobs
        lastDeparture = 0;
      }
    }
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
