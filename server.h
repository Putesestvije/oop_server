#ifndef SERVER_H
#define SERVER_H

#include "connection.h"
#include "requesthandler.h"
#include <memory>

class Server
{
public:
    Server();
    void serve();
    static void handleChild(int sig);
    std::shared_ptr<RequestHandler> makeHandler(Request r);

    ~Server();
private:

    Connection *_connection;
};

#endif // SERVER_H
