#include "logger.h"
#include "log_levels.h"
#include "colors.h"

LogMsg::LogMsg(std::string msg)
{
    _msg = msg;
}

std::string LogMsg::genLog()
{
    return _msg;
}

std::string DecorateLog::genLog()
{
    return _l->genLog();
}

std::string LogFile::genLog()
{
    string file("File: ");
    file.append(__FILE__);
    file.append(" ");

    string res = DecorateLog::genLog();
    return file.append(res);
}


std::string LogLine::genLog()
{
    string line("Line: ");
    line.append(std::to_string(__LINE__));
    line.append(" ");

    string res = DecorateLog::genLog();
    return line.append(res);
}

std::string LogFunction::genLog()
{
    string func("Function: ");
    func.append(__func__);
    func.append(" ");

    string res = DecorateLog::genLog();
    return func.append(res);
}

std::string ColorLogs::genLog()
{
    if (!mapInitiated){
        initColorMap();
        mapInitiated = true;
    }
    string colored(colorMap[_logLvl]);
    colored.append(DecorateLog::genLog());
    colored.append(string(BEAT_THE_DEVIL_OUTTA_TTY));

    return colored;
}

std::map<int, string> ColorLogs::colorMap;

void ColorLogs::initColorMap()
{
    colorMap[MYLOG_TRACE_LVL] = string(TURQUOISE);
    colorMap[MYLOG_TRACE_FUN_LVL] = string(MAGENTA);
    colorMap[MYLOG_WARNING_LVL] = string(CADMIUM_YELLOW);
    colorMap[MYLOG_ERROR_LVL] = string(BRIGHT_RED);
}
