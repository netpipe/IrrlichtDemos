/*
===============================================================================
	File:	Templates.h
	Desc:	Handy container classes.
===============================================================================
*/

#ifndef __MX_TEMPLATES_H__
#define __MX_TEMPLATES_H__

#include <iostream>

namespace mix {

//
// Helper templates.
//

// Value indicating invalid index.
enum { INDEX_NONE = -1 };

template< typename type >
void Swap( type & a, type & b )
{
	type  temp = a;
	a = b;
	b = temp;
}

// Linear interpolation.
template< typename T >
void Lerp( T &value, const T& a, const T& b, const f32 amount = 0.5f )
{
	value = a + (b - a) * amount;
}

template< typename T >
inline T Clamp( const T value, const T Min, const T Max )
{
	return (value < Min) ? Min : (value > Max) ? Max : value;
}

//==================================================================================

// For preventing compiler warnings "unreferenced formal parameters".
template< typename T >
void UnusedParameter( T param )
{ (void)param; }

//==================================================================================

//
//	TTriangle - a triple of some elements.
//
template< typename T >
class TTriangle {
public:
	union {
		struct { T   iA, iB, iC; };
		struct { T   Points[ 3 ]; };
	};

	TTriangle() {}

	TTriangle( const T & A, const T & B, const T & C )
		: iA( A ), iB( B ), iC( C )
	{}

	T & operator [] ( u32 index )
	{
		Assert( index >= 0 && index <= 3 );
		return Points[ index ];
	}

	const T & operator [] ( u32 index ) const
	{
		Assert( index >= 0 && index <= 3 );
		return Points[ index ];
	}
};

//
//	TFixedArray - Fixed-size array.
//
//	It's recommended to use this class instead of bare C-style arrays.
//
template< typename T, const u32 MaxNumItems >
class TFixedArray {
public:
	u32  GetCapacity() const { return MaxNumItems; }

	TFixedArray()
	{}

	// Assumes that elements are pointers. Deletes elements.
	void DeleteItems()
	{
		// TODO: StaticAssert( T.__is_pointer() );
		for ( u32 i = 0; i < MaxNumItems; ++i )
		{
			delete m_items[ i ];
		}
	}

	T & operator [] ( u32 index )
	{
		BREAK_IF( !(index >= 0 && index < MaxNumItems) );
		return m_items[ index ];
	}

	const T & operator [] ( u32 index ) const
	{
		BREAK_IF( !(index >= 0 && index < MaxNumItems) );
		return m_items[ index ];
	}

	// Zero out the elements of the array.
	void Zero()
	{
		MemSet( m_items, 0, sizeof( T ) * MaxNumItems );
	}

	T * Ptr()
	{ return m_items; }

	const T * Ptr() const
	{ return m_items; }

	// Make sure that array can hold the given number elements.
	void AssureSize( u32 size )
	{ BREAK_IF( size < 0 || size >= MaxNumItems ); }

private:
	T		m_items[ MaxNumItems ];
};

//============================================================================================

//
//	TArrayPool - a static array which can be used as a memory pool.
//

template< typename T, const u32 MaxNumItems >
class TArrayPool {
public:
	u32  GetCapacity() const { return MaxNumItems; }

	TArrayPool()
		: m_numItems( 0 )
	{}

	// Assumes that elements are pointers. Deletes elements.
	void DeleteItems()
	{
		// TODO: StaticAssert( T.__is_pointer() );
		for ( u32 i = 0; i < m_numItems; ++i ) {
			delete m_items[ i ];
		}
		m_numItems = 0;
	}

	T & operator [] ( u32 index )
	{
		BREAK_IF( !(index >= 0 && index < m_numItems) );
		return m_items[ index ];
	}

	const T & operator [] ( u32 index ) const
	{
		BREAK_IF( !(index >= 0 && index < m_numItems) );
		return m_items[ index ];
	}

	// Zero out the elements of the array.
	void Zero()
	{
		MemSet( m_items, 0, sizeof( T ) * MaxNumItems );
	}

	T * Ptr()
	{ return m_items; }

	const T * Ptr() const
	{ return m_items; }

