#include "deliveryttracker.h"
#include <iostream>

using std::cout;
using std::endl;

DeliveryTracker::DeliveryTracker()
{
    _successRate = 0.0;
    _deliveries = 0;
    _successes = 0;
}

void DeliveryTracker::updateMsg(std::string msg)
{
    _deliveries++;
    if(msg == "Success")
        _successes++;
    _successRate = ((_successes*1.0) / _deliveries) * 100;

    cout << "Number of requests: " << _deliveries << ", and the percentage of successes: " << _successRate << "%" << endl;
}
