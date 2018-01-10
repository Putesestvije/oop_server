#ifndef REQUEST_H
#define REQUEST_H

#define MAXREQSIZE 4098

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Request
{
    string req;
    vector<string> headers;
    int clientSocket;

    Request() {}

};

#endif // REQUEST_H
