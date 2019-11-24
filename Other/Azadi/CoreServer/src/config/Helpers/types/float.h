#ifndef FLOAT_H
#define FLOAT_H

#include <string>

using namespace std;

class Float
{
    public:
        static float parseFloat(const string& s);
        static string toFloat(const float& f);
};

#endif
