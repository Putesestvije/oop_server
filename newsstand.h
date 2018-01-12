#ifndef NEWSSTAND_H
#define NEWSSTAND_H

#include <map>
#include <vector>

#include "subscriber.h"

using std::map;
using std::vector;

class NewsStand
{
public:
    enum Topic{MIMETYPE, SUCCESS, TIME};
    static void subscribe(Subscriber *s, Topic t);

    static void notify(std::string msg, Topic t);

    static void listen(std::string fifo);

private:
    static NewsStand *p_instance;
    NewsStand();

    static map<Topic, vector<Subscriber*> > _subscriptions;
};

#endif // NEWSSTAND_H
