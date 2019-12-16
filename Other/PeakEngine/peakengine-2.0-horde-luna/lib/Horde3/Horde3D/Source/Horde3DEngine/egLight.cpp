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

#include "egLight.h"
#include "egMaterial.h"
#include "egModules.h"

#include "utDebug.h"


LightNode::LightNode( const LightNodeTpl &lightTpl ) :
	SceneNode( lightTpl )
{
	_materialRes = lightTpl.matRes;
	_lightingContext = lightTpl.lightingContext;
	_shadowContext = lightTpl.shadowContext;
	_radius = lightTpl.radius; _fov = lightTpl.fov;
	_diffCol_R = lightTpl.col_R; _diffCol_G = lightTpl.col_G; _diffCol_B =lightTpl.col_B;
	_shadowMapCount = lightTpl.shadowMapCount;
	_shadowSplitLambda = lightTpl.shadowSplitLambda;
	_shadowMapBias = lightTpl.shadowMapBias;
}


SceneNodeTpl *LightNode::parsingFunc( map< string, string > &attribs )
{
	bool result = true;
	
	map< string, string >::iterator itr;
	LightNodeTpl *lightTpl = new LightNodeTpl( "", 0x0, "", "" );

	itr = attribs.find( "material" );
	if( itr != attribs.end() )
	{
		uint32 res = Modules::resMan().addResource( ResourceTypes::Material, itr->second, 0, false );
		if( res != 0 )
			lightTpl->matRes = (MaterialResource *)Modules::resMan().resolveResHandle( res );
	}
	itr = attribs.find( "lightingContext" );
	if( itr != attribs.end() ) lightTpl->lightingContext = itr->second;
	else result = false;
	itr = attribs.find( "shadowContext" );
	if( itr != attribs.end() ) lightTpl->shadowContext = itr->second;
	else result = false;
	itr = attribs.find( "radius" );
	if( itr != attribs.end() ) lightTpl->radius = (float)atof( itr->second.c_str() );
	itr = attribs.find( "fov" );
	if( itr != attribs.end() ) lightTpl->fov = (float)atof( itr->second.c_str() );
	itr = attribs.find( "col_R" );
	if( itr != attribs.end() ) lightTpl->col_R = (float)atof( itr->second.c_str() );
	itr = attribs.find( "col_G" );
	if( itr != attribs.end() ) lightTpl->col_G = (float)atof( itr->second.c_str() );
	itr = attribs.find( "col_B" );
	if( itr != attribs.end() ) lightTpl->col_B = (float)atof( itr->second.c_str() );
	itr = attribs.find( "shadowMapCount" );
	if( itr != attribs.end() ) lightTpl->shadowMapCount = atoi( itr->second.c_str() );
	itr = attribs.find( "shadowSplitLambda" );
	if( itr != attribs.end() ) lightTpl->shadowSplitLambda = (float)atof( itr->second.c_str() );
	itr = attribs.find( "shadowMapBias" );
	if( itr != attribs.end() ) lightTpl->shadowMapBias = (float)atof( itr->second.c_str() );
	
	if( !result )
	{
		delete lightTpl; lightTpl = 0x0;
	}
	
	return lightTpl;
}


SceneNode *LightNode::factoryFunc( const SceneNodeTpl &nodeTpl )
{
	if( nodeTpl.type != SceneNodeTypes::Light ) return 0x0;

	return new LightNode( *(LightNodeTpl *)&nodeTpl );
}


void LightNode::onPostUpdate()
{
	_invTrans = _absTrans.inverted();
	
	_absPos = Vec3f( _absTrans.c[3][0], _absTrans.c[3][1], _absTrans.c[3][2] );

	Matrix4f m = _absTrans;
	m.c[3][0] = 0; m.c[3][1] = 0; m.c[3][2] = 0;
	_spotDir = m * Vec3f( 0, 0, -1 );
	_spotDir = _spotDir.normalized();
}


float LightNode::getParamf( int param )
{
	switch( param )
	{
	case LightNodeParams::Radius:
		return _radius;
	case LightNodeParams::Col_R:
		return _diffCol_R;
	case LightNodeParams::Col_G:
		return _diffCol_G;
	case LightNodeParams::Col_B:
		return _diffCol_B;
	case LightNodeParams::FOV:
		return _fov;
	case LightNodeParams::ShadowSplitLambda:
		return _shadowSplitLambda;
	case LightNodeParams::ShadowMapBias:
		return _shadowMapBias;
	default:
		return SceneNode::getParamf( param );
	}
}


bool LightNode::setParamf( int param, float value )
{
	switch( param )
	{
	case LightNodeParams::Radius:
		_radius = value;
		return true;
	case LightNodeParams::FOV:
		_fov = value;
		return true;
	case LightNodeParams::Col_R:
		_diffCol_R = value;
		return true;
	case LightNodeParams::Col_G:
		_diffCol_G = value;
		return true;
	case LightNodeParams::Col_B:
		_diffCol_B = value;
		return true;
	case LightNodeParams::ShadowSplitLambda:
		_shadowSplitLambda = value;
		return true;
	case LightNodeParams::ShadowMapBias:
		_shadowMapBias = value;
		return true;
	default:	
		return SceneNode::setParamf( param, value );
	}
}