	// Make sure that array can hold the given number elements.
	void AssureSize( u32 size )
	{ BREAK_IF( size < 0 || size >= MaxNumItems ); }

	// Allocates a new element at the end of the array and returns reference to this new data element.
	T & Alloc()
	{
		AssureSize( m_numItems + 1 );
		return m_items[ m_numItems++ ];
	}

	T & Alloc( u32  count )
	{
		AssureSize( m_numItems + count );
		m_numItems += count;
		return m_items[ m_numItems-1 ];
	}

private:
	T		m_items[ MaxNumItems ];
	u32		m_numItems;
};

}//end of namespace mix

/* TODO: how to implement this ?
//
// Overloaded operators for placement new.
// NOTE: must be global.

template< typename T >
inline void * operator new( size_t size, ::mix::TArrayPool< T,? > & rArray )
{
	return & rArray.Alloc();
	//return & ( rArray.All
}

// To prevent compiler warnings.
template< typename T >
inline void operator delete( void* p, ::mix::TArrayPool< T,? > & rArray )
{
	// nothing
}

*/

//============================================================================================

namespace mix {

//
//	RArray< T > - dynamic array.
//
//	Uses realloc() for growing, can expand a memory block in place,
//	the new location will contain the same data as the old location up to the size of the new request,
//	so it's safe to refer to the elements of this array by pointers.
//
template< class T >
class RArray {
public:
};

}//end of namespace mix

//============================================================================================

namespace mix {

template< class type >
inline int SortCompare( const type *a, const type *b ) {
	return *a - *b;

}

//
//	TArray< T > - dynamic array.
//
//	Does not allocate memory until the first item is added.
//	Uses 'new' and 'delete' so can change positions of items in memory when growing/shrinking.
//
//	Swiped from Id Software.
//

template< class type >
class TArray {
public:

	typedef u32		cmp_t( const type *, const type * );
	typedef type	new_t( void );

					TArray( u32 newgranularity = 16 );
					TArray( const TArray<type> &other );
					~TArray<type>( void );

	void			Clear( void );										// clear the list
	u32				Num( void ) const;									// returns number of elements in list
	u32				NumAllocated( void ) const;							// returns number of elements allocated for
	void			SetGranularity( u32 newgranularity );				// set new granularity
	u32				GetGranularity( void ) const;						// get the current granularity

	size_t			Allocated( void ) const;							// returns total size of allocated memory
	size_t			Size( void ) const;									// returns total size of allocated memory including size of list type
	size_t			MemoryUsed( void ) const;							// returns size of the used elements in the list

	TArray<type> &	operator = ( const TArray<type> &other );
	const type &	operator [] ( u32 index ) const;
	type &			operator [] ( u32 index );

	void			Condense( void );									// resizes list to exactly the number of elements it contains
	void			Resize( u32 newsize );								// resizes list to the given number of elements
	void			Resize( u32 newsize, u32 newgranularity	 );			// resizes list and sets new granularity
	void			SetNum( u32 newnum, bool resize = true );			// set number of elements in list and resize to exactly this number if necessary
	void			AssureSize( u32 newSize);							// assure list has given number of elements, but leave them uninitialized
	void			AssureSize( u32 newSize, const type &initValue );	// assure list has given number of elements and initialize any new elements
	void			AssureSizeAlloc( u32 newSize, new_t *allocator );	// assure the pointer list has the given number of elements and allocate any new elements

	type *			Ptr( void );										// returns a pointer to the list
	const type *	Ptr( void ) const;									// returns a const pointer to the list

	type &			GetLast();
	const type &	GetLast() const;

