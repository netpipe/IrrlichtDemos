/*
===============================================================================
	File:	TRenderMesh.cpp
	Desc:	CPool, swiped from flipcode.
			An optimised way of performing dynamic memory allocation, using a
			preallocated pool which grows in large blocks as more objects are required.

			To use it, add a static member variable to the class definition:
			static mxMemoryPool m_pool;
			Which has to be initialised in the class's .cpp file as:
			mxMemoryPool CItem::m_pool( 1000 );
			Where 1000 is the initial number of items in the pool.

			Then add overrides for the new and delete operator of your class:
				static void* operator new(const size_t size)
				{
					return m_pool.New(size);
				}
				static void operator delete(void* pObject);
				{
					m_pool.Delete(pObject);
				}
===============================================================================
*/

#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#ifdef __linux
typedef unsigned int WORD;
typedef bool BYTE;
#endif
namespace mix {

//
//	mxMemoryPool
//
template< typename T >
class mxMemoryPool {
public:
			mxMemoryPool( const WORD nPoolSize = 256 );
			~mxMemoryPool();

	void *	New( const size_t size );
	void	Delete( void* pVoid );
	void	Clear();

private:
	mxMemoryPool( mxMemoryPool* const pPrev );

	// New allocation pools are created and inserted into a doubly-linked list.
	mxMemoryPool* const	m_pPrev;
	mxMemoryPool*		m_pNext;

	const WORD		m_nPoolSize;	// Maximum number of items in the pool.
	BYTE*			m_pAvailable;	// Next available item.
	BYTE*			m_pPool;		// The allocation pool of items.
	BYTE*			m_pLast;		// End of the pool.
	WORD			m_nTOS;			// Top of the free stack.
	BYTE**			m_pFreeStack;	// The stack of deleted items.
};

// Create a new pool by defining the number of items to preallocate, and the
// size of each item.  For situations where you do not want the allocation
// pool increase in size, set bGrow to false.
//
template< typename T >
mxMemoryPool< T >::mxMemoryPool( const WORD nPoolSize )
	: m_pPrev( 0 )
	, m_pNext( 0 )
	, m_nPoolSize( nPoolSize )
	, m_nTOS( 0 )
{
	m_pPool = reinterpret_cast< BYTE* >( ::operator new( sizeof(T) * m_nPoolSize ) );
	m_pFreeStack = ::new BYTE* [ m_nPoolSize ];
	m_pAvailable = m_pPool;
	m_pLast = m_pPool + sizeof(T) * m_nPoolSize;
}

// Add a new pool to the end of the linked list, this can only be called from
// another mxMemoryPool.
//
template< typename T >
mxMemoryPool< T >::mxMemoryPool( mxMemoryPool* const pPrev )
	: m_pPrev( pPrev )
	, m_pNext( 0 )
	, m_nPoolSize( pPrev->m_nPoolSize )
	, m_nTOS( 0 )
{
	m_pPool = reinterpret_cast< BYTE* >( ::operator new( sizeof(T) * m_nPoolSize ) );
	m_pFreeStack = ::new BYTE* [ m_nPoolSize ];
	m_pAvailable = m_pPool;
	m_pLast = m_pPool + sizeof(T) * m_nPoolSize;
}

template< typename T >
mxMemoryPool< T >::~mxMemoryPool()
{
	::delete m_pNext;
	::delete m_pPool;
	::delete [] m_pFreeStack;
}

template< typename T >
void* mxMemoryPool< T >::New( const size_t size )
{
//!	Assert( sizeof(T) == size );

	// If there are any holes in the free stack then fill them up.
	if ( m_nTOS == 0 )
	{
		// If there is any space left in this pool then use it, otherwise move
		// on to the next in the linked list.
		if ( m_pAvailable < m_pLast )
		{
			BYTE*	pReturn = m_pAvailable;
			m_pAvailable += sizeof(T);
			return reinterpret_cast< void* >( pReturn );
		}
		else
		{
			// If there is another pool in the list then pass the request on to
			// it, otherwise try to create a new pool.
			if ( m_pNext ) {
				return m_pNext->New( size );
			} else {
				m_pNext = ::new mxMemoryPool( this );
				if ( m_pNext ) {
					return m_pNext->New(size);
				}
			}
		}
	}
	return reinterpret_cast< void* >( m_pFreeStack[ --m_nTOS ] );
}

template< typename T >
void mxMemoryPool< T >::Delete( void* pVoid )
{
	if ( pVoid )
	{
		BYTE*	pItem = reinterpret_cast< BYTE* >( pVoid );

		// Check if the item being deleted is within this pool's memory range.
		if ( pItem < m_pPool || pItem >= m_pLast )
		{
			// If there is another pool in the list then try to delete from it,
			// otherwise call the generalised delete operator.
			if ( m_pNext ) {
				m_pNext->Delete( pItem );
			} else {
				::operator delete( pVoid );
			}
		}
		else
		{
			// Add a hole to the free stack.
			m_pFreeStack[ m_nTOS++ ] = pItem;

			// If this pool is empty and it is the last in the linked list
			// then delete it and set the previous pool to the last.
			if ( m_pPrev && !m_pNext &&
				static_cast< long >( m_nTOS * sizeof(T) ) == m_pAvailable - m_pPool )
			{
				m_pPrev->m_pNext = 0;
				::operator delete( this );
			}
		}
	}
}

// Reset all the pointers and indices, effectively deleting the allocated
// items without actually calling their destructors.  This function should
// be used with extreme caution since all sorts of horrible things can result
// from it's misuse.
//
template< typename T >
void mxMemoryPool< T >::Clear()
{
	m_nTOS = 0;
	m_pAvailable = m_pPool;
	if ( m_pNext ) {
		m_pNext->Clear();
	}
}

}//end of namespace mix

//---------------------------------------------------------------------------------------

// operators 'new' and 'delete'

template< class T >
void * operator new( size_t size, ::mix::mxMemoryPool<T> & rMemPool )
{
	return rMemPool.New( size );
}

template< class T >
void operator delete( void* p, ::mix::mxMemoryPool<T> & rMemPool )
{
	return rMemPool.Delete( p );
}

#endif // !__MEMORY_POOL_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
