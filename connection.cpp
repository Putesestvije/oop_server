#include "connection.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>

#include "config.h"
#include "logging.h"

Connection::Connection()
{
    int err;

    char *errname;

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htonl(Config::Instance()->port());
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

    /*I believe this is to make the incoming same session requests
     * reuse the same socket for multiple requests*/
    //setSocketReuse(_listeningSocket);
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