	type &			Alloc( void );										// returns reference to a new data element at the end of the list
	u32				Append( const type & obj );							// append element
	u32				Append( const TArray<type> &other );				// append list
	u32				AddUnique( const type & obj );						// add unique element
	u32				Insert( const type & obj, u32 index = 0 );			// insert the element at the given index
	u32				FindIndex( const type & obj ) const;				// find the index for the given element
	type *			Find( type const & obj ) const;						// find pointer to the given element
	u32				FindNull( void ) const;								// find the index for the first NULL pointer in the list
	u32				IndexOf( const type *obj ) const;					// returns the index for the pointer to an element in the list
	bool			RemoveIndex( u32 index );							// remove the element at the given index
	bool			Remove( const type & obj );							// remove the element
	void			Sort( cmp_t *compare = ( cmp_t * )&SortCompare<type> );
	void			SortSubSection( u32 startIndex, u32 endIndex, cmp_t *compare = ( cmp_t * )&SortCompare<type> );
	void			Swap( TArray<type> &other );						// swap the contents of the lists
	void			DeleteContents( bool clear );						// delete the contents of the list

private:
	u32				num;			// number of elements in list
	u32				size;			// number of allocated elements
	u32				granularity;	// specifies by how many items the list will grow
	type *			list;			// pointer to allocated memory
};

/*
================
TArray<type>::TArray( u32 )
================
*/
template< class type >
inline TArray<type>::TArray( u32 newgranularity /* = 16 */ ) {
	Assert( newgranularity > 0 );

	list		= NULL;
	granularity	= newgranularity;
	Clear();
}

/*
================
TArray<type>::TArray( const TArray<type> &other )
================
*/
template< class type >
inline TArray<type>::TArray( const TArray<type> &other ) {
	list = NULL;
	*this = other;
}

/*
================
TArray<type>::~TArray<type>
================
*/
template< class type >
inline TArray<type>::~TArray( void ) {
	Clear();
}

/*
================
TArray<type>::Clear

Frees up the memory allocated by the list.  Assumes that type automatically handles freeing up memory.
================
*/
template< class type >
inline void TArray<type>::Clear( void ) {
	if ( list ) {
		delete[] list;
	}

	list	= NULL;
	num		= 0;
	size	= 0;
}

/*
================
TArray<type>::DeleteContents

Calls the destructor of all elements in the list.  Conditionally frees up memory used by the list.
Note that this only works on lists containing pointers to objects and will cause a compiler error
if called with non-pointers.  Since the list was not responsible for allocating the object, it has
no information on whether the object still exists or not, so care must be taken to ensure that
the pointers are still valid when this function is called.  Function will set all pointers in the
list to NULL.
================
*/
template< class type >
inline void TArray<type>::DeleteContents( bool clear ) {
	u32 i;

	for( i = 0; i < num; i++ ) {
		delete list[ i ];
		list[ i ] = NULL;
	}

	if ( clear ) {
		Clear();
	} else {
		MemSet( list, 0, size * sizeof( type ) );
	}
}

/*
================
TArray<type>::Allocated

return total memory allocated for the list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< class type >
inline size_t TArray<type>::Allocated( void ) const {
	return size * sizeof( type );
}

/*
================
TArray<type>::Size

return total size of list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< class type >
inline size_t TArray<type>::Size( void ) const {
	return sizeof( TArray<type> ) + Allocated();
}

/*
================
TArray<type>::MemoryUsed
================
*/
template< class type >
inline size_t TArray<type>::MemoryUsed( void ) const {
	return num * sizeof( *list );
}

/*
================
TArray<type>::Num

Returns the number of elements currently contained in the list.
Note that this is NOT an indication of the memory allocated.
================
*/
template< class type >
inline u32 TArray<type>::Num( void ) const {
	return num;
}

/*
================
TArray<type>::NumAllocated

Returns the number of elements currently allocated for.
================
*/
template< class type >
inline u32 TArray<type>::NumAllocated( void ) const {
	return size;
}

/*
================
TArray<type>::SetNum

Resize to the exact size specified irregardless of granularity
================
*/
template< class type >
inline void TArray<type>::SetNum( u32 newnum, bool resize ) {
	Assert( newnum >= 0 );
	if ( resize || newnum > size ) {
		Resize( newnum );
	}
	num = newnum;
}

/*
================
TArray<type>::SetGranularity

Sets the base size of the array and resizes the array to match.
================
*/
template< class type >
inline void TArray<type>::SetGranularity( u32 newgranularity ) {
	u32 newsize;

	Assert( newgranularity > 0 );
	granularity = newgranularity;

	if ( list ) {
		// resize it to the closest level of granularity
		newsize = num + granularity - 1;
		newsize -= newsize % granularity;
		if ( newsize != size ) {
			Resize( newsize );
		}
	}
}

/*
================
TArray<type>::GetGranularity

Get the current granularity.
================
*/
template< class type >
inline u32 TArray<type>::GetGranularity( void ) const {
	return granularity;
}

/*
================
TArray<type>::Condense

Resizes the array to exactly the number of elements it contains or frees up memory if empty.
================
*/
template< class type >
inline void TArray<type>::Condense( void ) {
	if ( list ) {
		if ( num ) {
			Resize( num );
		} else {
			Clear();
		}
	}
}

/*
================
TArray<type>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< class type >
inline void TArray<type>::Resize( u32 newsize ) {
	type	*temp;
	u32		i;

	Assert( newsize >= 0 );

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		Clear();
		return;
	}

	if ( newsize == size ) {
		// not changing the size, so just exit
		return;
	}

	temp	= list;
	size	= newsize;
	if ( size < num ) {
		num = size;
	}

	// copy the old list into our new one
	list = new type[ size ];
	for( i = 0; i < num; i++ ) {
		list[ i ] = temp[ i ];
	}

	// delete the old list if it exists
	if ( temp ) {
		delete[] temp;
	}
}

/*
================
TArray<type>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< class type >
inline void TArray<type>::Resize( u32 newsize, u32 newgranularity ) {
	type	*temp;
	u32		i;

	Assert( newsize >= 0 );

	Assert( newgranularity > 0 );
	granularity = newgranularity;

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		Clear();
		return;
	}

	temp	= list;
	size	= newsize;
	if ( size < num ) {
		num = size;
	}

	// copy the old list into our new one
	list = new type[ size ];
	for( i = 0; i < num; i++ ) {
		list[ i ] = temp[ i ];
	}

	// delete the old list if it exists
	if ( temp ) {
		delete[] temp;
	}
}

/*
================
TArray<type>::AssureSize

Makes sure the list has at least the given number of elements.
================
*/
template< class type >
inline void TArray<type>::AssureSize( u32 newSize ) {
	u32 newNum = newSize;

	if ( newSize > size ) {

		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		Resize( newSize );
	}

	num = newNum;
}

/*
================
TArray<type>::AssureSize

Makes sure the list has at least the given number of elements and initialize any elements not yet initialized.
================
*/
template< class type >
inline void TArray<type>::AssureSize( u32 newSize, const type &initValue ) {
	u32 newNum = newSize;

	if ( newSize > size ) {

		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for ( u32 i = num; i < newSize; i++ ) {
			list[i] = initValue;
		}
	}

	num = newNum;
}

/*
================
TArray<type>::AssureSizeAlloc

Makes sure the list has at least the given number of elements and allocates any elements using the allocator.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< class type >
inline void TArray<type>::AssureSizeAlloc( u32 newSize, new_t *allocator ) {
	u32 newNum = newSize;

	if ( newSize > size ) {

		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for ( u32 i = num; i < newSize; i++ ) {
			list[i] = (*allocator)();
		}
	}

	num = newNum;
}

/*
================
TArray<type>::operator=

Copies the contents and size attributes of another list.
================
*/
template< class type >
inline TArray<type> &TArray<type>::operator=( const TArray<type> &other ) {
	u32	i;

	Clear();

	num			= other.num;
	size		= other.size;
	granularity	= other.granularity;

	if ( size ) {
		list = new type[ size ];
		for( i = 0; i < num; i++ ) {
			list[ i ] = other.list[ i ];
		}
	}

	return *this;
}

/*
================
TArray<type>::operator[] const

Access operator.  Index must be within range or an Assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< class type >
inline const type &TArray<type>::operator[]( u32 index ) const {
	BREAK_IF( !( index >= 0 ));
	BREAK_IF( !( index < num ));

	return list[ index ];
}

/*
================
TArray<type>::operator[]

Access operator.  Index must be within range or an Assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< class type >
inline type &TArray<type>::operator[]( u32 index ) {
	/*Assert( index >= 0 );
	Assert( index < num );*/
	BREAK_IF( !( index >= 0 ));
	BREAK_IF( !( index < num ));

