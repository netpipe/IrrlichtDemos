#ifndef LONG_H
#define LONG_H

#include <string>

using namespace std;

class LongInt
{
    public:
        static long parseLong(const string& s);
        static string toLong(const long & l);
};

#endif
