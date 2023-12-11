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

// rcobject.h
//
// Support for reference counted object
//
#pragma once
/** \file */

#include "nvsgcommon.h"

#include "nvutil/Allocator.h" // for IAllocator interface
#include "nvutil/SWMRSync.h"

// memory leak detection
#if !defined(NDEBUG)
# ifndef new
#  define new new(__FILE__, __LINE__)
#  define _DEFINED_DBGNEW // restrict the 'new' macro to this header file only
# endif
#endif

namespace nvutil
{
  class RCObject;

#if !defined(DOXYGEN_IGNORE)
  // RefCount abstracts a reference counter and provides mutual exclusive access to the internal counter.
  template <typename T>
  class RefCount
  {
  public:
    // initializes the internal counter with the value of the passed 'initialValue' parameter. 
    // initializes the internal counter with 0, if the parameter is omitted.
    // the mutex object will always be default constructed (see SWMRSync::SWMRSync)
    RefCount(T initialValue = 0) : m_cnt(initialValue) {}

    // increments (increases by one) the internal counter.
    // this operator prevents more than one thread from accessing the internal counter simultaneously.
    T operator++() 
    {
      m_mutex.lockExclusive();
      T res = ++m_cnt;
      m_mutex.unlockExclusive();
      return res;
    }

    // decrements (decreases by one) the internal counter.
    // this operator prevents more than one thread from accessing the internal counter simultaneously.
    T operator--()
    {
      m_mutex.lockExclusive();
      NVSG_ASSERT(m_cnt>0); // must never turn negative
      T res = --m_cnt;
      m_mutex.unlockExclusive();
      return res;
    }

    // 'greater than' comparison of the internal counter.
    // this operator prevents more than one thread from accessing the internal counter simultaneously.
    bool operator>(const T& rhs) const
    {
      m_mutex.lockExclusive();
      bool res = m_cnt > rhs;
      m_mutex.unlockExclusive();
      return res;
    }

    // hide the built-in assignment operator!
    // the built-in assignment operator would assign the mutex, which is not allowed!
    // this operator prevents more than one thread from accessing the internal counter simultaneously.
    RefCount& operator=(const RefCount &rhs)
    {
      if ( this != &rhs )
      {
        // never copy a mutex! keep the original one instead!
        m_mutex.lockExclusive();
        m_cnt = rhs.m_cnt;
        m_mutex.unlockExclusive();
      }
      return *this;
    }

  private:
    SWMRSync m_mutex; // to realize mutual exclusive access to m_cnt
    T m_cnt; // holds the counter value 
  };
#endif

  /*! \brief Type definition for the callback on destruction.
   *  \par Namespace: nvsg
   *  \remarks An RCObject can have callbacks to be called, just before it is destroyed. This is
   *  the definition of the interface of these callbacks. The complete \a rcObject can be read at
   *  that moment, but not written.
   *  \note The behavior is undefined, if \a rcObject is written to.
   *  \sa RCObject::addOnDestroyCallback, RCObject::removeOnDestroyCallback */
  typedef void DestroyCallback( const RCObject * rcObject, void * data );

  /*! \brief Provides an interface for reference counted objects. 
    * \par Namespace: nvutil
    * \remarks
    * The class provides an interface for managing reference counted objects. RCObject inherits
    * from IAllocator to utilize an optimized heap manager for small object allocation.
    * \n\n
    * The creator of an RCObject is responsible for setting the object's reference count to its
    * proper value. For automatic reference counting, though, an RCObject can be managed by a RCPtr. 
    * \note An RCObject can only be constructed on heap. The compiler will reject any
    * attempt to construct an RCObject on stack.
    * \note It is prohibited to explicitly delete an RCObject by calling \c delete on a pointer
    * to an RCObject received from a previous call to \c new. The compiler will complain if any 
    * client code attempts to do so. If client code creates an RCObject, it must increment the 
    * object's reference count (RCObject::addRef) before using it and decrement the object's reference 
    * count (RCObject::removeRef) after usage. The object will be automatically deleted if its reference 
    * count reaches zero.
    * \note The reference count of a newly created RCObject is initially zero.
    * \sa nvutil::RCPtr
    */
  class RCObject : public IAllocator
  {
    public:
      /*! \brief Increments the object's reference count.
        * \remarks
        * For user code to ensure that the data of a reference counted object is valid as long as it
        * uses the object, it should first increment the objects reference count. After usage, 
        * client code should decrement the objects reference count to avoid resource leaks.
        *
        * \note The reference count of a newly created RCObject initially is zero.
        * \sa RCObject::removeRef
        */
      NVSG_API void addRef()            const; 