	return list[ index ];
}

/*
================
TArray<type>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< class type >
inline type *TArray<type>::Ptr( void ) {
	return list;
}

/*
================
TArray<type>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< class type >
const inline type * TArray<type>::Ptr( void ) const {
	return list;
}


template< class type >
inline type & TArray<type>::GetLast() {
	return list[ num-1 ];
}

template< class type >
inline const type & TArray<type>::GetLast() const {
	return list[ num-1 ];
}


/*
================
TArray<type>::Alloc

Returns a reference to a new data element at the end of the list.
================
*/
template< class type >
inline type &TArray<type>::Alloc( void ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		Resize( size + granularity );
	}

	return list[ num++ ];
}

/*
================
TArray<type>::Append

Increases the size of the list by one element and copies the supplied data into it.

Returns the index of the new element.
================
*/
template< class type >
inline u32 TArray<type>::Append( type const & obj ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		u32 newsize;

		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

	list[ num ] = obj;
	num++;

	return num - 1;
}


/*
================
TArray<type>::Insert

Increases the size of the list by at leat one element if necessary
and inserts the supplied data into it.

Returns the index of the new element.
================
*/
template< class type >
inline u32 TArray<type>::Insert( type const & obj, u32 index ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		u32 newsize;

		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

	if ( index < 0 ) {
		index = 0;
	}
	else if ( index > num ) {
		index = num;
	}
	for ( u32 i = num; i > index; --i ) {
		list[i] = list[i-1];
	}
	num++;
	list[index] = obj;
	return index;
}

