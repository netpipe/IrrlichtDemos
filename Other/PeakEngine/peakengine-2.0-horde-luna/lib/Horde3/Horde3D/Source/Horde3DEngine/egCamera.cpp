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

#include "egCamera.h"
#include "egModules.h"

#include "utDebug.h"


CameraNode::CameraNode( const CameraNodeTpl &cameraTpl ) :
	SceneNode( cameraTpl )
{
	_pipelineRes = cameraTpl.pipeRes;
	_outputTex = cameraTpl.outputTex;
	_outputBufferIndex = cameraTpl.outputBufferIndex;
	_frustLeft = cameraTpl.leftPlane;
	_frustRight = cameraTpl.rightPlane;
	_frustBottom = cameraTpl.bottomPlane;
	_frustTop = cameraTpl.topPlane;
	_frustNear = cameraTpl.nearPlane;
	_frustFar = cameraTpl.farPlane;
	_orthographic = cameraTpl.orthographic;
	_occSet = cameraTpl.occlusionCulling ? Modules::renderer().registerOccSet() : -1;
}


CameraNode::~CameraNode()
{
	_pipelineRes = 0x0;
	_outputTex = 0x0;
	if( _occSet >= 0 ) Modules::renderer().unregisterOccSet( _occSet );
}


SceneNodeTpl *CameraNode::parsingFunc( map< string, string > &attribs )
{
	bool result = true;
	
	map< string, string >::iterator itr;
	CameraNodeTpl *cameraTpl = new CameraNodeTpl( "", 0x0 );

	itr = attribs.find( "pipeline" );
	if( itr != attribs.end() )
	{
		uint32 res = Modules::resMan().addResource( ResourceTypes::Pipeline, itr->second, 0, false );
		cameraTpl->pipeRes = (PipelineResource *)Modules::resMan().resolveResHandle( res );
	}
	else result = false;
	itr = attribs.find( "outputTex" );
	if( itr != attribs.end() )
	{	
		cameraTpl->outputTex = (Texture2DResource *)Modules::resMan().findResource(
			ResourceTypes::Texture2D, itr->second );
	}
	itr = attribs.find( "outputBufferIndex" );
	if( itr != attribs.end() ) cameraTpl->outputBufferIndex = atoi( itr->second.c_str() );
	itr = attribs.find( "leftPlane" );
	if( itr != attribs.end() ) cameraTpl->leftPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "rightPlane" );
	if( itr != attribs.end() ) cameraTpl->rightPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "bottomPlane" );
	if( itr != attribs.end() ) cameraTpl->bottomPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "topPlane" );
	if( itr != attribs.end() ) cameraTpl->topPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "nearPlane" );
	if( itr != attribs.end() ) cameraTpl->nearPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "farPlane" );
	if( itr != attribs.end() ) cameraTpl->farPlane = (float)atof( itr->second.c_str() );
	itr = attribs.find( "orthographic" );
	if( itr != attribs.end() ) 
	{
		if ( _stricmp( itr->second.c_str(), "true" ) == 0 || _stricmp( itr->second.c_str(), "1" ) == 0 )
			cameraTpl->orthographic = true;
		else
			cameraTpl->orthographic = false;
	}
	itr = attribs.find( "occlusionCulling" );
	if( itr != attribs.end() ) 
	{
		if ( _stricmp( itr->second.c_str(), "true" ) == 0 || _stricmp( itr->second.c_str(), "1" ) == 0 )
			cameraTpl->occlusionCulling = true;
		else
			cameraTpl->occlusionCulling = false;
	}

	if( !result )
	{
		delete cameraTpl; cameraTpl = 0x0;
	}
	
	return cameraTpl;
}


SceneNode *CameraNode::factoryFunc( const SceneNodeTpl &nodeTpl )
{
	if( nodeTpl.type != SceneNodeTypes::Camera ) return 0x0;

	return new CameraNode( *(CameraNodeTpl *)&nodeTpl );
}


void CameraNode::onPostUpdate()
{
	_invTrans = _absTrans.inverted();
}


float CameraNode::getParamf( int param )
{
	switch( param )
	{
	case CameraNodeParams::LeftPlane:
		return _frustLeft;
	case CameraNodeParams::RightPlane:
		return _frustRight;
	case CameraNodeParams::BottomPlane:
		return _frustBottom;
	case CameraNodeParams::TopPlane:
		return _frustTop;
	case CameraNodeParams::NearPlane:
		return _frustNear;
	case CameraNodeParams::FarPlane:
		return _frustFar;
	default:
		return SceneNode::getParamf( param );
	}
}


