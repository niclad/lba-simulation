#ifndef JOBS_H
#define JOBS_H

class Job {
  public:
    Job(double arrival);
    void setDelay(double prevDeparture);
    double calcDeparture();
  private:
    double calcWait();
    double arrival;
    double delay;
    double service;
};

#endif