/*
================
TArray<type>::Append

adds the other list to this one

Returns the size of the new combined list
================
*/
template< class type >
inline u32 TArray<type>::Append( const TArray<type> &other ) {
	if ( !list ) {
		if ( granularity == 0 ) {	// this is a hack to fix our MemSet classes
			granularity = 16;
		}
		Resize( granularity );
	}

	u32 n = other.Num();
	for (u32 i = 0; i < n; i++) {
		Append(other[i]);
	}

	return Num();
}

/*
================
TArray<type>::AddUnique

Adds the data to the list if it doesn't already exist.  Returns the index of the data in the list.
================
*/
template< class type >
inline u32 TArray<type>::AddUnique( type const & obj ) {
	u32 index;

	index = FindIndex( obj );
	if ( index < 0 ) {
		index = Append( obj );
	}

	return index;
}

/*
================
TArray<type>::FindIndex

Searches for the specified data in the list and returns it's index.  Returns -1 if the data is not found.
================
*/
template< class type >
inline u32 TArray<type>::FindIndex( type const & obj ) const {
	u32 i;

	for( i = 0; i < num; i++ ) {
		if ( list[ i ] == obj ) {
			return i;
		}
	}

	// Not found
	return -1;
}

/*
================
TArray<type>::Find

Searches for the specified data in the list and returns it's address. Returns NULL if the data is not found.
================
*/
template< class type >
inline type *TArray<type>::Find( type const & obj ) const {
	u32 i;

	i = FindIndex( obj );
	if ( i >= 0 ) {
		return &list[ i ];
	}

	return NULL;
}

/*
================
TArray<type>::FindNull

Searches for a NULL pointer in the list.  Returns -1 if NULL is not found.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< class type >
inline u32 TArray<type>::FindNull( void ) const {
	u32 i;

	for( i = 0; i < num; i++ ) {
		if ( list[ i ] == NULL ) {
			return i;
		}
	}

	// Not found
	return -1;
}

/*
================
TArray<type>::IndexOf

Takes a pointer to an element in the list and returns the index of the element.
This is NOT a guarantee that the object is really in the list.
Function will Assert in debug builds if pointer is outside the bounds of the list,
but remains silent in release builds.
================
*/
template< class type >
inline u32 TArray<type>::IndexOf( type const *objptr ) const {
	u32 index;

	index = objptr - list;

	Assert( index >= 0 );
	Assert( index < num );

	return index;
}


