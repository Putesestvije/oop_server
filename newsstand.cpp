#include "newsstand.h"
#include "fifomessage.h"
#include "logging.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>


map<NewsStand::Topic, vector<Subscriber*> > NewsStand::_subscriptions;

void NewsStand::subscribe(Subscriber *s, NewsStand::Topic t)
{
    _subscriptions[t].push_back(s);
}

NewsStand::NewsStand()
{

}

void NewsStand::notify(std::string msg, NewsStand::Topic t)
{
    std::cout << "doin some notifications: " << msg << std::endl;

    for(auto &i: _subscriptions[t])
        i->updateMsg(msg);
}

void NewsStand::listen(std::string fifo)
{

    int rdfd, nread;

    fmsg m;
    std::string s;

    rdfd = open(fifo.c_str(), O_RDONLY);
    if (rdfd == -1){
        std::cerr << "couldn't open the fifo for reading" << std::endl;
        exit(1);
    }
    while((nread = read(rdfd, &m, sizeof(m))) > 0){
        std::cout << "reading them news" << std::endl;
        std::cout << "immediately after reading news " << m.content << std::endl;
        s = std::string(m.content);
        notify(s, m.topic);
    }
    perror("LOOP BROKE");
    std::cout << "HA HA SOMEHOW I BROKE FREE FROM THAT LISTENING LOOP, AFTER 20.00 YEARS MY REVENGE WILL BE COMPLETE" << std::endl;
}

