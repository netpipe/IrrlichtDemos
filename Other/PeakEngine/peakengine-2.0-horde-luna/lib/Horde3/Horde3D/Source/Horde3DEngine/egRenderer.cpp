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

#include "egRenderer.h"
#include "utOpenGL.h"
#include "egModules.h"
#include "egGeometry.h"
#include "egModel.h"
#include "egParticle.h"
#include "egMaterial.h"
#include "egTextures.h"
#include "egShader.h"
#include "egLight.h"

#include "utDebug.h"

const char *vsDefColor =
	"uniform mat4 worldMat;\n"
	"varying vec4 color;\n"
	"void main() {\n"
	"	color = gl_Color;\n"
	"	gl_Position = gl_ModelViewProjectionMatrix * worldMat * gl_Vertex;\n"
	"}\n";

const char *fsDefColor =
	"varying vec4 color;\n"
	"void main() {\n"
	"	gl_FragColor = color;\n"
	"}\n";

const char *vsOccBox =
	"void main() {\n"
	"	gl_Position = ftransform();\n"
	"}\n";

const char *fsOccBox =
	"void main() {\n"
	"	gl_FragColor = vec4( 1, 0, 0, 0 );\n"
	"}\n";


ShaderContext Renderer::defColorShader;
ShaderContext Renderer::occShader;


Renderer::Renderer() : RendererBase()
{
	_frameID = 0;
	_smFBO = 0; _smTex = 0;
	_defShadowMap = 0;
	_particleVBO = 0;
	_curCamera = 0x0;
	_curLight = 0x0;
	_curMatRes = 0x0;
	_curShader = 0x0;
	_curRenderTarget = 0x0;
	_curUpdateStamp = 1;
}


Renderer::~Renderer()
{
	destroyShadowBuffer();
	unloadTexture( _defShadowMap, false );
	if( _particleVBO != 0 ) unloadBuffers( _particleVBO, 0 );
}


void Renderer::initStates()
{
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glDepthFunc( GL_LEQUAL );
	glShadeModel( GL_SMOOTH );
	glDisable( GL_MULTISAMPLE );
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glClearDepth( 1.0f );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	
}


