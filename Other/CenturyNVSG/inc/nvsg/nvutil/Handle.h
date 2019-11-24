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

/*! \file */
#pragma once

#if defined(_WIN32)
# include <basetsd.h>
#endif
#if defined(LINUX)
# include <stdint.h>
typedef uintptr_t UINT_PTR;
#endif

#include "nvutil/Allocator.h"
#include "nvutil/RCObject.h"
#include "nvutil/SWMRSync.h"
#include "nvutil/Assert.h"
#include "nvutil/Conversion.h"

namespace nvutil
{

/*! \def CreateHandle(HandleType)
    \brief Instantiates a Handle of type \a HandleType.
    \param HandleType Type of the particular Handle to instantiate.
    \returns A pointer to the new instantiated Handle.

    Use this macro to default-construct a Handle of type \a HandleType.
*/
/*! \def CreateHandleFromBase(HandleType, BaseHandle)
    \brief Instantiates a Handle of type \a HandleType from the specified \a BaseHandle.
    \param HandleType Type of the particular Handle to instantiate.
    \param BaseHandle Pointer to a Handle which type is a base of \a HandleType.
    \returns A pointer to the new instantiated Handle.

    Use this macro to construct a Handle of type \a HandleType. 
    The instantiated Handle is a partial copy of the Handle pointed to by \a BaseHandle.

    This macro is useful, for example, if you intend to copy-convert sibling Handle types.
    That is, let B_Handle and C_Handle both inherit from A_HANDLE, than you can use 
    CreateHandleFromBase to instantiate a C_Handle as a partial copy from a B_Handle.
*/
#if !defined(NDEBUG) && !defined(DOXYGEN_IGNORE)
# define CreateHandle(HandleType) HandleType::create(__FILE__, __LINE__)
# define CreateHandleFromBase(HandleType,BaseHandle) HandleType::createFromBase(__FILE__, __LINE__, BaseHandle)
# define _new_ new(__FILE__, __LINE__)
#else
# define CreateHandle(HandleType) HandleType::create()
# define CreateHandleFromBase(HandleType,BaseHandle) HandleType::createFromBase(BaseHandle)
# if !defined(DOXYGEN_IGNORE)
#  define _new_ new
# endif
#endif

// special 'null' type used to build Handle class hierarchy
class NullType;
class EmptyType {};

// serves as base class for 'handled' objects
class HandledObject
{
protected:
  HandledObject() {/* do nothing! */}
  HandledObject(const HandledObject&) {/* do nothing! */}
  virtual ~HandledObject() {}
private:
  template <typename T> friend struct Holder;
  template <typename T> friend T* getHandle(const HandledObject*);
  enum { SIGNATURE=0x4753564e /* "NVSG" */};
  int m_signature; // initialized with SIGNATURE if the object is handled
  UINT_PTR m_handle; // holds the address of the corresponding handle
};

// enables back-referencing an object to its handle 
// and serves locking purposes
template <typename T>
struct Holder : public nvutil::IAllocator 
{
  // both, the default constructor and the copy constructor use the placement new 
  // to create an object of type T at &rawmem[0]
  Holder(UINT_PTR hdl) 
  {
    // early assign the handle!
    // note: 
    // need to early assign handle and signature here because some object 
    // constructors require a valid handle available at construction-time
    ptr = (T*)&rawmem[0];
    ptr->m_signature = HandledObject::SIGNATURE;  
    ptr->m_handle = hdl;  
    // construct the object at specified memory
    // note:
    // the following does not override already assigned handle and signature!
    // see HandleObject constructor for more details
    ptr = new(&rawmem[0]) T(); 
  }
  Holder(const Holder<T>& rhs, UINT_PTR hdl) 
  {
    // early assign the handle!
    // note: 
    // need to early assign handle and signature here because some object 
    // constructors require a valid handle available at construction-time
    ptr = (T*)&rawmem[0];
    ptr->m_signature = HandledObject::SIGNATURE;  
    ptr->m_handle = hdl;  
    // copy-construct the object at specified memory
    // note:
    // the following does not override already assigned handle and signature!
    // see HandleObject constructor for more details
    ptr = new(&rawmem[0]) T(*rhs.ptr); 
  }
  template <typename U>
  Holder(const Holder<U>& rhs, UINT_PTR hdl)
  {
    // early assign the handle!
    // note: 
    // need to early assign handle and signature here because some object 
    // constructors require a valid handle available at construction-time
    ptr = (T*)&rawmem[0];
    ptr->m_signature = HandledObject::SIGNATURE;  
    ptr->m_handle = hdl;  
    // copy-construct the object at specified memory
    // note:
    // the following does not override already assigned handle and signature!
    // see HandleObject constructor for more details
    ptr = new(&rawmem[0]) T(*rhs.ptr);
  }
  // the destructor explicitely calls T's destructor
  ~Holder() { ptr->~T(); }

