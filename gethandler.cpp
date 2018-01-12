#include "gethandler.h"
#include "logging.h"
#include "fifomessage.h"
#include "newsstand.h"

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <map>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <atomic>
#include <utility>
#include <chrono>
#include <ctime>
#include <ratio>

#define BUFSIZE 512

using std::string;
using namespace std::chrono;

GETHandler::~GETHandler()
{
    close(_req.clientSocket);
    MYLOG(MYLOG_TRACE_LVL, "closing a client")
}

GETHandler::GETHandler(Request r, Connection *c, int cons, std::string fifo) : RequestHandler(r, c), _cons(cons), _fifo(fifo)
{
    _closingTime = false;
}

void GETHandler::openNewsChannel()
{
    std::cout << "fifo path in handler is" << _fifo << std::endl;

    //MYLOG(MYLOG_TRACE_LVL, "about to open fifo in handler")
    _fifofd = open(_fifo.c_str(), O_WRONLY);
    //MYLOG(MYLOG_TRACE_LVL, "opened fifo in handler")
}

void GETHandler::postNews(std::string msg, NewsStand::Topic topic)
{
    std::cout << "Postan some news " << msg << std::endl;
    fmsg m;
    strcpy(m.content, msg.c_str());
    m.topic = topic;

    int err = write(_fifofd, &m, sizeof(fmsg));
    std::cout << "posted some news err is" << err << std::endl;
    if(err < 0){
        perror("couldn't write to fifo");
    }

}

int GETHandler::_handlerNo = 0;

int GETHandler::openPage()
{
    string path = getPath();
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1){
        _status = INTERROR;
        MYLOG(MYLOG_WARNING_LVL, "Couldn't serve the requested file to client")
        MYLOG(MYLOG_WARNING_LVL, strerror(errno))
        return 0;
    }
    //std::cout << path << std::endl;

    setContentLength(path);
    setMimeType(path);

    return fd;
}

std::string GETHandler::getPath()
{
    size_t blank, secondBlank;
    string path;

    blank = _req.req.find(' ');
    secondBlank = _req.req.find(' ', blank+1);

    path = _req.req.substr(blank+2, secondBlank-blank-2);

    //std::cout << path << std::endl;

    if (path.empty()){
        postNews("Success", NewsStand::SUCCESS);
        return "index.html";
    }
    else if(!access(path.c_str(),   R_OK)){
        postNews("Success", NewsStand::SUCCESS);
        return path;
    }
    else {
        postNews("Failure", NewsStand::SUCCESS);
        _status = FILENOTFOUND;
        return "404.html";
    }

}

void GETHandler::setContentLength(std::string path)
{
    struct stat buf;

    stat(path.c_str(), &buf);

    _responseHeaders["Content-Length"] = std::to_string(buf.st_size);
}

void GETHandler::setGenericHeaders()
{
    _responseHeaders["Server"] = "Orkimedez' Kustom Tellypager";
    _responseHeaders["Date"] = HTTPdate();
    _responseHeaders["Connection"] = "Keep-Alive";
}

void GETHandler::setMimeType(std::string path)
{
    int p[2], err, status, child;
    size_t blank, blank2, semiCol;
    char buf[BUFSIZE];
    string mime;

    err = pipe(p);
    if (err == -1){
        MYLOG(MYLOG_WARNING_LVL, "couldn't pipe")
        MYLOG(MYLOG_WARNING_LVL, strerror(errno))
        return;
    }
    if((child = fork()) == 0){
        close(p[0]);
        dup2(p[1], 1);
        execlp("file", "file", "--mime", path.c_str(), NULL);
        exit(1);
    }
    else if (child > 0){
        close(p[1]);
    }
    else {
        MYLOG(MYLOG_WARNING_LVL, "couldn't fork the FILE program")
        MYLOG(MYLOG_WARNING_LVL, strerror(errno))
        return;
    }
    waitpid(child, &status, 0);

    err = read(p[0], buf, BUFSIZE);
    if(err == -1){ /*it's no biggie if we can't get the mime type, but at least we'll log it as an error*/
        MYLOG(MYLOG_WARNING_LVL, "couldn't read piped mime/type")
        MYLOG(MYLOG_WARNING_LVL, strerror(errno))
        return;
    }
    /*we trim the returned string*/
    buf[err]='\0';
    if(buf[err-1] == '\n' || buf[err-1] == '\r')
        buf[err-1] = '\0';

    mime = string(buf);

    blank = mime.find(' ');
    blank2 = mime.find(' ', blank+1);

    /*if it's a text file we also note the charset*/
    if(mime.find("text") != string::npos){
        mime = mime.substr(blank+1);
        postNews(mime, NewsStand::MIMETYPE);
        //std::cout << "first substr " << mime.substr(blank+1) << std::endl;
    }
    else {
        mime = mime.substr(blank+1, blank2 - blank - 1);
        /*remove the semicolon fromt the end*/
        semiCol = mime.find_last_of(';');
        if(semiCol != string::npos){
            mime = mime.substr(0,semiCol);
            postNews(mime, NewsStand::MIMETYPE);
        }
        //std::cout << "second substr:" << mime << std::endl;

    }

    _responseHeaders["Content-Type"] = mime;

    return;
}

