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
   * This constructor assumes that the queue size will be nothing (that is no 
   * queue). In this case, the model might be single-queue, multi-server.
   * 
   * @param id The ID of the Service Node
   */
  ServiceNode(int id);

  /**
   * @brief Construct a new Service Node object with given ID and queue size
   * 
   * The Service Node will have a maximum queue size. No elements can be added
   * once the queue reaches that size.
   * 
   * @param id The Service Node's ID
   * @param maxQueueSz The maximum size of the Service Node's queue
   */
  ServiceNode(int id, size_t maxQueueSz);

  // ~ServiceNode();

  /**
   * @brief Update the Service Node server's utilization
   *
   * This will update the servers utilization. This will find the current 
   * utilization given the number of jobs, the departure time of the last job,
   * and the current running average service time. The formula for this is 
   * defined in slide deck 1.2, slide 22.
   * 
   * Equation: \Bar{x} = (n/c_n)*\Bar{s}
   * 
   * NOTE: As of right now, a "job" is still not concretely defined.
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

  /**
   * @brief Get the Service Node server's job queue length
   *
   * NOTE: This will have to be updated based on how jobs are processed
   * 
   * @return int the number of jobs in the service node's queue
   */
  int getQueueLength() const;

 private:
  // A service nodes indentifying number
  int id;

  // The servers current utilization 
  // (could also indicate a Server's "weight")
  double util;

  // The actual queue object
  // FIXME: This will have to change depending on how jobs are actually
  // represented
  std::queue<double> jobQueue;

  // The maximum number of jobs that can wait in the queue.
  // q.size() <= maxQueueSz
  size_t maxQueueSz;
};

// overload the << operator
std::ostream& operator<<(std::ostream& out, const ServiceNode& node);

#endif
