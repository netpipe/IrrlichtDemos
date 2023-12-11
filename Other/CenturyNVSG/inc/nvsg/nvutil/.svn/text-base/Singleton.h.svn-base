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

#include "nvsg/nvsgapi.h"  // storage-class defines

namespace nvutil {

//! A Singleton framework
/** Ensures a class \c T only has one instance. Defines a global point of access to that object.  
  */
template <typename T>
class Singleton
{
public:
  //! Single instance access
  /** Receive a pointer to the single instance.
    */
  static T* instance();

private:
  // not permitted 
  // client must call Singleton<T>::instance() for instantiation
  Singleton() {}
  Singleton(const Singleton<T>&) {}
  ~Singleton() {}
  // also not permitted
  Singleton<T>& operator=(const Singleton<T>&) {}

private:
  static NVSG_API T m_instance; //!< single instance of T
};

template <typename T>
inline T* Singleton<T>::instance()
{
  return &m_instance;
};

} // namespace nvutil
