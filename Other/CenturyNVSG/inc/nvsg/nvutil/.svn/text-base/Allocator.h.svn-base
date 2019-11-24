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

//#define ALLOCATION_COUNTER

// placement operator new does not have a matching delete operator!
// 
// objects of type T that are constructed at explicit specified memory 
// locations by using the placement operator new() (i.g. new((void*)p) T())
// should be destructed by explicitely calling their destructor (p->~T())
#pragma warning(disable:4291) 

#include <algorithm>
#include <vector>
#if defined( ALLOCATION_COUNTER )
#include <map>
#endif
#include "nvutil/Assert.h"
#include "nvutil/Singleton.h"
#include "nvutil/Trace.h"

namespace nvutil
{
  //! Chunk of memory for our low level memory managment of fixed size objects
  class Chunk
  {
    public:
      //! construct a chunk of blockSize bytes
      Chunk(size_t blockSize); 
      
      //! provide comparison of chunks
      bool operator==(const Chunk& rhs) const;
      bool operator!=(const Chunk& rhs) const;
      
      //! memory management interface
      void * alloc();
      void dealloc(void * p);
      //! explicitely free memory
      void freeMemory(); 
      
      //! provide number of blocks available in this chunk
      unsigned char blocksAvailable() const;
      //! for cleanup purposes, provide check if chunk is entirely unused
      bool isUnused() const; 
      //! provide address checking
      bool isInsideBounds(void * p) const;

    private:
      //! maximum number of blocks we can address
      enum { numBlocks = 255 };

      //! one time initialization
      void init();
      
      //! helpers used with address checking
      void * lowerBound() const;
      void * upperBound() const;

      unsigned char * m_rawMem; // raw memory

      //! bounds
      size_t m_blockSize;
      size_t m_chunkSize;

      unsigned char m_firstAvailableBlock;  // addresses first memory block available
      unsigned char m_blocksAvailable;      // total amount of blocks available 
  };

  //! Manages allocation requests for objects of a certain size only
  class FixedAllocator
  {
    public:
      //! default constructs a FixedAllocator object
      FixedAllocator(); 

      //! destructor - last chunk memory cleanup
      ~FixedAllocator();

      //! Allocate one memory block of size blockSize
      void * alloc();

      //! Free the single memory block pointed to by \a p
      void dealloc(void * p);

      //! one time initialization
      void init ( size_t blockSize );

    private:
      size_t m_blockSize;   // fixed block size

      typedef std::vector< Chunk > Chunks;
      Chunks m_chunks; // array of chunks
      
      Chunk  * m_lastAlloc;   // last chunk taken for allocation
      Chunk  * m_lastDealloc; // last chunk taken for deallocation
  };

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  //! Internal manager for memory allocations.
  /** This class is used as a \c Singleton by \c IAllocator, which is the base of all \c RCObject classes.
    * It manages the efficient allocation of small objects by using an array of \c FixedAllocator objects, one for
    * each small size up to \c maxBlockSize. Allocations larger than that size are redirected to the standard
    * allocation \c ::new.  */
  class Allocator
  {
    public:
      //! Default constructor
      /** Initializes the array of \c FixedAllocator objects. */
      Allocator();

      //! Destructor
      /** In debug mode warnings on not deleted objects are emitted.  */
      ~Allocator();

      //! Allocate continuous memory of size \a size.
      /** Forwards either to the \c FixedAllocator of the specified size, or to the global new operator. */
      void * alloc( size_t size   //!< size of memory to allocate
                  );

#if defined(_DEBUG)
      //! Allocate continuous memory of size \a size in debug mode.
      /** Forwards either to the \c FixedAllocator of the specified size, or to the global new operator.
        * In addition, it stores information for memory leak detection. */
      void * alloc( size_t size         //!<  size of memory to allocate
                  , const char * src    //!<  source file name where this memory is allocated
                  , unsigned int ln     //!<  line in the source file
                  );
#endif

      //! Deallocate previously allocated memory
      /** Forwards either to the \c FixedAllocator of the specified size, or to the global delete operator.
        * In debug mode, the corresponding information stored by alloc is erased. */
      void dealloc( void * p      //!<  pointer to the memory to deallocate
                  , size_t size   //!<  size of the allocated memory
                  );

    private:
      //! Helper allocation routine used with debug and non-debug mode
      void * palloc(size_t size);

    private:
      // forward to default new/delete operators if block size
      // exceeds the threshold given by maxBlockSize
      enum { maxBlockSize = 256 };

      // provide one allocator for each single size <= maxBlockSize
      FixedAllocator m_allocTbl[maxBlockSize];

      // leak detection in debug mode
#if defined(_DEBUG)
      struct DbgAllocInfo
      {
        void  * p;            //  memory location
        std::string  source;  //  source file of allocation
        unsigned int ln;      //  line of allocation

