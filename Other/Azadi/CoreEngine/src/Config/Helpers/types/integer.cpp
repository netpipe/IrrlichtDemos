#include "integer.h"
#include <sstream>

int Integer::parseInt(const string& s)
{
    return atoi(s.c_str());
}

string Integer::toInt(const int &i)
{
    stringstream temp;
    temp << i;
    return temp.str();
}
