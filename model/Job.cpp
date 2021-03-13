#include "Job.h"

Job::Job(double arrival) : arrival{arrival}, service{0} {
  //service needs to be generated via GetService
}

Job::Job(double arrival, double prevDeparture) : arrival{arrival}, service{0},
  delay{setDelay(prevDeparture)} {
  //service needs to be generated via GetService
}

double Job::setDelay(double prevDeparture) {
  delay = prevDeparture - arrival;
  
  if (delay < 0.0) {
    delay = 0.0;
  }

  return delay;
}

double Job::calcWait() {
  return delay + service;
}

double Job::calcDeparture() {
  return arrival + calcWait();
}