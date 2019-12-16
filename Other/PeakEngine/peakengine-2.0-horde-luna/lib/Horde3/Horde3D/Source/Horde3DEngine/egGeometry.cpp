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

#include "egGeometry.h"
#include "egAnimation.h"
#include "egResource.h"
#include "egModules.h"

#include "utDebug.h"


uint32 GeometryResource::defVertBuffer = 0;
uint32 GeometryResource::defIndexBuffer = 0;


void GeometryResource::initializationFunc()
{
	defVertBuffer = Modules::renderer().uploadVertices( 0x0, 0 );
	defIndexBuffer = Modules::renderer().uploadIndices( 0x0, 0 );
}


void GeometryResource::releaseFunc()
{
	Modules::renderer().unloadBuffers( defVertBuffer, defIndexBuffer );
}


GeometryResource::GeometryResource( const string &name, int flags ) :
	Resource( ResourceTypes::Geometry, name, flags )
{
	initDefault();
}


GeometryResource::~GeometryResource()
{
	release();
}


Resource *GeometryResource::clone()
{
	GeometryResource *res = new GeometryResource( "", _flags );

	*res = *this;

	// Make a deep copy of the data
	res->_vertData = new VertexData( _vertCount );
	memcpy( res->_vertData->memory, _vertData->memory,
			_vertCount * (sizeof( Vec3f ) * 4 + sizeof( VertexDataStatic ) ) );
	res->_vertBuffer = Modules::renderer().cloneVertexBuffer( _vertBuffer );
	res->_indexBuffer = Modules::renderer().cloneIndexBuffer( _indexBuffer );
	
	return res;
}


void GeometryResource::initDefault()
{
	_vertCount = 0;
	_vertData = 0x0;
	_16BitIndices = false;
	_vertBuffer = defVertBuffer;
	_indexBuffer = defIndexBuffer;
	_minMorphIndex = 0; _maxMorphIndex = 0;
}


void GeometryResource::release()
{
	if( _vertBuffer != 0 && _vertBuffer != defVertBuffer )
	{
		Modules::renderer().unloadBuffers( _vertBuffer, 0 );
		_vertBuffer = 0;
	}
	
	if( _indexBuffer != 0 && _indexBuffer != defIndexBuffer )
	{
		Modules::renderer().unloadBuffers( 0, _indexBuffer );
		_indexBuffer = 0;
	}

	delete _vertData; _vertData = 0x0;
	_indices.clear();
	_joints.clear();
	_morphTargets.clear();
}


bool GeometryResource::raiseError( const string &msg )
{
	// Reset
	release();
	initDefault();

	Modules::log().writeError( "Geometry resource '%s': %s", _name.c_str(), msg.c_str() );
	
	return false;
}


