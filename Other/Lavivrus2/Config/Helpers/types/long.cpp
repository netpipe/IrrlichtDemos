#include "long.h"
#include <sstream>
#include <stdlib.h>

long LongInt::parseLong(const string& s)
{
    return atol(s.c_str());
}

string LongInt::toLong(const long &l)
{
    stringstream temp;
    temp << l;
    return temp.str();
}
