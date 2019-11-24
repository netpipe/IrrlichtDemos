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

#ifndef LINUX
# include "nvsgwindefs.h"
# ifndef NOMINMAX
#  define NOMINMAX // avoid problems with Microsoft definitions in windef.h and min/max from stl
# endif
# include <windows.h>
# undef NOMINMAX
#endif

#include <string>
#include "nvutil/Assert.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
namespace nvutil {

typedef std::basic_string <TCHAR> tstring;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//! Template class for easy access to the Registry.
template <typename T> class RegVal
{
public:
  enum
  {
    DIRTY  = 1
  };

  /** constructor */
  RegVal( const TCHAR * value_name , const TCHAR * subkey, DWORD type, HKEY root=HKEY_CURRENT_USER
        , const T& def=T() ); // T() provides initialization for fundamental types
  /** destructor */
  ~RegVal() {}
  /** convenient writes throug simple assignment */
  RegVal<T>& operator=(const T& rhs);
  /** convenient reads through implicit type conversion */
  operator const T&(); 

private:
  // not allowed
  RegVal(const RegVal<T>&) {}   // copying
  RegVal<T>& operator=(const RegVal<T>&) {} // assignment
  // for private use only
  DWORD create();
  DWORD read(T& out, DWORD nbytes);
  DWORD write(const T& in, DWORD nbytes);

private:
  tstring  m_name;
  tstring  m_subkey;
  DWORD   m_type;
  HKEY    m_root;
  HKEY    m_hkey;
  DWORD   m_flags;
  T       m_default;
  T       m_data;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
inline RegVal<T>::RegVal( const TCHAR * value_name
                        , const TCHAR * subkey
                        , DWORD type 
                        , HKEY root
                        , const T& def )
: m_name(value_name)
, m_subkey(subkey)
, m_type(type)
, m_root(root) 
, m_flags(DIRTY)
, m_default(def) 
{
  if ( ERROR_SUCCESS==create() )
  {
    if ( ERROR_FILE_NOT_FOUND==read(m_data, sizeof(T)) )
    {
      if ( ERROR_SUCCESS==write(m_default, sizeof(T)) ) 
      {
        m_data = m_default; // update internal cache
        m_flags &= ~DIRTY; // no need to walk the registry with the very next read
      }
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
inline RegVal<T>& RegVal<T>::operator=(const T& rhs) 
{ 
  write(rhs, sizeof(T)); // write the registry
  m_data = rhs; // update internal data cache according 
  m_flags &= ~DIRTY; // no need to walk the registry with the very next read
  return *this; 
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
inline RegVal<T>::operator const T&() 
{
  if ( m_flags & DIRTY ) {
    // need to read the registry for valid data
    if ( !(ERROR_SUCCESS==read(m_data, sizeof(T))) )
    {
      // reading failed, set to default
      m_data = m_default;
    }
    m_flags &= ~DIRTY; // assign data cache valid, no need to walk the 
                        // registry with the very next read
  }
  return m_data;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
inline DWORD RegVal<T>::create()
{
  DWORD err;
  DWORD dwDisposition;

  // create the key if not already exists
  if ( ERROR_SUCCESS == (err = RegCreateKeyEx( m_root
                                             , m_subkey.c_str()
                                             , 0
                                             , NULL
                                             , REG_OPTION_NON_VOLATILE
                                             , KEY_ALL_ACCESS
                                             , NULL
                                             , &m_hkey
                                             , &dwDisposition ))
     )
  {
    // close key
    RegCloseKey(m_hkey);
  }

  return err;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template <class T>
inline DWORD RegVal<T>::read(T& out, DWORD nbytes)
{
  DWORD err;
  DWORD type;
  
  if ( ERROR_SUCCESS == (err = RegOpenKeyEx(m_root, m_subkey.c_str(), NULL, KEY_READ, &m_hkey)) ) {
    err = RegQueryValueEx(m_hkey, m_name.c_str(), NULL, &type, (LPBYTE)&out, &nbytes);
    NVSG_ASSERT((err==ERROR_SUCCESS)==(m_type==type)); // type mismatch!
    RegCloseKey(m_hkey);
  }
  return err;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<class T>
inline DWORD RegVal<T>::write(const T& in, DWORD nbytes)
{
  DWORD err;

  if ( ERROR_SUCCESS == (err = RegOpenKeyEx(m_root, m_subkey.c_str(), NULL, KEY_ALL_ACCESS, &m_hkey)) ) {
    err = RegSetValueEx(m_hkey, m_name.c_str(), NULL, m_type, (const LPBYTE)&in, nbytes);
    RegCloseKey(m_hkey);
  }
  return err;
}

//  DOXYGEN can't handle template specializations (warns about no matching class member found)
#if ! defined( DOXYGEN_IGNORE )
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// string type needs some specialization with read and write
template<>
inline DWORD RegVal<tstring>::read(tstring& out, DWORD nbytes)
{
  DWORD err;
  DWORD type;

  if ( ERROR_SUCCESS == (err = RegOpenKeyEx(m_root, m_subkey.c_str(), NULL, KEY_READ, &m_hkey)) ) {
    // (1) call RegQueryValueEx with output data set to NULL to get the exact byte count required  
    if ( ERROR_SUCCESS == (err = RegQueryValueEx(m_hkey, m_name.c_str(), NULL, &type, NULL, &nbytes)) ) {
      // create a temporary tstring and resize its controlled sequence according
      // to the byte count received with the previous call to RegQueryValueEx()
      tstring tmp;
      tmp.resize(nbytes);
      // (2) call RegQueryValueEx with valid output buffer now to receive the requested data  
      if ( ERROR_SUCCESS == (err = RegQueryValueEx(m_hkey, m_name.c_str(), NULL, &type, (LPBYTE)tmp.data(), &nbytes)) ) {
        NVSG_ASSERT((err==ERROR_SUCCESS)==(m_type==type)); // type mismatch!
        // assign the c-string of the temporary string object rather than the string object itself to
        // ensure that the length of the output string's controlled sequence matches exactly that
        // of the enclosed c-string
        out = tmp.c_str();
      }
    }
    RegCloseKey(m_hkey);
  }
  return err;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline DWORD RegVal<tstring>::write(const tstring& in, DWORD nbytes)
{
  DWORD err;

  if ( ERROR_SUCCESS == (err = RegOpenKeyEx(m_root, m_subkey.c_str(), NULL, KEY_ALL_ACCESS, &m_hkey)) ) {
    err = RegSetValueEx(m_hkey, m_name.c_str(), NULL, m_type, (const LPBYTE)in.c_str(), nbytes);
    RegCloseKey(m_hkey);
  }
  return err;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
template<>
inline RegVal<tstring>& RegVal<tstring>::operator=(const tstring& rhs) 
{ 
  // determine required byte count
  DWORD nbytes = (DWORD)rhs.length()+1; // including terminating 0
  nbytes *= sizeof(TCHAR);

  // write to registry now
  write(rhs, nbytes);

  m_data = rhs; // update internal data cache according 
  m_flags &= ~DIRTY; // no need to walk the registry with the very next read

  return *this; 
}
#endif  //  DOXYGEN_IGNORE
    
} // namespace nvutil

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// avoid tedious, repeated qualification of namespace scope
//using nvutil::RegVal;

