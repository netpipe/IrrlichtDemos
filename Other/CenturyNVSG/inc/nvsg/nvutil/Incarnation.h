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

#include "nvsgcommon.h"

namespace nvutil {

//! This class abstracts an incarnation count.
/** The main feature is that the embedded counter will always be initialized to a defined value.
  * This class also provides convenient assignment, comparison and incrementation of the embedded
  * counter. */
class Incarnation
{
public:
  //! Default constructor.
  /** The embedded counter will be initialized to some random number. */
  NVSG_API Incarnation();

  //! Comparison operator
  /** \returns \c true if the embedded counter of the Incarnation object referenced by \a rhs
    * is equal to this object's embedded counter, \c false otherwise. */
  NVSG_API bool operator==(const Incarnation& rhs) const;

  //! Comparison operator
  /** \returns \c true if the embedded counter of the Incarnation object referenced by \a rhs
    * is not equal to this object's embedded counter, \c false otherwise. */
  NVSG_API bool operator!=(const Incarnation& rhs) const;

  //! Pre-increment operator
  /** Increments the embedded counter by one.
    * \returns This object as a l-value object. */
  NVSG_API Incarnation& operator++(); // prefix ++

  //! Post-increment operator
  /** Increments the embedded counter by one.
    * \returns A const Incarnation object of which the embedded counter's value is equal to the
    * value, the embedded counter of this object had immediately prior to the call to this operator. */
  NVSG_API const Incarnation operator++(int); // postfix ++

  //! Addition operator
  NVSG_API Incarnation operator+( int i ) const;

private:
  NVSG_API explicit Incarnation(int incarnation);

private:
  int m_incarnation;
};

inline bool Incarnation::operator==(const Incarnation& rhs) const 
{ 
  return m_incarnation==rhs.m_incarnation; 
}

inline bool Incarnation::operator!=(const Incarnation& rhs) const
{ 
  return m_incarnation!=rhs.m_incarnation; 
}

inline Incarnation& Incarnation::operator++()
{
  ++m_incarnation;
  return *this;
}

inline const Incarnation Incarnation::operator++(int)
{
  return Incarnation(m_incarnation++);
}

inline Incarnation Incarnation::operator+( int i ) const
{
  return( Incarnation( m_incarnation + i ) );
}

} // namespace nvutil

