#include "config.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

Config *Config::p_instance = nullptr;

Config *Config::Instance()
{
    if(p_instance == nullptr)
        p_instance = new Config();
    return p_instance;
}

int Config::port() const
{
    return std::stoi(_options.at("port"));
}

int Config::logLevel() const
{
    std::string slvl = _options.at("logLevel");
    int res = _logMap.at(slvl);
    return res;
}

std::string Config::logTarget() const
{
    return _options.at("logTarget");
}

bool Config::colorLogs() const
{
    return (_options.at("colorLogs") == "YES");

}

std::string Config::contentDirectory() const
{
    return _options.at("contentDirectory");
}

void Config::confFromCommandArgs(int argc, char **argv)
{
    int c;

    struct option long_options[] = {
        {"port",        required_argument,  0,  0},
        {"loglevel",    required_argument,  0,  0},
        {"contentDir",  required_argument,  0,  0},
        {"logTarget",   required_argument,  0,  0},
        {"colorLogs",   required_argument,  0,  0},
        {0, 0, 0, 0}
    };

    while (1){
        int optIndex = 0;
        c = getopt_long(argc, argv, "", long_options, &optIndex);

        if (c == -1)
            break;

        switch (c){
        case 0:{
            std::string n(long_options[optIndex].name);
            std::string v(optarg);
            setOption(n, v);
            break;
        }
        case '?' :
            break;
        default:
            fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
        }
    }
}

void Config::initDefault()
{
    _port = DEFAULT_PORT;
    _logLevel = MYLOG_TRACE_LVL;
    _contentDirectory = std::string("/home/szikic/server/");

    _options["port"] = std::to_string(DEFAULT_PORT);
    _options["logLevel"] = "TRACE";
    _options["contentDirectory"] = "/home/szikic/server";
    _options["logTarget"] = "";
    _options["colorLogs"] = "YES";
}

void Config::initFromFile()
{
    std::ifstream confs;
    std::string line;

    confs.open("server.conf");
    if(confs.fail()){
        std::cerr << "Error while openning configuration file: " << strerror(errno);
        exit(1);
    }

    while(std::getline(confs, line)){
        parseLine(line);
    }
}

void Config::parseLine(std::string &line)
{
    size_t eqPos;

    if(line.empty())
        return;

    if(line.at(0) == '#')
        return;

    eqPos = line.find('=');

    if(eqPos != std::string::npos){
        /**althought unrecognized options are not a mistake
         * only those set during default initiation will be
         * updated here */
        auto it = _options.find(line.substr(0,eqPos));
        if(it == _options.end()){
            fprintf(stderr, "found odd option\n");
            return;
        }
        //std::cout << "option is " << line.substr(0,eqPos) << " and the value is " << line.substr(eqPos+1) << std::endl;
        _options[line.substr(0,eqPos)] = line.substr(eqPos+1);
    }
}

void Config::initLogMap()
{
    _logMap["TRACE"] = MYLOG_TRACE_LVL;
    _logMap["TRACE_FUN"] = MYLOG_TRACE_FUN_LVL;
    _logMap["ERROR"] = MYLOG_ERROR_LVL;
    _logMap["WARNING"] = MYLOG_WARNING_LVL;
    _logMap["NOLOG"] = 0;
}

void Config::setOption(std::string name, std::string value)
{
    _options[name] = value;
}

Config::Config()
{
    initLogMap();

    initDefault();

    initFromFile();
}
