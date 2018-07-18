#ifndef INTEGER_H
#define INTEGER_H

#include <string>

class Integer
{
    public:
        static int parseInt(const std::string& s);
        static std::string toInt(const int &i);
};

#endif