bool Renderer::init()
{
	bool failed = false;
	
	if( !RendererBase::init() ) failed = true;

	string vendor = (char *)glGetString( GL_VENDOR );
	string renderer = (char *)glGetString( GL_RENDERER );
	string version = (char *)glGetString( GL_VERSION );
	
	Modules::log().writeInfo( "Initializing OpenGL renderer using OGL '%s' by '%s' on '%s'",
							  version.c_str(), vendor.c_str(), renderer.c_str() );

	// Check that OpenGL 2.0 is available
	if( glExt::majorVersion < 2 || glExt::minorVersion < 0 )
	{
		Modules::log().writeError( "OpenGL 2.0 not supported" );
		failed = true;
	}
	
	// Check extensions
	if( !glExt::EXT_framebuffer_object )
	{
		Modules::log().writeError( "Extension EXT_framebuffer_object not supported" );
		failed = true;
	}
	if( !glExt::EXT_texture_filter_anisotropic )
	{
		Modules::log().writeError( "Extension EXT_texture_filter_anisotropic not supported" );
		failed = true;
	}
	if( !glExt::EXT_texture_compression_s3tc )
	{
		Modules::log().writeError( "Extension EXT_texture_compression_s3tc not supported" );
		failed = true;
	}
	if( !glExt::ARB_texture_float )
	{
		Modules::log().writeWarning( "Extension ARB_texture_float not supported" );
	}
	if( !glExt::ARB_texture_non_power_of_two )
	{
		Modules::log().writeWarning( "Extension ARB_texture_non_power_of_two not supported" );
	}
	if( !glExt::EXT_framebuffer_multisample )
	{
		Modules::log().writeWarning( "Extension EXT_framebuffer_multisample and/or EXT_framebuffer_blit not supported" );
	}

	if( failed )
	{
		Modules::log().writeError( "Failed to init renderer, debug info following" );
		string exts = (char *)glGetString( GL_EXTENSIONS );
		Modules::log().writeInfo( "Supported extensions: '%s'", exts.c_str() );

		return false;
	}

	// Init OpenGL states
	initStates();
	
	// Upload default shaders
	uploadShader( vsDefColor, fsDefColor, defColorShader );
	uploadShader( vsOccBox, fsOccBox, occShader );
	
	// Create shadow map
	if( !createShadowBuffer( Modules::config().shadowMapSize,
							 Modules::config().shadowMapSize ) )
	{
		Modules::log().writeError( "Failed to create shadow map" );
		return false;
	}

	// Create default shadow map
	float shadowTex[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	glGenTextures( 1, &_defShadowMap );
	glBindTexture( GL_TEXTURE_2D, _defShadowMap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 8, 8, 0,
				  GL_DEPTH_COMPONENT, GL_FLOAT, shadowTex );

	// Create particle geometry array
	ParticleVert v0( 0, 0, 0 );
	ParticleVert v1( 1, 0, 1 );
	ParticleVert v2( 1, 1, 2 );
	ParticleVert v3( 0, 1, 3 );
	
	ParticleVert parVerts[ParticlesPerBatch * 4];
	for( uint32 i = 0; i < ParticlesPerBatch; ++i )
	{
		parVerts[i * 4 + 0] = v0; parVerts[i * 4 + 0].index = (float)i;
		parVerts[i * 4 + 1] = v1; parVerts[i * 4 + 1].index = (float)i;
		parVerts[i * 4 + 2] = v2; parVerts[i * 4 + 2].index = (float)i;
		parVerts[i * 4 + 3] = v3; parVerts[i * 4 + 3].index = (float)i;

	}
	_particleVBO = Modules::renderer().uploadVertices(
		(float *)parVerts, ParticlesPerBatch * 4 * sizeof( ParticleVert ) );

	_overlays.reserve( 100 );

	_statTriCount = 0; _statBatchCount = 0; _statLightPassCount = 0;
	
	return true;
}


void Renderer::resize( int x, int y, int width, int height )
{
	RendererBase::resize( x, y, width, height );
	setRenderBuffer( _curRendBuf );
}


float Renderer::getStat( int param, bool reset )
{
	float value;	
	
	switch( param )
	{
	case EngineStats::TriCount:
		value = (float)_statTriCount;
		if( reset ) _statTriCount = 0;
		return value;
	case EngineStats::BatchCount:
		value = (float)_statBatchCount;
		if( reset ) _statBatchCount = 0;
		return value;
	case EngineStats::LightPassCount:
		value = (float)_statLightPassCount;
		if( reset ) _statLightPassCount = 0;
		return value;
	default:
		return 0;
	}
}


void Renderer::incStat( int param, float value )
{
	switch( param )
	{
	case EngineStats::TriCount:
		_statTriCount += (int)value;
		break;
	case EngineStats::BatchCount:
		_statBatchCount += (int)value;
		break;
	case EngineStats::LightPassCount:
		_statLightPassCount += (int)value;
		break;
	}
}


void Renderer::showOverlay( const Overlay &overlay, uint32 matRes )
{
	Resource *res = Modules::resMan().resolveResHandle( matRes );
	if( res != 0x0 && res->getType() == ResourceTypes::Material )
	{
		_overlays.push_back( overlay );
		_overlays[_overlays.size() - 1].materialRes = (MaterialResource *)res;
	}
}


void Renderer::clearOverlays()
{
	_overlays.resize( 0 );
}


int Renderer::registerOccSet()
{
	for( int i = 0; i < (int)_occSets.size(); ++i )
	{
		if( _occSets[i] == 0 )
		{
			_occSets[i] = 1;
			return i;
		}
	}

	_occSets.push_back( 1 );
	return (int)_occSets.size() - 1;
}


void Renderer::unregisterOccSet( int occSet )
{
	if( occSet >= 0 && occSet < (int)_occSets.size() )
		_occSets[occSet] = 0;
}


bool Renderer::uploadShader( const char *vertexShader, const char *fragmentShader, ShaderContext &sc )
{
	// Create shader
	uint32 shaderId = loadShader( vertexShader, fragmentShader );
	if( shaderId == 0 ) return false;
	
	// Bind attributes
	// Note: Index 0 is reserved for gl_Vertex when enabling GL_VERTEX_ARRAY
	// Mesh
	glBindAttribLocation( shaderId, 1, "normal" );
	glBindAttribLocation( shaderId, 2, "tangent" );
	glBindAttribLocation( shaderId, 3, "bitangent" );
	glBindAttribLocation( shaderId, 4, "joints" );
	glBindAttribLocation( shaderId, 5, "weights" );
	glBindAttribLocation( shaderId, 6, "texCoords0" );
	glBindAttribLocation( shaderId, 7, "texCoords1" );
	
	// Particles
	glBindAttribLocation( shaderId, 1, "parIdx" );
	glBindAttribLocation( shaderId, 2, "parCornerIdx" );

	// Link shader
	if( !linkShader( shaderId ) ) return false;
	glUseProgram( shaderId );
	sc.shaderObject = shaderId;
	
	// Set standard uniforms
	sc.uni_texs[0] = glGetUniformLocation( shaderId, "tex0" );
	if( sc.uni_texs[0] >= 0 ) glUniform1i( sc.uni_texs[0], 0 );
	sc.uni_texs[1] = glGetUniformLocation( shaderId, "tex1" );
	if( sc.uni_texs[1] >= 0 ) glUniform1i( sc.uni_texs[1], 1 );
	sc.uni_texs[2] = glGetUniformLocation( shaderId, "tex2" );
	if( sc.uni_texs[2] >= 0 ) glUniform1i( sc.uni_texs[2], 2 );
	sc.uni_texs[3] = glGetUniformLocation( shaderId, "tex3" );
	if( sc.uni_texs[3] >= 0 ) glUniform1i( sc.uni_texs[3], 3 );
	sc.uni_texs[4] = glGetUniformLocation( shaderId, "tex4" );
	if( sc.uni_texs[4] >= 0 ) glUniform1i( sc.uni_texs[4], 4 );
	sc.uni_texs[5] = glGetUniformLocation( shaderId, "tex5" );
	if( sc.uni_texs[5] >= 0 ) glUniform1i( sc.uni_texs[5], 5 );
	sc.uni_texs[6] = glGetUniformLocation( shaderId, "tex6" );
	if( sc.uni_texs[6] >= 0 ) glUniform1i( sc.uni_texs[6], 6 );
	sc.uni_texs[7] = glGetUniformLocation( shaderId, "tex7" );
	if( sc.uni_texs[7] >= 0 ) glUniform1i( sc.uni_texs[7], 7 );
	sc.uni_texs[8] = glGetUniformLocation( shaderId, "tex8" );
	if( sc.uni_texs[8] >= 0 ) glUniform1i( sc.uni_texs[8], 8 );
	sc.uni_texs[9] = glGetUniformLocation( shaderId, "tex9" );
	if( sc.uni_texs[9] >= 0 ) glUniform1i( sc.uni_texs[9], 9 );
	sc.uni_texs[10] = glGetUniformLocation( shaderId, "tex10" );
	if( sc.uni_texs[10] >= 0 ) glUniform1i( sc.uni_texs[10], 10 );
	sc.uni_texs[11] = glGetUniformLocation( shaderId, "tex11" );
	if( sc.uni_texs[11] >= 0 ) glUniform1i( sc.uni_texs[11], 11 );

	int loc = glGetUniformLocation( shaderId, "shadowMap" );
	if( loc >= 0 ) glUniform1i( loc, 12 );

	// Get uniform locations
	sc.uni_frameBufSize = glGetUniformLocation( shaderId, "frameBufSize" );
	sc.uni_worldMat = glGetUniformLocation( shaderId, "worldMat" );
	sc.uni_worldNormalMat = glGetUniformLocation( shaderId, "worldNormalMat" );
	sc.uni_viewer = glGetUniformLocation( shaderId, "viewer" );
	sc.uni_lightPos = glGetUniformLocation( shaderId, "lightPos" );
	sc.uni_lightDir = glGetUniformLocation( shaderId, "lightDir" );
	sc.uni_lightColor = glGetUniformLocation( shaderId, "lightColor" );
	sc.uni_lightCosCutoff = glGetUniformLocation( shaderId, "lightCosCutoff" );
	sc.uni_shadowSplitDists = glGetUniformLocation( shaderId, "shadowSplitDists" );
	sc.uni_shadowMats = glGetUniformLocation( shaderId, "shadowMats" );
	sc.uni_shadowMapSize = glGetUniformLocation( shaderId, "shadowMapSize" );
	sc.uni_shadowBias = glGetUniformLocation( shaderId, "shadowBias" );
	sc.uni_skinMatRows = glGetUniformLocation( shaderId, "skinMatRows[0]" );
	sc.uni_parCorners = glGetUniformLocation( shaderId, "parCorners" );
	sc.uni_parPosArray = glGetUniformLocation( shaderId, "parPosArray" );
	sc.uni_parSizeAndRotArray = glGetUniformLocation( shaderId, "parSizeAndRotArray" );
	sc.uni_parColorArray = glGetUniformLocation( shaderId, "parColorArray" );

	// Get attribute locations
	sc.attrib_normal = glGetAttribLocation( shaderId, "normal" );
	sc.attrib_tangent = glGetAttribLocation( shaderId, "tangent" );
	sc.attrib_bitangent = glGetAttribLocation( shaderId, "bitangent" );
	sc.attrib_joints = glGetAttribLocation( shaderId, "joints" );
	sc.attrib_weights = glGetAttribLocation( shaderId, "weights" );
	sc.attrib_texCoords0 = glGetAttribLocation( shaderId, "texCoords0" );
	sc.attrib_texCoords1 = glGetAttribLocation( shaderId, "texCoords1" );

	// Get custom uniforms
	int uniformCount;
	int size;
	uint32 type;
	char charBuf[128];
	glGetProgramiv( shaderId, GL_ACTIVE_UNIFORMS, &uniformCount );
	for( int i = 0; i < uniformCount; ++i )
	{
		glGetActiveUniform( shaderId, i, 127, 0x0, &size, &type, charBuf );
		string name = charBuf;

		if( name.find( "gl_" ) != 0 &&
			name != "tex0" && name != "tex1" && name != "tex2" && name != "tex3" &&
			name != "tex4" && name != "tex5" && name != "tex6" && name != "tex7" &&
			name != "tex8" && name != "tex9" && name != "tex10" && name != "tex11" &&
			name != "frameBufSize" && name != "worldMat" && name != "worldNormalMat" &&
			name != "viewer" && name != "lightPos" && name != "lightDir" && name != "lightColor" &&
			name != "lightCosCutoff" && name != "shadowSplitDists" && name != "shadowMats" &&
			name != "shadowMapSize" && name != "shadowMapSize" && name != "shadowBias" &&
			name != "skinMatRows[0]" && name != "parCorners" && name != "parPosArray" &&
			name != "parSizeAndRotArray" && name != "parColorArray" )
		{
			sc.customUniforms[name] = glGetUniformLocation( shaderId, name.c_str() );
		}
	}

	return true;
}


void Renderer::setShader( ShaderContext *sc )
{
	if( sc == 0x0 || sc->shaderObject == 0 )
	{
		_curShader = 0x0;
		glUseProgram( 0 );
	}
	else
	{
		_curShader = sc;
		glUseProgram( sc->shaderObject );
	}
}


bool Renderer::setMaterialRec( MaterialResource *materialRes, const string &shaderContext, bool firstRec )
{
	if( materialRes == 0x0 ) return false;
	bool result = true;
	
	// Setup shader and render config (ignore shader for links)
	if( firstRec && materialRes->_shaderRes != 0x0 )
	{
		ShaderContext *sc = materialRes->_shaderRes->findContext( shaderContext );
		if( sc == 0x0 ) return false;
		
		if( sc != _curShader ) setShader( sc );	

		if( sc->writeDepth ) glDepthMask( GL_TRUE );
		else glDepthMask( GL_FALSE );

		if( sc->blendMode == BlendModes::Replace )
		{
			glDisable( GL_BLEND );
		}
		else if( sc->blendMode == BlendModes::Blend )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		else if( sc->blendMode == BlendModes::Add )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
		}
		else if( sc->blendMode == BlendModes::AddBlended )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		else if( sc->blendMode == BlendModes::Mult )
		{
			glEnable( GL_BLEND );
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
		}
	}
	if( _curShader == 0x0 ) return false;
	
	// Setup standard shader uniforms
	// Note: Make sure that all functions which modify one of the following params increase stamp
	if( _curShader->lastUpdateStamp != _curUpdateStamp )
	{
		if( _curShader->uni_frameBufSize >= 0 )
				glUniform2f( _curShader->uni_frameBufSize, (float)_fbWidth, (float)_fbHeight );
		
		if( _curCamera != 0x0 )	 // Viewer params
		{
			if( _curShader->uni_viewer >= 0 )
				glUniform3fv( _curShader->uni_viewer, 1, &_curCamera->_absTrans.x[12] );
		}
		if( _curLight != 0x0 )	// Light params
		{
			if( _curShader->uni_lightPos >= 0 )
				glUniform4f( _curShader->uni_lightPos, _curLight->_absPos.x, _curLight->_absPos.y,
							 _curLight->_absPos.z, _curLight->_radius );
			
			if( _curShader->uni_lightDir >= 0 )
				glUniform3fv( _curShader->uni_lightDir, 1, &_curLight->_spotDir.x );
			
			if( _curShader->uni_lightColor >= 0 )
				glUniform3f( _curShader->uni_lightColor, _curLight->_diffCol_R,
							 _curLight->_diffCol_G, _curLight->_diffCol_B );
			
			if( _curShader->uni_lightCosCutoff >= 0 )
				glUniform1f( _curShader->uni_lightCosCutoff, cosf( degToRad( _curLight->_fov/ 2 ) ) );
			
			if( _curShader->uni_shadowSplitDists >= 0 )
				glUniform4fv( _curShader->uni_shadowSplitDists, 1, &_splitPlanes[1] );

			if( _curShader->uni_shadowMats >= 0 )
				glUniformMatrix4fv( _curShader->uni_shadowMats, 4, false, &_lightMats[0].x[0] );
			
			if( _curShader->uni_shadowMapSize >= 0 )
				glUniform1f( _curShader->uni_shadowMapSize,
							 (float)Modules::config().shadowMapSize );
			if( _curShader->uni_shadowBias >= 0 )
				glUniform1f( _curShader->uni_shadowBias, _curLight->_shadowMapBias );
		}

		_curShader->lastUpdateStamp = _curUpdateStamp;
	}
	
	// Setup material parameters
	for( uint32 i = 0; i < materialRes->_texUnits.size(); ++i )
	{
		TexUnit &texUnit = materialRes->_texUnits[i];
		
		if( texUnit.texRes != 0x0 && texUnit.unit < 12 && _curShader->uni_texs[texUnit.unit] >= 0 )
		{
			glActiveTexture( GL_TEXTURE0 + texUnit.unit );
			
			if( texUnit.texRes->getType() == ResourceTypes::Texture2D )
			{
				Texture2DResource *texRes = (Texture2DResource *)texUnit.texRes.getPtr();
				glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
				glBindTexture( GL_TEXTURE_2D, texRes->getTexObject() );
			}
			else if( texUnit.texRes->getType() == ResourceTypes::TextureCube )
			{
				TextureCubeResource *texRes = (TextureCubeResource *)texUnit.texRes.getPtr();
				glBindTexture( GL_TEXTURE_CUBE_MAP, texRes->getTexObject() );
			}
		}
	}

	glActiveTexture( GL_TEXTURE0 );

	// Setup material shader uniforms
	for( uint32 i = 0; i < materialRes->_uniforms.size(); ++i )
	{
		Uniform &uniform = materialRes->_uniforms[i];

		map< string, int >::iterator itr = _curShader->customUniforms.find( uniform.name );
		if( itr != _curShader->customUniforms.end() )
		{
			glUniform4f( (*itr).second, uniform.values[0], uniform.values[1],
						 uniform.values[2], uniform.values[3] );
		}
	}

	if( firstRec )
	{
		// Handle link of stage
		if( _curStageMatLink != 0x0 && _curStageMatLink != materialRes )
			result &= setMaterialRec( _curStageMatLink, shaderContext );

		// Handle material of light source
		if( _curLight != 0x0 && _curLight->_materialRes != 0x0 && _curLight->_materialRes != materialRes )
			result &= setMaterialRec( _curLight->_materialRes, shaderContext );
	}

	// Handle link of material resource
	if( materialRes->_matLink != 0x0 )
		result &= setMaterialRec( materialRes->_matLink, shaderContext );

	return result;
}


