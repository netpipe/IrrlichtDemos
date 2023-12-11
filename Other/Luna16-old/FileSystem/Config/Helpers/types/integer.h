#ifndef INTEGER_H
#define INTEGER_H

#include <string>

using namespace std;

class Integer
{
    public:
        static int parseInt(const string& s);
        static string toInt(const int &i);
};

#endif
