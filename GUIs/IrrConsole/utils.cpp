#include "IrrConsole/utils.h"

//=====================================================================================
IC_Error::IC_Error(const WideString str, const s32 code) : errorMessage(str), errorCode(code)
{
}
//=====================================================================================
IC_Error::IC_Error(const IC_Error& err)
{
	*this = err;
}
//=====================================================================================
IC_Error::~IC_Error()
{
}
//=====================================================================================	
const IC_Error& IC_Error::operator = (const IC_Error& err)
{
	setMessage(err.getMessage());
	setCode(err.getCode());
	return *this;
}
//=====================================================================================
const WideString IC_Error::getMessage() const
{ 
	return errorMessage; 
}
//=====================================================================================
const s32 IC_Error::getCode() const
{
	return errorCode; 
}
//=====================================================================================
void IC_Error::setMessage(const WideString str)
{
	errorMessage = str;
}
//=====================================================================================
void IC_Error::setCode(const s32 code)
{
	errorCode = code;
}
//=====================================================================================
WideString IC_StrConv::toWideString(const String str)
{
	int len = str.size() + 1;
	wchar_t* buf = new wchar_t[len];
	
	::mbstowcs(buf,str.c_str(),len);
	WideString wstr = buf;
	delete[] buf;
	return wstr;
}
//=====================================================================================
String     IC_StrConv::toString(const WideString str)
{
	int len = str.size() + 1;
	c8* buf = new c8[len];
	::wcstombs(buf,str.c_str(),len);
	String wstr = buf;
	delete[] buf;
	return wstr;
}
//=====================================================================================

IC_CmdLineParser::IC_CmdLineParser(const WideString& line) : cmdLine(line)
{
}
IC_CmdLineParser::~IC_CmdLineParser()
{
}
bool IC_CmdLineParser::parse(WideString& cmdName, array<WideString>& args)
{
	cout<<"Parsing : ["<<cmdLine<<"]"<<endl;
	static const wchar_t spaceChar = (wchar_t)' ';
	args.clear();
	cmdName = L"";

	if(cmdLine.findFirst(spaceChar) == -1)
	{
		cmdName = cmdLine;
		return true;
	}
	
	setQuoted(false);
	setEscaped(false);
	setNameDone(false);
	bShouldAddLast = true;
	resetTmpString();

	for(s32 x = 0; x < cmdLine.size(); x++)
	{
		if(!handleChar(cmdLine[x],cmdName,args))
		{
			return false;
		}
	}
	if(bShouldAddLast)
	{
		shoveTmpString(cmdLine,args);
	}
	return true;
}
bool IC_CmdLineParser::handleChar(wchar_t wc, WideString& cmdName, array<WideString>& args)
{
	static const wchar_t spaceChar = (wchar_t)' ';
	static const wchar_t escapeChar = (wchar_t)'\\';
	static const wchar_t quoteChar = (wchar_t)'\"';
	if(wc == spaceChar)
	{
		if(!isQuoted())
		{
			shoveTmpString(cmdName,args);
		}
		else
		{
			tmpString += (spaceChar);
			bShouldAddLast = true;
		}
	}
	else if(wc == quoteChar)
	{
		if(isEscaped())
		{
			
			tmpString += quoteChar;
			bShouldAddLast = true;
			setEscaped(false);
		}
		else if(isQuoted())
		{
			shoveTmpString(cmdName,args);
			setQuoted(false);
		}
		else
		{
			setQuoted(true);
		}

	}
	else if(wc == escapeChar)
	{
		if(isEscaped())
		{
			tmpString += escapeChar;
			bShouldAddLast = true;
			setEscaped(false);
		}
		else
		{
			setEscaped(true);
		}
	}
	else
	{
		if(isEscaped())
		{
			return false;
		}
		tmpString += wc;
		bShouldAddLast = true;
	}
	return true;
}
void IC_CmdLineParser::shoveTmpString(WideString& cmdName, array<WideString>& args)
{
	if(bNameDone)
	{
		cout<<"arg : "<<tmpString<<endl;
		args.push_back(tmpString);
	}
	else 
	{
		cout<<"name : "<<tmpString<<endl;
		cmdName = tmpString;
		setNameDone(true);
	}
	bShouldAddLast = false;
	resetTmpString();
}
void IC_CmdLineParser::resetTmpString()
{
	tmpString = L"";
}
bool IC_CmdLineParser::isQuoted()
{
	return bQuote;
}
bool IC_CmdLineParser::isEscaped()
{
	return bEscape;
}
bool IC_CmdLineParser::isNameDone()
{
	return bNameDone;
}
void IC_CmdLineParser::setQuoted(bool bVal)
{
	bQuote = bVal;
}
void IC_CmdLineParser::setEscaped(bool bVal)
{
	bEscape = bVal;
}
void IC_CmdLineParser::setNameDone(bool bVal)
{
	bNameDone = bVal;
}
//=====================================================================================