/*
================
TArray<type>::RemoveIndex

Removes the element at the specified index and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the index is outside the bounds of the list.
Note that the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< class type >
inline bool TArray<type>::RemoveIndex( u32 index ) {
	u32 i;

	Assert( list != NULL );
	Assert( index >= 0 );
	Assert( index < num );

	if ( ( index < 0 ) || ( index >= num ) ) {
		return false;
	}

	num--;
	for( i = index; i < num; i++ ) {
		list[ i ] = list[ i + 1 ];
	}

	return true;
}

/*
================
TArray<type>::Remove

Removes the element if it is found within the list and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the data is not found in the list.  Note that
the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< class type >
inline bool TArray<type>::Remove( type const & obj ) {
	u32 index;

	index = FindIndex( obj );
	if ( index >= 0 ) {
		return RemoveIndex( index );
	}

	return false;
}

/*
================
TArray<type>::Sort

Performs a qsort on the list using the supplied comparison function.  Note that the data is merely moved around the
list, so any pointers to data within the list may no longer be valid.
================
*/
template< class type >
inline void TArray<type>::Sort( cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	typedef u32 cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )list, ( size_t )num, sizeof( type ), vCompare );
}

/*
================
TArray<type>::SortSubSection

Sorts a subsection of the list.
================
*/
template< class type >
inline void TArray<type>::SortSubSection( u32 startIndex, u32 endIndex, cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	if ( startIndex < 0 ) {
		startIndex = 0;
	}
	if ( endIndex >= num ) {
		endIndex = num - 1;
	}
	if ( startIndex >= endIndex ) {
		return;
	}
	typedef u32 cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )( &list[startIndex] ), ( size_t )( endIndex - startIndex + 1 ), sizeof( type ), vCompare );
}

/*
================
TArray<type>::Swap

Swaps the contents of two lists
================
*/
template< class type >
inline void TArray<type>::Swap( TArray<type> &other ) {
	Swap( num, other.num );
	Swap( size, other.size );
	Swap( granularity, other.granularity );
	Swap( list, other.list );
}

}//end of namespace mix

//============================================================================================

//
// Overloaded operators for placement new.
// NOTE: must be global.

// NOTE: TArray cannot be used with pointers to its elements because its contents can be reallocated
// and pointers become invalid; in that case handles (integer offsets) must be used instead of pointers.

template< typename T >
inline void * operator new( size_t size, ::mix::TArray< T > & rArray )
{
	return & rArray.Alloc();
}

// For preventing compiler warnings.
template< typename T >
inline void operator delete( void* p, ::mix::TArray< T > & rArray )
{
	// nothing
}

//============================================================================================

namespace mix {

//
//	DefaultHashProvider
//
class DefaultHashProvider {
public:
	static inline u32 GetHash( int key )
	{
		return key;
	}

	static inline u32 GetHash( const char* key )
	{
		// hashpjw
		enum { PRIME = 211 };

		u32 h = 0, g;
		for ( const char *p = key; *p; p++ ) {
			if ( g = (h = (h << 4) + *p) & 0xF0000000 ) {
				h ^= g >> 24 ^ g;
			}
		}
		return h % PRIME;
	}
};

//
//	TMap
//
//	Maps unique keys to values.
//	TODO: fix bugs!
//
// TODO: add more features and custom allocators.
//
template< typename TKey, typename TValue, class HashProvider = DefaultHashProvider >
class TMap {
public:
	void Set( TKey key, TValue value )
	{
		u32  hash = HashProvider::GetHash( key ) & m_tableSizeMask;

		HashNode  **nextPtr = NULL, *node = NULL;

		for ( nextPtr = &(m_pHeads[hash]), node = *nextPtr; node != NULL; nextPtr = &(node->next), node = *nextPtr )
		{
			if ( node->key == key ) {
				node->value = value;
				return;
			}
		}

		++m_numEntries;

		*nextPtr = new HashNode( key, value, m_pHeads[ hash ] );
		(*nextPtr)->next = node;
	}

	// Returns NULL if failed to find.
	TValue * Find( const TKey& key )
	{
		u32  hash = HashProvider::GetHash( key ) & m_tableSizeMask;

		for ( HashNode* node = m_pHeads[hash]; node != NULL; node = node->next )
		{
			if ( node->key == key ) {
				return & node->value;
			}
		}
		return NULL;
	}

