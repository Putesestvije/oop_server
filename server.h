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
    std::unique_ptr<RequestHandler> makeHandler(Request r, Connection *c, int cons, std::string fifo);

    ~Server();
private:
    std::string makeNewsChannel();

    Connection *_connection;
};

#endif // SERVER_H
