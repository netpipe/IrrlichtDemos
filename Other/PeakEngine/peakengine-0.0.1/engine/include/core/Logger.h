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

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "Util.h"

#include <string>
#include <cstdio>

//tolua_begin
namespace peak
{
	enum ELoggingLevel
	{
		ELL_Nothing = 4,
		ELL_Error = 3,
		ELL_Warning = 2,
		ELL_Information = 1,
		ELL_Debug = 0
	};
	enum EPrefixType
	{
		EPT_None = 0,
		EPT_Debug = 1,
		EPT_Information = 2,
		EPT_Warning = 4,
		EPT_Error = 8,
		EPT_All = 15,
		EPT_BeforeTimestamp = 16,
		EPT_Abbreviations = 32
	};
	
	//tolua_end
	// Macros for easier logging
	#define LDEBUG(fmt, ...) peak::Logger::get()->write(ELL_Debug, \
		trimFilename(__FILE__) + std::string(":%d: ") + fmt, __LINE__, ##__VA_ARGS__);
	#define LERROR(fmt, ...) peak::Logger::get()->write(ELL_Error, \
		trimFilename(__FILE__) + std::string(":%d: ") + fmt, __LINE__, ##__VA_ARGS__);
	#define LWARNING(fmt, ...) peak::Logger::get()->write(ELL_Warning, \
		trimFilename(__FILE__) + std::string(":%d: ") + fmt, __LINE__, ##__VA_ARGS__);
	#define LINFO(fmt, ...) peak::Logger::get()->write(ELL_Information, \
		trimFilename(__FILE__) + std::string(":%d: ") + fmt, __LINE__, ##__VA_ARGS__);
	//tolua_begin
	
	/**
	 * \brief Class for logging and debug output
	 */
	class Logger
	{
		public:
			/**
			 * \brief Returns pointer to the Logger class.
			 *
			 * \return Pointer to the logger
			 */
			static Logger *get(void);
			~Logger();
			
			//tolua_end
			/**
			 * \brief Write data to file/stdout
			 * \param level Level of the data
			 * \param format Format string (like with printf)
			 * \return Returns true if the message was printed
			 */
			bool write(ELoggingLevel level, std::string format, ...);
			//tolua_begin
			bool log(ELoggingLevel level, std::string msg);
			
			/**
			 * \brief Sets the output file
			 */
			bool setFile(std::string filename);
			/**
			 * \brief Returns the output file
			 */
			std::string getFile(void);
			/**
			 * \brief Clears the output file
			 */
			bool clearFile(void);
			
			/**
			 * \brief Sets whether timestamps are written
			 *
			 * This affects both stdout and files.
			 */
			void setTimestamps(bool enabled);
			/**
			 * \brief Sets whether timestamps are written to stdout
			 *
			 * Defaults to false.
			 */
			void setConsoleTimestamps(bool enabled);
			/**
			 * \brief Returns whether timestamps are written to stdout
			 */
			bool getConsoleTimestamps(void);
			/**
			 * \brief Sets whether timestamps are written to the file
			 *
			 * Defaults to true.
			 */
			void setFileTimestamps(bool enabled);
			/**
			 * \brief Returns whether timestamps are written to the file
			 */
			bool getFileTimestamps(void);
			
			/**
			 * \brief Sets whether prefixes like "Error: " are written
			 *
			 * This affects both stdout and files.
			 */
			void setPrefix(EPrefixType type);
			/**
			 * \brief Sets whether prefixes are written to stdout
			 *
			 * Defaults to EPT_None.
			 */
			void setConsolePrefix(EPrefixType type);
			/**
			 * \brief Returns whether prefixes are written to stdout
			 */
			EPrefixType getConsolePrefix(void);
			/**
			 * \brief Sets whether prefixes are written to files
			 *
			 * Defaults to EPT_All | EPT_Abbreviations | EPT_BeforeTimestamp.
			 */
			void setFilePrefix(EPrefixType type);
			/**
			 * \brief Returns whether prefixes are written to files
			 */
			EPrefixType getFilePrefix(void);
			
			/**
			 * \brief Sets the minimal console logging level.
			 *
			 * Messages with a smaller level are not printed to stdout.
			 *
			 * Defaults to ELL_Warning
			 */
			void setMinimalConsoleLevel(ELoggingLevel level);
			/**
			 * \brief Returns the minimal console logging level.
			 */
			ELoggingLevel getMinimalConsoleLevel(void);
			/**
			 * \brief Sets the minimal file logging level.
			 *
			 * Messages with a smaller level are not printed to the file.
			 *
			 * Defaults to ELL_Debug
			 */
			void setMinimalFileLevel(ELoggingLevel level);
			/**
			 * \brief Returns the minimal file logging level.
			 */
			ELoggingLevel getMinimalFileLevel(void);
		private:
			//tolua_end
			Logger();
			
			bool consoletimestamps;
			bool filetimestamps;
			
			EPrefixType consoleprefix;
			EPrefixType fileprefix;
			
			ELoggingLevel consolelevel;
			ELoggingLevel filelevel;
			
			std::FILE *file;
			std::string filename;
			//tolua_begin
	};
}
//tolua_end

#endif

