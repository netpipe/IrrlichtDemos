/*!
	@file		IrrLogManager.cpp
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

//#define _HAS_EXCEPTIONS 0 //This is a test. I need fix this problem

#include "IrrLogManager.h"
//#include "IrrStringUtil.h"
#include "IrrPlatformInformation.h"
//#include <sstream>

namespace irr
{

	const std::string LogManager::LevelsName[EndLogLevel] =
	{
		"Info       ",
		"Warning    ",
		"Error      ",
		"Critical   ",
		"Information",
		"None       "
	};

	const std::string LogManager::General = "General";
	const std::string LogManager::separator = "  |  ";

	LogStreamEnd LogManager::endl;
	LogManager* LogManager::msInstance = 0;

	LogManager::LogManager()
	{
		msInstance = this;
		mSTDOut = true;
	}

	LogManager::~LogManager()
	{
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			LogStream * stream = iter->second;
			if (stream == 0) continue;

			// looking for all such flows, and reset the
			for (MapLogStream::iterator iter2=iter; iter2!=mapStream.end(); ++iter2) {
				if (iter2->second == stream) iter2->second = 0;
			}
			delete stream;
		}
		mapStream.clear();
		msInstance = 0;
	}

	void LogManager::shutdown()
	{
		if (0 != msInstance) delete msInstance;
	}

	void LogManager::initialise()
	{
		if (0 == msInstance) new LogManager();
	}

	LogStream& LogManager::out(const std::string& _section, LoggingLevel _level)
	{
		static LogStream empty;

		if (0 == msInstance) return empty;

		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end()) return empty;

		if (_level >= EndLogLevel) _level = Info;

		iter->second->start(_section, LevelsName[_level]);

		return *(iter->second);
	}

	//-----------------------------------------------------------------------
    void LogManager::logMessage( const String& message, LoggingLevel lml )
    {
		IRR_LOGGING_OUT(IRR_LOG_SECTION, Info, message);
    }
	//-----------------------------------------------------------------------
	void LogManager::registerSection(const std::string& _section, const std::string& _file)
	{
		if (0 == msInstance) new LogManager();

		// looking for the same section and remove it
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter != mapStream.end()) {
			delete iter->second;
			mapStream.erase(iter);
		}

		// looking stream with the same name, if not, create
		LogStream * stream = 0;
		for (MapLogStream::iterator iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			if (iter->second->getFileName() == _file) {
				stream = iter->second;
				break;
			}
		}
		if (0 == stream) stream = new LogStream(_file);

		mapStream[_section] = stream;

		irr::PlatformInformation::logPlatformInformation();

	}

	void LogManager::unregisterSection(const std::string& _section)
	{
		MapLogStream & mapStream = msInstance->mMapSectionFileName;
		MapLogStream::iterator iter = mapStream.find(_section);
		if (iter == mapStream.end()) return;

		LogStream * stream = iter->second;
		mapStream.erase(iter);

		// If the file is still using it do not delete
		for (iter=mapStream.begin(); iter!=mapStream.end(); ++iter) {
			if (iter->second == stream) return;
		}

		delete stream;
	}

	const std::string& LogManager::info(const char * _file /* = __FILE__*/, int _line /* = __LINE__*/)
	{
		std::ostringstream stream;
		stream << separator << _file << separator << _line;

		static std::string ret;
		ret = stream.str();
		return ret;
	}

	const LogStreamEnd& LogManager::end()
	{
		return endl;
	}

	void LogManager::setSTDOutputEnabled(bool _enable)
	{
		assert(msInstance);
		msInstance->mSTDOut = _enable;
	}

	bool LogManager::getSTDOutputEnabled()
	{
		assert(msInstance);
		return msInstance->mSTDOut;
	}

} // End of  irr namespace section
