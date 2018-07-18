#include "integer.h"
#include <sstream>
#include <stdlib.h>


int Integer::parseInt(const std::string& s)
{
    return atoi(s.c_str());
}

std::string Integer::toInt(const int &i)
{
    std::stringstream temp;
    temp << i;
    return temp.str();
}
