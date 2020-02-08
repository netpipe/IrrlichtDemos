// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "logging.h"
#include "compile_config.h"
#include <sstream>

#ifdef _IRR_ANDROID_PLATFORM_
#include <android/log.h>
#endif

Logging LOG;

Logging::Logging()
    : mPriorityStdout(LP_NONE)
    , mPriorityStderr(LP_INFO)
    , mPriorityFile(LP_NONE)
    , mPriorityAndroid(LP_NONE)
    , mPriorityStream(LP_NONE)
	, mFile(0)
	, mUseFlush(true)
{
}

Logging::~Logging()
{
    if ( mFile )
    {
        CloseFile();
    }
}

void Logging::SetStdoutPriority(LogPriority prio_)
{
    mPriorityStdout = prio_;
}

void Logging::SetStderrPriority(LogPriority prio_)
{
    mPriorityStderr = prio_;
}

void Logging::SetStreamPriority(LogPriority prio_)
{
	mPriorityStream = prio_;
}

bool Logging::OpenFile(LogPriority prio_, const char *file_, const char *mode_)
{
#ifndef _IRR_ANDROID_PLATFORM_
    mFile = fopen(file_, mode_);
#endif
    if ( !mFile )
        return false;
    mPriorityFile = prio_;
    return true;
}

void Logging::CloseFile()
{
    if ( mFile )
    {
        fclose(mFile);
        mFile = NULL;
    }
}

void Logging::Log(LogPriority priority_, const wchar_t * text_)
{
    if ( !text_ )
        return;

	if ( mPriorityStream <= priority_ )
	{
		mStringStream << text_;
	}

#ifndef _IRR_ANDROID_PLATFORM_
    if ( mPriorityStdout <= priority_ )
    {
        fwprintf(stdout, L"%ls", text_);
        if ( mUseFlush )
            fflush(stdout);
    }

    if ( mPriorityStderr <= priority_ )
    {
        fwprintf(stderr, L"%ls", text_);
        if ( mUseFlush )
            fflush(stderr);
    }

    if ( mFile && mPriorityFile <= priority_ )
    {
        fwprintf(mFile, L"%ls", text_);
        if ( mUseFlush )
            fflush(mFile);
    }
#else
	if ( mPriorityAndroid <= priority_ )
	{
		// Yeah - another conversion - lazy solution.
		// Also Android prevents logging long messages,
		// so we have to split up long messages (or maybe the limit is in
		// the netcat display... doesn't really matter - still an evil limit).
		std::wstring wmsg(text_);
		size_t len = wmsg.size();
		size_t start = 0;
		while ( start < len )
		{
			// Limit should be 4096 according to net, but was around 1000 for me for some reason.
			const std::size_t max_log_len = 1000;
			size_t blockSize = std::min(max_log_len, len-start);
			std::string msg(wmsg.begin()+start, wmsg.begin()+start+blockSize);
			start += blockSize;

			// We do the filtering on our side - so anything we send out should be logged.
			// We could also set Android log-levels I guess - doesn't matter too much.
			android_LogPriority p = ANDROID_LOG_INFO;
			// Using "Irrlicht" for now as it makes filtering with logcat easier (we want Irrlicht anyway)
			__android_log_print(p, "Irrlicht", "%s", msg.c_str());
		}
	}
#endif

}

void Logging::Log(LogPriority priority_, const char * text_)
{
    Log( priority_, std::string(text_) );
}

void Logging::Log(LogPriority priority_, const std::string& text_)
{
    Log( priority_, std::wstring( text_.begin(), text_.end() ) );
}

void Logging::Log(LogPriority priority_, const std::wstring& text_)
{
    Log ( priority_, text_.c_str() );
}

void Logging::LogLn(LogPriority priority_, const char * text_)
{
    std::string str;
    if ( text_ )
        str += text_;
    str += "\n";
    Log( priority_, str );
}

void Logging::LogLn(LogPriority priority_, const char * text_, const char * text2_)
{
    std::string str;
    if ( text_ )
        str += text_;
    if ( text2_ )
        str += text2_;
    str += "\n";
    Log( priority_, str );
}

void Logging::LogLn(LogPriority priority_, const char * text_, int value_)
{
    std::ostringstream strstream;
    if ( text_ )
        strstream << text_;
    strstream << value_ << std::endl;
    Log( priority_, strstream.str() );
}

void Logging::LogLn(LogPriority priority_, const char * text_, float value_)
{
    std::ostringstream strstream;
    if ( text_ )
        strstream << text_;
    strstream << value_ << std::endl;
    Log( priority_, strstream.str() );
}

void Logging::DebugLn(const char * text_, const char * text2_)
{
    LogLn( LP_DEBUG, text_, text2_);
}

void Logging::Debug(const char * text_)
{
    Log( LP_DEBUG, std::string(text_) );
}

void Logging::Debug(const wchar_t * text_)
{
    Log( LP_DEBUG, text_ );
}

void Logging::Debug(const std::string& text_)
{
    Log( LP_DEBUG, std::wstring( text_.begin(), text_.end() ) );
}

void Logging::Debug(const std::wstring& text_)
{
    Log ( LP_DEBUG, text_.c_str() );
}

void Logging::InfoLn(const char * text_,  const char * text2_)
{
    LogLn( LP_INFO, text_, text2_);
}

void Logging::Info(const char * text_)
{
    Log( LP_INFO, std::string(text_) );
}

void Logging::Info(const wchar_t * text_)
{
    Log( LP_INFO, text_ );
}

void Logging::Info(const std::string& text_)
{
    Log( LP_INFO, std::wstring( text_.begin(), text_.end() ) );
}

void Logging::Info(const std::wstring& text_)
{
    Log ( LP_INFO, text_.c_str() );
}

void Logging::WarnLn(const char * text_,  const char * text2_)
{
    LogLn( LP_WARN, text_, text2_);
}

void Logging::Warn(const char * text_)
{
    Log( LP_WARN, std::string(text_) );
}

void Logging::Warn(const wchar_t * text_)
{
    Log( LP_WARN, text_ );
}

void Logging::Warn(const std::string& text_)
{
    Log( LP_WARN, std::wstring( text_.begin(), text_.end() ) );
}

void Logging::Warn(const std::wstring& text_)
{
    Log ( LP_WARN, text_.c_str() );
}

void Logging::ErrorLn(const char * text_,  const char * text2_)
{
    LogLn( LP_ERROR, text_, text2_);
}

void Logging::Error(const char * text_)
{
    Log( LP_ERROR, std::string(text_) );
}

void Logging::Error(const wchar_t * text_)
{
    Log( LP_ERROR, text_ );
}

void Logging::Error(const std::string& text_)
{
    Log( LP_ERROR, std::wstring( text_.begin(), text_.end() ) );
}

void Logging::Error(const std::wstring& text_)
{
    Log ( LP_ERROR, text_.c_str() );
}
