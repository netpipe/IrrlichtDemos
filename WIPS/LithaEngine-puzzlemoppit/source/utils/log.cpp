
#include "utils/Variant.h" // hax
#include "utils/log.h"
#include <stdarg.h>

namespace utils
{
namespace log
{

// Default log location in case anything crashes before the engine sets
// the proper log location.
// This init log will never be cleared.
io::path logFilePath = "Litha Init.log";
io::path logFilePath2 = "";

void setfile(const io::path &logFile)
{
	logFilePath = logFile;
	
	// Attempt to clear current contents.
	if (logFilePath.size())
		os::path::ensure_delete(logFilePath);
}

void setfile2(const io::path &logFile)
{
	logFilePath2 = logFile;
	
	// Attempt to clear current contents.
	if (logFilePath2.size())
		os::path::ensure_delete(logFilePath2);
}

void backup(const io::path &ext)
{
	if (logFilePath.size())
		utils::file::put( logFilePath+ext, utils::file::get(logFilePath) );
	
	if (logFilePath2.size())
		utils::file::put( logFilePath2+ext, utils::file::get(logFilePath2) );
}

// Output to stdout and to the log file if one is set.
void out(const core::stringc &str)
{
	// Print to stdout
	printf("%s", str.c_str());
	
	// Write to log file, flush, then close.
	if (logFilePath.size())
	{
		FILE *fp = fopen(logFilePath.c_str(), "ab");
		
		if (fp)
		{
			fputs(str.c_str(), fp);
			fflush(fp);
			fclose(fp);
		}
		
		// Perhaps we oughtn't to silently fail... Bleh.
	}
	
	// Extra log location.
	if (logFilePath2.size())
	{
		FILE *fp = fopen(logFilePath2.c_str(), "ab");
		
		if (fp)
		{
			fputs(str.c_str(), fp);
			fflush(fp);
			fclose(fp);
		}
		
		// Perhaps we oughtn't to silently fail... Bleh.
	}
}

Logger::Logger(E_LOG_LEVEL logLevel, const core::stringc &fileName, u32 line, const core::stringc &funcName)
{
	this->logLevel = logLevel;
	
	// No variants inserted yet.
	inserted = false;
	
	// Build the string to output to the log.
	
	core::stringc outStr;
	
	switch (logLevel)
	{
	case ELL_NOTE:
		// Don't want the colon: as will not have line numbers etc with ELL_NOTE.
		inserted = true;
		break;
	case ELL_WARN:
		outStr += "*** WARNING at ";
		break;
	case ELL_FAIL:
		outStr += "*** FATAL ERROR at ";
		break;
	case ELL_ASSERT:
		outStr += "*** ASSERTION FAILED at ";
		break;
	};
	
	// For any log level but NOTE, we output the filename, line number, and function name.
	if (logLevel != ELL_NOTE)
	{
		outStr += os::path::basename(fileName) + ":";
		outStr += line;
		outStr += ":";
		outStr += funcName;
	}
	
	// Send to console and log file.
	out(outStr);
}

Logger::~Logger()
{
	out( os::newline() );
	
	// Maybe abort the app completely?
	if (logLevel == ELL_FAIL || logLevel == ELL_ASSERT)
		abort();
}

Logger &Logger::operator<<(const Variant &output)
{
	if (!inserted)
	{
		out(": ");
		inserted = true;
	}
	
	out( output.To<core::stringc>() );
	return *this;
}

}
}