std::string GETHandler::HTTPdate()
{
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    string res(buf);
    return res;
}

void GETHandler::printRespHeaders()
{
    for(auto i = _responseHeaders.cbegin(); i != _responseHeaders.cend(); i++){
        std::cout << i->first << " " << i->second << std::endl;
    }

}

std::string GETHandler::makeStatusLine()
{
    string status("HTTP/1.1 ");
    switch (_status) {
    case INTERROR:
        return status.append("500 INTERNAL SERVER ERROR");
    case EBADREQ:
        return status.append("400 BAD REQUEST");
    case FILENOTFOUND:
        return status.append("404 FILE NOT FOUND");
    case HTTPNOSUP:
        return status.append("505 HTTP NOT SUPPORTED");
    default:
        return status.append("200 OK");
    }
}

void GETHandler::sendStatus(std::string status)
{
    char CRLF[2] = {'\r','\n'};
    write(_req.clientSocket, status.c_str(), status.length());
    write(_req.clientSocket, CRLF, 2);
}

void GETHandler::sendHeaders()
{   char colon[1] = {':'};
    char CRLF[2] = {'\r','\n'};
    for(std::map<string, string>::iterator i = _responseHeaders.begin(); i != _responseHeaders.end(); i++){
        write(_req.clientSocket, i->first.c_str(), i->first.length());
        write(_req.clientSocket, colon , 1);
        write(_req.clientSocket, i->second.c_str(), i->second.length());
        write(_req.clientSocket, CRLF, 2);
    }
    write(_req.clientSocket, CRLF, 2);
}

void GETHandler::sendPayload(int page)
{
    char buf[BUFSIZE];
    int nread;

    while((nread = read(page, buf, BUFSIZE)) > 0){
        write(_req.clientSocket, buf, nread);
    }

}

void GETHandler::readNewRequest()
{
    ssize_t nRead;
    char reqRaw[MAXREQSIZE];
    char *line;

    _req.req.clear();
    _req.headers.clear();


    //MYLOG(MYLOG_TRACE_LVL, "waiting for client to resend")
    nRead = read(_req.clientSocket, reqRaw, MAXREQSIZE);
    if(nRead < 0) {
        if (errno == EINTR){
            close(_req.clientSocket);
            close(_fifofd);
            exit(1);
        }
        MYLOG(MYLOG_ERROR_LVL, "Couldn't read request from client")
        MYLOG(MYLOG_ERROR_LVL, strerror(errno))
    }

    _req.req = string(strtok(reqRaw, "\r\n"));

    while ((line = strtok(NULL, "\r\n"))){
        _req.headers.push_back(string(line));
    }
}

void handleAlarm(int sig)
{
    //MYLOG(MYLOG_TRACE_LVL, "closing connection upon alarm")
}

void GETHandler::setAlarmHandling()
{
    struct sigaction siga;
    siga.sa_handler = handleAlarm;
    sigemptyset(&siga.sa_mask);
    siga.sa_flags = 0 | SA_NODEFER;

    sigaction(SIGALRM, &siga, NULL);
}


void GETHandler::HandleRequest()
{
    MYLOG(MYLOG_TRACE_FUN_LVL, "enter GETHandler::handleRequest")
    setAlarmHandling();
    openNewsChannel();

    alarm(5);
    while (1){
        high_resolution_clock::time_point t1 = high_resolution_clock::now();

        std::cout << _req.req << std::endl;
        setGenericHeaders();
        chdir(Config::Instance()->contentDirectory().c_str());

        /* openPage returns 0 only when an internal server error happened
        * in which case we send no payload*/
        int page = openPage();

        //std::cout << "page fd is " << page << std::endl;

        string statusLine = makeStatusLine();
        std::cout << "Handler No: " << _cons << " is begining transimission" << std::endl;
        sendStatus(statusLine);
        sendHeaders();
        if(page){
            sendPayload(page);
            //MYLOG(MYLOG_TRACE_LVL, "finished sending payload")
        }
        close(page);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();

        duration<double, std::milli> time_span = t2 - t1;

        postNews(std::to_string(time_span.count()), NewsStand::TIME);

        readNewRequest();
        //printRespHeaders();
        alarm(5);

    }
    MYLOG(MYLOG_TRACE_FUN_LVL, "exit GETHandler::handleRequest")
}
