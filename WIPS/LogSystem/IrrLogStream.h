/*!
	@file		IrrLogStream.h
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

#ifndef __IRR_LOG_STREAM_H__
#define __IRR_LOG_STREAM_H__

#include "IrrStringUtil.h"
#include <cassert>
#include <map>

namespace irr
{

	struct LogStreamEnd { };

	class LogManager;

	class LogStream
	{
		friend class LogManager;

	public:
		LogStream& operator<<(const LogStreamEnd& _endl)
		{
			if (getSTDOutputEnabled()) std::cout << std::endl;
			if (mStream.is_open()) {
				mStream << std::endl;
				mStream.close();
			}
			release();

			return *this;
		}

		template <typename T>
		inline LogStream& operator<<(T _value)
		{
			if (getSTDOutputEnabled()) std::cout << _value;
			if (mStream.is_open()) mStream << _value;
			return *this;
		}

		const std::string& getFileName() {return mFileName;}

	private:
		LogStream();
		~LogStream();

		LogStream(const std::string& _file);

		void start(const std::string& _section, const std::string& _level);

		bool getSTDOutputEnabled();

		void lock()
		{
		}

		void release()
		{
		}

	private:
		std::ofstream mStream;
		std::string mFileName;
	};

} // End of  irr namespace section

#endif // __IRR_LOG_STREAM_H__