        // with this we can simply use a vector for our alloc infos
        bool operator==(void * _p) const { return p==_p; }
        bool operator!=(void * _p) const { return p!=_p; }
      };

      std::vector<DbgAllocInfo> m_dbgAllocInfos;   //  vector of allocation infos

      //  helper functor to put out a memory leak warning
      struct LeakWarning
      {
        void operator()(const DbgAllocInfo& allocInfo) const
        {
          std::string str;
          char cPtr[sizeof(void*)*2+1];          
          sprintf(cPtr, "%p", allocInfo.p); // cast to a hex-string for pointers
          char cLine[20];  // enough for 64-bit integers even
          sprintf(cLine, "%d", allocInfo.ln); 

          str = "****Memory Leak Detected*** Source: ";
          str += allocInfo.source;
          str += ", Line: ";
          str += cLine;
          str += ", Ptr: 0x";
          str += cPtr;
          str += "\n";
          traceDebugOutput()(str.c_str());
        }
      };
#endif
#if defined( ALLOCATION_COUNTER )
      std::map<size_t,size_t> m_allocSizeMap;
#endif
  };

  inline void * Allocator::alloc(size_t size)
  {
    NVSG_ASSERT(!"WARNING: Bypassing leak detection in debug mode!\n");
    return palloc(size);
  }

#if defined(_DEBUG)
  inline void * Allocator::alloc(size_t size, const char* src, unsigned int ln)
  {
    DbgAllocInfo allocInfo;
    // collect allocation info for memory leak detection
    allocInfo.p = palloc(size);
    allocInfo.source = src;
    allocInfo.ln = ln;
    m_dbgAllocInfos.push_back(allocInfo);
    return allocInfo.p;
  }
#endif

  inline void Allocator::dealloc(void *p, size_t size)
  {
#if defined(_DEBUG)
    m_dbgAllocInfos.erase(std::remove(m_dbgAllocInfos.begin(), m_dbgAllocInfos.end(), p), m_dbgAllocInfos.end());
#endif
    if ( size <= maxBlockSize )
    {
      m_allocTbl[size-1].dealloc(p); 
    } 
    else
    {
      ::operator delete(p);
    }
  }

  inline void * Allocator::palloc(size_t size)
  {
#if defined( ALLOCATION_COUNTER )
    m_allocSizeMap[size]++;
#endif
    // use suitable allocation for given size
    return (size<=maxBlockSize) ? m_allocTbl[size-1].alloc() : ::operator new(size);
  }
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  //! An allocator interface
  /** The \c IAllocator interface provides overloads of the \c new and \c delete operators for heap allocation.
    * This overloads make use of a specialized memory manager, that is highly optimized for small object allocation.
    * For large objects, i.e. objects greater than 128 bytes, the \c IAllocator interface utilizes the default
    * memory manager.
    * \note Typically a user defined class utilizes this interface through public inheritance.
    */
  class IAllocator
  {
    public:
      //! Convenient typedef
      typedef Singleton<Allocator> theAllocator;
      
      //! Default constructor
      /** Default constructs an \c IAllocator object.
        */
      IAllocator() {}
      //! Destructor
      virtual ~IAllocator() {}

      //! Operator new overload
      /** Allocates a memory block of size bytes from heap.
        * Returns start address of allocated memory block. */
      NVSG_API void * operator new( size_t size // Size in bytes of demanded memory block.
                                  );
#if defined(_DEBUG)
      //! Operator new overload
      /** Allocates a memory block of size bytes from heap. In addition it supports memory
        * leak detection in debug mode.
        * Returns start address of allocated memory block. */
      NVSG_API void * operator new( size_t size       // Size in bytes of demanded memory block.
                                  , const char * src  // Source file where allocation takes place.
                                  , unsigned int ln   // Line number within the source file where allocation takes place.
                                  );
#endif
      //! Placement operator new overload
      /** Provides object creation at a specified memory address.
        *
        * p must point to a valid block of memory, large enough to hold
        * an object of size bytes.
        *
        * Return start address of the specified memory block. */
      NVSG_API void * operator new( size_t size // Size in bytes of demanded memory block.
                                  , void * p    // Start address of a valid memory block where the object will be created.
                                  );
      //! Operator delete overload
      /** Frees size bytes of memory at address p. */
      NVSG_API void operator delete( void * p    // Start address of memory block to be freed.
                                  , size_t size // Size of memory block to be freed.
                                  ); 
  };


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // implementation following
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  /**
  * Chunk::init() 
  *
  * provides one time initialization
  */
  inline void Chunk::init()
  {
    m_rawMem = ::new unsigned char[m_chunkSize];
    
    m_firstAvailableBlock = 0;
    m_blocksAvailable = numBlocks;

    unsigned char * p = m_rawMem;
    for ( unsigned char i=0; i!=numBlocks; p+=m_blockSize ) 
    {
      // code indices of next available blocks in first byte of each block 
      *p = ++i;
    }
  }

