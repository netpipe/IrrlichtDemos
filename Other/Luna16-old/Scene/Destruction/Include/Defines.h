/*
===============================================================================
	File:	Defines.h
	Desc:	Macros used throughout the engine.
===============================================================================
*/

#ifndef __MX_DEFINES_H__
#define __MX_DEFINES_H__

//==================================
// Path to media content.
#define PATH_TO_MEDIA	"Media/"

//-------------------------------------------------------------

#define INLINE			inline
#define FORCEINLINE		__forceinline

#ifndef BOOL
#define BOOL	int
#endif

//-------------------------------------------------------------

#define SAFE_DELETE( p )         { if(p) { delete (p);     (p) = NULL; } }
#define SAFE_DELETE_ARRAY( p )   { if(p) { delete[] (p);   (p) = NULL; } }
#define SAFE_RELEASE( p )        { if(p) { (p)->Release(); (p) = NULL; } }

#define GRAB( p )		{ if(p) { p->grab(); } }
#define DROP( p )		{ if(p) { p->drop(); p = NULL; } }

#define ARRAY_SIZE( ptr )		( sizeof( ptr ) / sizeof( ptr[ 0 ] ) )

//-------------------------------------------------------------

#ifndef BIT
#define BIT(n)		(1 << (n))
#endif

//-------------------------------------------------------------

#define ALIGN_16	__declspec(align(16))
#define ALIGN_128	__declspec(align(128))

//-------------------------------------------------------------

#define MemCopy( dest, src, size )		memcpy( dest, src, size )
#define MemSet( dest, value, size )		memset( dest, value, size )

//-------------------------------------------------------------

#endif // !__MX_DEFINES_H__