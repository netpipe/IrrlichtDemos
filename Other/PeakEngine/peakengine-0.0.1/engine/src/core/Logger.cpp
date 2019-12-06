/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "core/Logger.h"

#include <cstdarg>
#include <ctime>

namespace peak
{
	void printTimestamp(FILE *file)
	{
		time_t rawtime;
		time(&rawtime);
		struct tm *timeinfo = localtime(&rawtime);
		char timestr[20];
		strftime(timestr, 20, "%x %X", timeinfo);
		fprintf(file, "%s: ", timestr);
	}
	void printPrefix(FILE *file, EPrefixType type, ELoggingLevel level)
	{
		// Check if level has a prefix
		if (!(type & (1 << level))) return;
		if (type & EPT_Abbreviations)
		{
			// Print abbreviation
			switch (level)
			{
				case ELL_Error:
					fprintf(file, "E: ");
					break;
				case ELL_Warning:
					fprintf(file, "W: ");
					break;
				case ELL_Information:
					fprintf(file, "I: ");
					break;
				case ELL_Debug:
					fprintf(file, "D: ");
					break;
				default:
					fprintf(file, "   ");
					break;
			}
		}
		else
		{
			// Print full prefix
			switch (level)
			{
				case ELL_Error:
					fprintf(file, "Error: ");
					break;
				case ELL_Warning:
					fprintf(file, "Warning: ");
					break;
				case ELL_Information:
					fprintf(file, "Information: ");
					break;
				case ELL_Debug:
					fprintf(file, "Debug: ");
					break;
				default:
					break;

			}
		}
	}

	Logger *Logger::get(void)
	{
		static Logger logger;
		return &logger;
	}
	Logger::~Logger()
	{
	}

	bool Logger::write(ELoggingLevel level, std::string format, ...)
	{
		if ((level < consolelevel) && ((level < filelevel) || !file))
		{
			return false;
		}

		std::va_list args;
		if (level >= consolelevel)
		{
			// Log to stdout
			va_start(args, format);
			if (consoleprefix & EPT_BeforeTimestamp)
			{
				printPrefix(stdout, consoleprefix, level);
			}
			if (consoletimestamps)
			{
				printTimestamp(stdout);
			}
			if (!(consoleprefix & EPT_BeforeTimestamp))
			{
				printPrefix(stdout, consoleprefix, level);
			}
			vprintf(format.c_str(), args);
			va_end(args);
		}
		if ((level >= filelevel) && file)
		{
			// Log to file
			va_start(args, format);
			if (fileprefix & EPT_BeforeTimestamp)
			{
				printPrefix(file, fileprefix, level);
			}
			if (filetimestamps)
			{
				printTimestamp(file);
			}
			if (!(fileprefix & EPT_BeforeTimestamp))
			{
				printPrefix(file, fileprefix, level);
			}
			vfprintf(file, format.c_str(), args);
			fflush(file);
			va_end(args);
		}
		return true;
	}
	bool Logger::log(ELoggingLevel level, std::string msg)
	{
		return write(level, msg);
	}

	bool Logger::setFile(std::string filename)
	{
		if (file)
		{
			std::fclose(file);
		}
		file = std::fopen(filename.c_str(), "wt+");
		if (!file)
		{
			this->filename = "";
			return false;
		}
		else
		{
			this->filename = filename;
			return true;
		}
	}
	std::string Logger::getFile(void)
	{
		return filename;
	}
	bool Logger::clearFile(void)
	{
		if (file)
		{
			std::fclose(file);
			file = std::fopen(filename.c_str(), "wt");
			fflush(file);
			return true;
		}
		else
		{
			return false;
		}
	}

	void Logger::setTimestamps(bool enabled)
	{
		consoletimestamps = enabled;
		filetimestamps = enabled;
	}
	void Logger::setConsoleTimestamps(bool enabled)
	{
		consoletimestamps = enabled;
	}
	bool Logger::getConsoleTimestamps(void)
	{
		return consoletimestamps;
	}
	void Logger::setFileTimestamps(bool enabled)
	{
		filetimestamps = enabled;
	}
	bool Logger::getFileTimestamps(void)
	{
		return filetimestamps;
	}

	void Logger::setPrefix(EPrefixType type)
	{
		consoleprefix = type;
		fileprefix = type;
	}
	void Logger::setConsolePrefix(EPrefixType type)
	{
		consoleprefix = type;
	}
	EPrefixType Logger::getConsolePrefix(void)
	{
		return consoleprefix;
	}
	void Logger::setFilePrefix(EPrefixType type)
	{
		fileprefix = type;
	}
	EPrefixType Logger::getFilePrefix(void)
	{
		return fileprefix;
	}

	void Logger::setMinimalConsoleLevel(ELoggingLevel level)
	{
		consolelevel = ELL_Warning;
	}
	ELoggingLevel Logger::getMinimalConsoleLevel(void)
	{
		return consolelevel;
	}
	void Logger::setMinimalFileLevel(ELoggingLevel level)
	{
		filelevel = ELL_Debug;
	}
	ELoggingLevel Logger::getMinimalFileLevel(void)
	{
		return filelevel;
	}

	Logger::Logger()
	{
		filename = "";
		file = 0;

		consoletimestamps = false;
		filetimestamps = true;
		consoleprefix = EPT_None;
		fileprefix = (EPrefixType)(EPT_All | EPT_Abbreviations | EPT_BeforeTimestamp);
		consolelevel = ELL_Warning;
		filelevel = ELL_Debug;
	}
}
