#include "timetracker.h"
#include "iostream"

TimeTracker::TimeTracker()
{
    _deliveries = 0;
    _average = 0.0;
}

void TimeTracker::updateMsg(std::string msg)
{
    _deliveries++;
    if(_deliveries == 1){
        _average = _deliveries * 1.0;
    }
    else {
        _average = (_average * (_deliveries - 1) + std::stoi(msg)) / _deliveries;
    }

    std::cout << "Average response time: " << _average << "ms" << std::endl;
}
