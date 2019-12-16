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

#ifndef _egRenderer_H_
#define _egRenderer_H_

#include "egPrerequisites.h"
#include "egRendererBase.h"
#include "egPrimitives.h"
#include "egLight.h"
#include "egCamera.h"
#include "egModel.h"
#include <vector>
#include <algorithm>
using namespace std;

class MaterialResource;
struct ShaderContext;

const uint32 ParticlesPerBatch = 64;	// Warning: The GPU must have enough registers

extern const char *vsDefColor;
extern const char *fsDefColor;
extern const char *vsOccBox;
extern const char *fsOccBox;	
	

struct EngineStats
{
	enum List
	{
		TriCount = 100,
		BatchCount,
		LightPassCount
	};
};


struct Overlay
{
	float				x_ll, y_ll, u_ll, v_ll;		// Lower left corner
	float				x_lr, y_lr, u_lr, v_lr;		// Lower right corner
	float				x_ur, y_ur, u_ur, v_ur;		// Upper right corner
	float				x_ul, y_ul, u_ul, v_ul;		// Upper left corner
	int					layer;
	PMaterialResource	materialRes;


	Overlay() {}

	Overlay( const float &x_ll, const float &y_ll, const float &u_ll, const float &v_ll,
			 const float &x_lr, const float &y_lr, const float &u_lr, const float &v_lr,
			 const float &x_ur, const float &y_ur, const float &u_ur, const float &v_ur,
			 const float &x_ul, const float &y_ul, const float &u_ul, const float &v_ul,
			 int layer ) :
		x_ll( x_ll ), y_ll( y_ll ), u_ll( u_ll ), v_ll( v_ll ),
		x_lr( x_lr ), y_lr( y_lr ), u_lr( u_lr ), v_lr( v_lr ),
		x_ur( x_ur ), y_ur( y_ur ), u_ur( u_ur ), v_ur( v_ur ),
		x_ul( x_ul ), y_ul( y_ul ), u_ul( u_ul ), v_ul( v_ul ),
		layer( layer ), materialRes( 0x0 )
	{	
	}
};


struct ParticleVert
{
	float	x, y, z;		// Position
	float	u, v;			// Texture coordinates
	float	cornerIndex;	// Index of corner of quad (for up/right vectors)
	float	index;			// Index in property array

	ParticleVert()
	{
	}
	
	ParticleVert( float u, float v, float cornerIndex ):
		x( 0 ), y( 0 ), z( 0 ), u( u ), v( v ), cornerIndex( cornerIndex ), index( 0 )
	{
	}
};


class Renderer : public RendererBase
{
protected:

	uint32				_frameID;
	uint32				_smFBO, _smTex;
	uint32				_defShadowMap;
	uint32				_particleVBO;
	MaterialResource	*_curStageMatLink;
	CameraNode			*_curCamera;
	LightNode			*_curLight;
	MaterialResource	*_curMatRes;
	ShaderContext		*_curShader;
	RenderTarget		*_curRenderTarget;
	Frustum				_camFrustum, _lightFrustum;
	uint32				_curUpdateStamp;
	
	float				_splitPlanes[5];
	Matrix4f			_lightMats[4];

	vector< char >		_occSets;	// Actually bool
	vector< Overlay >	_overlays;

	int					_statTriCount, _statBatchCount, _statLightPassCount;

	static bool nodeFrontToBackOrder( NodeListEntry e1, NodeListEntry e2 )
		{ return ((MeshNode *)e1.node)->tmpSortValue < ((MeshNode *)e2.node)->tmpSortValue; }
	static bool nodeBackToFrontOrder( NodeListEntry e1, NodeListEntry e2 )
		{ return ((MeshNode *)e1.node)->tmpSortValue > ((MeshNode *)e2.node)->tmpSortValue; }
	static bool meshMaterialOrder( NodeListEntry e1, NodeListEntry e2 )
		{ return ((MeshNode *)e1.node)->getMaterialRes() < ((MeshNode *)e2.node)->getMaterialRes(); }
	
	void setupViewMatrices( CameraNode *cam );
	bool setMaterialRec( MaterialResource *materialRes, const string &shaderContext, bool firstRec = false );
	void drawRenderables( const string &shaderContext, const string &theClass, bool debugView,
		const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
	
	Matrix4f calcLightMat( const Frustum &frustum );
	void updateShadowMap();
	void setupShadowMap( bool noShadows );

	void bindBuffer( RenderBuffer *rb, uint32 texUnit, uint32 bufIndex );
	void clear( bool depth, bool buf0, bool buf1, bool buf2, bool buf3, float r, float g, float b, float a );
	void drawOverlays( const string &shaderContext );
	void drawFSQuad( Resource *matRes, const string &shaderContext );
	void drawGeometry( const string &shaderContext, const string &theClass,
					   RenderingOrder::List order, int occSet );
	void drawLightGeometry( const string shaderContext, const string &theClass,
							bool noShadows, RenderingOrder::List order, int occSet );
	void drawLightShapes( const string shaderContext, bool noShadows );
	void renderDebugView();
	void finishRendering();

public:
	
	static ShaderContext	defColorShader;
	static ShaderContext	occShader;
	
	Renderer();
	~Renderer();
	
	void initStates();
	bool init();
	void resize( int x, int y, int width, int height );
	float getStat( int param, bool reset );
	void incStat( int param, float value );
	void showOverlay( const Overlay &overlay, uint32 matRes );
	void clearOverlays();

	int registerOccSet();
	void unregisterOccSet( int occSet );

	bool uploadShader( const char *vertexShader, const char *fragmentShader, ShaderContext &sc );
	void setShader( ShaderContext *sc );
	bool setMaterial( MaterialResource *materialRes, const string &shaderContext );
	
	bool createShadowBuffer( uint32 width, uint32 height );
	void destroyShadowBuffer();
	
	void drawAABB( const Vec3f &bbMin, const Vec3f &bbMax );
	void drawDebugAABB( const Vec3f &bbMin, const Vec3f &bbMax, bool saveStates );
	bool render( CameraNode *camNode );

	uint32 getFrameID() { return _frameID; }
	uint32 getVPWidth() { return _vpWidth; }
	uint32 getVPHeight() { return _vpHeight; }
	ShaderContext *getCurShader() { return _curShader; }
	CameraNode *getCurCamera() { return _curCamera; }
	uint32 getParticleVBO() { return _particleVBO; }

	static void drawModels( const string &shaderContext, const string &theClass, bool debugView,
		const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
	static void drawParticles( const string &shaderContext, const string &theClass, bool debugView,
		const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order, int occSet );
};

#endif // _egRenderer_H_
