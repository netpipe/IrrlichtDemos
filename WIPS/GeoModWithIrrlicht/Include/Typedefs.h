/*
===============================================================================
	File:	Typedefs.h
	Desc:
===============================================================================
*/

#ifndef __MX_TYPEDEFS_H__
#define __MX_TYPEDEFS_H__

//
// Global typedefs.
//
typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	dword;

//----------------------------------------------------------------------------

namespace mix {

//
// Base integer types.
//

typedef ::irr::s8		int8;
typedef ::irr::s16		int16;
typedef ::irr::s32		int32;
typedef ::irr::u32		uint32;

typedef ::irr::u16		u16;

typedef ::irr::s32		s32;
typedef ::irr::u32		u32;

typedef char			BYTE;
typedef unsigned char	UBYTE;

typedef int				INT;
typedef unsigned int	UINT;

typedef short			WORD;	// SWORD ?
typedef unsigned short	UWORD;

typedef int				FASTBOOL;
typedef unsigned long	BITFIELD;

//----------------------------------------------------------------------------
// Other base types.
//
typedef ::irr::f32		f32;

// Floating-point type used for real-time stuff like physics stepping, game ticking, etc.
//typedef f32				REAL;
typedef btScalar			REAL;

//----------------------------------------------------------------------------

typedef ::irr::core::vector2df		Vec2D;
typedef ::irr::core::vector3df		Vec3D;

inline f32 Get( Vec3D& v, uint32 index ) {
	return ( &( v.X ) )[ index ];
}

inline f32 Get( const Vec3D& v, uint32 index ) {
	return ( &( v.X ) )[ index ];
}

inline void Set( Vec3D& v, uint32 index, f32 value ) {
	( &( v.X ) )[ index ] = value;
}

//----------------------------------------------------------------------------
typedef ::irr::core::matrix4		TMatrix4;

typedef ::irr::video::SColor		TColorARGB;
typedef ::irr::video::S3DVertex		TVertex;	// Render vertex.

// Irrlicht supports only 16-bit index buffers.
//enum { MAX_VERTEX_COUNT = 65536 };

typedef ::irr::core::plane3df		TPlane3D;

typedef ::irr::core::aabbox3df		AABB;
typedef ::irr::video::SMaterial		TRenderMaterial;

typedef ::irr::scene::SViewFrustum	TViewFrustum;
//----------------------------------------------------------------------------

template< typename T, typename TAlloc = ::irr::core::irrAllocator<T> >
struct IrrArray {
	typedef ::irr::core::array< T, TAlloc >   Type;
};
/*
template< typename T > void * operator new( size_t num, ::irr::core::array<T> & rArray )
{
	rArray.push_back( T() );
	return & rArray[ rArray.size() - 1 ];
}
*/
//----------------------------------------------------------------------------

typedef ::irr::core::stringc		String;
typedef ::irr::core::stringw		WString;

//----------------------------------------------------------------------------
/*
// swiped from Lightfeather.
// A boolean type with three possible values ( states ), aka a "troolean".
enum E_TROOLEAN
{
	False	= 0,	// "false" value
	True	= 1,	// "true" value
	Unknown	= 2		// "whatever/dont_care" value
};

// So you can use "trool" like "bool".
typedef E_TROOLEAN	trool;
*/
}//end of namespace mix

#endif // !__MX_TYPEDEFS_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
