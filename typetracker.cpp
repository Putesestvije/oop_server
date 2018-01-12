#include "typetracker.h"
#include <string>
#include <iostream>

void TypeTracker::updateMsg(std::string msg)
{
    if(_mimes.find(msg) == _mimes.end())
        _mimes[msg] = 0;
    _mimes[msg] += 1;

    for(auto &i: _mimes){
        std::cout << i.first << " : " << i.second << " ";
    }
    std::cout << std::endl;
}

TypeTracker::TypeTracker()
{

}
