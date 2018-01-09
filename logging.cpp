#include "logging.h"

void _log(int level,  const char *msg, const char *File, const char *func, int Line)
{
    bool noTarget = Config::Instance()->logTarget().empty();

    if (level <= Config::Instance()->logLevel()){
        string mmsg = "";
        string log;
        /*make the basic message*/
        mmsg.append("File: ").append(File)
                .append(" Function: ").append(func)
                .append(" Line: ").append(std::to_string(Line))
                .append(" ").append(msg);

        Logger *l = new LogMsg(string(mmsg));

        /* if there is no target output file the apply colors
         * embelishments provided by decorator pattern */
        if(noTarget && Config::Instance()->colorLogs()){
            l = new ColorLogs(l, level);
        }
        log = l->genLog();
        /*output the finalized message*/
        if(noTarget){
            std::cerr << log << std::endl;
        }
        else{
            std::ofstream target;
            target.open(Config::Instance()->logTarget(), std::fstream::trunc);
            if(target.fail()){
                std::cerr << "Couldnt open log file for writing, check persmissions and if the path exists" << std::endl;
                exit(1);
            }
            target << log << std::endl;
        }
        delete l;
    }
}
