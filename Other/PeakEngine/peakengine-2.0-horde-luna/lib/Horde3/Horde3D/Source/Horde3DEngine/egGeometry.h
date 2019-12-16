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

#ifndef _egGeometry_H_
#define _egGeometry_H_

#include "egPrerequisites.h"
#include "egResource.h"
#include "egScene.h"
#include "egAnimation.h"
#include "utMath.h"


struct GeometryResParams
{
	enum List
	{
		VertexCount = 200,
		IndexCount,
		VertexData,
		IndexData
	};
};


struct VertexDataStatic		// Static data
{
	float			u0, v0;
	float			jointVec[4];		// Float faster than int for GPU
	float			weightVec[4];
	float			u1, v1;
};

struct VertexData
{
private:

	char				*memory;	// Continous memory block for data
	
public:

	// Dynamic stream data
	Vec3f				*positions;
	Vec3f				*normals;
	Vec3f				*tangents;
	Vec3f				*bitangents;

	// Interleaved static data
	VertexDataStatic	*staticData;

	friend class GeometryResource;
	friend class ModelNode;


	VertexData( uint32 vertCount )
	{
		memory = new char[(sizeof( Vec3f ) * 4 + sizeof( VertexDataStatic )) * vertCount];
		
		positions = (Vec3f *)memory;
		normals = (Vec3f *)(memory + (sizeof( Vec3f ) * vertCount));
		tangents = (Vec3f *)(memory + (sizeof( Vec3f ) * 2 * vertCount));
		bitangents = (Vec3f *)(memory + (sizeof( Vec3f ) * 3 * vertCount));
		staticData = (VertexDataStatic *)(memory + (sizeof( Vec3f ) * 4 * vertCount));

		// Fill with default data
		memset( memory, 0, (sizeof( Vec3f ) * 4 + sizeof( VertexDataStatic )) * vertCount );
		for( uint32 i = 0; i < vertCount; ++i ) staticData[i].weightVec[0] = 1;
	}

	~VertexData()
	{
		delete[] memory;
	}
};


struct Joint
{
	Matrix4f		invBindMat;
};


struct MorphDiff
{
	uint32		vertIndex;
	Vec3f		posDiff;
	Vec3f		normDiff, tanDiff, bitanDiff;
};


struct MorphTarget
{
	string					name;
	vector< MorphDiff >		diffs;
};


class GeometryResource : public Resource
{
private:

	uint32					_vertCount;
	VertexData				*_vertData;	
	bool					_16BitIndices;
	vector< uint32 >		_indices;
	
	vector< Joint >			_joints;
	vector< MorphTarget >	_morphTargets;
	uint32					_minMorphIndex, _maxMorphIndex;

	uint32					_vertBuffer, _indexBuffer;

	bool raiseError( const string &msg );

public:

	static uint32 defVertBuffer, defIndexBuffer;
	
	static void initializationFunc();
	static void releaseFunc();
	static Resource *factoryFunc( const string &name, int flags )
		{ return new GeometryResource( name, flags ); }
	
	GeometryResource( const string &name, int flags );
	~GeometryResource();
	Resource *clone();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );

	int getParami( int param );
	const void *getData( int param );

	void updateDynamicVertData();

	uint32 getVertCount() { return _vertCount; }
	VertexData *getVertData() { return _vertData; }
	uint32 getVertBuffer() { return _vertBuffer; }
	uint32 getIndexBuffer() { return _indexBuffer; }
	Matrix4f &getInvBindMat( uint32 jointIndex ) { return _joints[jointIndex].invBindMat; }

	friend class Renderer;
	friend class ModelNode;
	friend class MeshNode;
};

typedef SmartResPtr< GeometryResource > PGeometryResource;

#endif //_egGeometry_H_
