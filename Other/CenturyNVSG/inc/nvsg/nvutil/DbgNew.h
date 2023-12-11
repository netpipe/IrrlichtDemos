// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** \file */

#include <new>

#include "nvsgcommon.h"

// The following new operator overload forwards to default new, and hence
// has a matching delete operator. Disable the warning.
#pragma warning (disable:4291)

//! Dummy overload for objects that don't have an according new operator 
inline void * operator new(size_t size, const char * src, unsigned int ln)
{
  return operator new(size); 
}

//! Dummy overload for arrays of objects that don't have an according new operator 
#if !defined (_WIN64)
inline void * operator new[](size_t size, const char * src, unsigned int ln)
{
  return ::operator new[](size, std::nothrow); 
}
#endif

#if defined(_DEBUG)
# define new new(__FILE__, __LINE__)
#endif


