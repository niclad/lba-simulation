#include "Job.h"

Job::Job(double arrival) : arrival{arrival}, service{0} {
  //service needs to be generated via GetService
}

void Job::setDelay(double prevDeparture) {
  delay = prevDeparture - arrival;
  
  if (delay < 0) {
    delay = 0;
  }
}

double Job::calcWait() {
  return delay + service;
}

double Job::calcDeparture() {
  return arrival + calcWait();
}