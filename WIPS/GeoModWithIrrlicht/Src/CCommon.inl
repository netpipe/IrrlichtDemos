/*
===============================================================================
	File:	CCommon.inl
	Desc:	Implementation of ::mix::ICommon.
===============================================================================
*/

/*================================
	class CCommon
================================*/

class CCommon : public ::mix::ICommon {
public:
	CCommon()
	{
		m_buffer[ 0 ] = NULL;
		m_wbuffer[ 0 ] = NULL;
	}

	~CCommon()
	{}

	void Print( const char * format, ... )
	{
		va_list	 argptr;

//		va_start( argptr, format );
		vsprintf( m_buffer, format, argptr );
//		va_end( argptr );

		//game.Logger->log( m_buffer );
		printf( m_buffer );
	}

	void Warning( const char * format, ... )
	{
		va_list	 argptr;

//		va_start( argptr, format );
		vsprintf( m_buffer, format, argptr );
//		va_end( argptr );

//		MessageBox( NULL, (wchar_t*)m_buffer, L"Warning!", MB_OK );
		//game.Logger->log( m_buffer, ELOG_LEVEL::ELL_WARNING );
		printf( m_buffer );
	}

	void Error( const char * format, ... )
	{
		va_list	 argptr;

//		va_start( argptr, format );

		vsprintf( m_buffer, format, argptr );
////		MessageBox( NULL, (wchar_t*)m_buffer, L"Internal error, application will exit!", MB_OK );
		//game.Logger->log( m_buffer, ELOG_LEVEL::ELL_ERROR );
		printf( m_buffer );

		fprintf( stderr, "%s: Internal error: " );
		vfprintf( stderr, format, argptr );
//		va_end( argptr );

		fputc( '\n', stderr );
		exit( -1 );
	}

	void FatalError( const char * format, ... )
	{
		va_list	 argptr;

//		va_start( argptr, format );

		vsprintf( m_buffer, format, argptr );
//		MessageBox( NULL, (wchar_t*)m_buffer, L"Fatal error, application will exit!", MB_OK );

		fprintf( stderr, format, argptr );
//		va_end( argptr );

		fputc( '\n', stderr );
		exit( -1 );
	}

	void DebugPrint( const char * format, ... )
	{
		va_list	 argptr;

//		va_start( argptr, format );
		vsprintf( m_buffer, format, argptr );
//		va_end( argptr );

		printf( m_buffer );
	}

private:
	enum { BUFFER_SIZE	= 4096 };
	char	m_buffer[ BUFFER_SIZE ];
	char	m_wbuffer[ BUFFER_SIZE ];
};
