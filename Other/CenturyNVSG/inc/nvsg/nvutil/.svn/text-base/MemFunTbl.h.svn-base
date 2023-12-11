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

#include <vector>

namespace nvutil
{

//! Member Function Table template
template<typename T>
class MemFunTbl
{
  typedef void (T::*PMFN) ();         //!< pointer to member function type
public:
  explicit MemFunTbl(size_t size);    //!< reserve 'size' table entries, to avoid frequend allocations
  PMFN operator[](size_t i) const;    //!< read-only access the function pointer stored at index i; behavior is undefined for invalid indices
  template <typename U>
  void addEntry(size_t i, U pmfn); //!< register function pointer pmfn at index i; former entry at i will by overridden
  bool testEntry(size_t i) const;     //!< test if entry at i is valid
private:
  std::vector<PMFN> m_ftbl;           //!< simply use a plain vector as function table
};

// inline implementations can go here

template<typename T>
inline MemFunTbl<T>::MemFunTbl(size_t size)
{ // table entries to avoid frequent allocations at addEntry
  m_ftbl.resize(size, NULL); 
}

template<typename T>
inline bool MemFunTbl<T>::testEntry(size_t i) const
{ // test if entry at i is a valid entry
  return (i < m_ftbl.size()) && !!m_ftbl[i]; 
}

template<typename T>
inline typename MemFunTbl<T>::PMFN MemFunTbl<T>::operator[](size_t i) const
{ // read-only access to function pointer stored at index i
  NVSG_ASSERT(testEntry(i)); // undefined behavior for invalid entries
  return m_ftbl[i];
}

template<typename T>
template<typename U>
inline void MemFunTbl<T>::addEntry(size_t i, U pmfn)
{ // register function pointer
  if ( m_ftbl.size() <= i )
  { // add 32 table entries
    m_ftbl.resize(i+0x20, NULL); 
  }
  m_ftbl[i]=*(PMFN*)&pmfn;
}

} // end of namespace nvutil scope
