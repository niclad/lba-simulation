#ifndef JOBS_H
#define JOBS_H

class Job {
  public:
    Job(); 
    Job(double prevDeparture); 
    double setDelay(double prevDeparture);
    double calcDeparture();
  private:
    double getArrival();
    double getService();
    double calcWait();
    double arrival;
    double delay;
    double service;
};

#endif