int LightNode::getParami( int param )
{
	switch( param )
	{
	case LightNodeParams::MaterialRes:
		if( _materialRes != 0x0 ) return _materialRes->getHandle();
		else return 0;
	case LightNodeParams::ShadowMapCount:
		return _shadowMapCount;
	default:
		return SceneNode::getParami( param );
	}
}


bool LightNode::setParami( int param, int value )
{
	Resource *res;
	
	switch( param )
	{
	case LightNodeParams::MaterialRes:
		res = Modules::resMan().resolveResHandle( value );
		if( res != 0x0 && res->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource for Light node %i", _handle );
			return false;
		}
		_materialRes = (MaterialResource *)res;
		return true;
	case LightNodeParams::ShadowMapCount:
		if( value == 0 || value == 1 || value == 2 || value == 3 || value == 4 )
		{	
			_shadowMapCount = (uint32)value;
			return true;
		}
		else return false;
	default:	
		return SceneNode::setParami( param, value );
	}
}


void LightNode::setContexts( const char *lightingContext, const char *shadowContext )
{
	if( lightingContext != 0x0 ) _lightingContext = lightingContext;
	if( shadowContext != 0x0 ) _shadowContext = shadowContext;
}


void LightNode::genFrustum( Frustum &frustum )
{
	Vec3f lightRot = _spotDir.toRotation();
	lightRot.x = radToDeg( lightRot.x );
	lightRot.y = radToDeg( lightRot.y );
	
	if( _fov < 180 )
		frustum.buildViewFrustum( _absTrans, _fov, 1.0f, 0.1f, _radius );
	else
		frustum.buildBoxFrustum( _absTrans, -_radius, _radius, -_radius, _radius, _radius, -_radius );
}


void LightNode::calcScreenSpaceAABB( const Matrix4f &mat, float &x, float &y, float &w, float &h )
{
	uint32 numPoints = 0;
	Vec3f points[8];
	Vec4f pts[8];

	float min_x = Math::MaxFloat, min_y = Math::MaxFloat;
	float max_x = -Math::MaxFloat, max_y = -Math::MaxFloat;
	
	if( _fov < 180 )
	{
		// Generate frustum for spot light
		numPoints = 5;
		float val = 1.0f * tanf( degToRad( _fov / 2 ) );
		points[0] = _absTrans * Vec3f( 0, 0, 0 );
		points[1] = _absTrans * Vec3f( -val * _radius, -val * _radius, -_radius );
		points[2] = _absTrans * Vec3f(  val * _radius, -val * _radius, -_radius );
		points[3] = _absTrans * Vec3f(  val * _radius,  val * _radius, -_radius );
		points[4] = _absTrans * Vec3f( -val * _radius,  val * _radius, -_radius );
	}
	else
	{
		// Generate sphere for point light
		numPoints = 8;
		points[0] = _absPos + Vec3f( -_radius, -_radius, -_radius );
		points[1] = _absPos + Vec3f(  _radius, -_radius, -_radius );
		points[2] = _absPos + Vec3f(  _radius,  _radius, -_radius );
		points[3] = _absPos + Vec3f( -_radius,  _radius, -_radius );
		points[4] = _absPos + Vec3f( -_radius, -_radius,  _radius );
		points[5] = _absPos + Vec3f(  _radius, -_radius,  _radius );
		points[6] = _absPos + Vec3f(  _radius,  _radius,  _radius );
		points[7] = _absPos + Vec3f( -_radius,  _radius,  _radius );
	}

	// Project points to screen-space and find extents
	for( uint32 i = 0; i < numPoints; ++i )
	{
		pts[i] = mat * Vec4f( points[i].x, points[i].y, points[i].z, 1 );
		
		if( pts[i].w != 0 )
		{
			pts[i].x = (pts[i].x / pts[i].w) * 0.5f + 0.5f;
			pts[i].y = (pts[i].y / pts[i].w) * 0.5f + 0.5f;
		}

		if( pts[i].x < min_x ) min_x = pts[i].x;
		if( pts[i].y < min_y ) min_y = pts[i].y;
		if( pts[i].x > max_x ) max_x = pts[i].x;
		if( pts[i].y > max_y ) max_y = pts[i].y;
	}
	
	// Clamp values
	if( min_x < 0 ) min_x = 0; if( min_x > 1 ) min_x = 1;
	if( max_x < 0 ) max_x = 0; if( max_x > 1 ) max_x = 1;
	if( min_y < 0 ) min_y = 0; if( min_y > 1 ) min_y = 1;
	if( max_y < 0 ) max_y = 0; if( max_y > 1 ) max_y = 1;
	
	x = min_x; y = min_y;
	w = max_x - min_x; h = max_y - min_y;

	// Check if viewer is inside bounding box
	if( pts[0].w < 0 || pts[1].w < 0 || pts[2].w < 0 || pts[3].w < 0 || pts[4].w < 0 )
	{
		x = 0; y = 0; w = 1; h = 1;
	}
	else if( numPoints == 8 && (pts[5].w < 0 || pts[6].w < 0 || pts[7].w < 0) )
	{
		x = 0; y = 0; w = 1; h = 1;
	}
}
