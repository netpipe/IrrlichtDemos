#ifndef FLOAT_H
#define FLOAT_H

#include <string>

class Float
{
    public:
        static float parseFloat(const std::string& s);
        static std::string toFloat(const float& f);
};

#endif
