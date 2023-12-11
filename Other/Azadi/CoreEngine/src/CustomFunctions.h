//
// File:   CustomFunctions.h
// Author: Aren Villanueva
//
// Created on March 1, 2007, 10:51 PM
//
// Contains custom functions relating to C style datatypes
//

#ifndef _CustomFunctions_H
#define	_CustomFunctions_H

// String segment copy
inline char* strscpy(char *dest, const char *src, unsigned int start, unsigned int len)
{
    for (unsigned int i = start; i < (len+start); ++i)
        dest[i-start] = src[i];
    
    return dest;
}

inline void* convert(void* dest, void* src, unsigned int size)
{
    char *p = (char*)dest;
    char *pp = (char*)src;
    for(unsigned int i = 0; i < size; ++i)
        p[i] = pp[i];
    
    return (void*)p;
}

#endif	/* _CustomFunctions_H */

