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

#include "egCom.h"
#include "egModules.h"
#include <stdarg.h>

#include "utDebug.h"


// *************************************************************************************************
// Class EngineConfig
// *************************************************************************************************

EngineConfig::EngineConfig()
{
	maxLogLevel = 4;
	trilinearFiltering = true;
	anisotropyFactor = 1;
	texCompression = false;
	loadTextures = true;
	fastAnimation = true;
	occlusionCulling = true;
	shadowMapSize = 1024;
	sampleCount = 0;
	wireframeMode = false;
	debugViewMode = false;
}


float EngineConfig::getOption( EngineOptions::List param )
{
	switch( param )
	{
	case EngineOptions::MaxLogLevel:
		return (float)maxLogLevel;
	case EngineOptions::MaxNumMessages:
		return (float)Modules::log().getMaxNumMessages();
	case EngineOptions::TrilinearFiltering:
		return trilinearFiltering ? 1.0f : 0.0f;
	case EngineOptions::AnisotropyFactor:
		return (float)anisotropyFactor;
	case EngineOptions::TexCompression:
		return texCompression ? 1.0f : 0.0f;
	case EngineOptions::LoadTextures:
		return loadTextures ? 1.0f : 0.0f;
	case EngineOptions::FastAnimation:
		return fastAnimation ? 1.0f : 0.0f;
	case EngineOptions::ShadowMapSize:
		return (float)shadowMapSize;
	case EngineOptions::SampleCount:
		return (float)sampleCount;
	case EngineOptions::WireframeMode:
		return wireframeMode ? 1.0f : 0.0f;
	case EngineOptions::DebugViewMode:
		return debugViewMode ? 1.0f : 0.0f;
	default:
		return Math::NaN;
	}
}


bool EngineConfig::setOption( EngineOptions::List param, float value )
{
	int size;
	
	switch( param )
	{
	case EngineOptions::MaxLogLevel:
		maxLogLevel = (int)value;
		return true;
	case EngineOptions::MaxNumMessages:
		Modules::log().setMaxNumMessages( (uint32) value );
		return true;
	case EngineOptions::TrilinearFiltering:
		trilinearFiltering = (value != 0);
		return true;
	case EngineOptions::AnisotropyFactor:
		anisotropyFactor = (int)value;
		return true;
	case EngineOptions::TexCompression:
		texCompression = (value != 0);
		return true;
	case EngineOptions::LoadTextures:
		loadTextures = (value != 0);
		return true;
	case EngineOptions::FastAnimation:
		fastAnimation = (value != 0);
		return true;
	case EngineOptions::ShadowMapSize:
		size = (int)value;

		if( size == shadowMapSize ) return true;
		if( size != 128 && size != 256 && size != 512 && size != 1024 && size != 2048 ) return false;

		// Update shadow map
		Modules::renderer().destroyShadowBuffer();
		
		if( !Modules::renderer().createShadowBuffer( size, size ) )
		{
			Modules::log().writeWarning( "Failed to create shadow map" );
			// Restore old buffer
			Modules::renderer().createShadowBuffer( shadowMapSize, shadowMapSize );
			return false;
		}
		else
		{
			shadowMapSize = size;
			return true;
		}
	case EngineOptions::SampleCount:
		sampleCount = (int)value;
		return true;
	case EngineOptions::WireframeMode:
		wireframeMode = (value != 0);
		return true;
	case EngineOptions::DebugViewMode:
		debugViewMode = (value != 0);
		return true;
	default:
		return false;
	}
}


// *************************************************************************************************
// Class EngineLog
// *************************************************************************************************

EngineLog::EngineLog()
{
	_firstTick = clock();
	_maxNumMessages = 512;
}


void EngineLog::pushMessage( int level, const char *msg, va_list args )
{
	float time = (clock() - _firstTick) / (float)CLOCKS_PER_SEC;
	
	if( _messages.size() < _maxNumMessages - 1 )
	{
		#pragma warning( push )
		#pragma warning( disable:4996 )
	//	vsnprintf( _textBuf, 2048, msg, args );
		#pragma warning( pop )

		_messages.push( LogMessage( _textBuf, level, time ) );
	}
	else if( _messages.size() == _maxNumMessages - 1 )
	{
		_messages.push( LogMessage( "Message queue is full", 1, time ) );
	}
}


void EngineLog::writeError( const char *msg, ... )
{
	if( Modules::config().maxLogLevel < 1 ) return;

	va_list args;
	va_start( args, msg );
	pushMessage( 1, msg, args );
	va_end( args );
}


void EngineLog::writeWarning( const char *msg, ... )
{
	if( Modules::config().maxLogLevel < 2 ) return;

	va_list args;
	va_start( args, msg );
	pushMessage( 2, msg, args );
	va_end( args );
}


void EngineLog::writeInfo( const char *msg, ... )
{
	if( Modules::config().maxLogLevel < 3 ) return;

	va_list args;
	va_start( args, msg );
	pushMessage( 3, msg, args );
	va_end( args );
}


void EngineLog::writeDebugInfo( const char *msg, ... )
{
	if( Modules::config().maxLogLevel < 4 ) return;

	va_list args;
	va_start( args, msg );
	pushMessage( 4, msg, args );
	va_end( args );
}


bool EngineLog::getMessage( LogMessage &msg )
{
	if( !_messages.empty() )
	{
		msg = _messages.front();
		_messages.pop();
		return true;
	}
	else
		return false;
}