bool Renderer::setMaterial( MaterialResource *materialRes, const string &shaderContext )
{
	if( materialRes == 0x0 )
	{	
		_curMatRes = 0x0;
		_curShader = 0x0;
		return false;
	}
	else if( _curShader != 0x0 && materialRes == _curMatRes ) return true;
		
	_curMatRes = materialRes;
	
	bool result = setMaterialRec( materialRes, shaderContext, true );

	if( !result )
	{
		_curMatRes = 0x0;
		_curShader = 0x0;
	}

	return result;
}


void Renderer::setupViewMatrices( CameraNode *cam )
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( cam->calcProjectionMatrix().x );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( cam->_invTrans.x );
}


void Renderer::drawRenderables( const string &shaderContext, const string &theClass, bool debugView,
							    const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order,
								int occSet )
{
	if( Modules::config().wireframeMode && !Modules::config().debugViewMode )
	{
		glDisable( GL_CULL_FACE );
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	
	map< int, NodeRegEntry >::const_iterator itr = Modules::sceneMan()._registry.begin();
	while( itr != Modules::sceneMan()._registry.end() )
	{
		if( itr->second.renderFunc != 0x0 )
			(*itr->second.renderFunc)( shaderContext, theClass, debugView, frust1, frust2, order, occSet );

		++itr;
	}

	if( Modules::config().wireframeMode && !Modules::config().debugViewMode )
	{
		glEnable( GL_CULL_FACE );
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}


bool Renderer::createShadowBuffer( uint32 width, uint32 height )
{
	int curFBO;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_EXT, &curFBO );
	
	// Create framebuffer
	glGenFramebuffersEXT( 1, &_smFBO );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _smFBO );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );

	// Attach renderable textures
	glGenTextures( 1, &_smTex );
	glBindTexture( GL_TEXTURE_2D, _smTex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, _smTex, 0 );

	int depthBits;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH_SIZE, &depthBits );
	if( depthBits == 16 )
	{
		Modules::log().writeWarning( "Shadow map precision is limited to 16 bit" );
	}

	// Check if successful
	uint32 status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, curFBO );
	
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT ) return false;
	
	return true;
}


void Renderer::destroyShadowBuffer()
{
	if( _smTex != 0 ) glDeleteTextures( 1, &_smTex );
	if( _smFBO != 0 ) glDeleteFramebuffersEXT( 1, &_smFBO );

	_smTex = 0; _smFBO = 0;
}


Matrix4f Renderer::calcLightMat( const Frustum &frustum )
{
	// Find bounding box of visible geometry
	Modules::sceneMan().updateQueues( frustum, 0x0, RenderingOrder::None, false, true );
	BoundingBox bBox;
	for( size_t j = 0, s = Modules::sceneMan().getRenderableQueue().size(); j < s; ++j )
	{
		bBox.makeUnion( Modules::sceneMan().getRenderableQueue()[j]->getBBox() ); 
	}
	
	// Get light matrix
	float projMat[16];
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	myPerspective( _curLight->_fov, 1, _curCamera->_frustNear, _curLight->_radius );
	glGetFloatv( GL_PROJECTION_MATRIX, projMat );
	glMatrixMode( GL_MODELVIEW );

	Matrix4f lightMat = Matrix4f( projMat ) * _curLight->_invTrans;

	// Get frustum and bounding box extents in post-projective space
	float frustMinX =  Math::MaxFloat, bbMinX =  Math::MaxFloat;
    float frustMinY =  Math::MaxFloat, bbMinY =  Math::MaxFloat;
	float frustMinZ =  Math::MaxFloat, bbMinZ =  Math::MaxFloat;
	float frustMaxX = -Math::MaxFloat, bbMaxX = -Math::MaxFloat;
    float frustMaxY = -Math::MaxFloat, bbMaxY = -Math::MaxFloat;
	float frustMaxZ = -Math::MaxFloat, bbMaxZ = -Math::MaxFloat;
	
	for( uint32 i = 0; i < 8; ++i )
	{
		// Frustum
		Vec4f v1 = lightMat * Vec4f( frustum.getCorner( i ) );
		v1.w = fabs( v1.w );	// Use absolute value to reduce problems with back projection when v1.w < 0
		v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w;

		if( v1.x < frustMinX ) frustMinX = v1.x;
		if( v1.y < frustMinY ) frustMinY = v1.y;
		if( v1.z < frustMinZ ) frustMinZ = v1.z;
		if( v1.x > frustMaxX ) frustMaxX = v1.x;
		if( v1.y > frustMaxY ) frustMaxY = v1.y;
		if( v1.z > frustMaxZ ) frustMaxZ = v1.z;

		// Bounding box
		v1 = lightMat * Vec4f( bBox.getCorner( i ) );
		v1.w = fabs( v1.w );
		v1.x /= v1.w; v1.y /= v1.w; v1.z /= v1.w;

		if( v1.x < bbMinX ) bbMinX = v1.x;
		if( v1.y < bbMinY ) bbMinY = v1.y;
		if( v1.z < bbMinZ ) bbMinZ = v1.z;
		if( v1.x > bbMaxX ) bbMaxX = v1.x;
		if( v1.y > bbMaxY ) bbMaxY = v1.y;
		if( v1.z > bbMaxZ ) bbMaxZ = v1.z;
	}

	// Combine frustum and bounding box
	float minX = max( frustMinX, bbMinX );
	float minY = max( frustMinY, bbMinY );
	float minZ = min( frustMinZ, bbMinZ );
	float maxX = min( frustMaxX, bbMaxX );
	float maxY = min( frustMaxY, bbMaxY );
	float maxZ = min( frustMaxZ, bbMaxZ );
	
	// Clamp the min and max values to post projection range [-1, 1]
	minX = clamp( minX, -1, 1 );
	minY = clamp( minY, -1, 1 );
	minZ = clamp( minZ, -1, 1 );
	maxX = clamp( maxX, -1, 1 );
	maxY = clamp( maxY, -1, 1 );
	maxZ = clamp( maxZ, -1, 1 );

	// Zoom in current split and create appropriate matrix
	Matrix4f cropView;
	float scaleX = 2.0f / (maxX - minX);
	float scaleY = 2.0f / (maxY - minY);
	float offsetX = -0.5f * (maxX + minX) * scaleX;
	float offsetY = -0.5f * (maxY + minY) * scaleY;
	float scaleZ = 1.0f / (maxZ - minZ);
	float offsetZ = -minZ * scaleZ;

	cropView.x[0] = scaleX;
	cropView.x[5] = scaleY;
	cropView.x[10] = scaleZ;
	cropView.x[12] = offsetX;
	cropView.x[13] = offsetY;
	cropView.x[14] = offsetZ;

	return cropView * Matrix4f( projMat );
}


