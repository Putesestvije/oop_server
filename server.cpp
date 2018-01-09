#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

void Server::handleChild(int sig)
{
    struct sigaction siga;

    siga.sa_handler = &Server::handleChild;
    sigemptyset(&siga.sa_mask);
    siga.sa_flags = 0 | SA_NODEFER | SA_NOCLDWAIT;

    sigaction(SIGCHLD, &siga, nullptr);
}

Server::Server()
{
    handleChild(SIGCHLD);

    _connection = new Connection();
}

void Server::serve()
{
    while (1) {

    }
}


Server::~Server()
{
    delete _connection;
}
