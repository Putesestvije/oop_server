#ifndef TIMETRACKER_H
#define TIMETRACKER_H

#include "subscriber.h"

class TimeTracker : public Subscriber
{
public:
    TimeTracker();
    void updateMsg(std::string msg);
private :
    int _deliveries;
    double _average;
};

#endif // TIMETRACKER_H
