#ifndef GETHANDLER_H
#define GETHANDLER_H
#include "requesthandler.h"
#include "server.h"

#include <map>
#include <string>

using std::map;
using std::string;

class GETHandler : public RequestHandler
{
public:
    friend class Server;
    void HandleRequest();
    ~GETHandler();
private :
    GETHandler(Request r) : RequestHandler(r) {}

    enum statusCodes{OK = 200, FILENOTFOUND = 404, INTERROR = 500, EBADREQ = 400, HTTPNOSUP = 505};

    statusCodes _status;
    map<string, string> _responseHeaders;

    int openPage();
    string getPath();
    void setContentLength(std::string path);
};

#endif // GETHANDLER_H