bool CameraNode::setParamf( int param, float value )
{
	switch( param )
	{
	case CameraNodeParams::LeftPlane:
		_frustLeft = value;
		return true;
	case CameraNodeParams::RightPlane:
		_frustRight = value;
		return true;
	case CameraNodeParams::BottomPlane:
		_frustBottom = value;
		return true;
	case CameraNodeParams::TopPlane:
		_frustTop = value;
		return true;
	case CameraNodeParams::NearPlane:
		_frustNear = value;
		return true;
	case CameraNodeParams::FarPlane:
		_frustFar = value;
		return true;
	default:
		return SceneNode::setParamf( param, value );
	}
}


int CameraNode::getParami( int param )
{
	switch( param )
	{
	case CameraNodeParams::PipelineRes:
		return _pipelineRes != 0x0 ? _pipelineRes->getHandle() : 0;
	case CameraNodeParams::OutputTex:
		return _outputTex != 0x0 ? _outputTex->getHandle() : 0;
	case CameraNodeParams::OutputBufferIndex:
		return _outputBufferIndex;
	case CameraNodeParams::Orthographic:
		return _orthographic ? 1 : 0;
	case CameraNodeParams::OcclusionCulling:
		return _occSet >= 0 ? 1 : 0;
	default:
		return SceneNode::getParami( param );
	}
}


bool CameraNode::setParami( int param, int value )
{
	Resource *res;
	
	switch( param )
	{
	case CameraNodeParams::PipelineRes:
		res = Modules::resMan().resolveResHandle( value );
		if( res == 0x0 || res->getType() != ResourceTypes::Pipeline )
		{	
			Modules::log().writeDebugInfo( "Invalid Pipeline resource for Camera node %i", _handle );
			return false;
		}
		_pipelineRes = (PipelineResource *)res;
		return true;
	case CameraNodeParams::OutputTex:
		res = Modules::resMan().resolveResHandle( value );
		if( res != 0x0 && res->getType() != ResourceTypes::Texture2D )
		{	
			Modules::log().writeDebugInfo( "Invalid Texture2D resource for Camera node %i", _handle );
			return false;
		}
		_outputTex = (Texture2DResource *)res;
		return true;
	case CameraNodeParams::OutputBufferIndex:
		_outputBufferIndex = value;
		return true;
	case CameraNodeParams::Orthographic:
		_orthographic = (value == 1);
		return true;
	case CameraNodeParams::OcclusionCulling:
		if( _occSet < 0 && value != 0 )
		{		
			_occSet = Modules::renderer().registerOccSet();
		}
		else if( _occSet >= 0 && value == 0 )
		{
			Modules::renderer().unregisterOccSet( _occSet );
			_occSet = -1;
		}
		return true;
	default:	
		return SceneNode::setParami( param, value );
	}
}


void CameraNode::setupViewParams( float fov, float aspect, float near, float far )
{
	float ymax = near * tanf( degToRad( fov / 2 ) );
	float xmax = ymax * aspect;

	_frustLeft = -xmax;
	_frustRight = xmax;
	_frustBottom = -ymax;
	_frustTop = ymax;
	_frustNear = near;
	_frustFar = far;
}


void CameraNode::genFrustum( Frustum &frustum )
{
	update();
	
	frustum.buildViewFrustum( _invTrans, calcProjectionMatrix() );
}


Matrix4f CameraNode::calcProjectionMatrix()
{
	Matrix4f m;

	if( !_orthographic )	// Perspective frustum
	{
		m.x[0] = 2 * _frustNear / (_frustRight - _frustLeft);
		m.x[5] = 2 * _frustNear / (_frustTop - _frustBottom);
		m.x[8] = (_frustRight + _frustLeft) / (_frustRight - _frustLeft);
		m.x[9] = (_frustTop + _frustBottom) / (_frustTop - _frustBottom);
		m.x[10] = -(_frustFar + _frustNear) / (_frustFar - _frustNear);
		m.x[11] = -1;
		m.x[14] = -2 * _frustFar * _frustNear / (_frustFar - _frustNear);
		m.x[15] = 0;
	}
	else					// Orthographic frustum
	{
		m.x[0] = 2 / (_frustRight - _frustLeft);
		m.x[5] = 2 / (_frustTop - _frustBottom);
		m.x[10] = -2 / (_frustFar - _frustNear);
		m.x[12] = -(_frustRight + _frustLeft) / (_frustRight - _frustLeft);
		m.x[13] = -(_frustTop + _frustBottom) / (_frustTop - _frustBottom);
		m.x[14] = -(_frustFar + _frustNear) / (_frustFar - _frustNear);
	}

	return m;
}
