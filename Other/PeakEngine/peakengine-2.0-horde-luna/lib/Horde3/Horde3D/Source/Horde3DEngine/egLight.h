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

#ifndef _egLight_H_
#define _egLight_H_

#include "egPrerequisites.h"
#include "egMaterial.h"
#include "egScene.h"


struct LightNodeParams
{
	enum List
	{
		MaterialRes = 500,
		Radius,
		FOV,
		Col_R,
		Col_G,
		Col_B,
		ShadowMapCount,
		ShadowSplitLambda,
		ShadowMapBias
	};
};

struct LightNodeTpl : public SceneNodeTpl
{
	PMaterialResource	matRes;
	string				lightingContext, shadowContext;
	float				radius, fov;
	float				col_R, col_G, col_B;
	uint32				shadowMapCount;
	float				shadowSplitLambda;
	float				shadowMapBias;

	LightNodeTpl( const string &name, MaterialResource *materialRes,
				  const string &lightingContext, const string &shadowContext ) :
		SceneNodeTpl( SceneNodeTypes::Light, name ), matRes( materialRes ),
		lightingContext( lightingContext ), shadowContext( shadowContext ),
		radius( 100 ), fov( 90 ), col_R( 1 ), col_G( 1 ), col_B( 1 ), shadowMapCount( 0 ),
		shadowSplitLambda( 0.5f ), shadowMapBias( 0.005f )
	{
	}
};

class LightNode : public SceneNode
{
private:

	Matrix4f			_invTrans;
	Vec3f				_absPos, _spotDir;

	PMaterialResource	_materialRes;
	string				_lightingContext, _shadowContext;
	float				_radius, _fov;
	float				_diffCol_R, _diffCol_G, _diffCol_B;
	uint32				_shadowMapCount;
	float				_shadowSplitLambda, _shadowMapBias;

	void onPostUpdate();

	LightNode( const LightNodeTpl &lightTpl );

public:
	
	static SceneNodeTpl *parsingFunc( map< string, string > &attribs );
	static SceneNode *factoryFunc( const SceneNodeTpl &nodeTpl );
	
	float getParamf( int param );
	bool setParamf( int param, float value );
	int getParami( int param );
	bool setParami( int param, int value );
	void setContexts( const char *lightingContext, const char *shadowContext );

	void genFrustum( Frustum &frustum );
	void calcScreenSpaceAABB( const Matrix4f &mat, float &x, float &y, float &w, float &h );

	friend class Renderer;
};

#endif // _egLight_H_
