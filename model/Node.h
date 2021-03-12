#ifndef MY_NODE_H
#define MY_NODE_H

#include <iostream>
#include <queue>

// A service node is both a server and a queue.
// If the service node doesn't have a queue (a la single-queue, multi-server),
// then a service node has a max queue size of 0.
class ServiceNode {
 public:
  /**
   * @brief Construct a new Service Node object with given ID
   *
   * @param id The ID of the Service Node
   */
  ServiceNode(int id);

  /**
   * @brief Construct a new Service Node object with given ID and queue size
   *
   * @param id The Service Node's ID
   * @param maxQueueSz The maximum size of the Service Node's queue
   */
  ServiceNode(int id, size_t maxQueueSz);

  // ~ServiceNode();

  /**
   * @brief Update the Service Node server's utilization
   *
   * @param nJobs The number of jobs up until the time this is called.
   * @param departure The last departure time since this was called.
   * @param avgServiceTime The average service time up to this point.
   */
  void updateUtil(int nJobs, double departure, double avgServiceTime);

  /**
   * @brief Add the job to the Service Node's queue, if there's space
   *
   * @param job The job stats // FIXME: This will have to be updated depending
   *                                    on how our jobs are generated/processed
   * @return true The job was successfully added to the queue
   * @return false The queue is full and the job could not be added to the queue
   */
  bool enterQueue(int job);

  /**
   * @brief Get the Service Node's ID
   *
   * @return int The Service Node's ID
   */
  int getId() const;

  /**
   * @brief Get the Service Node server's utilization
   *
   * @return double The utilization of this service node
   */
  double getUtil() const;

 private:
  // A service nodes indentifying number
  int id;

  // The servers current utilization 
  // (could also indicate a Server's "weight")
  double util;

  // The actual queue object
  // FIXME: This will have to change depending on how jobs are actually
  // represented
  std::queue<double> q;

  // The maximum number of jobs that can wait in the queue.
  // q.size() <= maxQueueSz
  size_t maxQueueSz;
};

// overload the << operator
std::ostream& operator<<(std::ostream& out, const ServiceNode& node);

#endif