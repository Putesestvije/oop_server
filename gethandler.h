#ifndef GETHANDLER_H
#define GETHANDLER_H
#include "requesthandler.h"
#include "server.h"
#include "newsstand.h"

#include <map>
#include <string>
#include <iostream>

using std::map;
using std::string;



class GETHandler : public RequestHandler
{
public:
    friend class Server;
    void HandleRequest();
    ~GETHandler();
private :
    GETHandler(Request r, Connection *c, int cons, string fifo);

    enum statusCodes{OK = 200, FILENOTFOUND = 404, INTERROR = 500, EBADREQ = 400, HTTPNOSUP = 505};

    statusCodes _status;
    map<string, string> _responseHeaders;
    bool _closingTime;

    static int _handlerNo;
    int _myNo;
    int _cons;
    std::string _fifo;
    int _fifofd;

    void openNewsChannel();
    void postNews(string msg, NewsStand::Topic topic);
    int openPage();
    string getPath();
    void setContentLength(std::string path);
    void setGenericHeaders();
    void setMimeType(string path);
    string HTTPdate();
    void printRespHeaders();
    string makeStatusLine();
    void sendStatus(string status);
    void sendHeaders();
    void sendPayload(int page);
    void readNewRequest();
    void setAlarmHandling();

};

#endif // GETHANDLER_H
