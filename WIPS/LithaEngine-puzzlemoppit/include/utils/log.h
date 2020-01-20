
#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include "litha_internal.h"

namespace utils
{

class Variant;

namespace log
{

enum E_LOG_LEVEL
{
	ELL_NOTE,
	ELL_WARN,
	ELL_FAIL,
	ELL_ASSERT
};

// Set logging to occur to the specified file. Clears any existing file contents.
// If this is never called, logging will only occur to stdout.
void setfile(const io::path &logFile);

// An optional extra log location.
void setfile2(const io::path &logFile);

// Backup log files.
// (a copy is made with this new extension appended)
// You *should* include the dot in the extension. e.g. ".old"
void backup(const io::path &ext);

// Logger class, implementing the insertion << operator.
// An instance of this should be created for each line, as the newline is printed on destruction.
// Allows you to log anything that a Variant can construct from. (string, int, float)
// Don't use this directly, instead use the macros at the bottom of this file.
class Logger
{
	E_LOG_LEVEL logLevel;
	
	// Flag is set when at least one Variant to log has been inserted <<
	// Just used for formatting, on the first insert we add a colon ':'.
	bool inserted;
	
public:
	Logger(E_LOG_LEVEL logLevel, const core::stringc &fileName, u32 line, const core::stringc &funcName);
	~Logger();
	
	Logger &operator<<(const Variant &output);
};

}
}

// Macros for logging.
// Why are we using ugly macros? Because we want line numbers etc.
#define NOTE utils::log::Logger(utils::log::ELL_NOTE, "", 0, "")
#define WARN utils::log::Logger(utils::log::ELL_WARN, __FILE__, __LINE__, __func__)
#define FAIL utils::log::Logger(utils::log::ELL_FAIL, __FILE__, __LINE__, __func__)

// Should probably modify this so the test is done by Logger, that way assert could also output text...
#define ASSERT(test) do{if (!(test)) { utils::log::Logger(utils::log::ELL_ASSERT, __FILE__, __LINE__, __func__); }}while(0)

#endif
