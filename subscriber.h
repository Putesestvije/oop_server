#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <string>

class Subscriber
{
public:
    Subscriber();
    virtual void updateMsg(std::string) = 0;
};

#endif // SUBSCRIBER_H