  nvutil::SWMRSync lock; // handles exclusive and shared locking
  T * ptr; // points to the object created at &rawmem[sizeof(UINT_PTR)]

  // note: this array must reside at the very end of this structure
  // as its size varies for different types T and hence would alter
  // offsets for subsequent members!
  char rawmem[sizeof(T)]; 
};

// general definition helps to limit certain hierarchies, that is - 
// (1) a Handle must either be a stand-alone class (not derived from any other class) or 
// (2) must be derived from another Handle class

/*! \class Handle
    \brief Controls access to the the handled object.
    \param T Specifies the type of the handled object.
    \param U Specifies an optional base class for the particular Handle.

    The purpose of a Handle is to limit direct access to the handled object. 
    The user can access the interface of the handled object through the interface of the Handle.

    A Handle not only controls access to the handled object but also controls the lifetime of the
    handled object. The handled object will be created at Handle instantiation and will be 
    automatically destroyed when the Handle's lifetime ends.

    The instantiation of a Handle is limited. A Handle can live on heap only. 
    Attempts to create a Handle on stack will be rejected by the compiler. 
    It is also prohibited to explicitely call new or malloc on a Handle.
    The only ways possible to instantiate a Handle are  

    -# by calling \ref CreateHandle, or
    -# by calling \ref CreateHandleFromBase, or
    -# by instantiating a Handle as a copy from another Handle by invoking the Handle's clone member.
  
    The special design of the Handle template helps to limit the definition of different Handle
    types as folows:

    - A Handle type must either be a stand-alone class, not derived from any other class, or
    - must have another Handle type as base class.

    Examples:
    \code
        // valid typedefs
        typedef nvutil::Handle<nvsg::Object> ObjectHandle;
        typedef nvutil::Handle<nvsg::Node, ObjectHandle> NodeHandle;
    \endcode

    The purpose of the Handle template for NVSG core is to have the compiler generate a particular
    hierarchy of Handles. This enables us to create a kind of parallel hierarchy of Handles that
    reflect the same class hierarchy that we have for the NVSG core objects. That is, in the same
    way nvsg::Node inherits from nvsg::Object, NodeHandle inherits from ObjectHandle, and so on. 
    This special Handle types thus carry the associated NVSG core objects and protect the core
    objects from direct access.

    \sa Handle<T, NullType>, Handle<T, Handle<U,V> >
*/
template <typename T, typename U=NullType>
class Handle;

/*! \brief Specialized stand-alone Handle
 
    This specialized Handle is used as a template for stand-alone Handle types. 
    Stand-alone Handle types are types that do not inherit from other types.

    \sa Handle
*/
template <typename T>
class Handle<T, NullType> : public nvutil::RCObject
{
public:
  // deep copy
  // NOTE: For this function to work as expected, it needs to be 'virtual'
  // However, declaring it virtual disables lazy instantiation for this function.
  // That is - the compiler would fully instantiate the definition and not only
  // as much as is really needed. For a generated class hierarchy for which the wrapped
  // type is part of a class hierarchy with an abstract base class, for example, this would 
  // end up in a compiler error, because abstract base classes cannot be instantiated.
  // A workaround for this is to implement a pseudo-virtual call mechanism for the clone
  // function via a function pointer which will be overridden for each 'real' Handle instance. 

  /*! \brief Returns a copy of this Handle
      \return
      The function returns a pointer to the Handle that was instantiated as a deep-copy
      of this Handle.

      Use this function if you want to instantiate a real clone of this Handle. 
  */
  Handle * clone() const; 

