/*
===============================================================================
	File:	Debug.h
	Desc:	Debugging aids.
===============================================================================
*/

#ifndef __MX_DEBUG_H__
#define __MX_DEBUG_H__

/*
================================================================================

		Run-time checks and assertions.

================================================================================
*/

#ifdef MX_DEBUG

	//----------------------------------------------------------------------------
	//	Use this to mark unreachable locations
	//	( like an unreachable default case of a switch )
	//----------------------------------------------------------------------------
	#define  Unreachable()	\
		::mix::GCommon->Error( "Unreachable code in %s, at '%s', line %d", __FUNCTION__, __FILE__, __LINE__ );

	#define  Unreachable2( message )	\
		::mix::GCommon->Error( "'%s': unreachable code in %s, at '%s', line %d", message, __FUNCTION__, __FILE__, __LINE__ );
	//----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//	Examples of usage:
	//	Assert( a > b );
	//	Assert2( x == y, "X must be equal to Y!" );
	//----------------------------------------------------------------------------
	#define Assert( expr )	\
		(void)( (expr) ? 1 : (::mix::GCommon->Error( "Assertion failed: '%s' in %s, at '%s', line %d", #expr, __FUNCTION__, __FILE__, __LINE__ ) ))

	#define Assert2( expr, message )	\
		(void)( (expr) ? 1 : (::mix::GCommon->Error( "Assertion failed: '%s': '%s' in %s, at '%s', line %d", message, #expr, __FUNCTION__, __FILE__, __LINE__ ) ))

	//----------------------------------------------------------------------------

	#define DEBUG_CODE( code )		code

	//----------------------------------------------------------------------------
	//	Use this to mark unimplemented features
	//	which are not supported yet and will cause a crash
	//----------------------------------------------------------------------------
	#define  Unimplemented()	\
		::mix::GCommon->Error( "Unimplemented feature in %s, at '%s', line %d", __FUNCTION__, __FILE__, __LINE__ );

	#define  Unimplemented2( message )	\
		::mix::GCommon->Error( "Unimplemented feature '%s' in %s, at '%s', line %d", message, __FUNCTION__, __FILE__, __LINE__ );
	//----------------------------------------------------------------------------

	#define RECURSION_GUARD( threshold )	\
	{										\
		static int	counter;				\
		++counter;							\
		BREAK_IF( counter >= threshold );	\
	}

	// Works as a static_cast, except that it would use dynamic_cast to catch programming errors in debug mode.
	template < class TypeTo, class TypeFrom >
	FORCEINLINE TypeTo checked_cast( TypeFrom* ptr )
	{
		if ( ptr )
		{
			TypeTo dtto = dynamic_cast< TypeTo >( ptr );
			TypeTo stto = static_cast< TypeTo >( ptr );
			Assert( dtto != NULL );
			Assert( dtto == stto );
			return stto;
		}
		return NULL;
	}


	#define BREAK_IF( _CONDITION_ )		if (_CONDITION_) { _asm int 3 }
	#define DEBUG_BREAK					{ _asm int 3 }

#else // !defined( MX_DEBUG )

	#define Unreachable()
	#define Unreachable2( message )

	#define Assert( expr )
	#define Assert2( expr, message )

	#define DEBUG_CODE( code )

	#define Unimplemented()
	#define Unimplemented2( message )

	#define RECURSION_GUARD( threshold )

	template < class TypeTo, class TypeFrom >
	inline TypeTo checked_cast( TypeFrom* ptr )
	{
		return static_cast< TypeTo >( ptr );
	}

	#define BREAK_IF( _CONDITION_ )
	#define DEBUG_BREAK

#endif // !MX_DEBUG

/*
================================================================================

		Compile-time checks and assertions.

================================================================================
*/

/*
=================================================================
	StaticAssert< bool >();
		or
	DELAYED_ASSERT( expression );

	Valid only in function scope !

	Fires at link-time !
	( unresolved external symbol void StaticAssert<0>(void); )

	Usage example:
			void  Foo()
			{
				::Checks::StaticAssert< a == b >();

				// ....
			}
=================================================================
*/
namespace Checks {

	template< bool > void	StaticAssert();
	template<> inline void	StaticAssert< true >() {}

}//end of namespace Checks

#define DELAYED_ASSERT( expression )						\
		::Checks::StaticAssert< expression != false >();	\

/*
============================================================================
	STATIC_ASSERT( expression )
	STATIC_ASSERT2( expression, message )

	Fires at compile-time !

	Usage:
		STATIC_ASSERT( sizeof(*void) == 4, size_of_void_must_be_4_bytes );
============================================================================
*/
#define STATIC_ASSERT( expression )				\
struct CHECK_##__LINE__ {						\
	int m_error[ (expression) ? 1 : -1 ];		\
};

#define STATIC_ASSERT2( expression, message )	\
struct ERROR_##message {						\
	ERROR_##message() {							\
		int _##message[ expression == true ];	\
		(void)error;							\
	}											\
};

//-------------------------------------------------------
//	struct CompileTimeChecker
//-------------------------------------------------------
template< bool > struct CompileTimeChecker;
template<> struct CompileTimeChecker< true > {
	CompileTimeChecker( ... );
};

/*
============================================================================
	STATIC_ASSERT( expression, message )

	Fires at compile-time !

	Usage:
		STATIC_CHECK( sizeof(*void) == 4, size_of_void_must_be_4_bytes );
============================================================================
*/
#define STATIC_CHECK( expr, msg )					\
class ERROR_##msg {									\
	CompileTimeChecker< (expr) != false > error;	\
};

//--------------------------------------------------------------------------------------------------------------
//	These can be very useful for compile-time assersions :
//--------------------------------------------------------------------------------------------------------------

#define COMPILE_TIME_ASSERT( x )				{ typedef int compile_time_assert_failed[ (x) ? 1 : -1 ]; }
#define FILE_SCOPED_COMPILE_TIME_ASSERT( x )	extern int compile_time_assert_failed[ (x) ? 1 : -1 ]
#define ASSERT_SIZEOF( type, size )				FILE_SCOPED_COMPILE_TIME_ASSERT( sizeof( type ) == size )
#define ASSERT_OFFSETOF( type, field, offset )	FILE_SCOPED_COMPILE_TIME_ASSERT( offsetof( type, field ) == offset )
#define ASSERT_SIZEOF_8_BYTE_MULTIPLE( type )	FILE_SCOPED_COMPILE_TIME_ASSERT( ( sizeof( type ) & 8 ) == 0 )
#define ASSERT_SIZEOF_16_BYTE_MULTIPLE( type )	FILE_SCOPED_COMPILE_TIME_ASSERT( ( sizeof( type ) & 15 ) == 0 )
#define ASSERT_SIZEOF_32_BYTE_MULTIPLE( type )	FILE_SCOPED_COMPILE_TIME_ASSERT( ( sizeof( type ) & 31 ) == 0 )

#endif // !__MX_DEBUG_H__

//--------------------------------------------------------------//
//				End Of File.									//
//--------------------------------------------------------------//
