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

#include "egAnimation.h"
#include "egModules.h"

#include "utDebug.h"


AnimationResource::AnimationResource( const string &name, int flags ) :
	Resource( ResourceTypes::Animation, name, flags )
{
	initDefault();	
}


AnimationResource::~AnimationResource()
{
	release();
}


Resource *AnimationResource::clone()
{
	AnimationResource *res = new AnimationResource( "", _flags );

	*res = *this;
	
	return res;
}


void AnimationResource::initDefault()
{
	_numFrames = 0;
}


void AnimationResource::release()
{
	_entities.clear();
}


bool AnimationResource::raiseError( const string &msg )
{
	// Reset
	release();
	initDefault();

	Modules::log().writeError( "Animation resource '%s': %s", _name.c_str(), msg.c_str() );
	
	return false;
}


bool AnimationResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;

	// Make sure header is available
	if( size < 8 )
		return raiseError( "Invalid animation resource" );
	
	char *myData = (char *)data;
	
	// Check header and version
	char id[4];
	memcpy( &id, myData, 4 ); myData += 4;
	if( id[0] != 'H' || id[1] != '3' || id[2] != 'D' || id[3] != 'A' )
		return raiseError( "Invalid animation resource" );
	
	uint32 version;
	memcpy( &version, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
	if( version != 2 && version != 3 )
		return raiseError( "Unsupported version of animation resource" );
	
	// Load animation data
	uint32 numEntities;
	memcpy( &numEntities, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
	memcpy( &_numFrames, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

	_entities.resize( numEntities );

	for( uint32 i = 0; i < numEntities; ++i )
	{
		char name[256], compressed = 0;
		AnimResEntity &entity = _entities[i];
		
		memcpy( name, myData, 256 ); myData += 256;
		entity.name = name;
		
		// Animation compression
		if( version == 3 )
		{
			memcpy( &compressed, myData, sizeof( char ) ); myData += sizeof( char ); 
		}

		entity.frames.resize( compressed ? 1 : _numFrames );
		for( uint32 j = 0; j < (compressed ? 1 : _numFrames); ++j )
		{
			Frame &frame = entity.frames[j];

			memcpy( &frame.rotQuat.x, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.rotQuat.y, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.rotQuat.z, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.rotQuat.w, myData, sizeof( float ) ); myData += sizeof( float );

			memcpy( &frame.transVec.x, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.transVec.y, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.transVec.z, myData, sizeof( float ) ); myData += sizeof( float );

			memcpy( &frame.scaleVec.x, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.scaleVec.y, myData, sizeof( float ) ); myData += sizeof( float );
			memcpy( &frame.scaleVec.z, myData, sizeof( float ) ); myData += sizeof( float );

			// Prebake transformation matrix for fast animation path
			frame.bakedTransMat.scale( frame.scaleVec.x, frame.scaleVec.y, frame.scaleVec.z );
			frame.bakedTransMat = Matrix4f( frame.rotQuat ) * frame.bakedTransMat;
			frame.bakedTransMat.translate( frame.transVec.x, frame.transVec.y, frame.transVec.z );
		}

		if( !entity.frames.empty() )
			entity.firstFrameInvTrans = entity.frames[0].bakedTransMat.inverted();
	}
	
	return true;
}


int AnimationResource::getParami( int param )
{
	switch( param )
	{
	case AnimationResParams::FrameCount:
		return _numFrames;
	default:
		return Resource::getParami( param );
	}
}


AnimResEntity *AnimationResource::findEntity( const string &name )
{
	for( uint32 i = 0; i < _entities.size(); ++i )
	{
		if( _entities[i].name == name ) return &_entities[i];
	}

	return 0x0;
}
