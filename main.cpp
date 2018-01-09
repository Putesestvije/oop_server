#include <iostream>
#include <fstream>

#include "config.h"
#include "logging.h"

int main(int argc, char** argv)
{

    Config::Instance()->confFromCommandArgs(argc, argv);

    //std::cout << Config::Instance()->port() << std::endl;
    MYLOG(MYLOG_TRACE_LVL, "No error, just checking")
    std::cout << "Hello World!" << std::endl;



    return 0;
}