bool GeometryResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;

	// Make sure header is available
	if( size < 8 )
		return raiseError( "Invalid geometry resource" );
	
	char *myData = (char *)data;
	
	// Check header and version
	char id[4];
	memcpy( &id, myData, 4 ); myData += 4;
	if( id[0] != 'H' || id[1] != '3' || id[2] != 'D' || id[3] != 'G' )
		return raiseError( "Invalid geometry resource" );

	uint32 version;
	memcpy( &version, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
	if( version != 5 ) return raiseError( "Unsupported version of geometry file" );

	// Load joints
	uint32 count;
	memcpy( &count, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

	if( count > 75 )
		Modules::log().writeWarning( "Geometry resource '%s': Model has more than 75 joints; this may cause defective behavior", _name.c_str() );

	_joints.resize( count );
	for( uint32 i = 0; i < count; ++i )
	{
		Joint &joint = _joints[i];
		
		// Inverse bind matrix
		for( uint32 j = 0; j < 16; ++j )
		{
			memcpy( &joint.invBindMat.x[j], myData, sizeof( float ) ); myData += sizeof( float );
		}
	}
	
	// Load vertex stream data
	uint32 streamSize;
	memcpy( &count, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );			// Number of streams
	memcpy( &streamSize, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );	// Number of vertices

	_vertCount = streamSize;
	_vertData = new VertexData( streamSize );
	for( uint32 i = 0; i < count; ++i )
	{
		unsigned char uc;
		short sh;
		uint32 streamID, streamElemSize;
		memcpy( &streamID, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
		memcpy( &streamElemSize, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

		switch( streamID )
		{
		case 0:		// Position
			if( streamElemSize != 12 ) return raiseError( "Invalid position base stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &_vertData->positions[j].x, myData, sizeof( float ) ); myData += sizeof( float );
				memcpy( &_vertData->positions[j].y, myData, sizeof( float ) ); myData += sizeof( float );
				memcpy( &_vertData->positions[j].z, myData, sizeof( float ) ); myData += sizeof( float );
			}
			break;
		case 1:		// Normal
			if( streamElemSize != 6 ) return raiseError( "Invalid normal base stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->normals[j].x = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->normals[j].y = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->normals[j].z = sh / 32767.0f;
			}
			break;
		case 2:		// Tangent
			if( streamElemSize != 6 ) return raiseError( "Invalid tangent base stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->tangents[j].x = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->tangents[j].y = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->tangents[j].z = sh / 32767.0f;
			}
			break;
		case 3:		// Bitangent
			if( streamElemSize != 6 ) return raiseError( "Invalid bitangent base stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->bitangents[j].x = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->bitangents[j].y = sh / 32767.0f;
				memcpy( &sh, myData, sizeof( short ) ); myData += sizeof( short ); _vertData->bitangents[j].z = sh / 32767.0f;
			}
			break;
		case 4:		// Joint indices
			if( streamElemSize != 4 ) return raiseError( "Invalid joint stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].jointVec[0] = (float)uc;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].jointVec[1] = (float)uc;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].jointVec[2] = (float)uc;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].jointVec[3] = (float)uc;
			}
			break;
		case 5:		// Weights
			if( streamElemSize != 4 ) return raiseError( "Invalid weight stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].weightVec[0] = uc / 255.0f;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].weightVec[1] = uc / 255.0f;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].weightVec[2] = uc / 255.0f;
				memcpy( &uc, myData, sizeof( char ) ); myData += sizeof( char ); _vertData->staticData[j].weightVec[3] = uc / 255.0f;
			}
			break;
		case 6:		// Texture Coord Set 1
			if( streamElemSize != 8 ) return raiseError( "Invalid texCoord1 stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &_vertData->staticData[j].u0, myData, sizeof( float ) ); myData += sizeof( float );
				memcpy( &_vertData->staticData[j].v0, myData, sizeof( float ) ); myData += sizeof( float );
			}
			break;
		case 7:		// Texture Coord Set 2
			if( streamElemSize != 8 ) return raiseError( "Invalid texCoord2 stream" );
			for( uint32 j = 0; j < streamSize; ++j )
			{
				memcpy( &_vertData->staticData[j].u1, myData, sizeof( float ) ); myData += sizeof( float );
				memcpy( &_vertData->staticData[j].v1, myData, sizeof( float ) ); myData += sizeof( float );
			}
			break;
		default:
			myData += streamElemSize * streamSize;
			Modules::log().writeWarning( "Geometry resource '%s': Ignoring unsupported vertex base stream", _name.c_str() );
			continue;
		}
	}
		
	// Load triangle indices
	memcpy( &count, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

	_indices.resize( count );
	for( uint32 i = 0; i < count; ++i )
	{
		memcpy( &_indices[i], myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
	}

	// Load morph targets
	uint32 numTargets;
	memcpy( &numTargets, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

	_morphTargets.resize( numTargets );
	for( uint32 i = 0; i < numTargets; ++i )
	{
		MorphTarget &mt = _morphTargets[i];
		char name[256];
		
		memcpy( name, myData, 256 ); myData += 256;
		mt.name = name;
		
		// Read vertex indices
		uint32 streamSize;
		memcpy( &streamSize, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
		mt.diffs.resize( streamSize );
		for( uint32 j = 0; j < streamSize; ++j )
		{
			memcpy( &mt.diffs[j].vertIndex, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
		}
		
		// Loop over streams
		memcpy( &count, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
		for( uint32 j = 0; j < count; ++j )
		{
			uint32 streamID, streamElemSize;
			memcpy( &streamID, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );
			memcpy( &streamElemSize, myData, sizeof( uint32 ) ); myData += sizeof( uint32 );

			switch( streamID )
			{
			case 0:		// Position
				if( streamElemSize != 12 ) return raiseError( "Invalid position morph stream" );
				for( uint32 k = 0; k < streamSize; ++k )
				{
					memcpy( &mt.diffs[k].posDiff.x, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].posDiff.y, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].posDiff.z, myData, sizeof( float ) ); myData += sizeof( float );
				}
				break;
			case 1:		// Normal
				if( streamElemSize != 12 ) return raiseError( "Invalid normal morph stream" );
				for( uint32 k = 0; k < streamSize; ++k )
				{
					memcpy( &mt.diffs[k].normDiff.x, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].normDiff.y, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].normDiff.z, myData, sizeof( float ) ); myData += sizeof( float );
				}
				break;
			case 2:		// Tangent
				if( streamElemSize != 12 ) return raiseError( "Invalid tangent morph stream" );
				for( uint32 k = 0; k < streamSize; ++k )
				{
					memcpy( &mt.diffs[k].tanDiff.x, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].tanDiff.y, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].tanDiff.z, myData, sizeof( float ) ); myData += sizeof( float );
				}
				break;
			case 3:		// Bitangent
				if( streamElemSize != 12 ) return raiseError( "Invalid bitangent morph stream" );
				for( uint32 k = 0; k < streamSize; ++k )
				{
					memcpy( &mt.diffs[k].bitanDiff.x, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].bitanDiff.y, myData, sizeof( float ) ); myData += sizeof( float );
					memcpy( &mt.diffs[k].bitanDiff.z, myData, sizeof( float ) ); myData += sizeof( float );
				}
				break;
			default:
				myData += streamElemSize * streamSize;
				Modules::log().writeWarning( "Geometry resource '%s': Ignoring unsupported vertex morph stream", _name.c_str() );
				continue;
			}
		}
	}

	// Find min/max morph target vertex indices
	_minMorphIndex = (unsigned)_vertCount;
	_maxMorphIndex = 0;
	for( uint32 i = 0; i < _morphTargets.size(); ++i )
	{
		for( uint32 j = 0; j < _morphTargets[i].diffs.size(); ++j )
		{
			_minMorphIndex = min( _minMorphIndex, _morphTargets[i].diffs[j].vertIndex );
			_maxMorphIndex = max( _maxMorphIndex, _morphTargets[i].diffs[j].vertIndex );
		}
	}
	if( _minMorphIndex > _maxMorphIndex )
	{
		_minMorphIndex = 0; _maxMorphIndex = 0;
	}

	// Add default joint if necessary
	if( _joints.empty() )
	{
		_joints.push_back( Joint() );
	}

	// Upload data
	if( _vertCount > 0 && _indices.size() > 0 )
	{
		// Upload vertices
		_vertBuffer = Modules::renderer().uploadVertices( _vertData->memory,
			_vertCount * (sizeof( Vec3f ) * 4 + sizeof( VertexDataStatic )) );
		
		// Upload indices (convert indices to 16 bit if possible)
		if( _indices.size() < 65000 )
		{
			_16BitIndices = true;
			unsigned short *indices = new unsigned short[_indices.size()];
			for( uint32 i = 0; i < _indices.size(); ++i )
				indices[i] = (unsigned short)_indices[i];
			_indexBuffer = Modules::renderer().uploadIndices(
				indices, (uint32)_indices.size() * sizeof( short ) );
			delete[] indices;
		}
		else
		{
			_indexBuffer = Modules::renderer().uploadIndices(
				&_indices[0], (uint32)_indices.size() * sizeof( uint32 ) );
		}
	}
	
	return true;
}


int GeometryResource::getParami( int param )
{
	switch( param )
	{
	case GeometryResParams::IndexCount:
		return (int)_indices.size();
	case GeometryResParams::VertexCount:
		return (int)_vertCount;
	default:
		return Resource::getParami( param );
	}
}


const void *GeometryResource::getData( int param )
{
	switch( param )
	{
	case GeometryResParams::IndexData:
		return _indices.empty() ? 0x0 : &_indices[0];
	case GeometryResParams::VertexData:
		return _vertData != 0x0 ? _vertData->positions : 0x0;
	default:
		return Resource::getData( param );
	}
}


void GeometryResource::updateDynamicVertData()
{
	// Upload dynamic stream data
	if( _vertData != 0x0 )
	{
		Modules::renderer().updateVertices( _vertData->memory, 0, 
			_vertCount * (sizeof( Vec3f ) * 4 + sizeof( VertexDataStatic )), _vertBuffer );
	}
}
