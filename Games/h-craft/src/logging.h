// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef LOGGING_H
#define LOGGING_H

#include "logging_priorities.h"
#include <string>
#include <stdio.h>
#include <sstream>

class Logging
{
public:
    Logging();
     ~Logging();

    // only messages with priority >= prio_ will be logged
    void SetStdoutPriority(LogPriority prio_=LP_NONE);
    void SetStderrPriority(LogPriority prio_=LP_INFO);
    void SetStreamPriority(LogPriority prio_=LP_NONE);
    bool OpenFile(LogPriority prio_, const char *file_, const char *mode_);
    void CloseFile();
    LogPriority GetStdoutPriority() const   { return mPriorityStdout; }
    LogPriority GetStderrPriority() const   { return mPriorityStderr; }
    LogPriority GetFilePriority() const     { return mPriorityFile; }
    LogPriority GetStreamPriority() const 	{ return mPriorityStream; }

    // On Android all other logging methods will do nothing.
    // On other platforms Android logging will do nothing.
	void SetAndroidPriority(LogPriority priority) 	{ mPriorityAndroid = priority; }
	LogPriority GetAndroidPriority() const			{ return mPriorityAndroid; }

    void Log(LogPriority priority_, const char * text_);
    void Log(LogPriority priority_, const wchar_t * text_);
    void Log(LogPriority priority_, const std::string& text_);
    void Log(LogPriority priority_, const std::wstring& text_);
    void LogLn(LogPriority priority_, const char * text_);
    void LogLn(LogPriority priority_, const char * text_, const char * text2_);
    void LogLn(LogPriority priority_, const char * text_, int value_);
    void LogLn(LogPriority priority_, const char * text_, float value_);


    // Comfort functions
    // The functions with several parameters will just merge those into a single string
    // Functions ending with Ln do add a linefeed

    void DebugLn(const char * text_, const char * text2_);
    void Debug(const char * text_);
    void Debug(const wchar_t * text_);
    void Debug(const std::string& text_);
    void Debug(const std::wstring& text_);
    void InfoLn(const char * text_,  const char * text2_);
    void Info(const char * text_);
    void Info(const wchar_t * text_);
    void Info(const std::string& text_);
    void Info(const std::wstring& text_);
    void WarnLn(const char * text_,  const char * text2_);
    void Warn(const char * text_);
    void Warn(const wchar_t * text_);
    void Warn(const std::string& text_);
    void Warn(const std::wstring& text_);
    void ErrorLn(const char * text_,  const char * text2_);
    void Error(const char * text_);
    void Error(const wchar_t * text_);
    void Error(const std::string& text_);
    void Error(const std::wstring& text_);

    void SetFlushing(bool enable_)  { mUseFlush = enable_; }

	std::wstringstream& GetStream() { return mStringStream; }

private:
    LogPriority mPriorityStdout;
    LogPriority mPriorityStderr;
    LogPriority mPriorityFile;
	LogPriority mPriorityAndroid;
    LogPriority mPriorityStream;
    FILE *      mFile;
    std::wstringstream mStringStream;
    bool        mUseFlush;
};

// A global instance for easy access. Not having to type much is important for logging.
extern Logging LOG;

#endif //  LOGGING_H
