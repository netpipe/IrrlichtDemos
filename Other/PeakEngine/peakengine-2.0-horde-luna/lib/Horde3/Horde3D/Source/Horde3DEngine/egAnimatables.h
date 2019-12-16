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

#ifndef _egAnimatables_H_
#define _egAnimatables_H_

#include "egPrerequisites.h"
#include "egScene.h"
#include "egMaterial.h"
#include "utMath.h"

class ModelNode;

// The following class is not real SceneNode, rather some sort of interface
class AnimatableSceneNode : public SceneNode
{
protected:

	uint32			_animUpdateStamp;
	float			_weightAccum;
	Quaternion		_rotQuat;
	Vec3f			_transVec, _scaleVec;

public:

	AnimatableSceneNode( const SceneNodeTpl &tpl ) :
		SceneNode( tpl ), _animUpdateStamp( 0 ), _weightAccum( 0 )
	{
	}

	virtual void onPreUpdate();

	friend class ModelNode;
};


struct MeshNodeParams
{
	enum List
	{
		MaterialRes = 300,
		BatchStart,
		BatchCount,
		VertRStart,
		VertREnd
	};
};

struct MeshNodeTpl : public SceneNodeTpl
{
	PMaterialResource	matRes;
	uint32				batchStart, batchCount;
	uint32				vertRStart, vertREnd;

	MeshNodeTpl( const string &name, MaterialResource *materialRes, uint32 batchStart,
				 uint32 batchCount, uint32 vertRStart, uint32 vertREnd) :
		SceneNodeTpl( SceneNodeTypes::Mesh, name ), matRes( materialRes ), batchStart( batchStart ),
		batchCount( batchCount ), vertRStart( vertRStart ), vertREnd( vertREnd )
	{
	}
};

class MeshNode : public AnimatableSceneNode
{
protected:

	bool				_bBoxDirty;
	PMaterialResource	_materialRes;
	uint32				_batchStart, _batchCount;
	uint32				_vertRStart, _vertREnd;
	
	ModelNode			*_parentModel;
	BoundingBox			_localBBox;

	MeshNode( const MeshNodeTpl &meshTpl );

public:

	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );

	void markBBoxesDirty();
	BoundingBox *getLocalBBox() { return &_localBBox; }
	bool canAttach( SceneNode &parent );
	int getParami( int param );
	bool setParami( int param, int value );
	bool checkIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, Vec3f &intsPos ) const;

	void onAttach( SceneNode &parentNode );
	void onDetach( SceneNode &parentNode );
	void onPreUpdate();

	MaterialResource *getMaterialRes() { return _materialRes; }
	uint32 getBatchStart() { return _batchStart; }
	uint32 getBatchCount() { return _batchCount; }
	uint32 getVertRStart() { return _vertRStart; }
	uint32 getVertREnd() { return _vertREnd; }
	ModelNode *getParentModel() { return _parentModel; }

	friend class ModelNode;
};


struct JointNodeParams
{
	enum List
	{
		JointIndex = 400,
	};
};

struct JointNodeTpl : public SceneNodeTpl
{
	uint32	jointIndex;

	JointNodeTpl( const string &name, uint32 jointIndex ) :
		SceneNodeTpl( SceneNodeTypes::Joint, name ), jointIndex( jointIndex )
	{
	}
};

class JointNode : public AnimatableSceneNode
{
protected:

	ModelNode	*_parentModel;
	uint32		_jointIndex;
	Matrix4f	_relModelMat;		// Transformation relative to parent model

	JointNode( const JointNodeTpl &jointTpl );

public:
	
	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );
	
	bool canAttach( SceneNode &parent );
	int getParami( int param );

	void onPostUpdate();
	void onAttach( SceneNode &parentNode );
	void onDetach( SceneNode &parentNode );

	friend class ModelNode;
};

#endif // _egAnimatables_H_
