#ifdef LINUX

// Note: This file has to be included first in your application,
//       if you are using gcc 3.4 or 4.0 and it was not configured with
//       "--enable-libstdcxx-allocator=mt"    (use "g++ -v" to check)
// 
//       Include it either with #include <filename> or invoke g++ with the
//       "-include <filename>" parameter to have it automatically included first.


#ifndef FORCE_MTALLOC_H
#define FORCE_MTALLOC_H 1


// enforce usage of the mt_allocator
#define _CXX_ALLOCATOR_H 1
#include <ext/mt_allocator.h>
#define ___glibcxx_base_allocator  __gnu_cxx::__mt_alloc
#define __glibcxx_base_allocator  __gnu_cxx::__mt_alloc


#endif // FORCE_MTALLOC
#endif // LINUX
