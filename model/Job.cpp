#include "Job.h"

#include "rvgs.h"

Job::Job() : arrival{getArrival()}, service{getService()} {
  // service needs to be generated via GetService
}

Job::Job(double prevDeparture)
    : arrival{getArrival()}, service{getService()}, delay{setDelay(prevDeparture)} {
  // service needs to be generated via GetService
}

double Job::getArrival() {
  // get an exponential random variate in seconds
  return Normal(43200, 21600); 
  // FIXME: will also need to add a day value if the sim will run across 
  // multiple days
}

double Job::getService() {
  // get an exponential random variate in seconds
  return Exponential(
      4049);  // 4049 sec. is mean random service time on discovery
}

double Job::setDelay(double prevDeparture) {
  delay = prevDeparture - arrival;

  if (delay < 0.0) {
    delay = 0.0;
  }

  return delay;
}

double Job::calcWait() { return delay + service; }

double Job::calcDeparture() { return arrival + calcWait(); }