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

#include "egModules.h"
#include "egModel.h"
#include "egParticle.h"
#include "egSceneGraphRes.h"
#include "egTextures.h"
#include "utPlatform.h"
#include <cstdlib>
#include <cstring>
#include <string>
using namespace std;

#ifdef PLATFORM_WIN
#   define WIN32_LEAN_AND_MEAN 1
#	include <windows.h>
#endif


// *************************************************************************************************
// Engine core functions
// *************************************************************************************************

namespace Horde3D
{
	bool initialized;
	
	string safeStr( const char *str )
	{
		if( str != 0x0 ) return str;
		else return "";
	}

	
	// *********************************************************************************************
	// Basic functions
	// *********************************************************************************************
	
	DLLEXP const char *getVersionString()
	{
		return versionString;
	}


	DLLEXP bool checkExtension( const char *extensionName )
	{
		return Modules::extMan().checkExtension( safeStr( extensionName ) );
	}

	
	DLLEXP bool init()
	{
		if( initialized )
		{	
			// Init states for additional rendering context
			Modules::renderer().initStates();
			return true;
		}
		initialized = true;
	
		Modules::init();

		// Register renderer before resources so that they can upload data to the GPU
		if( !Modules::renderer().init() ) return false;

		// Register resource types
		Modules::resMan().registerType( ResourceTypes::SceneGraph, "SceneGraph", 0x0, 0x0,
			SceneGraphResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Geometry, "Geometry", GeometryResource::initializationFunc,
			GeometryResource::releaseFunc, GeometryResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Animation, "Animation", 0x0, 0x0,
			AnimationResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Material, "Material", 0x0, 0x0,
			MaterialResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Code, "Code", 0x0, 0x0,
			CodeResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Shader, "Shader", 0x0, 0x0,
			ShaderResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Texture2D, "Texture2D", Texture2DResource::initializationFunc,
			Texture2DResource::releaseFunc, Texture2DResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::TextureCube, "TextureCube", TextureCubeResource::initializationFunc,
			TextureCubeResource::releaseFunc, TextureCubeResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Effect, "Effect", 0x0, 0x0,
			EffectResource::factoryFunc );
		Modules::resMan().registerType( ResourceTypes::Pipeline, "Pipeline", 0x0, 0x0,
			PipelineResource::factoryFunc );

		// Register node types
		Modules::sceneMan().registerType( SceneNodeTypes::Group, "Group",
			GroupNode::parsingFunc, GroupNode::factoryFunc, 0x0 );
		Modules::sceneMan().registerType( SceneNodeTypes::Model, "Model",
			ModelNode::parsingFunc, ModelNode::factoryFunc, Renderer::drawModels );
		Modules::sceneMan().registerType( SceneNodeTypes::Mesh, "Mesh",
			MeshNode::parsingFunc, MeshNode::factoryFunc, 0x0 );
		Modules::sceneMan().registerType( SceneNodeTypes::Joint, "Joint",
			JointNode::parsingFunc, JointNode::factoryFunc, 0x0 );
		Modules::sceneMan().registerType( SceneNodeTypes::Light, "Light",
			LightNode::parsingFunc, LightNode::factoryFunc, 0x0 );
		Modules::sceneMan().registerType( SceneNodeTypes::Camera, "Camera",
			CameraNode::parsingFunc, CameraNode::factoryFunc, 0x0 );
		Modules::sceneMan().registerType( SceneNodeTypes::Emitter, "Emitter",
			EmitterNode::parsingFunc, EmitterNode::factoryFunc, Renderer::drawParticles );
		
		// Register extensions at last so that they can overwrite the registered resources and nodes
		if( !Modules::extMan().init() ) return false;

		return true;
	}


	DLLEXP void release()
	{
		Modules::release();
		initialized = false;
	}


	DLLEXP bool render( NodeHandle cameraNode )
	{
		SceneNode *node = Modules::sceneMan().resolveNodeHandle( cameraNode );
		if( node == 0x0 || node->getType() != SceneNodeTypes::Camera ) return false;
		
		return Modules::renderer().render( (CameraNode *)node );
	}


