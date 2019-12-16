// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _egExtensions_H_
#define _egExtensions_H_

#include "egPrerequisites.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
using namespace std;


typedef const char *(*ExtensionNameFunc)();
typedef bool (*ExtensionInitFunc)();
typedef void (*ExtensionReleaseFunc)();

struct Extension
{
	string					name;
	ExtensionInitFunc		initFunc;
	ExtensionReleaseFunc	releaseFunc;
};

class ExtensionManager
{
protected:

	vector< Extension >		_extensions;

	void installExtension( ExtensionNameFunc nameFunc, ExtensionInitFunc initFunc,
						   ExtensionReleaseFunc releaseFunc )
	{
		Extension ext;
		ext.name = (*nameFunc)();
		ext.initFunc = initFunc;
		ext.releaseFunc = releaseFunc;
		_extensions.push_back( ext );
	}

public:

	~ExtensionManager()
	{
		for( uint32 i = 0; i < _extensions.size(); ++i )
		{	
			(*_extensions[i].releaseFunc)();
		}
	}
	
	bool init()
	{
		installExtensions();
		for( uint32 i = 0; i < _extensions.size(); ++i )
		{
			if( !(*_extensions[i].initFunc)() ) return false;
		}
		return true;
	}

	bool checkExtension( const string &name )
	{
		for( uint32 i = 0; i < _extensions.size(); ++i )
		{
			if( _extensions[i].name == name ) return true;
		}
		return false;
	}

	bool installExtensions();
};

#endif // _egExtensions_H_