      /*! \brief Decrements the object's reference count. 
        * \remarks
        * It is prohibited to explicitly delete an RCObject. 
        * The object will be automatically deleted, if the reference count changes from one to zero.
        * \sa RCObject::addRef
        */
      NVSG_API void removeRef()         const;  

      /*! \brief Marks the object as not shareable.
        * \remarks
        * The function marks the object as not shareable. The shareable flag of an RCObject will be 
        * evaluated if a managing RCPtr (reference counter pointer) will be copied. If the managed 
        * RCObject is flagged as shareable (default), only the reference count of the RCObject will
        * be incremented, indicating that two or more RCPtr objects share the same RCObject. The
        * managed RCObject will be copied not before the first write access (lazy copying). \n
        * If instead the RCObject is flagged as not shareable, the managed RCObject will be copied 
        * as the managing RCPtr object will be copied (immediate copying).
        * \sa RCObject::isShareable
        */
      NVSG_API void markUnshareable();
      
      /*! \brief Tests whether this particular object is flagged shareable.
        * \return
        * \c true if shareable, \c false otherwise.
        * \remarks
        * Marking an RCObject as shareable enables lazy copying of the RCObject if managed by a
        * RCPtr (reference counted pointer) object. That is, a RCPtr object can share its 
        * embedded RCObject with other RCPtr objects as long as no write access happens.
        * \sa RCObject::markUnshareable
        */
      NVSG_API bool isShareable()       const; 

      /*! \brief Tests whether this RCobject is shared.
        * \return
        * \c true if the internal reference count is greater than one, \c false otherwise.
        * \remarks
        * \sa RCObject::addRef, RCObject::removeRef
        */
      NVSG_API bool isShared()          const; 

      /*! \brief Add a callback to be called on destruction.
       *  \param dcb A pointer to the DestroyCallback to add.
       *  \param data A pointer to be used as the second argument of the DestroyCallback.
       *  \note The behavior is undefined, if the same combination of DestroyCallback and void *
       *  is added more than once.
       *  \par Example
       *  \code
       *    void myDestroyCallback( const RCObject * rcObject, void * data )
       *    {
       *      printf("Destroyed... wheeeeee\n");
       *    }
       *    [...]
       *
       *    RCObject_AutoPtr< SomeRCdObject > o( new SomeRCdObject );
       *    o->addOnDestroyCallback( myDestroyCallback, NULL );
       *  \endcode
       *  \sa DestroyCallback, removeOnDestroyCallback */
      NVSG_API void addOnDestroyCallback( DestroyCallback *dcb, void * data );

      /*! \brief Remove a callback from the list of callbacks called on destruction.
       *  \param dcb A pointer to the DestroyCallback to remove.
       *  \param data The pointer that is used as the data pointer in the corresponding
       *  addOnDestroyCallback.
       *  \note The behavior is undefined if the combination of \a dcb and \a data is not
       *  registered as a callback on destruction via addOnDestroyCallback.
       *  \par Example
       *  \code
       *    void myDestroyCallback( const RCObject * rcObject, void * data )
       *    {
       *      printf("Destroyed... wheeeeee\n");
       *    }
       *    [...]
       *
       *    RCObject * o = someObject;
       *    o->removeOnDestroyCallback( myDestroyCallback, NULL );
       *  \endcode
       *  \sa DestroyCallback, addOnDestroyCallback */
      NVSG_API void removeOnDestroyCallback( DestroyCallback * dcb, void * data );

    protected:
      
      /*! \brief Constructs an RCObject.
        * \remarks
        * Initializes the reference count to zero. The creator is responsible for setting the
        * reference count to its proper value.
        * \n\n
        * Initially the RCObject is marked as shareable.
        * \n\n
        * An RCObject is intended to serve as base class only. That is, an RCObject can be 
        * instantiated only as an object of a class derived from RCObject.
        */
      RCObject() : m_shareable(true) {};

      /*! \brief Constructs an RCObject.
        * \remarks
        * A new RCObject is created, and hence, the reference count will be initialized to zero 
        * because no one but the creator is currently referencing this object, and the
        * creator is responsible for setting the reference count to its proper value.
        * \n\n
        * Initially the RCObject is marked as shareable.
        * \n\n
        * An RCObject is intended to serve as base class only. That is, an RCObject can be 
        * instantiated only as an object of a class derived from RCObject.
        */
      RCObject(const RCObject& rhs) : m_shareable(true) {}

