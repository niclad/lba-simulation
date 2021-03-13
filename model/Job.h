#ifndef JOBS_H
#define JOBS_H

class Job {
  public:
    Job(double arrival); 
    Job(double arrival, double prevDeparture); 
    double setDelay(double prevDeparture);
    double calcDeparture();
  private:
    double getService();
    double calcWait();
    double arrival;
    double delay;
    double service;
};

#endif