void Renderer::updateShadowMap()
{
	if( _curLight == 0x0 || _curCamera == 0x0 ) return;
	
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _smFBO );
	glViewport( 0, 0, Modules::config().shadowMapSize, Modules::config().shadowMapSize );
	_fbWidth = Modules::config().shadowMapSize;
	_fbHeight = Modules::config().shadowMapSize;
	
	glDepthMask( GL_TRUE );
	glClearDepth( 1.0f );
    glClear( GL_DEPTH_BUFFER_BIT );

	// ********************************************************************************************
	// Cascaded Shadow Maps
	// ********************************************************************************************

	Frustum splitFrust;
	
	// Find bounding box of lit geometry
	_curLight->genFrustum( _lightFrustum );
	Modules::sceneMan().updateQueues( _lightFrustum, 0x0, RenderingOrder::None, false, true );
	BoundingBox bBox;
	for( size_t j = 0, s = Modules::sceneMan().getRenderableQueue().size(); j < s; ++j )
	{
		bBox.makeUnion( Modules::sceneMan().getRenderableQueue()[j]->getBBox() ); 
	}
	// Adjust camera planes
	float minDist = Math::MaxFloat, maxDist = 0.0f;
	for( uint32 i = 0; i < 8; ++i )
	{
		float dist = -(_curCamera->_invTrans * bBox.getCorner( i )).z;
		if( dist < minDist ) minDist = dist;
		if( dist > maxDist ) maxDist = dist;
	}

	// Don't adjust near plane; this means less precision if scene is far away from viewer
	// but that shouldn't be too noticeable and brings better performance since the nearer
	// split volumes are empty
	minDist = _curCamera->_frustNear;
	
	// Caluclate split distances using PSSM scheme
	const float nearDist = maxf( minDist, _curCamera->_frustNear );
	const float farDist = maxf( maxDist, minDist + 1 );
	const uint32 numMaps = _curLight->_shadowMapCount;
	const float t = _curLight->_shadowSplitLambda;
	
	_splitPlanes[0] = nearDist;
	_splitPlanes[numMaps] = farDist;

	for( uint32 i = 1; i < numMaps; i++ )
	{
		float f = i / (float)numMaps;
		float log = nearDist * pow( farDist / nearDist, f );
		float uni = nearDist + (farDist - nearDist) * f;
		
		_splitPlanes[i] = t * log + (1 - t) * uni;
	}
	
	// Prepare shadow map rendering
	glEnable( GL_DEPTH_TEST );
	//glCullFace( GL_FRONT );	// Front face culling reduces artefacts but produces more "peter-panning"
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( _curLight->_invTrans.x );
	
	// Build split frustums and render shadow maps
	for( uint32 i = 0; i < numMaps; ++i )
	{
		// Build split frustum
		if( !_curCamera->_orthographic )
		{
			float newLeft = _curCamera->_frustLeft * _splitPlanes[i] / _curCamera->_frustNear;
			float newRight = _curCamera->_frustRight * _splitPlanes[i] / _curCamera->_frustNear;
			float newBottom = _curCamera->_frustBottom * _splitPlanes[i] / _curCamera->_frustNear;
			float newTop = _curCamera->_frustTop * _splitPlanes[i] / _curCamera->_frustNear;
			splitFrust.buildViewFrustum( _curCamera->_absTrans, newLeft, newRight, newBottom, newTop,
										 _splitPlanes[i], _splitPlanes[i + 1] );
		}
		else
		{
			splitFrust.buildBoxFrustum( _curCamera->_absTrans, _curCamera->_frustLeft, _curCamera->_frustRight,
										_curCamera->_frustBottom, _curCamera->_frustTop,
										-_splitPlanes[i], -_splitPlanes[i + 1] );
		}
		
		// Build light projection matrix
		_lightMats[i] = calcLightMat( splitFrust );
		glMatrixMode( GL_PROJECTION );
		glLoadMatrixf( &_lightMats[i].x[0] );
		
		// Frustum Culling
		_lightFrustum.buildViewFrustum( _curLight->_invTrans, _lightMats[i] );
		Modules::sceneMan().updateQueues( _lightFrustum, 0x0, RenderingOrder::None, false, true );
		
		_lightMats[i] = _lightMats[i] * _curLight->_invTrans;

		if( numMaps > 1 )
		{
			// Select quadrant of shadow map atlas
			const int sh = Modules::config().shadowMapSize / 2;
			if( i == 0 ) glViewport( 0, 0, sh, sh );
			else if( i == 1 ) glViewport( sh, 0, sh, sh );
			else if( i == 2 ) glViewport( sh, sh, sh, sh );
			else if( i == 3 ) glViewport( 0, sh, sh, sh );
		}
		else
		{
			const int sh = Modules::config().shadowMapSize;
			glViewport( 0, 0, sh, sh );
		}
	
		// Render
		drawRenderables( _curLight->_shadowContext, "", false, &_lightFrustum, 0x0, RenderingOrder::None, -1 );
	}

	// Setup light matrices for rendering:
	if( numMaps > 1 )
	{
		// Map from [-1,1] to [0,1] and select appropriate quadrant of shadow map
		_lightMats[0].scale( 0.25f, 0.25f, 0 );
		_lightMats[0].translate( 0.25f, 0.25f, 0 );
		_lightMats[1].scale( 0.25f, 0.25f, 0 );
		_lightMats[1].translate( 0.75f, 0.25f, 0 );
		_lightMats[2].scale( 0.25f, 0.25f, 0 );
		_lightMats[2].translate( 0.75f, 0.75f, 0 );
		_lightMats[3].scale( 0.25f, 0.25f, 0 );
		_lightMats[3].translate( 0.25f, 0.75f, 0 );
	}
	else
	{
		_lightMats[0].scale( 0.5f, 0.5f, 0 );
		_lightMats[0].translate( 0.5f, 0.5f, 0 );
	}

	// ********************************************************************************************

	glMatrixMode( GL_MODELVIEW );
	glCullFace( GL_BACK );
		
	setRenderBuffer( _curRendBuf );
}


void Renderer::setupShadowMap( bool noShadows )
{
	// Bind shadow map
	glActiveTexture( GL_TEXTURE12 );
	if( !noShadows && _curLight->_shadowMapCount > 0 )glBindTexture( GL_TEXTURE_2D, _smTex );
	else glBindTexture( GL_TEXTURE_2D, _defShadowMap );
	
	glActiveTexture( GL_TEXTURE0 );
}