      /*! \brief Assignment operator.
       * \remarks
       * Assigning new content from another object does not change the number of users referencing
       * the object nor does it change its shareable state. Hence, this assignment operator leaves
       * the reference count and the shareable state unchanged.
       */
      RCObject& operator=(const RCObject& rhs) { return *this; }

      /*! \brief Destructs an RCObject.
        * \remarks
        * An RCObject will be automatically deleted when its reference count changes from one to 
        * zero.
        * \sa RCObject::removeRef
        */
      virtual ~RCObject() {}

    private:
      
      /** m_refcnt must be mutable, so we can also call addRef() / removeRef() from const pointers. */
      mutable RefCount<int>                             m_refcnt;
      bool                                              m_shareable;
      std::vector<std::pair<DestroyCallback *,void *> > m_destroyCallbacks;
  };

  /*! \brief A reference counted vector.
    * \par Namespace : nvutil
    * \remarks
    * A RCVector is simply a STL vector providing the RCObject interface. A RCVector, once 
    * constructed using the limited set of available constructors, can be used same as a STL vector. 
    */
  template<typename T>
  class RCVector : public std::vector<T>, public RCObject
  {
    public:
      /*! \brief Constructs an initially empty RCVector
       */
      RCVector() {}
      /*! \brief Constructs a RCVector of the specified size.
        * \param
        * n Number of elements in the constructed vector.
        * \remarks
        * The constructor specifies a repetition of the specified number \a n of elements of the 
        * default value for type T.
        * \sa std::vector
        */
      explicit RCVector(size_t n) : std::vector<T>(n) {}
  };

  /*! \brief A smart pointer template to manage reference counted objects.
    * \par Namespave: nvutil
    * \remarks
    * Offers automatic reference counting through utilizing the RCObject interface. 
    * Like a smart pointer this can be used as an ordinary built-in pointer.
    * 
    * \note The type T must be convertible to RCObject. The compiler will reject any attempt to 
    * instantiate a RCPtr object for types other than RCObject convertible types.
    */
  template <typename T>
  class RCPtr
  {
    /*! \brief Initiates write access to the managed object.
      * \param
      * rcPtr RCPtr object for which to initiate write access.
      * \remarks
      * This must be called before modifying the managed data of the specified RCPtr object.
      * \n\n
      * This has been introduced to manually decide whether the underlying data really gets modified 
      * by a non-const access. This helps to avoid cloning every shared object when a non-const access 
      * occurs but the underlying data remains untouched.
      * \n\n
      * With the introduction of writeAccess, full automatic seceding of a shared object with a 
      * non-const access has been removed. To nevertheless maintain correctness with shared data, 
      * you must explicitly initiate every modifying code with a call to writeAccess on the 
      * corresponding RCPtr object. 
      *
      * \note As U is used as a template parameter for RCPtr, U must be
      * convertible to RCObject.
      */
    template <typename U>
    friend void writeAccess(RCPtr<U> &rcPtr);
   
    public: 
      /*! \brief Constructs a RCPtr object.
        * \param
        * pRCObj Optional pointer to an RCObject.
        * \remarks
        * The embedded dump pointer will be assigned to \a pRCObj.
        */
      explicit RCPtr(T* pRCObj=0);
      
      /*! \brief Copy constructor.
        */
      RCPtr(const RCPtr& rhs);
      
      /*! \brief Destructs a RCPtr 
      */
      ~RCPtr();                                 
      
      /*! \brief Assigns new content.
        * \param
        * rhs RCPtr from which to assign the new content.
        * \return
        * The function returns a reference to this object.
        * \remarks
        * If the embedded dump pointer points to some valid RCObject, the reference count of that 
        * RCObject will be decremented before re-assigning the dump pointer with the dump pointer
        * from the right hand sided RCPtr.
        */
      RCPtr<T>& operator=(const RCPtr<T>& rhs);
      
      /*! \brief Dereferences the RCPtr.
        * \return
        * A pointer to the managed RCObject.
        * \remarks
        * This operator helps a RCPtr object to behave like an ordinary pointer. It will be called 
        * with dereferencing a non-const RCPtr object to access the managed RCObject. 
        */
      T* operator->();

