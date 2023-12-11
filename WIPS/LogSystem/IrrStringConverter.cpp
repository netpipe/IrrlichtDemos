/*!
	@file		IrrStringConverter.cpp
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

#include "IrrStringConverter.h"


namespace irr {

    //-----------------------------------------------------------------------
    String StringConverter::toString(Real val, unsigned short precision, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringUtil::StrStreamType stream;
        stream.precision(precision);
        stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(int val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringUtil::StrStreamType stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(long val, 
        unsigned short width, char fill, std::ios::fmtflags flags)
    {
        StringUtil::StrStreamType stream;
		stream.width(width);
        stream.fill(fill);
        if (flags)
            stream.setf(flags);
        stream << val;
        return stream.str();
    }
    //-----------------------------------------------------------------------
    String StringConverter::toString(bool val, bool yesNo)
    {
        if (val)
        {
            if (yesNo)
            {
                return "yes";
            }
            else
            {
                return "true";
            }
        }
        else
            if (yesNo)
            {
                return "no";
            }
            else
            {
                return "false";
            }
    }
    //-----------------------------------------------------------------------
    Real StringConverter::parseReal(const String& val)
    {
		// Use istringstream for direct correspondence with toString
		std::istringstream str(val);
		Real ret = 0;
		str >> ret;

        return ret;
    }
    //-----------------------------------------------------------------------
    int StringConverter::parseInt(const String& val)
    {
		// Use istringstream for direct correspondence with toString
		std::istringstream str(val);
		int ret = 0;
		str >> ret;

        return ret;
    }
    //-----------------------------------------------------------------------
    unsigned int StringConverter::parseUnsignedInt(const String& val)
    {
		// Use istringstream for direct correspondence with toString
		std::istringstream str(val);
		unsigned int ret = 0;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    long StringConverter::parseLong(const String& val)
    {
		// Use istringstream for direct correspondence with toString
		std::istringstream str(val);
		long ret = 0;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    unsigned long StringConverter::parseUnsignedLong(const String& val)
    {
		// Use istringstream for direct correspondence with toString
		std::istringstream str(val);
		unsigned long ret = 0;
		str >> ret;

		return ret;
    }
    //-----------------------------------------------------------------------
    bool StringConverter::parseBool(const String& val)
    {
		return (StringUtil::startsWith(val, "true") || StringUtil::startsWith(val, "yes")
			|| StringUtil::startsWith(val, "1"));
    }
	//-----------------------------------------------------------------------
	bool StringConverter::isNumber(const String& val)
	{
		std::istringstream str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
	}
}// End of  irr namespace section


