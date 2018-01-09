#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include "log_levels.h"
#include <map>
#define DEFAULT_PORT 3030


class Config
{
public:
    static Config *Instance();
    int port() const;

    int logLevel() const;

    std::string contentDirectory() const;

    std::string logTarget() const;

    bool colorLogs() const;

    void confFromCommandArgs(int argc, char** argv);

private:

    std::map<std::string, std::string> _options;
    std::map<std::string, int> _logMap;

    static Config *p_instance;

    int _port;
    int _logLevel;
    std::string _contentDirectory;

    void initDefault();

    void initFromFile();

    void parseLine(std::string &line);

    void initLogMap();

    void setOption(std::string name, std::string value);

    Config();
    void operator=(const Config&);
    Config(const Config&);
};

#endif // CONFIG_H
