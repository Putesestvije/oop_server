#include "gethandler.h"
#include "logging.h"

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

using std::string;

GETHandler::~GETHandler()
{
    close(req.clientSocket);
    MYLOG(MYLOG_TRACE_LVL, "closing a client")
}

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

    return fd;
}

std::string GETHandler::getPath()
{
    size_t blank, secondBlank;
    string path;

    blank = req.req.find(' ');
    secondBlank = req.req.find(' ', blank+1);

    path = req.req.substr(blank+2, secondBlank-blank-2);

    //std::cout << path << std::endl;

    if (path.empty())
        return "index.html";
    else if(!access(path.c_str(),   R_OK)){
        return path;
    }
    else {
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

void GETHandler::HandleRequest()
{
    chdir(Config::Instance()->contentDirectory().c_str());
    int page = openPage();

    MYLOG(MYLOG_TRACE_LVL, "I have stopped here")
}