	DLLEXP void resize( int x, int y, int width, int height )
	{
		if( !initialized ) return;
		
		Modules::renderer().resize( x, y, width, height );

		// Update pipeline resources
		for( uint32 i = 0; i < Modules::resMan().getResources().size(); ++i )
		{
			Resource *res = Modules::resMan().getResources()[i];
			
			if( res != 0x0 && res->getType() == ResourceTypes::Pipeline )
			{
				((PipelineResource *)res)->resize();
			}
		}
	}


	DLLEXP void clear()
	{
		Modules::sceneMan().removeNode( RootNode );
		Modules::resMan().clear();
	}


	// *********************************************************************************************
	// General functions
	// *********************************************************************************************

	DLLEXP const char *getMessage( int *level, float *time )
	{
		static string msgText;
		static LogMessage msg;
		
		if( Modules::log().getMessage( msg ) )
		{
			if( level != 0x0 ) *level = msg.level;
			if( time != 0x0 ) *time = msg.time;
			return msg.text.c_str();
		}
		else
			return "";
	}

	
	DLLEXP float getOption( EngineOptions::List param )
	{
		return Modules::config().getOption( param );
	}
	
	
	DLLEXP bool setOption( EngineOptions::List param, float value )
	{
		return Modules::config().setOption( param, value );
	}


	DLLEXP float getStat( EngineStats::List param, bool reset )
	{
		return Modules::renderer().getStat( param, reset );
	}


	DLLEXP void showOverlay( float x_ll, float y_ll, float u_ll, float v_ll,
							 float x_lr, float y_lr, float u_lr, float v_lr,
							 float x_ur, float y_ur, float u_ur, float v_ur,
							 float x_ul, float y_ul, float u_ul, float v_ul,
							 int layer, ResHandle materialRes )
	{
		Resource *res = Modules::resMan().resolveResHandle( materialRes ); 
		if( res != 0x0 && res->getType() == ResourceTypes::Material )
		{
			Modules::renderer().showOverlay(
				Overlay( x_ll, y_ll, u_ll, v_ll, x_lr, y_lr, u_lr, v_lr,
						 x_ur, y_ur, u_ur, v_ur, x_ul, y_ul, u_ul, v_ul, layer ), materialRes );
		}
		else
			Modules::log().writeDebugInfo( "Invalid Material resource handle %i in showOverlay", materialRes );
	}


	DLLEXP void clearOverlays()
	{
		Modules::renderer().clearOverlays();
	}


	// *********************************************************************************************
	// Resource functions
	// *********************************************************************************************