      /*! \brief Dereferences the RCPtr.
        * \return
        * A pointer to the managed RCObject.
        * \remarks
        * This operator helps a RCPtr object to behave like an ordinary pointer. It will be called 
        * with dereferencing a const RCPtr object to access the managed RCObject. 
        */
      const T* operator->() const;

      /*! \brief Dereferences the RCPtr.
        * \return
        * A reference to the managed RCObject.
        * \remarks
        * This operator helps a RCPtr object to behave like an ordinary pointer. It be called with 
        * dereferencing a non-const RCPtr object to access the managed RCObject. 
        */
      T& operator*();                 

      /*! \brief Dereferences the RCPtr.
        * \return
        * A reference to the managed RCObject.
        * \remarks
        * This operator helps a RCPtr object to behave like an ordinary pointer. It be called with 
        * dereferencing a const RCPtr object to access the managed RCObject. 
        */
      const T& operator*() const;

    private:
      void init();                              // initialize a RCPtr<T> object
      void clone();                             // deep-copy the embedded object

      // the dump pointer
      T * m_ptr;
  };

  /*! \brief A smart pointer template for reference counted objects.
    * \par Namespace: nvutil
    * \remarks
    * We have no access to object sources, so we need an additional level of indirection
    * because we can't make them publicly inherit from RCObject. The 'I' stands for 'indirect'.
    */
  template <typename T>
  class RCIPtr
  {
    /*! \brief Initiates write access to the managed object.
      * \param
      * rcPtr RCIPtr object for which to initiate write access.
      * \remarks
      * This must be called before modifying the managed data of the specified RCIPtr object.
      * \n\n
      * This has been introduced to manually decide whether the underlying data really gets modified 
      * by a non-const access. This helps to avoid cloning every shared object when a non-const access 
      * occurs but the underlying data remains untouched.
      * \n\n
      * With the introduction of writeAccess, full automatic seceding of a shared object with a 
      * non-const access has been removed. To nevertheless maintain correctness with shared data, 
      * you must explicitly initiate every modifying code with a call to writeAccess on the 
      * corresponding RCIPtr object. 
      *
      * \note As U is used as a template parameter for RCIPtr, U must be
      * convertible to RCObject.
      */
    template <typename U>
    friend void writeAccess(RCIPtr<U> &rcPtr);
   
    public: 
      /*! \brief Constructs a RCIPtr object.
        * \param
        * pRCObj Optional pointer to an RCObject.
        * \remarks
        * The embedded dump pointer will be assigned to \a pRCObj.
        */
      explicit RCIPtr(T* pRCObj=0);
      
      /*! \brief Copy constructor.
        */
      RCIPtr(const RCIPtr& rhs);
      
      /*! \brief Destructs a RCIPtr 
      */
      ~RCIPtr();                                 
      
      /*! \brief Assigns new content.
        * \param
        * rhs RCIPtr from which to assign the new content.
        * \return
        * The function returns a reference to this object.
        * \remarks
        * If the embedded dump pointer points to some valid RCObject, the reference count of that 
        * RCObject will be decremented before re-assigning the dump pointer with the dump pointer
        * from the right hand sided RCIPtr.
        */
      RCIPtr<T>& operator=(const RCIPtr<T>& rhs);
      
      /*! \brief Dereferences the RCIPtr.
        * \return
        * A pointer to the managed RCObject.
        * \remarks
        * This operator helps a RCIPtr object to behave like an ordinary pointer. It is called 
        * with dereferencing a non-const RCIPtr object to access the managed RCObject. 
        */
      T* operator->();

      /*! \brief Dereferences the RCIPtr.
        * \return
        * A pointer to the managed RCObject.
        * \remarks
        * This operator helps a RCIPtr object to behave like an ordinary pointer. It is called 
        * with dereferencing a const RCIPtr object to access the managed RCObject. 
        */
      const T* operator->() const;

      /*! \brief Dereferences the RCIPtr.
        * \return
        * A reference to the managed RCObject.
        * \remarks
        * This operator helps a RCIPtr object to behave like an ordinary pointer. It is called with 
        * dereferencing a non-const RCIPtr object to access the managed RCObject. 
        */
      T& operator*();                 

      /*! \brief Dereferences the RCIPtr.
        * \return
        * A reference to the managed RCObject.
        * \remarks
        * This operator helps a RCIPtr object to behave like an ordinary pointer. It is called with  
        * dereferencing a const RCIPtr object to access the managed RCObject. 
        */
      const T& operator*() const;

