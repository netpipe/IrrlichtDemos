#ifndef _IrrConsole_Utils_h_
#define _IrrConsole_Utils_h_
#include "includes.h"
//=====================================================================================
//! a basic error class
class IC_Error
{
public:
	IC_Error(const WideString str=L"", const s32 code=-1);
	IC_Error(const IC_Error& err);

	virtual ~IC_Error();
	
	const IC_Error& operator = (const IC_Error& err);
	virtual const WideString getType() const{ return L"Generic Error"; }
	const WideString getMessage() const;
	const s32 getCode() const;
protected:
	void setMessage(const WideString str);
	void setCode(const s32 code);
private:
	WideString errorMessage;
	s32		   errorCode;
};
//=====================================================================================
//! a string conversion class for converting between UTF-8 and UTF-16 strings
class IC_StrConv
{
public:
	static WideString toWideString(const String str);
	static String     toString(const WideString str);
};
//=====================================================================================
class IC_CmdLineParser
{
public:
	IC_CmdLineParser(const WideString& line);
	virtual ~IC_CmdLineParser();
	bool parse(WideString& cmdName, array<WideString>& args);
private:
	void shoveTmpString(WideString& cmdName, array<WideString>& args);
	bool handleChar(wchar_t wc, WideString& cmdName, array<WideString>& args);
	void resetTmpString();
	bool isQuoted();
	bool isEscaped();
	bool isNameDone();
	void setQuoted(bool bVal);
	void setEscaped(bool bVal);
	void setNameDone(bool bVal);

	WideString cmdLine;
	bool bQuote,bEscape, bNameDone,bShouldAddLast;
	WideString tmpString;
};
//=====================================================================================
//! overloaded operator for inserting a wide string to the stl ostream
inline ostream& operator<<(ostream& os, const String& s)
{
	return (os << s.c_str());
}
//=====================================================================================
//! overloaded operator for inserting a wide string to the stl ostream
inline ostream& operator<<(ostream& os, const WideString& ws)
{
	return (os << IC_StrConv::toString(ws));
}
//=====================================================================================

#endif