	DLLEXP ResourceTypes::List getResourceType( ResHandle res )
	{
		Resource *r = Modules::resMan().resolveResHandle( res );
		
		if( r != 0x0 ) return r->getType();
		else
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceType", res );
			return ResourceTypes::Undefined;
		}
	}


	DLLEXP const char *getResourceName( ResHandle res )
	{
		static char emptyString = '\0';
		
		Resource *r = Modules::resMan().resolveResHandle( res );
		
		if( r != 0x0 ) return r->getName().c_str();
		else
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceName", res );
			return &emptyString;
		}
	}


	DLLEXP ResHandle findResource( ResourceTypes::List type, const char *name )
	{
		Resource *res = Modules::resMan().findResource( type, safeStr( name ) );
		
		if( res != 0x0 ) return res->getHandle();
		else return 0;
	}

	
	DLLEXP ResHandle addResource( ResourceTypes::List type, const char *name, int flags )
	{
		return Modules::resMan().addResource( type, safeStr( name ), flags, true );
	}


	DLLEXP ResHandle cloneResource( ResHandle sourceRes, const char *name )
	{
		return Modules::resMan().cloneResource( sourceRes, safeStr( name ) );
	}


	DLLEXP int removeResource( ResHandle res )
	{
		return Modules::resMan().removeResource( res, true );
	}


	DLLEXP bool isResourceLoaded( ResHandle res )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in isResourceLoaded", res );
			return false;
		}
		
		return resObj->isLoaded();
	}

	
	DLLEXP bool loadResource( ResHandle res, const char *data, int size )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in loadResource", res );
			return false;
		}
		
		Modules::log().writeInfo( "Loading resource '%s'", resObj->getName().c_str() );
		return resObj->load( data, size );
	}


	DLLEXP bool unloadResource( ResHandle res )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );

		if( resObj != 0x0 )
		{
			resObj->unload();
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid resource handle %i in unloadResource", res );
			return false;
		}
	}


	DLLEXP int getResourceParami( ResHandle res, int param )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceParami", res );
			return 0x0;
		}

		return resObj->getParami( param );
	}


	DLLEXP bool setResourceParami( ResHandle res, int param, int value )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in setResourceParami", res );
			return false;
		}

		return resObj->setParami( param, value );
	}


	DLLEXP float getResourceParamf( ResHandle res, int param )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceParamf", res );
			return 0x0;
		}

		return resObj->getParamf( param );
	}


	DLLEXP bool setResourceParamf( ResHandle res, int param, float value )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in setResourceParamf", res );
			return false;
		}

		return resObj->setParamf( param, value );
	}

	DLLEXP const char *getResourceParamstr( ResHandle res, int param )
	{
		static char emptyString = '\0';
		
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceParamstr", res );
			return &emptyString;
		}

		return resObj->getParamstr( param );
	}


	DLLEXP bool setResourceParamstr( ResHandle res, int param, const char *value )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in setResourceParamstr", res );
			return false;
		}
		if( value == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid value for setResourceParamstr: NULL" );
			return false;
		}
		
		return resObj->setParamstr( param, value );
	}


	DLLEXP const void *getResourceData( ResHandle res, int param )
	{
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in getResourceData", res );
			return 0x0;
		}

		return resObj->getData( param );
	}
	
	
	DLLEXP bool updateResourceData( ResHandle res, int param, const void *data, int size )
	{
		if( data == 0x0 ) return false;
	
		Resource *resObj = Modules::resMan().resolveResHandle( res );
		if( resObj == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid resource handle %i in updateResourceData", res );
			return false;
		}

		if( !resObj->isLoaded() )
		{	
			Modules::log().writeDebugInfo( "Problem in updateResourceData: Resource %i is not loaded", res );
			return false;
		}
		return resObj->updateData( param, data, size );
	}
	

	DLLEXP ResHandle queryUnloadedResource( int index )
	{
		return Modules::resMan().queryUnloadedResource( index );
	}


	DLLEXP void releaseUnusedResources()
	{
		Modules::resMan().releaseUnusedResources();
	}


	DLLEXP ResHandle createTexture2D( const char *name, int flags, int width, int height, bool renderable )
	{
		Texture2DResource *texRes =
			new Texture2DResource( safeStr( name ), flags, width, height, renderable );

		ResHandle res = Modules::resMan().addNonExistingResource( *texRes, true );
		if( res == 0 )
		{	
			Modules::log().writeDebugInfo( "Failed to add resource in createTexture2D; might be the name is already in use?", res );
			delete texRes;
		}

		return res;
	}


	DLLEXP void setShaderPreambles( const char *vertPreamble, const char *fragPreamble )
	{
		ShaderResource::setPreambles( safeStr( vertPreamble ), safeStr( fragPreamble ) );
	}
	
	
	DLLEXP bool setMaterialUniform( ResHandle materialRes, const char *name, float a, float b, float c, float d )
	{
		Resource *res = Modules::resMan().resolveResHandle( materialRes );

		if( res != 0x0 && res->getType() == ResourceTypes::Material )
			return ((MaterialResource *)res)->setUniform( safeStr( name ), a, b, c, d );
		else
		{
			Modules::log().writeDebugInfo( "Invalid Material resource handle %i in setMaterialUniform", materialRes );
			return false;
		}
	}


	DLLEXP bool setPipelineStageActivation( ResHandle pipelineRes, const char *stageName, bool enabled )
	{
		Resource *res = Modules::resMan().resolveResHandle( pipelineRes );
		
		if( res != 0x0 && res->getType() == ResourceTypes::Pipeline )
			return ((PipelineResource *)res)->setStageActivation( safeStr( stageName ), enabled );
		else
		{
			Modules::log().writeDebugInfo( "Invalid Pipeline resource handle %i in setPipelineStageActivation", pipelineRes );
			return false;
		}
	}

	
	DLLEXP bool getPipelineRenderTargetData( ResHandle pipelineRes, const char *targetName,
											 int bufIndex, int *width, int *height, int *compCount,
											 float *dataBuffer, int bufferSize )
	{
		Resource *res = Modules::resMan().resolveResHandle( pipelineRes );
		
		if( res != 0x0 && res->getType() == ResourceTypes::Pipeline )
			return ((PipelineResource *)res)->getRenderTargetData(
				safeStr( targetName ), bufIndex, width, height, compCount, dataBuffer, bufferSize );
		else
		{
			Modules::log().writeDebugInfo( "Invalid Pipeline resource handle %i in getPipelineRenderTargetData", pipelineRes );
			return false;
		}
	}


	// *********************************************************************************************
	// Scene graph functions
	// *********************************************************************************************
	
	DLLEXP int getNodeType( NodeHandle node )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 ) return (SceneNodeTypes::List)sn->getType();
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeType", node );
			return SceneNodeTypes::Undefined;
		}
	}


	DLLEXP NodeHandle getNodeParent( NodeHandle node )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if ( sn == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeParent", node );
			return 0;
		}
		if ( sn->getParent() != 0x0 ) return sn->getParent()->getHandle();
		else return 0;
	}


	DLLEXP bool setNodeParent( NodeHandle node, NodeHandle parent )
	{
		return Modules::sceneMan().relocateNode( node, parent );
	}


	DLLEXP NodeHandle getNodeChild( NodeHandle parent, int index )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( parent );
		if( sn == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeChild", parent );
			return 0;
		}

		if( (unsigned)index < sn->getChildren().size() )
			return sn->getChildren()[index]->getHandle();
		else
			return 0;
	}


	DLLEXP NodeHandle addNodes( NodeHandle parent, ResHandle sceneGraphRes )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addNodes", parent );
			return 0;
		}
		
		Resource *res = Modules::resMan().resolveResHandle( sceneGraphRes );
		if( res == 0x0 || res->getType() != ResourceTypes::SceneGraph )
		{
			Modules::log().writeDebugInfo( "Invalid SceneGraph resource handle %i in addNodes", sceneGraphRes );
			return 0;
		}
		
		if( !res->isLoaded() )
		{
			Modules::log().writeDebugInfo( "Unloaded SceneGraph resource specified for addNodes (handle: %i)", sceneGraphRes );
			return 0;
		}
		
		Modules::log().writeInfo( "Adding nodes from SceneGraph resource '%s'", res->getName().c_str() );
		return Modules::sceneMan().addNodes( *parentNode, *(SceneGraphResource *)res );
	}


	DLLEXP bool removeNode( NodeHandle node )
	{
		Modules::log().writeInfo( "Removing node %i", node );
		
		return Modules::sceneMan().removeNode( node );
	}


	DLLEXP bool setNodeActivation( NodeHandle node, bool active )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 )
		{
			sn->setActivation( active );
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeActivation", node );
			return false;
		}
	}


	DLLEXP bool checkNodeTransformFlag( NodeHandle node, bool reset )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in checkNodeTransformFlag", node );
			return false;
		}
		
		return sn->checkTransformFlag( reset );
	}

	
	DLLEXP bool getNodeTransform( NodeHandle node, float *tx, float *ty, float *tz,
								  float *rx, float *ry, float *rz, float *sx, float *sy, float *sz )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 )
		{
			Vec3f trans, rot, scale;
			sn->getTransform( trans, rot, scale );

			if( tx != 0x0 ) *tx = trans.x; if( ty != 0x0 ) *ty = trans.y; if( tz != 0x0 ) *tz = trans.z;
			if( rx != 0x0 ) *rx = rot.x; if( ry != 0x0 ) *ry = rot.y; if( rz != 0x0 ) *rz = rot.z;
			if( sx != 0x0 ) *sx = scale.x; if( sy != 0x0 ) *sy = scale.y; if( sz != 0x0 ) *sz = scale.z;
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeTransform", node );
			return false;
		}
	}
	
	
	DLLEXP bool setNodeTransform( NodeHandle node, float tx, float ty, float tz,
								  float rx, float ry, float rz, float sx, float sy, float sz )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 )
		{
			sn->setTransform( Vec3f( tx, ty, tz ), Vec3f( rx, ry, rz ), Vec3f( sx, sy, sz ) );
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeTransform", node );
			return false;
		}
	}


	DLLEXP bool getNodeTransformMatrices( NodeHandle node, const float **relMat, const float **absMat )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 )
		{
			sn->getTransMatrices( relMat, absMat );
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeTransformMatrices", node );
			return false;
		}
	}


	DLLEXP bool setNodeTransformMatrix( NodeHandle node, const float *mat4x4 )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 && mat4x4 != 0x0 )
		{
			static Matrix4f mat;
			memcpy( mat.c, mat4x4, 16 * sizeof( float ) );
			sn->setTransform( mat );
			return true;
		}
		else if( sn == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeTransformMatrix", node );
			return false;
		}
		else 
		{
			Modules::log().writeDebugInfo( "Invalid transformation data in setNodeTransformMatrix");
			return false;
		}
	}


	DLLEXP float getNodeParamf( NodeHandle node, int param )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 ) return sn->getParamf( param );
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeParamf", node );
			return Math::NaN;
		}
	}


	DLLEXP bool setNodeParamf( NodeHandle node, int param, float value )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 ) return sn->setParamf( param, value );
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeParamf", node );
			return false;
		}
	}


	DLLEXP int getNodeParami( NodeHandle node, int param )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 ) return sn->getParami( param );
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeParami", node );
			return Math::MinInt32;
		}
	}


	DLLEXP bool setNodeParami( NodeHandle node, int param, int value )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 ) return sn->setParami( param, value );
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeParami", node );
			return false;
		}
	}


	DLLEXP const char *getNodeParamstr( NodeHandle node, int param )
	{
		static char emptyString = '\0';
		
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 ) return sn->getParamstr( param );
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeParamstr", node );
			return &emptyString;
		}
	}


	DLLEXP bool setNodeParamstr( NodeHandle node, int param, const char *name )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		
		if( sn != 0x0 )
		{	
			sn->setParamstr( param, safeStr( name ).c_str() );
			return true;
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid node handle %i in setNodeParamstr", node );
			return false;
		}
	}


	DLLEXP bool getNodeAABB( NodeHandle node, float *minX, float *minY, float *minZ,
							 float *maxX, float *maxY, float *maxZ )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 )
		{
			Modules::sceneMan().updateNodes();
			
			if( minX != 0x0 ) *minX = sn->getBBox().getMinCoords().x;
			if( minY != 0x0 ) *minY = sn->getBBox().getMinCoords().y;
			if( minZ != 0x0 ) *minZ = sn->getBBox().getMinCoords().z;
			if( maxX != 0x0 ) *maxX = sn->getBBox().getMaxCoords().x;
			if( maxY != 0x0 ) *maxY = sn->getBBox().getMaxCoords().y;
			if( maxZ != 0x0 ) *maxZ = sn->getBBox().getMaxCoords().z;
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in getNodeAABB", node );
			return false;
		}
	}


	DLLEXP int findNodes( NodeHandle startNode, const char *name, int type )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( startNode );
		if( sn == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in findNodes", startNode );
			return 0;
		}
		
		Modules::sceneMan().clearFindResults();
		return Modules::sceneMan().findNodes( sn, safeStr( name ), type );
	}


	DLLEXP NodeHandle getNodeFindResult( int index )
	{
		SceneNode *sn = Modules::sceneMan().getFindResult( index );
		
		if( sn != 0x0 ) return sn->getHandle();
		else return 0;
	}


	DLLEXP NodeHandle castRay( NodeHandle node, float ox, float oy, float oz, float dx, float dy, float dz, int numNearest )
	{
		SceneNode* sn = Modules::sceneMan().resolveNodeHandle( node );
		if ( sn == 0x0 )
		{
			Modules::log().writeDebugInfo( "Invalid node handle %i in castRay", node );
			return 0;
		}

		Modules::sceneMan().updateNodes();
		
		return Modules::sceneMan().castRay( sn, Vec3f( ox, oy, oz ), Vec3f( dx, dy, dz ), numNearest );
	}


	DLLEXP bool getCastRayResult( int index, NodeHandle *node, float *distance, float *intersection )
	{
		CastRayResult crr;
		if( Modules::sceneMan().getCastRayResult( index, crr ) )
		{
			if( node ) *node = crr.node->getHandle();
			if( distance ) *distance = crr.distance;
			if( intersection )
			{
				intersection[0] = crr.intersection.x;
				intersection[1] = crr.intersection.y;
				intersection[2] = crr.intersection.z;
			}

			return true;
		}

		return false;
	}


	DLLEXP NodeHandle addGroupNode( NodeHandle parent, const char *name )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addGroupNode", parent );
			return 0;
		}
		
		GroupNodeTpl groupTpl( "" );

		Modules::log().writeInfo( "Adding Group node '%s'", safeStr( name ).c_str() );

		GroupNodeTpl tpl( safeStr( name ) );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Group )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP NodeHandle addModelNode( NodeHandle parent, const char *name, ResHandle geometryRes )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addModelNode", parent );
			return 0;
		}
		
		Resource *res = Modules::resMan().resolveResHandle( geometryRes );
		if( res == 0x0 || res->getType() != ResourceTypes::Geometry ) return 0;

		Modules::log().writeInfo( "Adding Model node '%s'", safeStr( name ).c_str() );
		
		ModelNodeTpl tpl( safeStr( name ), (GeometryResource *)res );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Model )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP bool setupModelAnimStage( NodeHandle modelNode, int stage, ResHandle animationRes,
									 const char *startNode, bool additive )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( modelNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Model )
		{
			string firstNode( safeStr( startNode ) );
			return ((ModelNode *)sn)->setupAnimStage( stage, animationRes, firstNode, additive );
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Model node handle %i in setupModelAnimStage", modelNode );
			return false;
		}
	}


	DLLEXP bool setModelAnimParams( NodeHandle modelNode, int stage, float time, float weight )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( modelNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Model )
		{
			return ((ModelNode *)sn)->setAnimParams( stage, time, weight );
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Model node handle %i in setModelAnimParams", modelNode );
			return false;
		}
	}


	DLLEXP bool setModelMorpher( NodeHandle modelNode, const char *target, float weight )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( modelNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Model )
		{
			return ((ModelNode *)sn)->setMorphParam( safeStr( target ), weight );
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Model node handle %i in setModelMorpher", modelNode );
			return false;
		}
	}


	DLLEXP NodeHandle addMeshNode( NodeHandle parent, const char *name, ResHandle materialRes,
								   int batchStart, int batchCount, int vertRStart, int vertREnd )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addMeshNode", parent );
			return 0;
		}
		
		Resource *res = Modules::resMan().resolveResHandle( materialRes );
		if( res == 0x0 || res->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource %i in addMeshNode", materialRes );
			return 0;
		}

		Modules::log().writeInfo( "Adding Mesh node '%s'", safeStr( name ).c_str() );

		MeshNodeTpl tpl( safeStr( name ), (MaterialResource *)res, (unsigned)batchStart,
						 (unsigned)batchCount, (unsigned)vertRStart, (unsigned)vertREnd );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Mesh )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP NodeHandle addJointNode( NodeHandle parent, const char *name, int jointIndex )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addJointNode", parent );
			return 0;
		}

		Modules::log().writeInfo( "Adding Joint node '%s'", safeStr( name ).c_str() );

		JointNodeTpl tpl( safeStr( name ), (unsigned)jointIndex );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Joint )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP NodeHandle addLightNode( NodeHandle parent, const char *name, ResHandle materialRes,
									const char *lightingContext, const char *shadowContext )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addLightNode", parent );
			return 0;
		}

		Resource *res = Modules::resMan().resolveResHandle( materialRes );
		if( res != 0x0 && res->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource %i in addLightNode", materialRes );
			return 0;
		}
		
		Modules::log().writeInfo( "Adding Light node '%s'", safeStr( name ).c_str() );
		
		LightNodeTpl tpl( safeStr( name ), (MaterialResource *)res,
						  safeStr( lightingContext ), safeStr( shadowContext ) );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Light )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP bool setLightContexts( NodeHandle lightNode, const char *lightingContext, const char *shadowContext )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( lightNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Light )
		{
			((LightNode *)sn)->setContexts( lightingContext, shadowContext );
			return true;
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Light node handle %i in setLightContexts", lightNode );
			return false;
		}
	}


	DLLEXP NodeHandle addCameraNode( NodeHandle parent, const char *name, ResHandle pipelineRes )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addCameraNode", parent );
			return 0;
		}

		Resource *res = Modules::resMan().resolveResHandle( pipelineRes );
		if( res != 0x0 && res->getType() != ResourceTypes::Pipeline )
		{	
			Modules::log().writeDebugInfo( "Invalid Pipeline resource %i in addCameraNode", pipelineRes );
			return 0;
		}
		
		Modules::log().writeInfo( "Adding Camera node '%s'", safeStr( name ).c_str() );
		
		CameraNodeTpl tpl( safeStr( name ), (PipelineResource *)res );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Camera )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP bool setupCameraView( NodeHandle cameraNode, float fov, float aspect, float nearDist, float farDist )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( cameraNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Camera )
		{
			((CameraNode *)sn)->setupViewParams( fov, aspect, nearDist, farDist );
			return true;
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Camera node handle %i in setupCameraView", cameraNode );
			return false;	
		}
	}


	DLLEXP bool calcCameraProjectionMatrix( NodeHandle cameraNode, float *projMat )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( cameraNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Camera && projMat != 0x0 )
		{
			memcpy( projMat, ((CameraNode *)sn)->calcProjectionMatrix().x, 16 * sizeof( float ) );
			return true;
		}
		else
		{
			Modules::log().writeDebugInfo( "Invalid Camera node handle %i in calcCameraProjectionMatrix", cameraNode );
			return false;
		}
	}


	DLLEXP NodeHandle addEmitterNode( NodeHandle parent, const char *name, ResHandle materialRes,
									  ResHandle effectRes, int maxParticleCount, int respawnCount )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 )
		{	
			Modules::log().writeDebugInfo( "Invalid parent node handle %i in addEmitterNode", parent );
			return 0;
		}

		Resource *matRes = Modules::resMan().resolveResHandle( materialRes );
		if( matRes == 0x0 || matRes->getType() != ResourceTypes::Material )
		{	
			Modules::log().writeDebugInfo( "Invalid Material resource %i in addEmitterNode", materialRes );
			return 0;
		}
		Resource *effRes = Modules::resMan().resolveResHandle( effectRes );
		if( effRes == 0x0 || effRes->getType() != ResourceTypes::Effect )
		{	
			Modules::log().writeDebugInfo( "Invalid Effect resource %i in addEmitterNode", effectRes );
			return 0;
		}
		
		Modules::log().writeInfo( "Adding Emitter node '%s'", safeStr( name ).c_str() );

		EmitterNodeTpl tpl( safeStr( name ), (MaterialResource *)matRes, (EffectResource *)effRes,
							(unsigned)maxParticleCount, respawnCount );
		SceneNode *sn = Modules::sceneMan().findType( SceneNodeTypes::Emitter )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}


	DLLEXP bool advanceEmitterTime( NodeHandle emitterNode, float timeDelta )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( emitterNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Emitter )
		{
			((EmitterNode *)sn)->advanceTime( timeDelta );
			return true;
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Emitter node handle %i in advanceEmitterTime", emitterNode );
			return false;
		}
	}


	DLLEXP bool hasEmitterFinished( NodeHandle emitterNode )
	{
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( emitterNode );
		if( sn != 0x0 && sn->getType() == SceneNodeTypes::Emitter )
		{
			return ((EmitterNode *)sn)->hasFinished();
		}
		else
		{	
			Modules::log().writeDebugInfo( "Invalid Emitter node handle %i in hasEmitterFinished", emitterNode );
			return false;
		}
	}
}


// *************************************************************************************************
// DLL entry point
// *************************************************************************************************


#ifdef PLATFORM_WIN
BOOL APIENTRY DllMain( HANDLE /*hModule*/, DWORD /*ul_reason_for_call*/, LPVOID /*lpReserved*/ )
{
	#if defined( _MSC_VER ) && defined( _DEBUG )
	//_crtBreakAlloc = 1397;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
	
	/*switch( ul_reason_for_call )
	{
	case DLL_PROCESS_DETACH:
		_CrtDumpMemoryLeaks();
		break;
	}*/
	
	return TRUE;
}
#endif
