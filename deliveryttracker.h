#ifndef DELIVERYTTRACKER_H
#define DELIVERYTTRACKER_H

#include "subscriber.h"

class DeliveryTracker : public Subscriber
{
public:
    DeliveryTracker();
    void updateMsg(std::string msg);
private:
    double _successRate;
    int _deliveries;
    int _successes;
};

#endif // DELIVERYTTRACKER_H
