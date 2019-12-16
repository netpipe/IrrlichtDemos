// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
//
// Sample Application
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This sample source file is not covered by the LGPL as the rest of the SDK
// and may be used without any restrictions
//
// *************************************************************************************************

#include "app.h"
#include "Horde3D.h"
#include "Horde3DUtils.h"
#include <math.h>
#include <iomanip>


// Convert from degrees to radians
inline float degToRad( float f ) 
{
	return f * (3.1415926f / 180.0f);
}


Application::Application( const string &contentDir )
{
	for( unsigned int i = 0; i < 320; ++i ) _keys[i] = false;

	_x = 5; _y = 3; _z = 19; _rx = 7; _ry = 15; _velocity = 10.0f;
	_curFPS = 30;

	_freeze = false; _showStats = false; _debugViewMode = false; _wireframeMode = false;
	_animTime = 0; _weight = 1.0f;
	_cam = 0;

	_contentDir = contentDir;
}


bool Application::init()
{	
	// Initialize engine
	if( !Horde3D::init() )
	{	
		Horde3DUtils::dumpMessages();
		return false;
	}
	
	// Set paths for resources
	Horde3DUtils::setResourcePath( ResourceTypes::SceneGraph, "models" );
	Horde3DUtils::setResourcePath( ResourceTypes::Geometry, "models" );
	Horde3DUtils::setResourcePath( ResourceTypes::Animation, "models" );
	Horde3DUtils::setResourcePath( ResourceTypes::Material, "materials" );
	Horde3DUtils::setResourcePath( ResourceTypes::Code, "shaders" );
	Horde3DUtils::setResourcePath( ResourceTypes::Shader, "shaders" );
	Horde3DUtils::setResourcePath( ResourceTypes::Texture2D, "textures" );
	Horde3DUtils::setResourcePath( ResourceTypes::TextureCube, "textures" );
	Horde3DUtils::setResourcePath( ResourceTypes::Effect, "effects" );
	Horde3DUtils::setResourcePath( ResourceTypes::Pipeline, "pipelines" );

	// Set options
	Horde3D::setOption( EngineOptions::LoadTextures, 1 );
	Horde3D::setOption( EngineOptions::TexCompression, 0 );
	Horde3D::setOption( EngineOptions::FastAnimation, 0 );
	Horde3D::setOption( EngineOptions::AnisotropyFactor, 8 );
	Horde3D::setOption( EngineOptions::ShadowMapSize, 2048 );

	// Add resources
	// Pipelines
	_hdrPipeRes = Horde3D::addResource( ResourceTypes::Pipeline, "hdr.pipeline.xml", 0 );
	_forwardPipeRes = Horde3D::addResource( ResourceTypes::Pipeline, "forward.pipeline.xml", 0 );
	// Font
	_fontMatRes = Horde3D::addResource( ResourceTypes::Material, "font.material.xml", 0 );
	// Logo
	_logoMatRes = Horde3D::addResource( ResourceTypes::Material, "logo.material.xml", 0 );
	// Environment
	ResHandle envRes = Horde3D::addResource( ResourceTypes::SceneGraph, "sphere.scene.xml", 0 );
	// Knight
	ResHandle knightRes = Horde3D::addResource( ResourceTypes::SceneGraph, "knight.scene.xml", 0 );
	ResHandle knightAnim1Res = Horde3D::addResource( ResourceTypes::Animation, "knight_order.anim", 0 );
	ResHandle knightAnim2Res = Horde3D::addResource( ResourceTypes::Animation, "knight_attack.anim", 0 );
	// Particle system
	ResHandle particleSysRes = Horde3D::addResource( ResourceTypes::SceneGraph, "particleSys1.scene.xml", 0 );
	
	// Load resources
	Horde3DUtils::loadResourcesFromDisk( _contentDir.c_str() );

	// Add scene nodes
	// Add camera
	_cam = Horde3D::addCameraNode( RootNode, "Camera", _hdrPipeRes );
	//Horde3D::setNodeParami( _cam, CameraNodeParams::OcclusionCulling, 1 );
	// Add environment
	NodeHandle env = Horde3D::addNodes( RootNode, envRes );
	Horde3D::setNodeTransform( env, 0, -20, 0, 0, 0, 0, 20, 20, 20 );
	// Add knight
	_knight = Horde3D::addNodes( RootNode, knightRes );
	Horde3D::setNodeTransform( _knight, 0, 0, 0, 0, 180, 0, 0.1f, 0.1f, 0.1f );
	Horde3D::setupModelAnimStage( _knight, 0, knightAnim1Res, "", false );
	Horde3D::setupModelAnimStage( _knight, 1, knightAnim2Res, "", false );
	// Attach particle system to hand joint
	Horde3D::findNodes( _knight, "Bip01_R_Hand", SceneNodeTypes::Joint );
	NodeHandle hand = Horde3D::getNodeFindResult( 0 );
	_particleSys = Horde3D::addNodes( hand, particleSysRes );
	Horde3D::setNodeTransform( _particleSys, 0, 40, 0, 90, 0, 0, 1, 1, 1 );

	// Add light source
	NodeHandle light = Horde3D::addLightNode( RootNode, "Light1", 0, "LIGHTING", "SHADOWMAP" );
	Horde3D::setNodeTransform( light, 0, 15, 10, -60, 0, 0, 1, 1, 1 );
	Horde3D::setNodeParamf( light, LightNodeParams::Radius, 30 );
	Horde3D::setNodeParamf( light, LightNodeParams::FOV, 90 );
	Horde3D::setNodeParami( light, LightNodeParams::ShadowMapCount, 1 );
	Horde3D::setNodeParamf( light, LightNodeParams::ShadowMapBias, 0.01f );
	Horde3D::setNodeParamf( light, LightNodeParams::Col_R, 1.0f );
	Horde3D::setNodeParamf( light, LightNodeParams::Col_G, 0.8f );
	Horde3D::setNodeParamf( light, LightNodeParams::Col_B, 0.7f );

	// Customize post processing effects
	NodeHandle matRes = Horde3D::findResource( ResourceTypes::Material, "postHDR.material.xml" );
	// hdrParams: exposure, brightpass threshold, brightpass offset (see shader for description)
	Horde3D::setMaterialUniform( matRes, "hdrParams", 2.5f, 0.5f, 0.08f, 0 );
	
	return true;
}


