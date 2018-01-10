#include <iostream>
#include <fstream>

#include "config.h"
#include "logging.h"
#include "server.h"

int main(int argc, char** argv)
{

    Config::Instance()->confFromCommandArgs(argc, argv);

    //std::cout << Config::Instance()->port() << std::endl;
    MYLOG(MYLOG_TRACE_LVL, "No error, just checking" )
    std::cout << "Hello World!" << std::endl;

    Server server;
    server.serve();


    //TODO
    /* napraviti request strukturu/klasu; napraviti upravljac zahtevom
     * kroz factory metodu/ mozda cak abstract request factory, koji daje
     * get factory, koji daje get html dispatcher i binary dispatcher
     * mada mozda ne mora toliko komplikovano
     * /

    /*
     * Server server; maybe put connection openning into the constructor
     * server.openConnection();
     * server.serve();
     *
     *      while(1){
     *      Request r = connection.getRequest();
     *      Handler *h = HandlerFactory::GetHandler(r); or even a private method instead of a whole factory
     *      h->handleThatShizzle();
     *      }
     *          Response re = MakeResponse();
     *          sendResponseAndPayload();
     *
     */
    return 0;
}
