#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "request.h"

class RequestHandler
{
public:
    virtual void HandleRequest() = 0;
protected:
    RequestHandler(Request r) : req(r) {}
    Request req;
};

#endif // REQUESTHANDLER_H