void Application::mainLoop( float fps )
{
	_curFPS = fps;

	keyHandler();
	
	Horde3D::setOption( EngineOptions::DebugViewMode, _debugViewMode ? 1.0f : 0.0f );
	Horde3D::setOption( EngineOptions::WireframeMode, _wireframeMode ? 1.0f : 0.0f );
	
	if( !_freeze )
	{
		_animTime += 1.0f / _curFPS;

		// Do animation blending
		Horde3D::setModelAnimParams( _knight, 0, _animTime * 24.0f, _weight );
		Horde3D::setModelAnimParams( _knight, 1, _animTime * 24.0f, 1.0f - _weight );

		// Animate particle systems (several emitters in a group node)
		unsigned int cnt = cnt = Horde3D::findNodes( _particleSys, "", SceneNodeTypes::Emitter );
		for( unsigned int i = 0; i < cnt; ++i )
			Horde3D::advanceEmitterTime( Horde3D::getNodeFindResult( i ), 1.0f / _curFPS );
	}
	
	// Set camera parameters
	Horde3D::setNodeTransform( _cam, _x, _y, _z, _rx ,_ry, 0, 1, 1, 1 );
	
	if( _showStats )
	{
		Horde3DUtils::showFrameStats( _fontMatRes, _curFPS );

		// Display weight
		_text.str( "" );
		_text << fixed << setprecision( 2 ) << "Weight: " << _weight;
		Horde3DUtils::showText( _text.str().c_str(), 0, 0.78f, 0.03f, 0, _fontMatRes );
	}

	// Show logo
	Horde3D::showOverlay( 0.75f, 0, 0, 0, 1, 0, 1, 0,
						  1, 0.2f, 1, 1, 0.75f, 0.2f, 0, 1,
						  7, _logoMatRes );
	
	// Render scene
	Horde3D::render( _cam );

	// Remove all overlays
	Horde3D::clearOverlays();

	// Write all mesages to log file
	Horde3DUtils::dumpMessages();
}


void Application::release()
{
	// Release engine
	Horde3D::release();
}


void Application::resize( int width, int height )
{
	// Resize viewport
	Horde3D::resize( 0, 0, width, height );
	
	// Set virtual camera parameters
	Horde3D::setupCameraView( _cam, 45.0f, (float)width / height, 0.1f, 1000.0f );
}


void Application::keyPressEvent( int key )
{
	if( key == 32 )		// Space
		_freeze = !_freeze;

	if( key == 260 )	// F3
	{
		if( Horde3D::getNodeParami( _cam, CameraNodeParams::PipelineRes ) == _hdrPipeRes )
			Horde3D::setNodeParami( _cam, CameraNodeParams::PipelineRes, _forwardPipeRes );
		else
			Horde3D::setNodeParami( _cam, CameraNodeParams::PipelineRes, _hdrPipeRes );
	}
	
	if( key == 264 )	// F7
		_debugViewMode = !_debugViewMode;

	if( key == 265 )	// F8
		_wireframeMode = !_wireframeMode;
	
	if( key == 266 )	// F9
		_showStats = !_showStats;
}


void Application::keyHandler()
{
	float curVel = _velocity / _curFPS;
	
	if( _keys[287] ) curVel *= 5;	// LShift
	
	if( _keys['W'] )
	{
		// Move forward
		_x -= sinf( degToRad( _ry ) ) * cosf( -degToRad( _rx ) ) * curVel;
		_y -= sinf( -degToRad( _rx ) ) * curVel;
		_z -= cosf( degToRad( _ry ) ) * cosf( -degToRad( _rx ) ) * curVel;
	}

	if( _keys['S'] )
	{
		// Move backward
		_x += sinf( degToRad( _ry ) ) * cosf( -degToRad( _rx ) ) * curVel;
		_y += sinf( -degToRad( _rx ) ) * curVel;
		_z += cosf( degToRad( _ry ) ) * cosf( -degToRad( _rx ) ) * curVel;
	}

	if( _keys['A'] )
	{
		// Strafe left
		_x += sinf( degToRad( _ry - 90) ) * curVel;
		_z += cosf( degToRad( _ry - 90 ) ) * curVel;
	}

	if( _keys['D'] )
	{
		// Strafe right
		_x += sinf( degToRad( _ry + 90 ) ) * curVel;
		_z += cosf( degToRad( _ry + 90 ) ) * curVel;
	}

	if( _keys['1'] )
	{
		// Change blend weight
		_weight += 2 / _curFPS;
		if( _weight > 1 ) _weight = 1;
	}
	
	if( _keys['2'] )
	{
		// Change blend weight
		_weight -= 2 / _curFPS;
		if( _weight < 0 ) _weight = 0;
	}
}


void Application::mouseMoveEvent( float dX, float dY )
{
	// Look left/right
	_ry -= dX / 100 * 30;
	
	// Loop up/down but only in a limited range
	_rx += dY / 100 * 30;
	if( _rx > 90 ) _rx = 90; 
	if( _rx < -90 ) _rx = -90;
}
