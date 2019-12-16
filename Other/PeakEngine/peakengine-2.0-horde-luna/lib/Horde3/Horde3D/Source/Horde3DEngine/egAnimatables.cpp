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

#include "egModel.h"
#include "egMaterial.h"
#include "egModules.h"
#include "utDebug.h"


// *************************************************************************************************
// Class AnimatableSceneNode
// *************************************************************************************************

void AnimatableSceneNode::onPreUpdate()
{
	if( _animUpdateStamp != 0 )		// Node was updated by animation
	{
		_animUpdateStamp = 0;
		_weightAccum = 0;

		// Build matrix from animation data
		Matrix4f mat;
		mat.scale( _scaleVec.x, _scaleVec.y, _scaleVec.z );
		mat = Matrix4f( _rotQuat ) * mat;
		mat.translate( _transVec.x, _transVec.y, _transVec.z );
		_relTrans = mat;
	}
}


// *************************************************************************************************
// Class MeshNode
// *************************************************************************************************

MeshNode::MeshNode( const MeshNodeTpl &meshTpl ) :
	AnimatableSceneNode( meshTpl ), _bBoxDirty( true ),
	_materialRes( meshTpl.matRes ), _batchStart( meshTpl.batchStart ), _batchCount( meshTpl.batchCount ),
	_vertRStart( meshTpl.vertRStart ), _vertREnd( meshTpl.vertREnd )
{
}


SceneNodeTpl *MeshNode::parsingFunc( map< string, string > &attribs )
{
	bool result = true;
	
	map< string, string >::iterator itr;
	MeshNodeTpl *meshTpl = new MeshNodeTpl( "", 0x0, 0, 0, 0, 0 );

	itr = attribs.find( "material" );
	if( itr != attribs.end() )
	{
		uint32 res = Modules::resMan().addResource( ResourceTypes::Material, itr->second, 0, false );
		if( res != 0 )
			meshTpl->matRes = (MaterialResource *)Modules::resMan().resolveResHandle( res );
	}
	else result = false;
	itr = attribs.find( "batchStart" );
	if( itr != attribs.end() ) meshTpl->batchStart = atoi( itr->second.c_str() );
	else result = false;
	itr = attribs.find( "batchCount" );
	if( itr != attribs.end() ) meshTpl->batchCount = atoi( itr->second.c_str() );
	else result = false;
	itr = attribs.find( "vertRStart" );
	if( itr != attribs.end() ) meshTpl->vertRStart = atoi( itr->second.c_str() );
	else result = false;
	itr = attribs.find( "vertREnd" );
	if( itr != attribs.end() ) meshTpl->vertREnd = atoi( itr->second.c_str() );
	else result = false;

	if( !result )
	{
		delete meshTpl; meshTpl = 0x0;
	}
	
	return meshTpl;
}


SceneNode *MeshNode::factoryFunc( const SceneNodeTpl &nodeTpl )
{
	if( nodeTpl.type != SceneNodeTypes::Mesh ) return 0x0;
	
	return new MeshNode( *(MeshNodeTpl *)&nodeTpl );
}


void MeshNode::markBBoxesDirty()
{
	_bBoxDirty = true;
	
	for( size_t i = 0, s = _children.size(); i < s; ++i )
	{
		if( _children[i]->getType() == SceneNodeTypes::Mesh )
			((MeshNode *)_children[i])->markBBoxesDirty();
	}
}


bool MeshNode::canAttach( SceneNode &parent )
{
	// Important: Meshes may not live outside of models
	return (parent.getType() == SceneNodeTypes::Model) ||
		   (parent.getType() == SceneNodeTypes::Mesh) ||
		   (parent.getType() == SceneNodeTypes::Joint);
}


int MeshNode::getParami( int param )
{
	switch( param )
	{
	case MeshNodeParams::MaterialRes:
		if( _materialRes != 0x0 ) return _materialRes->getHandle();
		else return 0;
	case MeshNodeParams::BatchStart:
		return _batchStart;
	case MeshNodeParams::BatchCount:
		return _batchCount;
	case MeshNodeParams::VertRStart:
		return _vertRStart;
	case MeshNodeParams::VertREnd:
		return _vertREnd;
	default:
		return SceneNode::getParami( param );
	}
}


bool MeshNode::setParami( int param, int value )
{
	Resource *res;
	
	switch( param )
	{
	case MeshNodeParams::MaterialRes:
		res = Modules::resMan().resolveResHandle( value );
		if( res == 0x0 || res->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource for Mesh node %i", _handle );
			return false;
		}
		_materialRes = (MaterialResource *)res;
		return true;
	default:
		return SceneNode::setParami( param, value );
	}
}


bool MeshNode::checkIntersection( const Vec3f &rayOrig, const Vec3f &rayDir, Vec3f &intsPos ) const
{
	GeometryResource *geoRes = _parentModel->getGeometryResource();
	if( geoRes == 0x0 || geoRes->getVertData() == 0x0 ) return false;
	
	// Transform ray to local space
	Matrix4f m = _absTrans.inverted();
	Vec3f orig = m * rayOrig;
	Vec3f dir = m * (rayOrig + rayDir) - orig;

	Vec3f nearestIntsPos = Vec3f( Math::MaxFloat, Math::MaxFloat, Math::MaxFloat );
	bool intersection = false;
	
	// Check triangles
	for( uint32 i = _batchStart; i < _batchStart + _batchCount; i += 3 )
	{
		Vec3f &vert0 = geoRes->getVertData()->positions[geoRes->_indices[i + 0]];
		Vec3f &vert1 = geoRes->getVertData()->positions[geoRes->_indices[i + 1]];
		Vec3f &vert2 = geoRes->getVertData()->positions[geoRes->_indices[i + 2]];
		
		if( rayTriangleIntersection( orig, dir, vert0, vert1, vert2, intsPos ) )
		{
			intersection = true;
			if( (intsPos - orig).length() < (nearestIntsPos - orig).length() )
				nearestIntsPos = intsPos;
		}
	}

	intsPos = _absTrans * nearestIntsPos;
	
	return intersection;
}


