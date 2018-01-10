#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "logging.h"
#include "request.h"
#include "connection.h"

Connection::Connection()
{
    int err;

    char *errname;

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(Config::Instance()->port());
    sa.sin_addr.s_addr = inet_addr("0.0.0.0");

    /*make a socket*/
    _listeningSocket = socket(AF_INET, SOCK_STREAM, 0);


    /*bind it to an address*/
    err = bind(_listeningSocket, (struct sockaddr *)(&sa), sizeof(sa));
    if (err == -1){
        errname = strerror(errno);
        MYLOG(MYLOG_ERROR_LVL, "Couldn't bind socket" )
        MYLOG(MYLOG_ERROR_LVL, errname)
    }
    /*set it to listen*/
    err = listen(_listeningSocket, 200); /*oooh a magic number*/
    if (err == -1){
        errname = strerror(errno);
        MYLOG(MYLOG_ERROR_LVL, "Couldn't bind socket" )
        MYLOG(MYLOG_ERROR_LVL, errname)
    }
    std::cout << "listening on port " << Config::Instance()->port() << std::endl;

    /*I believe this is to make the incoming same session requests
     * reuse the same socket for multiple requests*/
    setSocketReuse(_listeningSocket);
}

void Connection::recvRequest(Request &r)
{
    int clientSocket;
    ssize_t nRead;
    char reqRaw[MAXREQSIZE];
    char *line;

    while (1) { /*polling loop*/
        clientSocket = accept(_listeningSocket, nullptr, 0);
        if (clientSocket > 0)
            break;
        else {
            switch (errno){ /*in case accept returned an error*/
            case EINTR : /*interrupted by a child process exiting*/
                continue;
            default:
                MYLOG(MYLOG_ERROR_LVL, "Couldn't accept connection")
                MYLOG(MYLOG_ERROR_LVL, strerror(errno))
            }
        }
    }
    r.clientSocket = clientSocket;

    nRead = read(clientSocket, reqRaw, MAXREQSIZE);
    if(nRead < 0) {
        MYLOG(MYLOG_ERROR_LVL, "Couldn't read request from client")
        MYLOG(MYLOG_ERROR_LVL, strerror(errno))
    }

    r.req = string(strtok(reqRaw, "\r\n"));

    while ((line = strtok(NULL, "\r\n"))){
        r.headers.push_back(string(line));
    }

    /*std::cout << r.req << std::endl;
    for (int i = 0; i < r.headers.size(); i++)
        std::cout << r.headers[i] << std::endl;*/


    setSocketReuse(clientSocket);

}

void Connection::setSocketReuse(int sock)
{
    const int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

#ifdef SO_REUSEPORT
    //printf("SO_REUSEPORT\n");
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");
#endif
}