  /**
  * Chunk::freeMemory() 
  *
  * explicitely free chunk's raw memory
  */
  inline void Chunk::freeMemory()
  {
    // should not attempt to free memory of a chunk in use
    NVSG_ASSERT(isUnused());
    ::delete[] m_rawMem;
  }

  /**
  * Chunk::alloc() 
  *
  * get one block from chunk; returns NULL if no blocks available
  */
  inline void * Chunk::alloc()
  {
    if ( !m_blocksAvailable ) 
    {
      // this needs to be handled in superior layers
      return NULL;
    }

    unsigned char * p = &m_rawMem[m_firstAvailableBlock*m_blockSize];
    // index of next block available is coded in first byte
    m_firstAvailableBlock = *p; 
    // one block less available in this chunk
    --m_blocksAvailable; 

    return (void *)p;
  }

  /**
  * Chunk::dealloc()
  *
  * deallocate a block pointed to by p
  */
  inline void Chunk::dealloc(void * p)
  {
    unsigned char * ptr = (unsigned char*)p;
    
    // range check
    NVSG_ASSERT(ptr>=m_rawMem);
    NVSG_ASSERT(ptr<&m_rawMem[m_chunkSize]);
    // alignment check
    NVSG_ASSERT(!((ptr-m_rawMem)%m_blockSize));
    
    *ptr = m_firstAvailableBlock;
    m_firstAvailableBlock = (unsigned char)((ptr - m_rawMem) / m_blockSize);

    // truncation check
    NVSG_ASSERT(m_firstAvailableBlock==((ptr-m_rawMem)/m_blockSize));
    ++m_blocksAvailable;
  }

  /**
  * Chunk::operator==()
  *
  * returns true if rhs equals this chunk
  */
  inline bool Chunk::operator==(const Chunk& rhs) const
  {
    // consider chunks to be equal if they point to same raw memory location
    NVSG_ASSERT(    ( m_rawMem != rhs.m_rawMem )
                ||  (     ( m_blockSize           == rhs.m_blockSize )
                      &&  ( m_firstAvailableBlock == rhs.m_firstAvailableBlock )
                      &&  ( m_blocksAvailable     == rhs.m_blocksAvailable ) ) );
    return (  m_rawMem==rhs.m_rawMem );
  }

  /**
  * Chunk::operator!=()
  *
  * returns true if rhs not equals this chunk
  */
  inline bool Chunk::operator!=(const Chunk& rhs) const
  {
    return !operator==(rhs);
  }

  /** 
  * Chunk::blocksAvailable()
  * 
  * provides number of blocks available in this chunk 
  */
  inline unsigned char Chunk::blocksAvailable() const 
  { 
    return m_blocksAvailable; 
  }

  /**
  * Chunk::isInsideBounds() 
  *
  * returns true if the address given belongs to this chunk; false otherwise  
  */
  inline bool Chunk::isInsideBounds(void * p) const
  {
    return p>=lowerBound() && p<upperBound();
  }

  /** 
  * Chunk::isUnused()
  * 
  * for cleanup purposes, provide check if chunk is entirely unused 
  */
  inline bool Chunk::isUnused() const 
  { 
    return m_blocksAvailable==numBlocks; 
  }

  /** 
  * Chunk::lowerBound()
  * 
  * query lower bound of memory chunk 
  */
  inline void * Chunk::lowerBound() const 
  { 
    return (void*)m_rawMem; 
  }

  /** 
  * Chunk::upperBound()
  * 
  * query upper bound of memory chunk 
  */
  inline void * Chunk::upperBound() const 
  { 
    return (void*)&m_rawMem[m_chunkSize]; 
  }

  /**
  * FixedAllocator::init()
  *
  * one time initialization
  */
  inline void FixedAllocator::init(size_t blockSize)
  { 
    m_blockSize = blockSize; 
  }

  /**
  * IAllocator::operator new()
  *
  * overrides operator new()  
  */
  inline void * IAllocator::operator new(size_t size) 
  {
    return theAllocator::instance()->alloc(size);
  }

#if defined(_DEBUG)
  // provide memory leak detection in debug mode
  inline void * IAllocator::operator new(size_t size, const char * src, unsigned int ln)
  {
    return theAllocator::instance()->alloc(size, src, ln);
  }
#endif

  /**
  * IAllocator::operator new()
  *
  * overides placement operator new()
  */
  inline void * IAllocator::operator new(size_t size, void * p)
  {
    // as this is used when constructing an object at a explicit
    // specified memory location, that location shoud be a valid 
    // one - assert this!
    NVSG_ASSERT(p);
    return p;
  }

  /**
  * IAllocator::operator delete()
  *
  * overrides operator delete()  
  */
  inline void IAllocator::operator delete(void * p, size_t size)
  {
    theAllocator::instance()->dealloc(p, size);
  }

} // namespace nvutil

