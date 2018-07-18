#include "long.h"
#include <sstream>
#include <stdlib.h>

long LongInt::parseLong(const std::string& s)
{
    return atol(s.c_str());
}

std::string LongInt::toLong(const long &l)
{
    std::stringstream temp;
    temp << l;
    return temp.str();
}
