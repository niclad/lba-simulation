#include "SimServer.h"

void SimServer::updateUtil(int nJobs, double departure, double avgServiceTime)
{
  util = ((double)nJobs / departure) * avgServiceTime;
}

SimServer::SimServer() : id{0}, util{0} {}

SimServer::SimServer(int id) : id{id}, util{0} {}

SimServer::SimServer(int id, double util) : id{id}, util{util} {}

SimServer::~SimServer() {};

int SimServer::getId() const
{
  return id;
}

double SimServer::getUtil() const
{
  return util;
}

std::ostream& operator<<(std::ostream& out, const SimServer& server)
{
  out << "ID: " << server.getId()
      << ", util.: " << server.getUtil();

  return out;
}