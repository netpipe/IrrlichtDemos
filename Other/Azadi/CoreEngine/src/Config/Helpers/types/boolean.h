#ifndef BOOLEAN_H
#define BOOLEAN_H

#include <string>

using namespace std;

class Bool
{
    public:
        static bool parseBool(const string& s);
        static string toBool(const bool &flag);
};

#endif