	TValue * const Find( const TKey& key ) const
	{
		u32  hash = HashProvider::GetHash( key ) % m_pHeads.Num();

		for ( HashNode* node = m_pHeads[hash]; node != NULL; node = node->next )
		{
			if ( node->key == key ) {
				return & node->value;
			}
		}
		return NULL;
	}

	TMap( u32 tablesize = 256 )
	{
		Assert( tablesize > 0 && IsPowerOfTwo( tablesize ) );

		m_tableSize = tablesize;
		m_pHeads = new HashNode* [ m_tableSize ];
		MemSet( m_pHeads, NULL, sizeof( *m_pHeads ) * m_tableSize );

		m_numEntries = 0;
		m_tableSizeMask = m_tableSize - 1;
	}

	~TMap()
	{
		Clear();
		delete [] m_pHeads;
	}

	void Clear()
	{
		for ( u32 i = 0; i < m_tableSize; i++ )
		{
			HashNode * next = m_pHeads[ i ];
			while ( next != NULL ) {
				HashNode * node = next;
				next = next->next;
				delete node;
			}
			m_pHeads[ i ] = NULL;
		}
		m_numEntries = 0;
	}

private:
	struct HashNode
	{
		TKey		key;
		TValue		value;
		HashNode *	next;

		HashNode()
		{}

		HashNode( TKey inKey, TValue inValue, HashNode* pNext )
			: key( inKey ), value( inValue )
			, next( pNext )
		{}
	};
	HashNode **				m_pHeads;
	u32						m_tableSize;
	u32						m_numEntries;
	int						m_tableSizeMask;
};

}//end of namespace mix

//============================================================================================

namespace mix {

//
//	TCircularList
//
//	Swiped from Id Software.
//
template< typename T >
class TCircularList {
public:
						TCircularList( T* Owner = NULL );
						~TCircularList();

	bool				IsListEmpty( void ) const;
	bool				InList( void ) const;
	int					Num( void ) const;
	void				Clear( void );

	void				InsertBefore( TCircularList &node );
	void				InsertAfter( TCircularList &node );
	void				AddToEnd( TCircularList &node );
	void				AddToFront( TCircularList &node );

	void				Remove( void );

	T *				Next( void ) const;
	T *				Prev( void ) const;

	T *				Owner( void ) const;
	void				SetOwner( T *object );

