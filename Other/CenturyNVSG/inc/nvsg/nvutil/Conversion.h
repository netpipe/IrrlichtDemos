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

namespace nvutil
{

template<typename T, typename U>
class Conversion
{
  typedef char Small;
  class Big { char dummy[2]; };
  static Small test(U);
  static Big test(...);
  static T makeT();
public:
  enum { exists = sizeof(test(makeT()))==sizeof(Small),
         sameType = 0 };
};

template<typename T>
class Conversion<T, T>
{
public:
  enum { exists = 1,
         sameType = 1 };
};

#define SUPERSUBCLASS(T,U) \
  nvutil::Conversion<const U*, const T*>::exists && \
  !nvutil::Conversion<const T*, const void*>::sameType

} // nvutil

