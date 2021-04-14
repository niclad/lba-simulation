#include "Job.h"

#include "rvgs.h"

Job::Job(double arrival) : arrival{arrival}, delay{0}, service{getService()} {
  // service needs to be generated via GetService
}

Job::Job(double arrival, double prevDeparture)
    : arrival{arrival}, delay{setDelay(prevDeparture)}, service{getService()} {
  // service needs to be generated via GetService
}

double Job::setDelay(double prevDeparture) {
  delay = prevDeparture - arrival;

  if (delay < 0.0) {
    delay = 0.0;
  }

  return delay;
}

double Job::getServiceTime() const { return service; }

double Job::getArrival() const { return arrival; }

double Job::getService() {
  // get an exponential random variate in seconds
  return Exponential(4049);  // 4049 sec. is mean random service time on
                             // discovery -- switch to minutes?
}

double Job::calcWait() { return delay + service; }

double Job::calcDeparture() { return arrival + calcWait(); }

double Job::getDelay() const { return delay; }
