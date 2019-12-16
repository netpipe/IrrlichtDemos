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

#ifndef _egAnimation_H_
#define _egAnimation_H_

#include "egPrerequisites.h"
#include "egResource.h"
#include "utMath.h"


struct AnimationResParams
{
	enum List
	{
		FrameCount = 300
	};
};

struct Frame
{
	Quaternion			rotQuat;
	Vec3f				transVec, scaleVec;
	Matrix4f			bakedTransMat;
};


struct AnimResEntity
{
	string				name;
	Matrix4f			firstFrameInvTrans;
	vector< Frame >		frames;
};

class AnimationResource : public Resource
{
private:

	uint32						_numFrames;
	vector< AnimResEntity >		_entities;

	bool raiseError( const string &msg );

public:

	static Resource *factoryFunc( const string &name, int flags )
		{ return new AnimationResource( name, flags ); }
	
	AnimationResource( const string &name, int flags );
	~AnimationResource();
	Resource *clone();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );
	int getParami( int param );

	AnimResEntity *findEntity( const string &name );

	friend class Renderer;
	friend class ModelNode;
};

typedef SmartResPtr< AnimationResource > PAnimationResource;

#endif // _egAnimation_H_