    private:
      // CountHolder acts as an additional indirection and holds
      // a dump pointer to the object being reference counted
      struct CountHolder : public RCObject
      {
        // destructor only gets called when the objects 
        // reference count reaches zero
        ~CountHolder() { delete m_ptr; }
        T * m_ptr;
      };

      CountHolder * m_cntHolder;

      void init();
      void clone(); // copy on write
  };

  /*! \brief AutoPtr for reference counted objects.
    * \par Namespace: nvutil
    * An RCObject_AutoPtr turns a pointer to an RCObject into an automatic object that will be 
    * automatically destroyed when running out of scope. As all auto objects, an RCObject_AutoPtr
    * will be correctly cleaned up when it comes to a runtime exception, and hence, helps to use
    * RCObjects in a more exception-safe manner.
    */
  template <typename T>
  class RCObject_AutoPtr
  {
    template<typename U> 
    friend class RCObject_AutoPtr;  

    public:
      /*! \brief Constructs an RCObject_AutoPtr
        * \param
        * ptr Optional pointer to an RCObject to be managed by the RCObject
        * \remarks
        * The dump pointer will be initialized with \a ptr.
        */
      explicit RCObject_AutoPtr(T * ptr=0);

      /*! \brief Copy constructor.
        */
      template <typename U>
      RCObject_AutoPtr(const RCObject_AutoPtr<U>& rhs);
      
      /*! \brief Destructs an RCObject_AutoPtr
       * \remarks
       * If the RCObject the dump pointer points to was not released before this destructor is 
       * called, the RCObject will be properly freed, if no other user is referencing the RCObject.
       * \sa RCObject_AutoPtr::release
       */
      ~RCObject_AutoPtr();

      /*! \brief Assigns new content.
        * \remarks
        * If the dump pointer points to a valid RCObject, that RCObject will be properly freed 
        * before the dump pointer will be re-assigned to the dump pointer from the right-hand-sided
        * RCObject_AutoPtr.
        */
      template <typename U>
      RCObject_AutoPtr<T>& operator=(const RCObject_AutoPtr<U>& rhs); 

      /*! \brief Returns the dump pointer.
        * \return
        * A pointer to the managed RCObject.
        * \sa RCObject_AutoPtr::operator->()
        */
      T * get() const;

      /*! \brief Dereferences an RCObject_AutoPtr.
        * \return
        * A pointer to the managed RCObject.
        * \remarks
        * This operator helps an RCObject_AutoPtr object to behave like an ordinary pointer. 
        * \sa RCObject_AutoPtr::operator*()
        */
      T * operator->() const;     //!<  Member selection operator to make this behave like a dump pointer.

      /*! \brief Dereferences an RCObject_AutoPtr.
        * \return
        * A reference to the managed RCObject.
        * \remarks
        * This operator helps an RCObject_AutoPtr object to behave like an ordinary pointer. 
        * \sa RCObject_AutoPtr::operator->()
        */
      T&  operator*() const;

      /*! \brief Releases the managed RCObject.
        * \return
        * A pointer to the previously managed RCObject.
        * \remarks
        * The function transfers ownership of the previously managed RCObject to the caller. The
        * caller then is responsible for setting the reference count for the retrieved RCObject
        * to its proper value.
        */
      T * release(); 

      /*! \brief Resets the dump pointer to the managed RCObject.
        * \param
        * ptr Pointer to reset the dump pointer to.
        * \remarks
        * A previously managed RCObject will be properly freed before the function resets the
        * dump pointer to the new value.
        */
      void reset(T * ptr);

    private:
      void deletePointer();
      T * m_ptr;
  };

  /*! \brief Provides reference counting for pointers
    * \par Namespace: nvutil
    * \remarks
    * This class is a pointer object which lets you reference count ordinary pointers. 
    * In situations where a user needs to keep track of how often a certain pointer is referenced 
    * by some owning object, this class provides a simple interface for manipulating the reference 
    * count associated with a certain pointer.
    */
  template<typename T>
  class PointerReference
  {
    public:
      /*! \brief Constructs a PointerReference
        * \param
        * ptr Pointer to reference count.
        * \remarks
        * The dump pointer will be initialized with the pointer value passed through \a ptr.
        * The reference counter for the managed pointer will be initialized to 1.
        * \note 
        * A PointerReference must not be initialized with an invalid pointer. Doing so will certainly 
        * cause the dereference operators to crash.
        */
      PointerReference(T * ptr) : m_ptr(ptr), m_refcnt(1) {}
        
