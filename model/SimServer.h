#ifndef SIM_SERVER_H
#define SIM_SERVER_H

#include <iostream>

class SimServer
{
public:
  // initialize a default server with default id and util
  SimServer();

  // create a server with given id and util 0
  SimServer(int id);

  // create a server with given id and util
  SimServer(int id, double util);

  // default destructor
  ~SimServer();

  // update the server util with the given values
  void updateUtil(int nJobs, double departure, double avgServiceTime);

  // get the server's id
  int getId() const;

  // get the server's utilization
  double getUtil() const;

private:
  int id;       // a servers id
  double util;  // this server's utilization
};

// overload << operator
std::ostream& operator<<(std::ostream& out, const SimServer& server);

#endif