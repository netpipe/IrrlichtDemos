#include "float.h"
#include <sstream>
#include <stdlib.h>

float Float::parseFloat(const std::string& s)
{
    return (float)atof(s.c_str());
}

std::string Float::toFloat(const float &f)
{
    std::stringstream temp;
    temp << f;
    return temp.str();
}