      /*! \brief Increments the reference count for the managed pointer by 1. */
      void addRef() { ++m_refcnt; }
     
      /*! \brief Decrements the reference count for the managed pointer by 1. */
      void removeRef() { --m_refcnt; }

      /*! \brief Tests whether the managed pointer is referenced more than once. 
       * \return
       * The function returns \c true if the managed pointer is referenced more than once. 
       * Otherwise the function returns \c false.
       */
      bool isShared() const { return m_refcnt>1; }

      /*! \brief Provides implicit conversion to a pointer to type T.
        * \returns
        * The function returns the managed pointer.
        * \remarks
        * Wherever a pointer to type T is expected, a PointerReferenc<T> object can be passed.
        * This operator implicitly converts this PointerReferenc<T> object to a pointer to type T.
        */
      operator T*() const { return m_ptr; }

      /*! \brief Dereferences a PointerReference object.
        * \return
        * The function returns the managed pointer.
        * \remarks
        * This operator helps a PointerReference object to behave like an ordinary pointer. 
        * \sa PointerReference::operator*()
        */
      T* operator->() const { return m_ptr; }

      /*! \brief Dereferences a PointerReference object.
        * \return
        * The function returns a reference to the object pointed to by the managed pointer.
        * \remarks
        * This operator helps a PointerReference object to behave like an ordinary pointer. 
        * \sa PointerReference::operator->()
        */
      T& operator*() const { return *m_ptr; }

      /*! \brief Compares the dump pointer with another pointer.
       * \return
       * The function returns \c true if the pointer values are equal.
       * Otherwise, the function returns \c false.
       */
      bool operator==(T* ptr) const { return ptr==m_ptr; }

      /*! \brief Compares the dump pointer with another pointer.
       * \return
       * The function returns \c true if the pointer values are different.
       * Otherwise, the function returns \c false.
       */
      bool operator!=(T* ptr) const { return !this->operator==(ptr); }

    private:
      T * m_ptr;
      RefCount<int> m_refcnt;
  };

  //----------------------------------------------------------------------------
  // inline implementations following
  //----------------------------------------------------------------------------

  inline void RCObject::addRef() const 
  { 
    ++m_refcnt; 
  } 

  inline void RCObject::removeRef() const
  { 
    if ( !--m_refcnt ) 
    {
      for ( size_t i=0 ; i<m_destroyCallbacks.size() ; i++ )
      {
        (*m_destroyCallbacks[i].first)( this, m_destroyCallbacks[i].second );
      }
      delete this; 
    } 
  }

  inline void RCObject::markUnshareable() 
  { 
    m_shareable=false; 
  }

  inline bool RCObject::isShareable() const 
  { 
    return m_shareable==true; 
  }

  inline bool RCObject::isShared() const 
  { 
    return m_refcnt>1;
  }

  inline void RCObject::addOnDestroyCallback( DestroyCallback * dcb, void * data )
  {
    NVSG_ASSERT( std::find( m_destroyCallbacks.begin(), m_destroyCallbacks.end()
                     , std::make_pair( dcb, data ) )
               == m_destroyCallbacks.end() );
    m_destroyCallbacks.push_back( std::make_pair( dcb, data ) );
  }

  inline void RCObject::removeOnDestroyCallback( DestroyCallback * dcb, void * data )
  {
    std::vector<std::pair<DestroyCallback*,void*> >::iterator it;
    it = std::find( m_destroyCallbacks.begin(), m_destroyCallbacks.end(), std::make_pair( dcb, data ) );
    NVSG_ASSERT( it != m_destroyCallbacks.end() );
    m_destroyCallbacks.erase( it );
  }

  //>>
  template <typename T>
  inline RCPtr<T>::RCPtr(T* pRCObj) 
  : m_ptr(pRCObj) 
  { 
    init(); 
  }

  template <typename T>
  inline RCPtr<T>::RCPtr(const RCPtr& rhs) 
  : m_ptr(rhs.m_ptr) 
  { 
    init(); 
  }

  template <typename T>
  inline RCPtr<T>::~RCPtr() 
  { 
    if ( m_ptr ) 
    { 
      m_ptr->removeRef(); 
    } 
  }

