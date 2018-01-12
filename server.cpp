#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "server.h"
#include "logging.h"
#include "gethandler.h"

void Server::handleChild(int sig)
{
    //MYLOG(MYLOG_TRACE_FUN_LVL, "enter: ")
    //MYLOG(MYLOG_TRACE_FUN_LVL, __PRETTY_FUNCTION__)
    struct sigaction siga;

    siga.sa_handler = &Server::handleChild;
    sigemptyset(&siga.sa_mask);
    siga.sa_flags = 0 | SA_NODEFER | SA_NOCLDWAIT;

    sigaction(SIGCHLD, &siga, nullptr);
    //MYLOG(MYLOG_TRACE_FUN_LVL, "exit: handleChild")
}

std::shared_ptr<RequestHandler> Server::makeHandler(Request r, Connection *c, int cons)
{
    size_t blank = r.req.find(' ');

    if (r.req.substr(0,blank) == "GET"){
        /*std::shared_ptr<GETHandler> res;
        GETHandler g = GETHandler(r, c);
        res = std::make_shared<GETHandler>(g);
        return res;*/

        auto res = std::shared_ptr<GETHandler>(new GETHandler(r, c, cons));
        return res;
    }
}

Server::Server()
{
    MYLOG(MYLOG_TRACE_FUN_LVL, "ctor Server")

    handleChild(SIGCHLD);

    _connection = new Connection();
    MYLOG(MYLOG_TRACE_FUN_LVL, "ctor Server done")
}

void Server::serve()
{
    MYLOG(MYLOG_TRACE_FUN_LVL, "enter Server::serve")
    Request req;
    int cons = 0;
    while (1) {
        cons++;
        _connection->recvRequest(req);
        MYLOG(MYLOG_TRACE_LVL, "Recieved a new request")
        //std::cout << "recieved request" << std::endl;
        if (fork() == 0){
            std::shared_ptr<RequestHandler> handler = makeHandler(req, _connection, cons);
            handler->HandleRequest();
            exit(1);
        }
        else {
            close(req.clientSocket);
        }
    }
}


Server::~Server()
{
    delete _connection;
}
