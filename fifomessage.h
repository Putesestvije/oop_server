#ifndef FIFOMESSAGE_H
#define FIFOMESSAGE_H

#include "newsstand.h"

struct fmsg{
    long mtype;
    char content[100];
    NewsStand::Topic topic;
};

#endif // FIFOMESSAGE_H