void Renderer::bindBuffer( RenderBuffer *rb, uint32 texUnit, uint32 bufIndex )
{
	if( rb == 0x0 )
	{
		for( uint32 i = 0; i < 12; ++i )
		{
			glActiveTexture( GL_TEXTURE0 + i );
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
	}
	else
	{
		if( texUnit < 12 )
		{
			glActiveTexture( GL_TEXTURE0 + texUnit );
			if( bufIndex < 4 && rb->colBufs[bufIndex] != 0 )
				glBindTexture( GL_TEXTURE_2D, rb->colBufs[bufIndex] );
			else if( bufIndex == 32 && rb->depthBuf != 0 )
				glBindTexture( GL_TEXTURE_2D, rb->depthBuf );
		}
	}

	glActiveTexture( GL_TEXTURE0 );
}


void Renderer::clear( bool depth, bool buf0, bool buf1, bool buf2, bool buf3,
					  float r, float g, float b, float a )
{
	int mask = 0;

	glPushAttrib( GL_COLOR_BUFFER_BIT );	// Store state of glDrawBuffers
	
	glDisable( GL_BLEND );	// Clearing floating point buffers causes problems when blending is enabled on Radeon 9600
	glDepthMask( GL_TRUE );
	glClearColor( r, g, b, a );

	if( _curRendBuf != 0x0 )
	{
		uint32 buffers[4], cnt = 0;

		if( depth && _curRendBuf->depthBuf != 0 ) mask |= GL_DEPTH_BUFFER_BIT;
		
		if( buf0 && _curRendBuf->colBufs[0] != 0 ) buffers[cnt++] = GL_COLOR_ATTACHMENT0_EXT;
		if( buf1 && _curRendBuf->colBufs[1] != 0 ) buffers[cnt++] = GL_COLOR_ATTACHMENT1_EXT;
		if( buf2 && _curRendBuf->colBufs[2] != 0 ) buffers[cnt++] = GL_COLOR_ATTACHMENT2_EXT;
		if( buf3 && _curRendBuf->colBufs[3] != 0 ) buffers[cnt++] = GL_COLOR_ATTACHMENT3_EXT;

		if( cnt > 0 )
		{	
			mask |= GL_COLOR_BUFFER_BIT;
			glDrawBuffers( cnt, buffers );
		}
	}
	else
	{
		if( depth ) mask |= GL_DEPTH_BUFFER_BIT;
		if( buf0 ) mask |= GL_COLOR_BUFFER_BIT;
		glScissor( _vpX, _vpY, _vpWidth, _vpHeight );
		glEnable( GL_SCISSOR_TEST );
	}
	
	if( mask != 0 ) glClear( mask );
	glDisable( GL_SCISSOR_TEST );
	glPopAttrib();
}


void Renderer::drawOverlays( const string &shaderContext )
{
	setMaterial( 0x0, "" );
	++_curUpdateStamp;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, 1, 0, 1, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glDisable( GL_DEPTH_TEST );
	
	for( int i = 0; i < 8; ++i )
	{
		for( uint32 j = 0; j < _overlays.size(); ++j )
		{
			if( _overlays[j].layer != i ) continue;

			Overlay &overlay = _overlays[j];

			if( !setMaterial( overlay.materialRes, shaderContext ) ) continue;
			
			glBegin( GL_QUADS );
			glTexCoord2fv( &overlay.u_ll ); glVertex2fv( &overlay.x_ll );
			glTexCoord2fv( &overlay.u_lr ); glVertex2fv( &overlay.x_lr );
			glTexCoord2fv( &overlay.u_ur ); glVertex2fv( &overlay.x_ur );
			glTexCoord2fv( &overlay.u_ul ); glVertex2fv( &overlay.x_ul );
			glEnd();
		}
	}

	glEnable( GL_DEPTH_TEST );
}


void Renderer::drawFSQuad( Resource *matRes, const string &shaderContext )
{
	if( matRes == 0x0 || matRes->getType() != ResourceTypes::Material ) return;
	
	// Reset current material
	setMaterial( 0x0, "" );
	++_curUpdateStamp;

	if( !setMaterial( (MaterialResource *)matRes, shaderContext ) ) return;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, 1, 0, 1, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glDepthFunc( GL_ALWAYS );
	
	glBegin(GL_QUADS);
	glTexCoord2f( 0, 0 ); glVertex2f( 0, 0 );
	glTexCoord2f( 1, 0 ); glVertex2f( 1, 0 );
	glTexCoord2f( 1, 1 ); glVertex2f( 1, 1 );
	glTexCoord2f( 0, 1 ); glVertex2f( 0, 1 );
	glEnd();

	glDepthFunc( GL_LEQUAL );
}


void Renderer::drawGeometry( const string &shaderContext, const string &theClass,
							 RenderingOrder::List order, int occSet )
{
	if( _curCamera == 0x0 ) return;
	
	++_curUpdateStamp;
	
	_curCamera->genFrustum( _camFrustum );
	Modules::sceneMan().updateQueues( _camFrustum, 0x0, order, false, true );
	
	setupViewMatrices( _curCamera );
	
	drawRenderables( shaderContext, theClass, false, &_camFrustum, 0x0, order, occSet );
}


void Renderer::drawLightGeometry( const string shaderContext, const string &theClass,
								  bool noShadows, RenderingOrder::List order, int occSet )
{
	if( _curCamera == 0x0 ) return;
	
	string context = shaderContext;
	
	_curCamera->genFrustum( _camFrustum );
	Modules::sceneMan().updateQueues( _camFrustum, 0x0, RenderingOrder::None, true, false );
	
	for( size_t i = 0, s = Modules::sceneMan().getLightQueue().size(); i < s; ++i )
	{
		++_curUpdateStamp;
		_curLight = (LightNode *)Modules::sceneMan().getLightQueue()[i];
		_curLight->genFrustum( _lightFrustum );

		// Check if light is not visible
		if( _camFrustum.cullFrustum( _lightFrustum ) ) continue;
	
		// Calculate light screen space position
		float bbx, bby, bbw, bbh;
		_curLight->calcScreenSpaceAABB( _curCamera->calcProjectionMatrix() * _curCamera->_invTrans,
										bbx, bby, bbw, bbh );

		// Update shadow map
		if( !noShadows && _curLight->_shadowMapCount > 0 ) updateShadowMap();

		// Set scissor rectangle
		if( bbx != 0 || bby != 0 || bbw != 1 || bbh != 1 )
		{
			glScissor( (int)(bbx * _fbWidth), (int)(bby * _fbHeight),
					   (int)(bbw * _fbWidth), (int)(bbh * _fbHeight) );
			glEnable( GL_SCISSOR_TEST );
		}
		
		setupShadowMap( noShadows );

		if( shaderContext == "" ) context = _curLight->_lightingContext;
		
		// Render
		_curCamera->genFrustum( _camFrustum );
		_curLight->genFrustum( _lightFrustum );
		Modules::sceneMan().updateQueues( _camFrustum, &_lightFrustum, RenderingOrder::None, false, true );
		setupViewMatrices( _curCamera );
		drawRenderables( context, theClass, false, &_camFrustum, &_lightFrustum, order, occSet );
		incStat( EngineStats::LightPassCount, 1 );

		// Reset
		glDisable( GL_SCISSOR_TEST );
		glActiveTexture( GL_TEXTURE12 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glActiveTexture( GL_TEXTURE0 );
	}

	_curLight = 0x0;
}


void Renderer::drawLightShapes( const string shaderContext, bool noShadows )
{
	if( _curCamera == 0x0 ) return;
	
	string context = shaderContext;
	
	_curCamera->genFrustum( _camFrustum );
	Modules::sceneMan().updateQueues( _camFrustum, 0x0, RenderingOrder::None, true, false );
	
	for( size_t i = 0, s = Modules::sceneMan().getLightQueue().size(); i < s; ++i )
	{
		++_curUpdateStamp;
		_curLight = (LightNode *)Modules::sceneMan().getLightQueue()[i];
		_curLight->genFrustum( _lightFrustum );
		
		// Check if light is not visible
		if( _camFrustum.cullFrustum( _lightFrustum ) ) continue;
		
		// Calculate light screen space position
		float bbx, bby, bbw, bbh;
		_curLight->calcScreenSpaceAABB( _curCamera->calcProjectionMatrix() * _curCamera->_invTrans,
										bbx, bby, bbw, bbh );
		
		// Update shadow map
		if( !noShadows && _curLight->_shadowMapCount > 0 ) updateShadowMap();

		// Prepare postprocessing step (set the camera transformation in MV matrix)
		glDisable( GL_DEPTH_TEST );
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, 1, 0, 1, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		glLoadMatrixf( _curCamera->_invTrans.x );
		
		setupShadowMap( noShadows );

		if( shaderContext == "" ) context = _curLight->_lightingContext;

		setMaterial( 0x0, "" );		// Reset material
		if( !setMaterial( _curLight->_materialRes, context ) ) continue;
		
		// Draw quad
		glBegin( GL_QUADS );
		glTexCoord2f( bbx, bby ); glVertex2f( bbx, bby );
		glTexCoord2f( bbx + bbw, bby ); glVertex2f( bbx + bbw, bby );
		glTexCoord2f( bbx + bbw, bby + bbh ); glVertex2f( bbx + bbw, bby + bbh );
		glTexCoord2f( bbx, bby + bbh ); glVertex2f( bbx, bby + bbh );
		glEnd();
		incStat( EngineStats::LightPassCount, 1 );

		// Reset
		glEnable( GL_DEPTH_TEST );
		glActiveTexture( GL_TEXTURE12 );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glActiveTexture( GL_TEXTURE0 );
	}

	_curLight = 0x0;
}


void Renderer::drawAABB( const Vec3f &bbMin, const Vec3f &bbMax )
{
	static const unsigned int indices[24] = {
		0, 1, 2, 3,
		7, 6, 5, 4,
		1, 5, 6, 2,
		4, 0, 3, 7,
		3, 2, 6, 7,
		4, 5, 1, 0
	};
	
	Vec3f corners[8] = {
		Vec3f( bbMin.x, bbMin.y, bbMin.z ),
		Vec3f( bbMax.x, bbMin.y, bbMin.z ),
		Vec3f( bbMax.x, bbMax.y, bbMin.z ),
		Vec3f( bbMin.x, bbMax.y, bbMin.z ),
		Vec3f( bbMin.x, bbMin.y, bbMax.z ),
		Vec3f( bbMax.x, bbMin.y, bbMax.z ),
		Vec3f( bbMax.x, bbMax.y, bbMax.z ),
		Vec3f( bbMin.x, bbMax.y, bbMax.z )
	};

	glBegin( GL_QUADS );
	for( unsigned int i = 0; i < 24; ++i )
	{
		glVertex3fv( &corners[indices[i]].x );
	}
	glEnd();
}


void Renderer::drawDebugAABB( const Vec3f &bbMin, const Vec3f &bbMax, bool saveStates )
{
	static const unsigned int indices[24] = {
		0, 1, 2, 3,
		7, 6, 5, 4,
		1, 5, 6, 2,
		4, 0, 3, 7,
		3, 2, 6, 7,
		4, 5, 1, 0
	};
	
	Vec3f corners[8] = {
		Vec3f( bbMin.x, bbMin.y, bbMin.z ),
		Vec3f( bbMax.x, bbMin.y, bbMin.z ),
		Vec3f( bbMax.x, bbMax.y, bbMin.z ),
		Vec3f( bbMin.x, bbMax.y, bbMin.z ),
		Vec3f( bbMin.x, bbMin.y, bbMax.z ),
		Vec3f( bbMax.x, bbMin.y, bbMax.z ),
		Vec3f( bbMax.x, bbMax.y, bbMax.z ),
		Vec3f( bbMin.x, bbMax.y, bbMax.z )
	};

	GLint array_buffer = 0; 
	GLint element_buffer = 0; 
	GLint shader = 0; 
	GLboolean vertexArray = 0;
	GLint vSize = 0; 
	GLint vType = 0; 
	GLint vStride = 0; 
	GLvoid *vArray = 0x0; 
	
	// Save old states
	if( saveStates )
	{
		glGetIntegerv( GL_ARRAY_BUFFER_BINDING, &array_buffer );
		glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &element_buffer );
		glGetIntegerv( GL_CURRENT_PROGRAM, &shader );
		glIsEnabled( GL_VERTEX_ARRAY );	
		glGetIntegerv( GL_VERTEX_ARRAY_SIZE, &vSize );
		glGetIntegerv( GL_VERTEX_ARRAY_TYPE, &vType );
		glGetIntegerv( GL_VERTEX_ARRAY_STRIDE, &vStride );
		glGetPointerv( GL_VERTEX_ARRAY_POINTER, &vArray );
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT | GL_CLIENT_VERTEX_ARRAY_BIT );
	}

	glPushAttrib(GL_POLYGON_BIT | GL_CURRENT_BIT); 
	
	// Prepare rendering box
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_CULL_FACE );
	glUseProgram( defColorShader.shaderObject );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glColor4f( 1, 1, 1, 1 );
	glVertexPointer( 3, GL_FLOAT, 0, corners );
	glEnableClientState( GL_VERTEX_ARRAY );
	glDrawElements( GL_QUADS, 24, GL_UNSIGNED_INT, indices );	
	glEnable(GL_CULL_FACE);
	
	// Restore old states
	glPopAttrib();
	
	if( saveStates )
	{
		glUseProgram( shader );
		glBindBuffer( GL_ARRAY_BUFFER, array_buffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_buffer );
		if( vertexArray ) glVertexPointer( vSize, vType, vStride, vArray );
		glPopClientAttrib();
	}
}


