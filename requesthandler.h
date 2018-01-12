#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "request.h"
#include "connection.h"

class RequestHandler
{
public:
    virtual void HandleRequest() = 0;
protected:
    RequestHandler(Request r, Connection *c) : _req(r),
                                                _connection(c) {}
    Request _req;
    Connection *_connection;
};

#endif // REQUESTHANDLER_H