  template <typename T>
  inline RCPtr<T>& RCPtr<T>::operator=(const RCPtr<T>& rhs) 
  {
    if ( m_ptr != rhs.m_ptr )
    {
      // remove reference to former value first
      m_ptr->removeRef();
      m_ptr = rhs.m_ptr;
      init();
    }
    return *this;
  }
    
  template <typename T>
  inline T* RCPtr<T>::operator->() 
  { 
    return m_ptr; 
  }

  template <typename T>
  inline const T* RCPtr<T>::operator->() const 
  { 
    return m_ptr; 
  }

  template <typename T>
  inline T& RCPtr<T>::operator*() 
  { 
    return *m_ptr; 
  }

  template <typename T>
  inline const T& RCPtr<T>::operator*() const 
  { 
    return *m_ptr; 
  }

  template <typename T>
  inline void RCPtr<T>::init()
  {
    if ( !m_ptr ) return;
    
    if ( !m_ptr->isShareable() )
    {
      // deep copy required!
      m_ptr = new T(*m_ptr);
    }
    m_ptr->addRef();
  }

  template <typename T>
  inline void RCPtr<T>::clone()
  {
    if ( m_ptr->isShared() )
    { 
      // need to wedge off an own copy if original data is shared 
      T * pOldVal = m_ptr; // remember the old object pointer for deep copying below
      m_ptr->removeRef(); // unreference the object before beeing new assigned
      m_ptr = new T(*pOldVal); // deep copy -> T(const T&) required!
      m_ptr->addRef();
    }
  }
  //>>
  template <typename T>
  inline RCIPtr<T>::RCIPtr(T * pObj)
  : m_cntHolder(new CountHolder)
  {
    m_cntHolder->m_ptr = pObj;
    init();
  }

  template <typename T>
  inline RCIPtr<T>::RCIPtr(const RCIPtr<T>& rhs)
  : m_cntHolder(rhs.m_cntHolder)
  {
    init();
  }

  template <typename T>
  inline RCIPtr<T>::~RCIPtr()
  {
    m_cntHolder->removeRef();
  }

  template <typename T>
  inline RCIPtr<T>& RCIPtr<T>::operator=(const RCIPtr<T>& rhs)
  {
    if ( m_cntHolder != rhs.m_cntHolder )
    {
      m_cntHolder->removeRef(); // unreference the object before beeing new assigned
      m_cntHolder = rhs.m_cntHolder;
      init();
    }
    return *this;
  }

  template <typename T>
  inline const T* RCIPtr<T>::operator->() const
  {
    // const access, no need for copy on write here
    return m_cntHolder->m_ptr;
  }

  template <typename T>
  inline T* RCIPtr<T>::operator->() 
  {
    // perform copy on write with every non-const access to the pointed-to object
    // clone();
    return m_cntHolder->m_ptr;
  }

  template <typename T>
  inline const T& RCIPtr<T>::operator*() const
  {
    // const access, no need for copy on write here
    return *(m_cntHolder->m_ptr);
  }

  template <typename T>
  inline T& RCIPtr<T>::operator*() 
  {
    // perform copy on write with every non-const access to the pointed-to object
    // clone();
    return *(m_cntHolder->m_ptr);
  }

  template <typename T>
  inline void RCIPtr<T>::init()
  {
    if ( !m_cntHolder->isShareable() )
    {
      // non-shareables must not be shared (refcnt==1)
      _ASSERTE(!m_cntHolder->isShared());
      // must not unreference unshared objects because this 
      // would delete the pointed-to object immediately! 
      T * pOldVal = m_cntHolder->m_ptr;
      m_cntHolder = new CountHolder;
      m_cntHolder->m_ptr = new T(*pOldVal);
    }
    m_cntHolder->addRef();
  }

  template <typename T>
  inline void RCIPtr<T>::clone()
  {
    if ( m_cntHolder->isShared() )
    {
      T * pOldVal = m_cntHolder->m_ptr; // remember the old object pointer for deep copying below
      m_cntHolder->removeRef(); // unreference the object before beeing new assigned
      m_cntHolder = new CountHolder;
      m_cntHolder->m_ptr = new T(*pOldVal); // deep copy
      m_cntHolder->addRef();
    }
  }
  //>>

  template <typename T>
  inline RCObject_AutoPtr<T>::RCObject_AutoPtr(T * ptr)
  : m_ptr(ptr)
  {
  }

  template <typename T>
  template <typename U>
  inline RCObject_AutoPtr<T>::RCObject_AutoPtr(const RCObject_AutoPtr<U>& rhs)
  : m_ptr(rhs.release())
  {
    // !! don't add reference here !!
  }

