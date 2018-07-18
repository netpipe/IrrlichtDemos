#ifndef LONG_H
#define LONG_H

#include <string>

class LongInt
{
    public:
        static long parseLong(const std::string& s);
        static std::string toLong(const long & l);
};

#endif
