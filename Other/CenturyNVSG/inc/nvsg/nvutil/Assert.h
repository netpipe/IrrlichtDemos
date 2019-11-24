#pragma once

#include "nvsggnudefs.h" // GNU compiler specific defines
#include "nvsg/nvsgapi.h"  // storage-class defines

namespace nvutil
{
  extern NVSG_API void AssertionFailed(const char* assertion, const char* file, unsigned int line, const char * function); 
} // namespace nvutil

#if !defined(NDEBUG) && !defined(NVSG_HYBRID)
// assert macro
#if defined(LINUX)
#define NVSG_ASSERT(exp) \
  if (exp){ \
  } else { \
    nvutil::AssertionFailed(#exp, __FILE__, __LINE__, __func__); \
  }
#else
#include <assert.h>
#define NVSG_ASSERT assert
#endif

// verify the expression f succeeds
#define NVSG_VERIFY(f) NVSG_ASSERT((f))

// NOTE: GNU compilers < 3.3.0 have serious problems with meta programing 
// techniques that we use e.g. to implement our compile-time assert 
#if defined(__GNUC__) && GCC_VERSION < 30300

#define NVSG_CTASSERT(exp)
#define NVSG_CTASSERT_BYTESIZE(type,size)
#define NVSG_CTASSERT_BYTESIZEi(type,size,ln)
#define NVSG_CTASSERT_BYTESIZEii(type,size,ln)
#define NVSG_CTASSERT_MODULO_BYTESIZE(type,size)
#define NVSG_CTASSERT_MODULO_BYTESIZEi(type,size,ln)
#define NVSG_CTASSERT_MODULO_BYTESIZEii(type,size,ln)

#else 

// compile time assert
template <bool> class CompileTimeAssert;
template<> class CompileTimeAssert<true> {};
#define NVSG_CTASSERT(exp) sizeof(CompileTimeAssert<(exp)!=0>)

#define NVSG_CTASSERT_BYTESIZE(type,size) NVSG_CTASSERT_BYTESIZEi(type,size, __LINE__)
#define NVSG_CTASSERT_BYTESIZEi(type,size,ln) NVSG_CTASSERT_BYTESIZEii(type,size,ln)
#define NVSG_CTASSERT_BYTESIZEii(type,size,ln) \
  static CompileTimeAssert<sizeof(type)==size> error_at_line_##ln##__##type##_has_unexpected_size

#define NVSG_CTASSERT_MODULO_BYTESIZE(type,size) NVSG_CTASSERT_MODULO_BYTESIZEi(type,size, __LINE__)
#define NVSG_CTASSERT_MODULO_BYTESIZEi(type,size,ln) NVSG_CTASSERT_MODULO_BYTESIZEii(type,size,ln)
#define NVSG_CTASSERT_MODULO_BYTESIZEii(type,size,ln) \
  static CompileTimeAssert<sizeof(type)%size==0> error_at_line_##ln##__##type##_has_unexpected_size

#endif

#else // non-debug configuration

#define NVSG_ASSERT(exp) static_cast<void>(0)
#define NVSG_VERIFY(f)  (static_cast<void>(f))
#define NVSG_CTASSERT(exp)
#define NVSG_CTASSERT_BYTESIZE(type,size)
#define NVSG_CTASSERT_BYTESIZEi(type,size,ln)
#define NVSG_CTASSERT_BYTESIZEii(type,size,ln)
#define NVSG_CTASSERT_MODULO_BYTESIZE(type,size)
#define NVSG_CTASSERT_MODULO_BYTESIZEi(type,size,ln)
#define NVSG_CTASSERT_MODULO_BYTESIZEii(type,size,ln)
#endif

