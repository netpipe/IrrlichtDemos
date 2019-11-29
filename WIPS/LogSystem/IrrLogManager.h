/*!
	@file		IrrLogManager.h
	@author		manguonden
	@date		11/2009
	@module
*//*
	This file is part of Irrlicht Engine.
	
	Irrlicht Engine is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	Irrlicht Engine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with Irrlicht Engine.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef __IRR_LOG_MANAGER_H__
#define __IRR_LOG_MANAGER_H__

#include "IrrLogStream.h"

namespace irr
{


	#define IRR_LOG_SECTION "General"
	#define IRR_LOG_FILENAME "Irrlicht.log"
	#define IRR_LOGGING(section, level, text) \
		irr::LogManager::out(section, irr::LogManager::level) \
		<< text \
		<< irr::LogManager::info(__FILE__, __LINE__) \
		<< irr::LogManager::end()
	#define IRR_LOG(level, text) IRR_LOGGING(IRR_LOG_SECTION, level, text)
	#define IRR_LOGGING_OUT(section, level, text) \
		irr::LogManager::out(section, irr::LogManager::level) \
		<< text \
		<< irr::LogManager::end()

	class LogStream;
	struct LogStreamEnd;
	typedef std::map<String, LogStream*> MapLogStream;

	class LogManager
	{

	public:
		enum LoggingLevel
		{
			Info,
			Warning,
			Error,
			Critical,
			Information,
			None,
			EndLogLevel
		};

	public:
		static void shutdown();
		static void initialise();

		static void registerSection(const std::string& _section, const std::string& _file);
		static void unregisterSection(const std::string& _section);

		static LogStream& out(const std::string& _section, LoggingLevel _level);
		static const std::string& info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/);

        static void logMessage( const String& message, LoggingLevel lml);

		static const LogStreamEnd& end();

		// set logging enabled on std output device
		static void setSTDOutputEnabled(bool _enable);
		static bool getSTDOutputEnabled();

	private:
		LogManager();
		~LogManager();

	public:
		static const std::string General;
		static const std::string separator;

		static LogStreamEnd endl;
		static const std::string LevelsName[EndLogLevel];

	private:
		static LogManager * msInstance;
		MapLogStream mMapSectionFileName;
		bool mSTDOut;

	};

} // End of  irr namespace section
#endif // __IRR_LOG_MANAGER_H__