	TCircularList *		ListHead( void ) const;
	TCircularList *		NextNode( void ) const;
	TCircularList *		PrevNode( void ) const;

private:
	TCircularList *		head;
	TCircularList *		next;
	TCircularList *		prev;
	T *					owner;
};

/*
================
TCircularList<T>::TCircularList

Node is initialized to be the head of an empty list
================
*/

template< typename T >
TCircularList<T>::TCircularList( T* Owner )
{
	owner	= Owner;
	head	= this;
	next	= this;
	prev	= this;
}

/*
================
TCircularList<T>::~TCircularList

Removes the node from the list, or if it's the head of a list, removes
all the nodes from the list.
================
*/
template< typename T >
TCircularList<T>::~TCircularList() {
	Clear();
}

/*
================
TCircularList<T>::IsListEmpty

Returns true if the list is empty.
================
*/
template< typename T >
bool TCircularList<T>::IsListEmpty( void ) const {
	return head->next == head;
}

/*
================
TCircularList<T>::InList

Returns true if the node is in a list.  If called on the head of a list, will always return false.
================
*/
template< typename T >
bool TCircularList<T>::InList( void ) const {
	return head != this;
}

/*
================
TCircularList<T>::Num

Returns the number of nodes in the list.
================
*/
template< typename T >
int TCircularList<T>::Num( void ) const {
	TCircularList<T>	*node;
	int					num;

	num = 0;
	for( node = head->next; node != head; node = node->next ) {
		num++;
	}

	return num;
}

/*
================
TCircularList<T>::Clear

If node is the head of the list, clears the list.  Otherwise it just removes the node from the list.
================
*/
template< typename T >
void TCircularList<T>::Clear( void ) {
	if ( head == this ) {
		while( next != this ) {
			next->Remove();
		}
	} else {
		Remove();
	}
}

/*
================
TCircularList<T>::Remove

Removes node from list
================
*/
template< typename T >
void TCircularList<T>::Remove( void ) {
	prev->next = next;
	next->prev = prev;

	next = this;
	prev = this;
	head = this;
}

/*
================
TCircularList<T>::InsertBefore

Places the node before the existing node in the list.  If the existing node is the head,
then the new node is placed at the end of the list.
================
*/
template< typename T >
void TCircularList<T>::InsertBefore( TCircularList &node ) {
	Remove();

	next		= &node;
	prev		= node.prev;
	node.prev	= this;
	prev->next	= this;
	head		= node.head;
}

/*
================
TCircularList<T>::InsertAfter

Places the node after the existing node in the list.  If the existing node is the head,
then the new node is placed at the beginning of the list.
================
*/
template< typename T >
void TCircularList<T>::InsertAfter( TCircularList &node ) {
	Remove();

	prev		= &node;
	next		= node.next;
	node.next	= this;
	next->prev	= this;
	head		= node.head;
}

/*
================
TCircularList<T>::AddToEnd

Adds node at the end of the list
================
*/
template< typename T >
void TCircularList<T>::AddToEnd( TCircularList &node ) {
	InsertBefore( *node.head );
}

/*
================
TCircularList<T>::AddToFront

Adds node at the beginning of the list
================
*/
template< typename T >
void TCircularList<T>::AddToFront( TCircularList &node ) {
	InsertAfter( *node.head );
}

/*
================
TCircularList<T>::ListHead

Returns the head of the list.  If the node isn't in a list, it returns
a pointer to itself.
================
*/
template< typename T >
TCircularList<T> *TCircularList<T>::ListHead( void ) const {
	return head;
}

/*
================
TCircularList<T>::Next

Returns the next object in the list, or NULL if at the end.
================
*/
template< typename T >
T *TCircularList<T>::Next( void ) const {
	if ( !next || ( next == head ) ) {
		return NULL;
	}
	return next->owner;
}

/*
================
TCircularList<T>::Prev

Returns the previous object in the list, or NULL if at the beginning.
================
*/
template< typename T >
T *TCircularList<T>::Prev( void ) const {
	if ( !prev || ( prev == head ) ) {
		return NULL;
	}
	return prev->owner;
}

/*
================
TCircularList<T>::NextNode

Returns the next node in the list, or NULL if at the end.
================
*/
template< typename T >
TCircularList<T> *TCircularList<T>::NextNode( void ) const {
	if ( next == head ) {
		return NULL;
	}
	return next;
}

/*
================
TCircularList<T>::PrevNode

Returns the previous node in the list, or NULL if at the beginning.
================
*/
template< typename T >
TCircularList<T> *TCircularList<T>::PrevNode( void ) const {
	if ( prev == head ) {
		return NULL;
	}
	return prev;
}

/*
================
TCircularList<T>::Owner

Gets the object that is associated with this node.
================
*/
template< typename T >
T * TCircularList<T>::Owner( void ) const {
	return owner;
}

/*
================
TCircularList<T>::SetOwner

Sets the object that this node is associated with.
================
*/
template< typename T >
void TCircularList<T>::SetOwner( T *object ) {
	owner = object;
}

}//end of namespace mix

//============================================================================================

namespace mix {

//--------------------------------------------------------
//
//	void PrependItem< T >( T *& pList, T * item )
//
//	Prepends the given item to the linked list.
//	Modifies both parameters !
//	The type 'T' must have a member function
//		void SetNext( T * elem );
//
/* Usage example:
		Face * face = polygonList;
		Face * OutPolys = NULL;
		while ( face )
		{
			Face * next = face->GetNext();
			PrependItem( OutPolys, face );
			face = next;
		}

		'OutPolys' are the same as 'polygonList'.
*/
template< typename T >
void PrependItem( T *& pList, T * item )
{	BREAK_IF( !item || (pList == item) );

	item->SetNext( pList );
	pList = item;
}

}//end of namespace mix

#endif /* ! __MX_TEMPLATES_H__ */

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