  /*! \brief Provides write access to the handle object.
      \return 
      The function returns a pointer to the handled object, through which the user can access the 
      objects interface.

      If NVSG-runtime was configured to run in a multithreaded environment, the function locks the
      handled object for exclusive write access, before returning a pointer to that object.

      The object remains locked until the write-lock will be released through endEdit.

      Attempts to further use the returned pointer after the write-lock was released is an error and 
      results in undefined runtime behavior.
  */
  T * beginEdit(); 

  /*! \brief Releases write access to the handle object.

      The function releases the write access to the handled object that was previously obtained
      through beginEdit.
  */
  void endEdit();        

  /*! \brief Provides read-only access to the handle object.
      \return 
      The function returns a pointer to the handled object, through which the user can access the 
      objects const, thus read only interface.

      If NVSG-runtime was configured to run in a multithreaded environment, the function locks the
      handled object for shared read-only access, before returning a pointer to that const object.

      The object remains locked until the read-lock will be released through endRead.

      Attempts to further use the returned pointer after the read-lock was released is an error and 
      results in undefined runtime behavior.
  */
  const T * beginRead(); 

  /*! \brief Releases read-only access to the handle object.

      The function releases the read-only access to the handled object that was previously obtained
      through beginRead.

      \note 
      Attempts to write-lock an object that already was read-locked by the same thread dead-locks
      your application!
  */
  void endRead();       

#if !defined(DOXYGEN_IGNORE)

public:
  static Handle * create();
#if !defined(NDEBUG)
  // debug create provides leak tracking through CreateHandle macro
  static Handle * create(const char* file, unsigned int line);
#endif

  // read-only access to holder
  const Holder<T> * holder() const;

#if !defined(NDEBUG)
  void assertWriteLocked() const;
  void assertReadLocked() const;
#endif

protected:
  // default constructor will be invoked from derived class's constructors only
  Handle(); 
  virtual ~Handle();

protected:
  // function pointer through which to call the pseudo-virtual clone function.
  Handle * (Handle::*m_pfnVirtualClone)(void) const;
  // holds the object to be 'handled'
  Holder<T> * m_holder;

#if !defined(NDEBUG)
  int m_dbgWriteCount;
  int m_dbgReadCount;
  bool m_dbgInsideConstructor;
  bool m_dbgInsideDestructor;
#endif

private:
  // copy constructor will be invoked by clone
  Handle(const Handle& rhs);
  // init on default construction; will be invoked by create
  void default_init();
  // implementation of clone, called through m_pfnVirtualClone
  Handle * clone_impl() const;

  // not permitted ...
  // ... assignment
  Handle& operator=(const Handle& rhs);
  // ... comparison 
  bool operator==(const Handle& rhs) const;
  bool operator!=(const Handle& rhs) const;

#endif // DOXYGEN_IGNORE
};

/*! \brief Specialization template for Handle-derived Handle types
 
    This specialized Handle is used as a template for Handle-derived handle types. 

    \sa Handle, Handle<T, NullType>
*/
template <typename T, typename U, typename V>
class Handle<T, Handle<U,V> > : public Handle<U,V>
{
public:
  /*! \brief Returns a copy of this Handle
      \return
      The function returns a pointer to the Handle that was instantiated as a deep-copy
      of this Handle.

      Use this function if you want to instantiate a real clone of this Handle. 
  */
  Handle * clone() const; // hides! base class implementation

#if !defined(DOXYGEN_IGNORE)
public:
  // instantiation
  static Handle * create();
  static Handle * createFromBase(const Handle<U,V>& base);
#if !defined(NDEBUG)
  static Handle * create(const char * file, unsigned int line);
  static Handle * createFromBase(const char * file, unsigned int line, const Handle<U,V>& base);
#endif
  
  // accessors  
  T * beginEdit();       // hides! base class implementation
  const T * beginRead(); // hides! base class implementation
  // no need to implement end[Edit/Read] functions

protected:
  Handle();
  virtual ~Handle();

private:
  // private typedef used in private constructors below  
  typedef Handle* (Handle::*PFNVIRTUALCLONE) (void) const;
  // copy constructor, called through clone
  Handle(const Handle& rhs);
  // construct from a base Handle
  explicit Handle(const Handle<U,V>& base);
  // init on default construction; will be invoked by create
  void default_init();
  // implementation of clone, called via m_pfnVirtualClone
  Handle * clone_impl() const;

