#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "colors.h"

using std::string;

class Logger
{
public:
    virtual string genLog() = 0;
};

class LogMsg : public Logger
{
public:
    LogMsg(string msg);
    string genLog();
private:
    string _msg;
};

class DecorateLog : public Logger{
public:
    DecorateLog(Logger *l) : _l(l){}
    string genLog();
private:
    Logger *_l;
};

class LogFile : public DecorateLog{
public:
    LogFile(Logger *l) : DecorateLog(l){}
    string genLog();
private:

};

class LogLine : public DecorateLog{
public:
    LogLine(Logger *l) : DecorateLog(l){}
    string genLog();
private:
    int _line;
};

class LogFunction : public DecorateLog
{
public:
    LogFunction(Logger *l, string func) : DecorateLog(l), _function(func) {}
    string genLog();
private:
    string _function;
};

class ColorLogs : public DecorateLog
{
public:
    ColorLogs(Logger * l, int logLvl) : DecorateLog(l), _logLvl(logLvl), mapInitiated (false) {}
    string genLog();
private:
    void initColorMap();
    int _logLvl;
    bool mapInitiated;
    static std::map<int, string> colorMap;
};


#endif // LOGGER_H