  template <typename T>
  inline RCObject_AutoPtr<T>::~RCObject_AutoPtr()
  {
    deletePointer();
  }
    
  template <typename T>
  template <typename U>
  inline RCObject_AutoPtr<T>& RCObject_AutoPtr<T>::operator=(const RCObject_AutoPtr<U>& rhs)
  {
    if ( this != &rhs )
    {
      reset(rhs.release());
    }
    return *this;
  }

  template <typename T>
  inline T * RCObject_AutoPtr<T>::get() const
  {
    return m_ptr;
  }

  template <typename T>
  inline T * RCObject_AutoPtr<T>::operator->() const
  {
    return m_ptr;
  }

  template <typename T>
  inline T&  RCObject_AutoPtr<T>::operator*() const
  {
    return *m_ptr;
  }
    
  template <typename T>
  inline T * RCObject_AutoPtr<T>::release()
  {
    T * ptr = m_ptr;
    m_ptr = 0;
    return ptr;
  }

  template <typename T>
  void RCObject_AutoPtr<T>::reset(T * ptr)
  {
    if ( m_ptr != ptr )
    {
      deletePointer();
      m_ptr = ptr;
      // !! don't ad reference here !!
    }
  }

  template <typename T>
  void RCObject_AutoPtr<T>::deletePointer()
  {
    if ( m_ptr )
    {
      m_ptr->addRef();
      m_ptr->removeRef();
    }
  }

  //>>
  template <typename T>
  inline void writeAccess( RCPtr<T> &rcPtr )
  {
    // perform a deep copy of the embedded object if shared
    rcPtr.clone();
  }

  //>>
  template <typename T>
  inline void writeAccess( RCIPtr<T> &rcPtr )
  {
    // perform a deep copy of the embedded object if shared
    rcPtr.clone();
  }

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //! Convenient helper functor that can be used in STL algorithms to add a reference to reference counted objects.
  /** With this helper functor it's easy and efficient to add a reference to each object contained in a STL container.
    * To copy a container of reference counted objects and increase their reference count you could do something 
    * similar to the following:
    *   \code
    *   for ( iter=src.begin() ; iter!=src.end() ; ++iter )
    *   {
    *     dst.push_back( *iter );
    *     (*iter)->addRef();
    *   }
    *   \endcode
    * or, equivalently but more efficiently, using this functor:
    *   \code
    *   transform( src.begin(), src.end(), inserter( dst, dst.begin()), addReference() );
    *   \endcode
    */
  struct addReference
  {
    //! Function call operator for one argument, being the pointer to an object.
    /** The member function \a addRef() of an RCObject is called on the argument.
      * \return the argument for usage in the algorithm */
    template <typename RCOBJPTR>
    RCOBJPTR operator()( RCOBJPTR pRCObject   //!<  pointer to reference counted object
                      );

    //! Function call operator for a pair of arguments.
    /** The member function \a addRef() of an RCObject is called on the second element of the pair of arguments.
      * \return the argument for usage in the algorithm */
    template <typename AnyType, typename RCOBJPTR>
      const std::pair<AnyType, RCOBJPTR>& operator()( std::pair<AnyType,RCOBJPTR>& p  //!< pointer to a pair of any argument and a reference counted object
                                            );
  };

  template <typename RCOBJPTR>
  RCOBJPTR addReference::operator()( RCOBJPTR pRCObject )
  {
    pRCObject->addRef();
    return pRCObject;
  }

  template <typename AnyType, typename RCOBJPTR>
  const std::pair<AnyType, RCOBJPTR>& addReference::operator()( std::pair<AnyType,RCOBJPTR>& p )
  {
    p.second->addRef();
    return p;
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  //! Convenient helper functor that can be used in STL algorithms
  struct removeReference
  {
    //! Function call operator for one argument, being the pointer to an object.
    template <typename RCOBJPTR>
    RCOBJPTR operator()(RCOBJPTR pRCObject) 
      { pRCObject->removeRef(); return pRCObject; }

    //! Function call operator for a pair of arguments.
    template <typename AnyType, typename RCOBJPTR>
    const std::pair<AnyType, RCOBJPTR>& operator() (std::pair<AnyType,RCOBJPTR>& p) 
      { p.second->removeRef(); return p; }
  };

} // namespace nvutil

#ifdef _DEFINED_DBGNEW
# undef new
#endif
