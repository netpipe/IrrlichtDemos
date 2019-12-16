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

#ifndef _egCom_H_
#define _egCom_H_

#include "egPrerequisites.h"
#include <stdarg.h>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <queue>
#include <vector>
using namespace std;


struct EngineOptions
{
	enum List
	{
		MaxLogLevel = 1,
		MaxNumMessages,
		TrilinearFiltering,
		AnisotropyFactor,
		TexCompression,
		LoadTextures,
		FastAnimation,
		ShadowMapSize,
		SampleCount,
		WireframeMode,
		DebugViewMode
	};
};

class EngineConfig
{
public:
	
	int		maxLogLevel;
	bool	trilinearFiltering;
	int		anisotropyFactor;
	bool	texCompression;
	bool	loadTextures;
	bool	fastAnimation;
	bool	occlusionCulling;
	int		shadowMapSize;
	int		sampleCount;
	bool	wireframeMode;
	bool	debugViewMode;


	EngineConfig();
	float getOption( EngineOptions::List param );
	bool setOption( EngineOptions::List param, float value );
};


struct LogMessage
{
	string		text;
	int			level;
	float		time;

	LogMessage()
	{
	}

	LogMessage( const string &text, int level, float time ) :
		text( text ), level( level ), time( time )
	{
	}
};


class EngineLog
{
protected:
	
	unsigned long			_firstTick;
	char					_textBuf[2048];
	uint32					_maxNumMessages;
	queue< LogMessage >		_messages;

	void pushMessage( const string &text, uint32 level );
	void pushMessage( int level, const char *msg, va_list ap );

public:
	
	EngineLog();

	void writeError( const char *msg, ... );
	void writeWarning( const char *msg, ... );
	void writeInfo( const char *msg, ... );
	void writeDebugInfo( const char *msg, ... );

	bool getMessage( LogMessage &msg );

	uint32 getMaxNumMessages() { return _maxNumMessages; }
	void setMaxNumMessages( uint32 maxNumMessages ) { _maxNumMessages = maxNumMessages; }
	
};

#endif // _egCom_H_
