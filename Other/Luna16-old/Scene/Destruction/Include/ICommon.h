/*
===============================================================================
	File:	ICommon.h
	Desc:
===============================================================================
*/

#ifndef __MX_ICOMMON_H__
#define __MX_ICOMMON_H__

namespace mix {

//
//	ICommon
//
//	Pointer to ICommon must be initialized first.
//
class ICommon {
public:
	virtual			~ICommon() {}

					// Prints a message to the console.
	virtual void	Print( const char * format, ... ) = 0;

					// Prints a warning message.
	virtual void	Warning( const char * format, ... ) = 0;

					// Prints an error message and exits the program.
	virtual void	Error( const char * format, ... ) = 0;

					// Fatal error - terminate execution immediately; does not return;
					// may write something into the error log.
	virtual void	FatalError( const char * format, ... ) = 0;

	virtual void	DebugPrint( const char * format, ... ) = 0;
};

extern ICommon * GCommon;

}//end of namespace mix

#endif // !__MX_ICOMMON_H__
