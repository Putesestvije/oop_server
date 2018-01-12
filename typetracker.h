#ifndef TYPETRACKER_H
#define TYPETRACKER_H

#include "subscriber.h"
#include <map>

class TypeTracker : public Subscriber
{
public:
    void updateMsg(std::string msg);
    TypeTracker();
private:
    std::map<std::string, int> _mimes;
};

#endif // TYPETRACKER_H
