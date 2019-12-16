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

#ifndef _egModel_H_
#define _egModel_H_

#include "egPrerequisites.h"
#include "egScene.h"
#include "egAnimatables.h"
#include "egGeometry.h"
#include "egAnimation.h"
#include "egMaterial.h"
#include "utMath.h"


const uint32 MaxNumAnimStages = 16;


struct ModelNodeParams
{
	enum List
	{
		GeometryRes = 200,
		SoftwareSkinning
	};
};


struct Morpher	// Morph modifier
{
	string		name;
	uint32		index;		// Index of morph target in Geometry resource
	float		weight;
};

struct AnimStage
{
	PAnimationResource			anim;
	string						startNode;
	bool						additive;
	float						animTime;
	float						blendWeight;
};

struct NodeListEntry
{
	AnimatableSceneNode			*node;
	AnimResEntity				*animEntities[MaxNumAnimStages];


	NodeListEntry()
	{
		node = 0x0;
		for( uint32 i = 0; i < MaxNumAnimStages; ++i ) animEntities[i] = 0x0;
	}

	NodeListEntry( AnimatableSceneNode *node )
	{
		NodeListEntry();
		this->node = node;
	}
};


struct ModelNodeTpl : public SceneNodeTpl
{
	PGeometryResource	geoRes;
	bool				softwareSkinning;

	ModelNodeTpl( const string &name, GeometryResource *geoRes ) :
		SceneNodeTpl( SceneNodeTypes::Model, name ), geoRes( geoRes ), softwareSkinning( false )
	{
	}
};

class ModelNode : public SceneNode
{
protected:

	GeometryResource			*_geometryRes;
	GeometryResource			*_baseGeoRes;
	vector< Vec4f >				_skinMatRows;
	bool						_softwareSkinning;
	
	bool						_morpherUsed, _morpherDirty;
	vector< Morpher >			_morphers;
	
	AnimStage					*_animStages[MaxNumAnimStages];
	bool						_animDirty;				// Animation has changed		
	bool						_nodeListDirty;			// An animatable node has been attached to model
	uint32						_animTimeStamp;

	uint32						_meshCount;			// Number of meshes in _animatedNodes
	vector< NodeListEntry >		_nodeList;			// List of the model's meshes followed by joints

	vector< uint32 >			_occQueries;
	vector< uint32 >			_lastVisited;

	ModelNode( const ModelNodeTpl &modelTpl );
	void recreateNodeListRec( SceneNode *node, bool firstCall );
	void updateStageAnimations( uint32 stage, const string &startNode );
	void markMeshBBoxesDirty();

public:

	~ModelNode();
	
	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );

	void recreateNodeList();
	bool setupAnimStage( int stage, uint32 animRes, const string &startNode, bool additive );
	bool setAnimParams( int stage, float time, float weight );
	bool setMorphParam( const string &targetName, float weight );

	int getParami( int param );
	bool setParami( int param, int value );

	void onPostUpdate();

	bool updateGeometry( bool skinningDirty );

	GeometryResource *getGeometryResource() { return _geometryRes; }
	bool jointExists( uint32 jointIndex ) { return jointIndex < _skinMatRows.size() / 3; }
	void setSkinningMat( uint32 index, const Matrix4f &mat )
		{ _skinMatRows[index * 3 + 0] = mat.getRow( 0 );
		  _skinMatRows[index * 3 + 1] = mat.getRow( 1 );
		  _skinMatRows[index * 3 + 2] = mat.getRow( 2 ); }
	void markNodeListDirty() { _nodeListDirty = true; }

	friend class Renderer;
};

#endif // _egModel_H_
