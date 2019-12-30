#include "float.h"
#include <sstream>
#include <stdlib.h>

float Float::parseFloat(const string& s)
{
    return (float)atof(s.c_str());
}

string Float::toFloat(const float &f)
{
    stringstream temp;
    temp << f;
    return temp.str();
}