void MeshNode::onAttach( SceneNode &parentNode )
{
	_bBoxDirty = true;
	
	// Find parent model node
	SceneNode *node = &parentNode;
	while( node->getType() != SceneNodeTypes::Model ) node = node->getParent();
	_parentModel = (ModelNode *)node;
	_parentModel->markNodeListDirty();
}


void MeshNode::onDetach( SceneNode &/*parentNode*/ )
{
	if( _parentModel != 0x0 ) _parentModel->markNodeListDirty();
}


void MeshNode::onPreUpdate()
{
	AnimatableSceneNode::onPreUpdate();
	// Calculate local bounding box
	if( _bBoxDirty )
	{
		_bBoxDirty = false;
		
		GeometryResource *geoRes = _parentModel->getGeometryResource();
		if( geoRes == 0x0 || geoRes->getVertData() == 0x0 ) return;

		Vec3f &bBMin = _localBBox.getMinCoords();
		Vec3f &bBMax = _localBBox.getMaxCoords();
		
		if( _vertRStart < geoRes->getVertCount() && _vertREnd < geoRes->getVertCount() )
		{
			bBMin = Vec3f( Math::MaxFloat, Math::MaxFloat, Math::MaxFloat );
			bBMax = Vec3f( -Math::MaxFloat, -Math::MaxFloat, -Math::MaxFloat );
			for( uint32 i = _vertRStart; i <= _vertREnd; ++i )
			{
				Vec3f &vertPos = geoRes->getVertData()->positions[i];

				if( vertPos.x < bBMin.x ) bBMin.x = vertPos.x;
				if( vertPos.y < bBMin.y ) bBMin.y = vertPos.y;
				if( vertPos.z < bBMin.z ) bBMin.z = vertPos.z;
				if( vertPos.x > bBMax.x ) bBMax.x = vertPos.x;
				if( vertPos.y > bBMax.y ) bBMax.y = vertPos.y;
				if( vertPos.z > bBMax.z ) bBMax.z = vertPos.z;
			}

			// Avoid zero box dimensions for planes
			if( bBMax.x - bBMin.x == 0 ) bBMax.x += 0.1f;
			if( bBMax.y - bBMin.y == 0 ) bBMax.y += 0.1f;
			if( bBMax.z - bBMin.z == 0 ) bBMax.z += 0.1f;
		}
		else
		{
			bBMin = Vec3f( 0, 0, 0 );
			bBMax = Vec3f( 0, 0, 0 );
		}
	}
}


// *************************************************************************************************
// Class JointNode
// *************************************************************************************************

JointNode::JointNode( const JointNodeTpl &jointTpl ) :
	AnimatableSceneNode( jointTpl ), _jointIndex( jointTpl.jointIndex )
{
}


SceneNodeTpl *JointNode::parsingFunc( map< string, string > &attribs )
{
	bool result = true;
	
	map< string, string >::iterator itr;
	JointNodeTpl *jointTpl = new JointNodeTpl( "", 0 );

	itr = attribs.find( "jointIndex" );
	if( itr != attribs.end() ) jointTpl->jointIndex = atoi( itr->second.c_str() );
	else result = false;

	if( !result )
	{
		delete jointTpl; jointTpl = 0x0;
	}

	return jointTpl;
}


SceneNode *JointNode::factoryFunc( const SceneNodeTpl &nodeTpl )
{
	if( nodeTpl.type != SceneNodeTypes::Joint ) return 0x0;
	
	return new JointNode( *(JointNodeTpl *)&nodeTpl );
}


bool JointNode::canAttach( SceneNode &parent )
{
	// Important: Joints may not live outside of models
	return (parent.getType() == SceneNodeTypes::Model) ||
		   (parent.getType() == SceneNodeTypes::Joint);
}


int JointNode::getParami( int param )
{
	switch( param )
	{
	case JointNodeParams::JointIndex:
		return (int)_jointIndex;
	default:
		return SceneNode::getParami( param );
	}
}


void JointNode::onPostUpdate()
{
	if( _parentModel->getGeometryResource() == 0x0 ) return;
	
	if( _parent->getType() != SceneNodeTypes::Joint )
		_relModelMat = _relTrans;
	else
		//_relModelMat = ((JointNode *)_parent)->_relModelMat * _relTrans;
		_relModelMat.fastMult( ((JointNode *)_parent)->_relModelMat, _relTrans );

	if( _parentModel->jointExists( _jointIndex ) )
	{
		Matrix4f mat = _relModelMat * _parentModel->getGeometryResource()->getInvBindMat( _jointIndex );
		_parentModel->setSkinningMat( _jointIndex, mat );
	}
}


void JointNode::onAttach( SceneNode &parentNode )
{
	// Find parent model node
	SceneNode *node = &parentNode;
	while( node->getType() != SceneNodeTypes::Model ) node = node->getParent();
	_parentModel = (ModelNode *)node;
	
	_parentModel->markNodeListDirty();
}


void JointNode::onDetach( SceneNode &/*parentNode*/ )
{
	if( _parentModel != 0x0 ) _parentModel->markNodeListDirty();
}