void Renderer::renderDebugView()
{
	if( _curCamera == 0x0 ) return;
	
	setRenderBuffer( 0x0 );
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glDisable( GL_CULL_FACE );

	glClearColor( 0, 0, 0, 1 );
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

	_curCamera->genFrustum( _camFrustum );
	Modules::sceneMan().updateQueues( _camFrustum, 0x0, RenderingOrder::None, true, true );
	setupViewMatrices( _curCamera );

	// Draw nodes
	glColor4f( 0.5f, 0.75f, 1, 1 );
	drawRenderables( "", "", true, &_camFrustum, 0x0, RenderingOrder::None, -1 );

	// Draw bounding boxes
	setShader( &defColorShader );
	glUniformMatrix4fv( defColorShader.uni_worldMat, 1, false, &Matrix4f().x[0] );
	glColor4f( 0.4f, 0.4f, 0.4f, 1 );
	for( uint32 i = 0, s = (uint32)Modules::sceneMan().getRenderableQueue().size(); i < s; ++i )
	{
		SceneNode *sn = Modules::sceneMan().getRenderableQueue()[i];
		
		drawDebugAABB( sn->_bBox.getMinCoords(), sn->_bBox.getMaxCoords(), false );
	}

	// Draw light volumes
	glColor4f( 1, 1, 0, 0.25f );
	glLineWidth( 2 );
	for( size_t i = 0, s = Modules::sceneMan().getLightQueue().size(); i < s; ++i )
	{
		LightNode *lightNode = (LightNode *)Modules::sceneMan().getLightQueue()[i];
		
		if( lightNode->_fov < 180 )
		{
			glPushMatrix();
			glMultMatrixf( lightNode->_absTrans.x );
			
			// Render cone
			float r = lightNode->_radius * tanf( degToRad( lightNode->_fov / 2 ) );
			glBegin( GL_TRIANGLE_FAN );
			glVertex3f( 0, 0, 0 );
			for( int j = 32; j >= 0; --j )
			{
				glVertex3f( r * sinf( j / 32.0f * Math::TwoPi ),
							r * cosf( j / 32.0f * Math::TwoPi ),
							-lightNode->_radius );
			}
			glEnd();

			glPopMatrix();
		}
		else
		{
			drawDebugAABB( Vec3f( lightNode->_absPos.x - lightNode->_radius,
								  lightNode->_absPos.y - lightNode->_radius,
								  lightNode->_absPos.z - lightNode->_radius ),
						   Vec3f( lightNode->_absPos.x + lightNode->_radius,
								  lightNode->_absPos.y + lightNode->_radius,
								  lightNode->_absPos.z + lightNode->_radius ), 
								  false );
		}
	}
	glLineWidth( 1 );

	// Draw screen space projection of light sources
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glColor4f( 1, 1, 1, 0.25f );
	
	for( size_t i = 0, s = Modules::sceneMan().getLightQueue().size(); i < s; ++i )
	{
		LightNode *lightNode = (LightNode *)Modules::sceneMan().getLightQueue()[i];
		lightNode->genFrustum( _lightFrustum );
		
		if( _camFrustum.cullFrustum( _lightFrustum ) ) continue;

		// Calculate light screen space position
		float bbx, bby, bbw, bbh;
		lightNode->calcScreenSpaceAABB( _curCamera->calcProjectionMatrix() * _curCamera->_invTrans,
										bbx, bby, bbw, bbh );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		glOrtho( 0, 1, 0, 1, -1, 1 );
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		
		glBegin( GL_QUADS );
		glVertex2f( bbx, bby );
		glVertex2f( bbx + bbw, bby );
		glVertex2f( bbx + bbw, bby + bbh );
		glVertex2f( bbx, bby + bbh );
		glEnd();
	}

	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );
}


void Renderer::finishRendering()
{
	// Reset states for apps using debug mode to draw custom geometry
	// with direct OpenGL calls (e.g. Horde scene editor)
	setRenderBuffer( 0x0 );
	glDisable( GL_BLEND );
	glDepthMask( 1 );
	glDepthFunc( GL_LEQUAL );
	setShader( 0x0 );
	setupViewMatrices( _curCamera );
}


bool Renderer::render( CameraNode *camNode )
{
	_curCamera = camNode;
	if( _curCamera == 0x0 ) return false;

	++_frameID;
	
	if( Modules::config().debugViewMode || _curCamera->_pipelineRes == 0x0 )
	{
		renderDebugView();
		finishRendering();
		return true;
	}
	
	// Initialize
	_outputBufferIndex = _curCamera->_outputBufferIndex;
	if( _curCamera->_outputTex != 0x0 )
		setRenderBuffer( _curCamera->_outputTex->getRenderBuffer() );
	else 
		setRenderBuffer( 0x0 );

	// Process pipeline commands
	for( uint32 i = 0; i < _curCamera->_pipelineRes->_stages.size(); ++i )
	{
		PipelineStage &stage = _curCamera->_pipelineRes->_stages[i];
		if( !stage.enabled ) continue;
		_curStageMatLink = stage.matLink;
		
		for( uint32 j = 0; j < stage.commands.size(); ++j )
		{
			PipelineCommand &pc = stage.commands[j];
			RenderTarget *rt;

			switch( pc.command )
			{
			case PipelineCommands::SwitchTarget:
				// Unbind all textures
				bindBuffer( 0x0, 0, 0 );
				if( _curRenderTarget != 0x0 && _curRenderTarget->samples > 0 )
				{
					// Current render target is multisampled, so resolve it now
					blitRenderBuffer( _curRenderTarget->rendBufMultisample, _curRenderTarget->rendBuf );
				}
				
				// Bind new render target
				rt = (RenderTarget *)pc.refParams[0];
				_curRenderTarget = rt;

				if( rt != 0x0 )
				{
					if( rt->samples > 0 ) setRenderBuffer( &rt->rendBufMultisample );
					else setRenderBuffer( &rt->rendBuf );
				}
				else
				{
					if( _curCamera->_outputTex != 0x0 )
						setRenderBuffer( _curCamera->_outputTex->getRenderBuffer() );
					else 
						setRenderBuffer( 0x0 );
				}
				break;

			case PipelineCommands::BindBuffer:
				rt = (RenderTarget *)pc.refParams[0];
			
				bindBuffer( &rt->rendBuf, (uint32)((PCFloatParam *)pc.valParams[0])->get(),
							(uint32)((PCFloatParam *)pc.valParams[1])->get() );
				break;

			case PipelineCommands::ClearTarget:
				clear( ((PCBoolParam *)pc.valParams[0])->get(), ((PCBoolParam *)pc.valParams[1])->get(),
					   ((PCBoolParam *)pc.valParams[2])->get(), ((PCBoolParam *)pc.valParams[3])->get(),
					   ((PCBoolParam *)pc.valParams[4])->get(), ((PCFloatParam *)pc.valParams[5])->get(),
					   ((PCFloatParam *)pc.valParams[6])->get(), ((PCFloatParam *)pc.valParams[7])->get(),
					   ((PCFloatParam *)pc.valParams[8])->get() );
				break;

			case PipelineCommands::DrawGeometry:
				drawGeometry( ((PCStringParam *)pc.valParams[0])->get(), ((PCStringParam *)pc.valParams[1])->get(),
					(RenderingOrder::List)((PCIntParam *)pc.valParams[2])->get(), _curCamera->_occSet );
				break;

			case PipelineCommands::DrawOverlays:
				drawOverlays( ((PCStringParam *)pc.valParams[0])->get() );
				break;

			case PipelineCommands::DrawQuad:
				drawFSQuad( pc.resParams[0], ((PCStringParam *)pc.valParams[0])->get() );
			break;

			case PipelineCommands::DoForwardLightLoop:
				drawLightGeometry( ((PCStringParam *)pc.valParams[0])->get(), ((PCStringParam *)pc.valParams[1])->get(),
					((PCBoolParam *)pc.valParams[2])->get(), (RenderingOrder::List)((PCIntParam *)pc.valParams[3])->get(),
					_curCamera->_occSet );
				break;

			case PipelineCommands::DoDeferredLightLoop:
				drawLightShapes( ((PCStringParam *)pc.valParams[0])->get(), ((PCBoolParam *)pc.valParams[1])->get() );
				break;

			case PipelineCommands::SetUniform:
				if( pc.resParams[0] != 0x0 && pc.resParams[0]->getType() == ResourceTypes::Material )
				{
					((MaterialResource *)pc.resParams[0].getPtr())->setUniform(
						((PCStringParam *)pc.valParams[0])->get(), ((PCFloatParam *)pc.valParams[1])->get(),
						((PCFloatParam *)pc.valParams[2])->get(), ((PCFloatParam *)pc.valParams[3])->get(),
						((PCFloatParam *)pc.valParams[4])->get() );
				}
				break;
			}
		}
	}
	
	finishRendering();
	return true;
}