  // not permitted ...
  // ... assignment
  Handle& operator=(const Handle& rhs);
  // ... comparison 
  bool operator==(const Handle& rhs) const;
  bool operator!=(const Handle& rhs) const;

#endif // DOXYGEN_IGNORE
};

//
// implementation of member functions
//

template <typename T>
inline Handle<T, NullType> * Handle<T, NullType>::create() 
{ 
  Handle<T, NullType> * hdl = new Handle<T, NullType>();
  hdl->default_init();
  return hdl;
}

#if !defined(NDEBUG)
  // debug create provides leak tracking through CreateHandle macro
template <typename T>
inline Handle<T, NullType> * Handle<T, NullType>::create(const char* file, unsigned int line) 
{ 
  Handle<T, NullType> * hdl = new(file, line) Handle<T,NullType>(); 
  hdl->default_init();
  return hdl;
}
#endif

template <typename T>
inline Handle<T, NullType>::Handle() 
#if !defined(NDEBUG)
: m_dbgWriteCount(0)
, m_dbgReadCount(0)
, m_dbgInsideConstructor(true)
, m_dbgInsideDestructor(false)
#endif
{}

template <typename T>
inline Handle<T, NullType>::Handle(const Handle<T,NullType>& rhs) 
: m_pfnVirtualClone(&Handle<T, NullType>::clone_impl)
#if !defined(NDEBUG)
, m_dbgWriteCount(0)
, m_dbgReadCount(0)
, m_dbgInsideConstructor(true)
, m_dbgInsideDestructor(false)
#endif
{ 
  // read-lock source while being copied
  Handle<T,NullType> * prhs = const_cast<Handle<T,NullType>*>(&rhs);
  prhs->beginRead(); 
  m_holder = _new_ Holder<T>(*rhs.m_holder, (UINT_PTR)this);
  prhs->endRead(); 
#if !defined(NDEBUG)
  m_dbgInsideConstructor = false;
#endif
}

template <typename T>
inline Handle<T, NullType>::~Handle() 
{ 
#if !defined(NDEBUG)
  NVSG_ASSERT(m_dbgWriteCount==0); // somebody still writing the object ->undefined behavior
  NVSG_ASSERT(m_dbgReadCount==0); // somebody still reading the object ->undefined behavior
  m_dbgInsideDestructor = true;
#endif
  delete m_holder; 
}

template <typename T>
inline void Handle<T, NullType>::default_init()
{
  NVSG_ASSERT(m_dbgInsideConstructor);
  m_pfnVirtualClone = &Handle<T, NullType>::clone_impl;
  m_holder = _new_ Holder<T>((UINT_PTR)this);
#if !defined(NDEBUG)
  m_dbgInsideConstructor = false;
#endif
}

template <typename T>
inline Handle<T, NullType> * Handle<T, NullType>::clone() const 
{ // pseudo-virtual call via member function pointer solves problems with 
  // the compiler's instantiation policy for virtual functions 
  return (this->*m_pfnVirtualClone)();
} 

template <typename T>
inline Handle<T, NullType> * Handle<T, NullType>::clone_impl() const 
{
  return _new_ Handle<T, NullType>(*this);
}

template <typename T>
inline T * Handle<T, NullType>::beginEdit()       
{
  // the following waits until exclusive access was given
  m_holder->lock.lockExclusive(); 
#if !defined(NDEBUG)
  m_dbgWriteCount++;
#endif
  return m_holder->ptr; 
}

template <typename T>
inline void Handle<T, NullType>::endEdit()       
{
  // release exclusive access
#if !defined(NDEBUG)
  // catch endEdit without prior beginEdit
  NVSG_ASSERT(m_dbgWriteCount>0); 
  m_dbgWriteCount--;
#endif
  m_holder->lock.unlockExclusive();
}

template <typename T>
inline const T * Handle<T, NullType>::beginRead()       
{ 
  // the following waits until shared access was given
  m_holder->lock.lockShared(); 
#if !defined(NDEBUG)
  m_dbgReadCount++; 
#endif
  return m_holder->ptr; 
}

template <typename T>
inline void Handle<T, NullType>::endRead()       
{
  // release shared access
#if !defined(NDEBUG)
  // catch endRead without prior beginRead
  NVSG_ASSERT(m_dbgReadCount>0); 
  m_dbgReadCount--;
#endif
  m_holder->lock.unlockShared();
}

template <typename T>
inline const Holder<T> * Handle<T, NullType>::holder() const
{
  return m_holder;
}

#if !defined(NDEBUG)
template <typename T>
inline void Handle<T, NullType>::assertWriteLocked() const 
{ 
  // NOTE: Actually, this needs additional thread info.
  // That is - this is not 100% correct, for example,
  // when the object is write-locked from thread A and
  // a write access without locking is performed from 
  // thread B!
  NVSG_ASSERT(  m_dbgInsideConstructor 
             || m_dbgInsideDestructor 
             || m_dbgWriteCount>0 ); 
}
template <typename T>
inline void Handle<T, NullType>::assertReadLocked() const 
{ 
  // NOTE: Actually, this needs additional thread info!
  NVSG_ASSERT(  m_dbgInsideConstructor 
             || m_dbgInsideDestructor 
             || m_dbgReadCount>0 
             || m_dbgWriteCount>0 ); 
}
#endif

template <typename T, typename U, typename V>
inline Handle<T, Handle<U,V> > * Handle<T, Handle<U,V> >::create() 
{ 
  Handle<T,Handle<U,V> > * hdl = new Handle<T,Handle<U,V> >();
  hdl->default_init();
  return hdl;
}

template <typename T, typename U, typename V>
inline Handle<T, Handle<U,V> > * Handle<T, Handle<U,V> >::createFromBase(const Handle<U,V>& base)
{
  return new Handle<T,Handle<U,V> >(base);
}

#if !defined(NDEBUG)
template <typename T, typename U, typename V>
inline Handle<T, Handle<U,V> > * Handle<T, Handle<U,V> >::create(const char* file, unsigned int line) 
{ 
  Handle<T,Handle<U,V> > * hdl = new(file, line) Handle<T,Handle<U,V> >();
  hdl->default_init();
  return hdl;
}

template <typename T, typename U, typename V>
inline Handle<T, Handle<U,V> > * Handle<T, Handle<U,V> >::createFromBase(const char* file, unsigned int line, const Handle<U,V>& base)
{
  return new(file, line) Handle<T,Handle<U,V> >(base);
}
#endif

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> >::Handle() 
{ 
  NVSG_ASSERT((Handle<U,V>::m_dbgInsideConstructor));
  /* do nothing ! */ 
}

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> >::Handle(const Handle<T,Handle<U,V> >& rhs) 
// note: must not call base classes copy constructor here!!
{ 
  NVSG_ASSERT((Handle<U,V>::m_dbgInsideConstructor));
  // override function pointer! 
  *((PFNVIRTUALCLONE*)&this->m_pfnVirtualClone) = &Handle<T,Handle<U,V> >::clone_impl;
  // read-lock source while being copied
  Handle<T,Handle<U,V> > * prhs = const_cast<Handle<T,Handle<U,V> >*>(&rhs);
  prhs->beginRead();
  *((Holder<T>**)&this->m_holder) = _new_ Holder<T>(**((Holder<T>* const *)&rhs.m_holder), (UINT_PTR)this); 
  prhs->endRead();
#if !defined(NDEBUG)
  Handle<U,V>::m_dbgInsideConstructor = false;
#endif
} 

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> >::Handle(const Handle<U,V>& base) 
// note: must not call base classes copy constructor here!!
{ 
  NVSG_ASSERT((Handle<U,V>::m_dbgInsideConstructor));
  // override function pointer! 
  *((PFNVIRTUALCLONE*)&this->m_pfnVirtualClone) = &Handle<T,Handle<U,V> >::clone_impl;
  *((Holder<T>**)&this->m_holder) = _new_ Holder<T>(*((const Holder<U>*)base.holder()), (UINT_PTR)this); 
#if !defined(NDEBUG)
  Handle<U,V>::m_dbgInsideConstructor = false;
#endif
} 

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> >::~Handle() 
{ 
  // cleanup will be managed by uppermost base class!
}

template <typename T, typename U, typename V>
inline void Handle<T,Handle<U,V> >::default_init()
{
  NVSG_ASSERT((Handle<U,V>::m_dbgInsideConstructor));
  // override function pointer! 
  *((PFNVIRTUALCLONE*)&this->m_pfnVirtualClone) = &Handle<T,Handle<U,V> >::clone_impl;
  *((Holder<T>**)&this->m_holder) = _new_ Holder<T>((UINT_PTR)this);
#if !defined(NDEBUG)
  Handle<U,V>::m_dbgInsideConstructor = false;
#endif
}

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> > * Handle<T,Handle<U,V> >::clone() const 
{ // pseudo-virtual call via member function pointer solves problems with 
  // the compiler's instantiation policy for virtual functions   
  return (Handle<T,Handle<U,V> >*)(this->*Handle::m_pfnVirtualClone)();
} 

template <typename T, typename U, typename V>
inline Handle<T,Handle<U,V> > * Handle<T,Handle<U,V> >::clone_impl() const 
{ 
  return _new_ Handle<T,Handle<U,V> >(*this);
} 

template <typename T, typename U, typename V>
inline T * Handle<T,Handle<U,V> >::beginEdit()       
{ 
  // the following waits until exclusive access was given
  this->m_holder->lock.lockExclusive(); 
#if !defined(NDEBUG)
  Handle<U,V>::m_dbgWriteCount++;
#endif
  return ((Holder<T>*)this->m_holder)->ptr; 
}

template <typename T, typename U, typename V>
inline const T * Handle<T,Handle<U,V> >::beginRead() 
{ 
  // the following waits until exclusive access was given
  this->m_holder->lock.lockShared(); 
#if !defined(NDEBUG)
  Handle<U,V>::m_dbgReadCount++;
#endif
  return ((Holder<T>*)this->m_holder)->ptr; 
}

//
// implementation of convenient helper functions and structs
//

// returns the object's corresponding handle
template <typename T>
inline T * getHandle(const HandledObject * obj)
{
  NVSG_ASSERT(HandledObject::SIGNATURE==obj->m_signature);
  return (T*)obj->m_handle;
}

template<typename T>
class WritableObject
{
  typedef T* pointer;
  typedef T& reference;
  typedef Handle<EmptyType, NullType> DummyHandle;
public:
  template <typename U> 
  explicit WritableObject(U * h) {
    NVSG_ASSERT(h); obj=h->beginEdit();
  }
  ~WritableObject() { 
    DummyHandle * h = getHandle<DummyHandle>(obj);
    h->endEdit(); 
  }
  // make it behave like a pointer
  pointer operator->() const { return obj; }  
  reference operator*() const { return *obj; }  
  // implicit conversion to T*
  operator pointer() const { return obj; }
  // assigning an object of type T
  WritableObject& operator=(const T& rhs) { *obj = rhs; return *this; }
private:
  WritableObject(const WritableObject&); // copying not permitted!
  WritableObject& operator=(const WritableObject&); // assigning a WritableObject not permitted!
private:
  pointer obj;
};

template<typename T>
class ReadableObject
{
  typedef const T* pointer;
  typedef const T& reference;
  typedef Handle<EmptyType, NullType> DummyHandle;
public:  
  template <typename U> 
  explicit ReadableObject(U * h) {
    NVSG_ASSERT(h); obj=h->beginRead();
  }
  ~ReadableObject() { 
    DummyHandle * h = getHandle<DummyHandle>(obj);
    h->endRead();
  }
  // make it behave like a pointer
  pointer operator->() const { return obj; }  
  reference operator*() const { return *obj; }  
  // implicit conversion to const T*
  operator pointer() const { return obj; }
private:
  ReadableObject(const ReadableObject&); // copying not permitted   
  ReadableObject& operator=(const ReadableObject&); // assignment not permitted   
private:
  pointer obj;
};

struct CloneObject
{
  template<typename T>
  T* operator() (const T* src) 
  {
    T* clone = src->clone();
    clone->addRef();
    return clone;
  }
};

#undef _new_
} // namespace nvutil


