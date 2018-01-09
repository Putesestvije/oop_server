#ifndef LOGGING_H
#define LOGGING_H

#include <map>
#include <fstream>

#include "config.h"
#include "log_levels.h"
#include "colors.h"
#include "logger.h"

using std::string;

#define MYLOG(level, msg) do{_log(level, msg, __FILE__, __func__, __LINE__);}while(0);


void _log(int level,const  char *msg, const char *File, const char *func, int Line);



#endif // LOGGING_H