void Renderer::drawModels( const string &shaderContext, const string &theClass, bool debugView,
						   const Frustum *frust1, const Frustum *frust2, RenderingOrder::List order,
						   int occSet )
{
	if( frust1 == 0x0 ) return;
	
	GeometryResource *curGeoRes = 0x0;

	Modules::renderer().setMaterial( 0x0, "" );

	// Enable vertex array
	glEnableClientState( GL_VERTEX_ARRAY );

	// Loop over model queue
	for( size_t i = 0, s = Modules::sceneMan().getRenderableQueue().size(); i < s; ++i )
	{
		if( Modules::sceneMan().getRenderableQueue()[i]->getType() != SceneNodeTypes::Model ) continue;
		
		ModelNode *modelNode = (ModelNode *)Modules::sceneMan().getRenderableQueue()[i];
		if( modelNode->getGeometryResource() == 0x0 ) continue;

		bool occCulling = false;
		bool modelChanged = true;

		// Occlusion culling
		if( occSet >= 0 )
		{
			if( occSet > (int)modelNode->_occQueries.size() - 1 )
			{
				modelNode->_occQueries.resize( occSet + 1, 0 );
				modelNode->_lastVisited.resize( occSet + 1, 0 );
			}
			if( modelNode->_occQueries[occSet] == 0 )
			{
				modelNode->_occQueries[occSet] = Modules::renderer().createOccQuery();
				modelNode->_lastVisited[occSet] = 0;
			}
			else
			{
				if( modelNode->_lastVisited[occSet] != Modules::renderer().getFrameID() )
				{
					modelNode->_lastVisited[occSet] = Modules::renderer().getFrameID();
				
					// Check query result (viewer must be outside of bounding box)
					if( nearestDistToAABB( frust1->getOrigin(), modelNode->getBBox().getMinCoords(),
										   modelNode->getBBox().getMaxCoords() ) != 0 &&
						Modules::renderer().getOccQueryResult( modelNode->_occQueries[occSet] ) < 1 )
					{
						// Draw occlusion box
						glColorMask( 0, 0, 0, 0 );
						glDepthMask( 0 );
						Modules::renderer().beginOccQuery( modelNode->_occQueries[occSet] );
						Modules::renderer().setShader( &Modules::renderer().occShader );
						Modules::renderer().drawAABB( modelNode->getBBox().getMinCoords(),
													  modelNode->getBBox().getMaxCoords() );
						Modules::renderer().endOccQuery( modelNode->_occQueries[occSet] );
						Modules::renderer().setMaterial( 0x0, "" );
						glDepthMask( 1 );
						glColorMask( 1, 1, 1, 1 );

						continue;
					}
					else
						occCulling = true;
				}
			}
		}
		
		// Bind geometry
		if( curGeoRes != modelNode->getGeometryResource() )
		{
			curGeoRes = modelNode->getGeometryResource();
			Modules::renderer().setMaterial( 0x0, "" );
		
			// Indices
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, curGeoRes->getIndexBuffer() );

			// Vertices
			uint32 vertCount = curGeoRes->_vertCount;
			glBindBuffer( GL_ARRAY_BUFFER, curGeoRes->getVertBuffer() );
			glVertexPointer( 3, GL_FLOAT, 0, (char *)0 );
			glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + vertCount * 12 );
			glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + vertCount * 24 );
			glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, (char *)0 + vertCount * 36 );
			glVertexAttribPointer( 4, 4, GL_FLOAT, GL_FALSE,
								   sizeof( VertexDataStatic ), (char *)0 + vertCount * 48 + 8 );
			glVertexAttribPointer( 5, 4, GL_FLOAT, GL_FALSE,
								   sizeof( VertexDataStatic ), (char *)0 + vertCount * 48 + 24 );
			glVertexAttribPointer( 6, 2, GL_FLOAT, GL_FALSE,
								   sizeof( VertexDataStatic ), (char *)0 + vertCount * 48 );
			glVertexAttribPointer( 7, 2, GL_FLOAT, GL_FALSE,
								   sizeof( VertexDataStatic ), (char *)0 + vertCount * 48 + 40 );
		}
		
		// Sort meshes
		if( order == RenderingOrder::FrontToBack || order == RenderingOrder::BackToFront && frust1 != 0x0 )
		{
			for( uint32 j = 0; j < modelNode->_meshCount; ++j )
			{
				MeshNode *meshNode = (MeshNode *)modelNode->_nodeList[j].node;

				meshNode->tmpSortValue = nearestDistToAABB( frust1->getOrigin(),
					meshNode->_bBox.getMinCoords(), meshNode->_bBox.getMaxCoords() );
			}
		}

		if( order == RenderingOrder::FrontToBack )
			std::sort( modelNode->_nodeList.begin(), modelNode->_nodeList.begin() + modelNode->_meshCount,
					   nodeFrontToBackOrder );
		else if( order == RenderingOrder::BackToFront )
			std::sort( modelNode->_nodeList.begin(), modelNode->_nodeList.begin() + modelNode->_meshCount,
					   nodeBackToFrontOrder );
		else if( order == RenderingOrder::StateChanges )
			// Sort meshes by material to minimize state changes
			std::sort( modelNode->_nodeList.begin(), modelNode->_nodeList.begin() + modelNode->_meshCount,
					   meshMaterialOrder );
		
		
		if( occCulling )
			Modules::renderer().beginOccQuery( modelNode->_occQueries[occSet] );
		
		for( uint32 j = 0; j < modelNode->_meshCount; ++j )
		{
			MeshNode *meshNode = (MeshNode *)modelNode->_nodeList[j].node;

			if( !meshNode->_active ) continue;

			// Frustum culling for meshes
			if( (frust1 != 0x0 && frust1->cullBox( meshNode->_bBox )) ||
				(frust2 != 0x0 && frust2->cullBox( meshNode->_bBox )) )
			{
				continue;
			}
			
			// Check that mesh is valid
			if( meshNode->getBatchStart() + meshNode->getBatchCount() > curGeoRes->_indices.size() )
				continue;
			
			ShaderContext *prevShader = Modules::renderer().getCurShader();
			
			if( !debugView )
			{
				if( !meshNode->getMaterialRes()->isOfClass( theClass ) ) continue;
				if( !Modules::renderer().setMaterial( meshNode->getMaterialRes(), shaderContext ) ) continue;
			}
			else
			{
				Modules::renderer().setShader( &defColorShader );
			}

			ShaderContext *curShader = Modules::renderer().getCurShader();

			if( modelChanged || curShader != prevShader )
			{
				// Skeleton
				if( curShader->uni_skinMatRows >= 0 && !modelNode->_skinMatRows.empty() )
				{
					// Note:	OpenGL 2.1 supports mat4x3 but it is internally realized as mat4 on most
					//			hardware so it would require 4 instead of 3 uniform slots per joint
					
					glUniform4fv( curShader->uni_skinMatRows, (int)modelNode->_skinMatRows.size(),
								  (float *)&modelNode->_skinMatRows[0] );
				}

				modelChanged = false;
			}

			// World transformation
			if( curShader->uni_worldMat >= 0 )
			{
				glUniformMatrix4fv( curShader->uni_worldMat, 1, false, &meshNode->_absTrans.x[0] );
			}
			if( curShader->uni_worldNormalMat >= 0 )
			{
				// TODO: Optimize this
				Matrix4f normalMat4 = meshNode->_absTrans.inverted().transposed();
				float normalMat[9] = { normalMat4.x[0], normalMat4.x[1], normalMat4.x[2],
									   normalMat4.x[4], normalMat4.x[5], normalMat4.x[6],
									   normalMat4.x[8], normalMat4.x[9], normalMat4.x[10] };
				glUniformMatrix3fv( curShader->uni_worldNormalMat, 1, false, normalMat );
			}

			// Enable required vertex streams and disable others to save bandwidth
			if( curShader != prevShader )
			{
				if( curShader->attrib_normal >= 0 ) glEnableVertexAttribArray( 1 );
				else glDisableVertexAttribArray( 1 );
				if( curShader->attrib_tangent >= 0 ) glEnableVertexAttribArray( 2 );
				else glDisableVertexAttribArray( 2 );
				if( curShader->attrib_bitangent >= 0 ) glEnableVertexAttribArray( 3 );
				else glDisableVertexAttribArray( 3 );
				if( curShader->attrib_joints >= 0 ) glEnableVertexAttribArray( 4 );
				else glDisableVertexAttribArray( 4 );
				if( curShader->attrib_weights >= 0 ) glEnableVertexAttribArray( 5 );
				else glDisableVertexAttribArray( 5 );
				if( curShader->attrib_texCoords0 >= 0 ) glEnableVertexAttribArray( 6 );
				else glDisableVertexAttribArray( 6 );
				if( curShader->attrib_texCoords1 >= 0 ) glEnableVertexAttribArray( 7 );
				else glDisableVertexAttribArray( 7 );
			}

			// Render
			glDrawRangeElements( GL_TRIANGLES, meshNode->getVertRStart(), meshNode->getVertREnd(),
								 meshNode->getBatchCount(), 
								 curGeoRes->_16BitIndices ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
								 (char *)0 + meshNode->getBatchStart() *
								 (curGeoRes->_16BitIndices ? sizeof( short ) : sizeof( int )) );
			Modules::renderer().incStat( EngineStats::BatchCount, 1 );
			Modules::renderer().incStat( EngineStats::TriCount, meshNode->getBatchCount() / 3.0f );
		}

		if( occCulling )
			Modules::renderer().endOccQuery( modelNode->_occQueries[occSet] );
	}

	// Disable vertex streams
	glDisableClientState( GL_VERTEX_ARRAY );
	for( uint32 i = 1; i < 8; ++i ) glDisableVertexAttribArray( i );
}


