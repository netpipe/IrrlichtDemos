
#ifndef __STRINGALING_H
#define __STRINGALING_H

#include <string>
#include <sstream>

template<typename T>
inline std::string stringify(const T& x)
{
    std::ostringstream o;
    if (!(o << x)) return "";
    return o.str();
}

template<typename T>
inline T fromString(char *s)
{
    std::string str = s;
    std::istringstream i(str);
    T x;
    i >> x;
    return x;
}

inline std::string strparty(const char *str1, int n, const char *str2)
{
    std::string str = str1;
    str += stringify(n);
    str += str2;
    return str;
}

#endif
