#include "boolean.h"

bool Bool::parseBool(const string& s)
{
    if (s == "true")
        return true;
    else
        return false;
}

string Bool::toBool(const bool &b)
{
    if (b)
        return "true";
    else
        return "false";
}
