#ifndef CONNECTION_H
#define CONNECTION_H

#include "request.h"

class Connection
{
public:
    Connection();
    void recvRequest(Request &r);
private:
    void setSocketReuse(int sock);
    int _listeningSocket;

};

#endif // CONNECTION_H
