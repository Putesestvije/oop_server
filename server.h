#ifndef SERVER_H
#define SERVER_H

#include "connection.h"


class Server
{
public:
    Server();
    void serve();
    static void handleChild(int sig);

    ~Server();
private:

    Connection *_connection;
};

#endif // SERVER_H
