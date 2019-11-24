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

#include "nvsgcommon.h"

namespace nvutil
{

  inline unsigned int bit( unsigned int index, size_t value )
  {
    NVSG_ASSERT( index < 8*sizeof(size_t) );
    return( !!( value & ( (size_t)1 << index ) ) );
  }

typedef unsigned long bitset32;
typedef unsigned long long bitset64;

//! Template class for easy definition of a single-bit bitmask.
template <int n>
class Bit
{
public:
  enum { mask = 1<<n };
};

//  Don't need to document these tiny little helper defines !!!
#if ! defined( DOXYGEN_IGNORE )

#define BIT(n) nvutil::Bit<n>::mask
#define BIT64(n) ((nvutil::bitset64)nvutil::Bit<0>::mask << (n))

#define BIT0  BIT(0)
#define BIT1  BIT(1)
#define BIT2  BIT(2)
#define BIT3  BIT(3)
#define BIT4  BIT(4)
#define BIT5  BIT(5)
#define BIT6  BIT(6)
#define BIT7  BIT(7)
#define BIT8  BIT(8)
#define BIT9  BIT(9)
#define BIT10 BIT(10)
#define BIT11 BIT(11)
#define BIT12 BIT(12)
#define BIT13 BIT(13)
#define BIT14 BIT(14)
#define BIT15 BIT(15)
#define BIT16 BIT(16)
#define BIT17 BIT(17)
#define BIT18 BIT(18)
#define BIT19 BIT(19)
#define BIT20 BIT(20)
#define BIT21 BIT(21)
#define BIT22 BIT(22)
#define BIT23 BIT(23)
#define BIT24 BIT(24)
#define BIT25 BIT(25)
#define BIT26 BIT(26)
#define BIT27 BIT(27)
#define BIT28 BIT(28)
#define BIT29 BIT(29)
#define BIT30 BIT(30)
#define BIT31 BIT(31)

#define BIT32 BIT64(32)
#define BIT33 BIT64(33)
#define BIT34 BIT64(34)
#define BIT35 BIT64(35)
#define BIT36 BIT64(36)
#define BIT37 BIT64(37)
#define BIT38 BIT64(38)
#define BIT39 BIT64(39)
#define BIT40 BIT64(40)
#define BIT41 BIT64(41)
#define BIT42 BIT64(42)
#define BIT43 BIT64(43)
#define BIT44 BIT64(44)
#define BIT45 BIT64(45)
#define BIT46 BIT64(46)
#define BIT47 BIT64(47)
#define BIT48 BIT64(48)
#define BIT49 BIT64(49)
#define BIT50 BIT64(50)
#define BIT51 BIT64(51)
#define BIT52 BIT64(52)
#define BIT53 BIT64(53)
#define BIT54 BIT64(54)
#define BIT55 BIT64(55)
#define BIT56 BIT64(56)
#define BIT57 BIT64(57)
#define BIT58 BIT64(58)
#define BIT59 BIT64(59)
#define BIT60 BIT64(60)
#define BIT61 BIT64(61)
#define BIT62 BIT64(62)
#define BIT63 BIT64(63)

#endif  //  DOXYGEN_IGNORE

} // namespace nvutil