void Renderer::drawParticles( const string &shaderContext, const string &theClass, bool debugView,
							  const Frustum *frust1, const Frustum * /*frust2*/, RenderingOrder::List /*order*/,
							  int occSet )
{
	if( frust1 == 0x0 ) return;
	if( debugView ) return;		// Don't render particles in debug view
	
	Modules::renderer().setMaterial( 0x0, "" );

	// Calculate right and up vectors for camera alignment
	float mat[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, mat );
	Vec3f right = Vec3f( mat[0], mat[4], mat[8] );
	Vec3f up = Vec3f (mat[1], mat[5], mat[9] );
	Vec3f corners[4] = { -right - up, right - up, right + up, -right + up };

	// Bind particle geometry arrays
	glBindBuffer( GL_ARRAY_BUFFER, Modules::renderer().getParticleVBO() );
	glVertexPointer( 3, GL_FLOAT, sizeof( ParticleVert ), (char *)0 );
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, sizeof( ParticleVert ), (char *)0 + sizeof( float ) * 6 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, sizeof( ParticleVert ), (char *)0 + sizeof( float ) * 5 );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 6, 3, GL_FLOAT, GL_FALSE, sizeof( ParticleVert ), (char *)0 + sizeof( float ) * 3 );
	glEnableVertexAttribArray( 6 );

	// Loop through emitter queue
	for( uint32 i = 0; i < Modules::sceneMan().getRenderableQueue().size(); ++i )
	{
		if( Modules::sceneMan().getRenderableQueue()[i]->getType() != SceneNodeTypes::Emitter ) continue; 
		
		EmitterNode *emitter = (EmitterNode *)Modules::sceneMan().getRenderableQueue()[i];
		
		if( emitter->_particleCount == 0 ) continue;
		if( !emitter->_materialRes->isOfClass( theClass ) ) continue;
		
		// Occlusion culling
		bool occCulling = false;
		if( occSet >= 0 )
		{
			if( occSet > (int)emitter->_occQueries.size() - 1 )
			{
				emitter->_occQueries.resize( occSet + 1, 0 );
				emitter->_lastVisited.resize( occSet + 1, 0 );
			}
			if( emitter->_occQueries[occSet] == 0 )
			{
				emitter->_occQueries[occSet] = Modules::renderer().createOccQuery();
				emitter->_lastVisited[occSet] = 0;
			}
			else
			{
				if( emitter->_lastVisited[occSet] != Modules::renderer().getFrameID() )
				{
					emitter->_lastVisited[occSet] = Modules::renderer().getFrameID();
				
					// Check query result (viewer must be outside of bounding box)
					if( nearestDistToAABB( frust1->getOrigin(), emitter->getLocalBBox()->getMinCoords(),
										   emitter->getLocalBBox()->getMaxCoords() ) != 0 &&
						Modules::renderer().getOccQueryResult( emitter->_occQueries[occSet] ) < 1 )
					{
						// Draw occlusion box
						glColorMask( 0, 0, 0, 0 );
						glDepthMask( 0 );
						Modules::renderer().beginOccQuery( emitter->_occQueries[occSet] );
						Modules::renderer().setShader( &Modules::renderer().occShader );
						Modules::renderer().drawAABB( emitter->getLocalBBox()->getMinCoords(),
													  emitter->getLocalBBox()->getMaxCoords() );
						Modules::renderer().endOccQuery( emitter->_occQueries[occSet] );
						Modules::renderer().setMaterial( 0x0, "" );
						glDepthMask( 1 );
						glColorMask( 1, 1, 1, 1 );

						continue;
					}
					else
						occCulling = true;
				}
			}
		}
		
		if( !Modules::renderer().setMaterial( emitter->_materialRes, shaderContext ) ) continue;

		if( occCulling )
			Modules::renderer().beginOccQuery( emitter->_occQueries[occSet] );
		
		// Shader uniforms
		ShaderContext *curShader = Modules::renderer().getCurShader();
		if( curShader->uni_parCorners >= 0 ) glUniform3fv( curShader->uni_parCorners, 4, (float *)corners );

		// Divide particles in batches and render them
		for( uint32 j = 0; j < emitter->_particleCount / ParticlesPerBatch; ++j )
		{
			// Check if batch needs to be rendered
			bool allDead = true;
			for( uint32 k = 0; k < ParticlesPerBatch; ++k )
			{
				if( emitter->_particles[j*ParticlesPerBatch + k].life > 0 )
				{
					allDead = false;
					break;
				}
			}
			if( allDead ) continue;

			// Render batch
			if( curShader->uni_parPosArray >= 0 )
				glUniform3fv( curShader->uni_parPosArray, ParticlesPerBatch, (float *)emitter->_parPositions + j*ParticlesPerBatch*3 );
			if( curShader->uni_parSizeAndRotArray >= 0 )
				glUniform2fv( curShader->uni_parSizeAndRotArray, ParticlesPerBatch, (float *)emitter->_parSizesANDRotations + j*ParticlesPerBatch*2 );
			if( curShader->uni_parColorArray >= 0 )
				glUniform4fv( curShader->uni_parColorArray, ParticlesPerBatch, (float *)emitter->_parColors + j*ParticlesPerBatch*4 );

			glDrawArrays( GL_QUADS, 0, ParticlesPerBatch * 4 );
			Modules::renderer().incStat( EngineStats::BatchCount, 1 );
			Modules::renderer().incStat( EngineStats::TriCount, ParticlesPerBatch * 2.0f );
		}

		uint32 count = emitter->_particleCount % ParticlesPerBatch;
		if( count > 0 )
		{
			uint32 offset = (int)(emitter->_particleCount / ParticlesPerBatch) * ParticlesPerBatch;
			
			// Check if batch needs to be rendered
			bool allDead = true;
			for( uint32 k = 0; k < count; ++k )
			{
				if( emitter->_particles[offset + k].life > 0 )
				{
					allDead = false;
					break;
				}
			}
			if( allDead ) continue;
			
			// Render batch
			if( curShader->uni_parPosArray >= 0 )
				glUniform3fv( curShader->uni_parPosArray, count, (float *)emitter->_parPositions + offset*3 );
			if( curShader->uni_parSizeAndRotArray >= 0 )
				glUniform2fv( curShader->uni_parSizeAndRotArray, count, (float *)emitter->_parSizesANDRotations + offset*2 );
			if( curShader->uni_parColorArray >= 0 )
				glUniform4fv( curShader->uni_parColorArray, count, (float *)emitter->_parColors + offset*4 );
			
			glDrawArrays( GL_QUADS, 0, count * 4 );
			Modules::renderer().incStat( EngineStats::BatchCount, 1 );
			Modules::renderer().incStat( EngineStats::TriCount, count * 2.0f );
		}

		if( occCulling )
			Modules::renderer().endOccQuery( emitter->_occQueries[occSet] );
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
	glDisableVertexAttribArray( 6 );
}
