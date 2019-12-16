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

#include "egRendererBase.h"
#include "egModules.h"
#include "utOpenGL.h"

#include "utDebug.h"


RendererBase::RendererBase()
{
	_vpWidth = 320; _vpHeight = 240;
	_curRendBuf = 0x0; _outputBufferIndex = 0;
}


RendererBase::~RendererBase()
{
}


bool RendererBase::init()
{
	return initOpenGLExtensions();
}


void RendererBase::resize( int x, int y, int width, int height )
{
	_vpX = x; _vpY = y; _vpWidth = width; _vpHeight = height;
}


void RendererBase::myPerspective( float fovy, float aspect, float zNear, float zFar ) 
{    
	double ymax = zNear * tan( degToRad( fovy / 2 ) );
	double xmax = ymax * aspect;
	glFrustum( -xmax, xmax, -ymax, ymax, zNear, zFar );
}


uint32 RendererBase::uploadVertices( void *data, uint32 size, uint32 bufId )
{
	// Note:
	// Interleaved arrays are generally faster than sperated arrays
	// Vertex size should be a multiple of 32 bytes (gives good performance increase)
	
	if( bufId == 0 )
	{	
		glGenBuffers( 1, &bufId );
		glBindBuffer( GL_ARRAY_BUFFER, bufId );
		glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
	}
	else
	{
		glBindBuffer( GL_ARRAY_BUFFER, bufId );
		glBufferData( GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW );
	}
	
	return bufId;
}


void RendererBase::updateVertices( void *data, uint32 offset, uint32 size, uint32 bufId )
{
	glBindBuffer( GL_ARRAY_BUFFER, bufId );
	glBufferSubData( GL_ARRAY_BUFFER, offset, size, data );
}


uint32 RendererBase::uploadIndices( void *indices, uint32 size, uint32 bufId )
{
	if( bufId == 0 ) 
	{	
		glGenBuffers( 1, &bufId );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufId );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
	}
	else
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufId );
		//glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, size, indices );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW );
	}
	
	return bufId;
}


void RendererBase::unloadBuffers( uint32 vertBufId, uint32 idxBufId )
{
	if( vertBufId != 0 )
	{
		glBindBuffer( GL_ARRAY_BUFFER, vertBufId );
		glBufferData( GL_ARRAY_BUFFER, 0, 0x0, GL_STATIC_DRAW );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDeleteBuffers( 1, &vertBufId );
	}

	if( idxBufId != 0 )
	{
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idxBufId );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, 0, 0x0, GL_STATIC_DRAW );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
		glDeleteBuffers( 1, &idxBufId );
	}
}


uint32 RendererBase::cloneVertexBuffer( uint32 vertBufId )
{
	int size;
	
	glBindBuffer( GL_ARRAY_BUFFER, vertBufId );
	glGetBufferParameteriv( GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );
	char *buffer = new char[size];
	glGetBufferSubData( GL_ARRAY_BUFFER, 0, size, buffer );
	uint32 bufId = uploadVertices( buffer, size );
	delete[] buffer;

	return bufId;
}


uint32 RendererBase::cloneIndexBuffer( uint32 idxBufId )
{
	int size;
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, idxBufId );
	glGetBufferParameteriv( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size );
	char *buffer = new char[size];
	glGetBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, size, buffer );
	uint32 bufId = uploadIndices( buffer, size );
	delete[] buffer;

	return bufId;
}


bool RendererBase::supportsNPOTTextures()
{
	return glExt::ARB_texture_non_power_of_two;
}


uint32 RendererBase::uploadTexture2D( void *pixels, int width, int height, int comps, bool hdr,
									  bool allowCompression, bool mipmaps, bool filtering,
									  bool repeatMode, uint32 texId )
{
	if( texId == 0 ) glGenTextures( 1, &texId );
	glBindTexture( GL_TEXTURE_2D, texId );
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode ? GL_REPEAT : GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode ? GL_REPEAT : GL_CLAMP_TO_EDGE );
	if( mipmaps )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Modules::config().anisotropyFactor );
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		if( Modules::config().trilinearFiltering )
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		else
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST );
	}
	
	int format = (comps == 4) ? GL_RGBA : GL_RGB;
	int type = hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
	int internalFormat;
	if( Modules::config().texCompression && allowCompression )
	{	
		internalFormat = comps == 4 ? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
	}
	else
	{
		if( hdr )
			internalFormat = comps == 4 ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		else
			internalFormat = comps == 4 ? GL_RGBA8 : GL_RGB8;
	}

	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels );
	
	return texId;
}


void RendererBase::updateTexture2D( unsigned char *pixels, int width, int height, int comps,
								    bool filtering, uint32 texId )
{
	glBindTexture( GL_TEXTURE_2D, texId );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
	
	int format = (comps == 4) ? GL_BGRA : GL_BGR;
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels );
}


uint32 RendererBase::uploadTextureCube( void *pixels, int width, int height, int comps, bool hdr,
										uint32 cubeFace, bool allowCompression, bool mipmaps,
										bool filtering, uint32 texId )
{
	if( texId == 0 ) glGenTextures( 1, &texId );
	glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
	
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	
	if( mipmaps )
	{
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, Modules::config().anisotropyFactor );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE );
		if( Modules::config().trilinearFiltering )
			glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		else
			glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST );
	}
	
	int format = (comps == 4) ? GL_RGBA : GL_RGB;
	int type = hdr ? GL_FLOAT : GL_UNSIGNED_BYTE;
	int internalFormat;
	if( Modules::config().texCompression && allowCompression )
	{
		internalFormat = comps == 4 ? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
	}
	else
	{	
		if( hdr )
			internalFormat = comps == 4 ? GL_RGBA16F_ARB : GL_RGB16F_ARB;
		else
			internalFormat = comps == 4 ? GL_RGBA8 : GL_RGB8;
	}

	glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + cubeFace, 0, internalFormat, width, height,
				  0, format, type, pixels );
	
	return texId;
}


void RendererBase::unloadTexture( uint32 texId, bool cubeMap )
{
	if( !cubeMap )
	{	
		glBindTexture( GL_TEXTURE_2D, texId );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	else 
	{	
		glBindTexture( GL_TEXTURE_CUBE_MAP, texId );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB8, 0, 0, 0, GL_RGB, GL_UNSIGNED_BYTE, 0x0 );
		glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
	}
	
	glDeleteTextures( 1, &texId );
}


float *RendererBase::downloadTexture2DData( uint32 texId, int *width, int *height )
{
	glBindTexture( GL_TEXTURE_2D, texId );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, width );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, height );
	
	float *pixels = new float[*width * *height * 4];
	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels );

	return pixels;
}


uint32 RendererBase::loadShader( const char *vertexShader, const char *fragmentShader )
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog = 0x0;
	int status;

	_shaderLog = "";

	// Vertex shader
	uint32 vs = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vs, 1, &vertexShader, 0x0 );
	glCompileShader( vs );
	glGetShaderiv( vs, GL_COMPILE_STATUS, &status );
	if( !status )
	{	
		// Get info
		glGetShaderiv( vs, GL_INFO_LOG_LENGTH, &infologLength );
		if( infologLength > 1 )
		{
			infoLog = new char[infologLength];
			glGetShaderInfoLog( vs, infologLength, &charsWritten, infoLog );
			_shaderLog = _shaderLog + "[Vertex Shader] " + "<pre>\n" + infoLog + "</pre>";
			delete[] infoLog; infoLog = 0x0;
		}

		glDeleteShader( vs );
		return 0;
	}

	// Fragment shader
	uint32 fs = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fs, 1, &fragmentShader, 0x0 );
	glCompileShader( fs );
	glGetShaderiv( fs, GL_COMPILE_STATUS, &status );
	if( !status )
	{	
		glGetShaderiv( fs, GL_INFO_LOG_LENGTH, &infologLength );
		if( infologLength > 1 )
		{
			infoLog = new char[infologLength];
			glGetShaderInfoLog( fs, infologLength, &charsWritten, infoLog );
			_shaderLog = _shaderLog + "[Fragment Shader] " + "<pre>\n" + infoLog + "</pre>";
			delete[] infoLog; infoLog = 0x0;
		}

		glDeleteShader( vs );
		glDeleteShader( fs );
		return 0;
	}

	// Shader program
	uint32 program = glCreateProgram();
	glAttachShader( program, vs );
	glAttachShader( program, fs );
	glDeleteShader( vs );
	glDeleteShader( fs );

	return program;
}


bool RendererBase::linkShader( uint32 shaderId )
{
	int infologLength = 0;
	int charsWritten = 0;
	char *infoLog = 0x0;
	int status;

	_shaderLog = "";
	
	glLinkProgram( shaderId );
	glGetProgramiv( shaderId, GL_INFO_LOG_LENGTH, &infologLength );
	if( infologLength > 1 )
	{
		infoLog = new char[infologLength];
		glGetProgramInfoLog( shaderId, infologLength, &charsWritten, infoLog );
		_shaderLog = _shaderLog + "[Linking] " + "<pre>\n" + infoLog + "</pre>";
		delete[] infoLog; infoLog = 0x0;
	}
	
	glGetProgramiv( shaderId, GL_LINK_STATUS, &status );
	if( !status ) return false;

	return true;
}


uint32 RendererBase::uploadShader( const char *vertexShader, const char *fragmentShader )
{
	uint32 shaderId = RendererBase::loadShader( vertexShader, fragmentShader );
	if( shaderId == 0 ) return 0;
	if( !linkShader( shaderId ) ) return 0;;

	return shaderId;
}


void RendererBase::unloadShader( uint32 shaderId )
{
	glDeleteProgram( shaderId );
}


RenderBuffer RendererBase::createRenderBuffer( uint32 width, uint32 height,
											   RenderBufferFormats::List format, bool depth,
											   uint32 numColBufs, bool bilinear, uint32 samples )
{
	if( (format == RenderBufferFormats::RGBA16F || format == RenderBufferFormats::RGBA32F) &&
		!glExt::ARB_texture_float )
	{
		return RenderBuffer();
	}

	if( numColBufs > RenderBuffer::MaxColorAttachmentCount ) return RenderBuffer();

	uint32 maxSamples = 0;
	if( glExt::EXT_framebuffer_multisample )
	{
		GLint value;
		glGetIntegerv( GL_MAX_SAMPLES_EXT, &value );
		maxSamples = (uint32)value;
	}
	if( samples > maxSamples )
	{
		samples = maxSamples;
		Modules::log().writeWarning( "GPU does not support desired multisampling quality for render target" );
	}

	RenderBuffer rb;
	rb.width = width;
	rb.height = height;
	rb.samples = samples;

	// Create framebuffer
	glGenFramebuffersEXT( 1, &rb.fbo );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, rb.fbo );

	if( numColBufs == 0 )
	{	
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );
	}
	else
	{
		// Attach color buffers
		uint32 glFormat = 0;
		if( format == RenderBufferFormats::RGBA8 ) glFormat = GL_RGBA8;
		else if( format == RenderBufferFormats::RGBA16F ) glFormat = GL_RGBA16F_ARB;
		else if( format == RenderBufferFormats::RGBA32F ) glFormat = GL_RGBA32F_ARB;

		for( uint32 j = 0; j < numColBufs; ++j )
		{
			if( samples > 0 )
			{
				// Create a multisampled renderbuffer
				glGenRenderbuffersEXT( 1, &rb.colBufs[j] );
				glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rb.colBufs[j] );
				glRenderbufferStorageMultisampleEXT( GL_RENDERBUFFER_EXT, rb.samples,
													 glFormat, rb.width, rb.height );

				// Attach the renderbuffer
				glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + j,
											  GL_RENDERBUFFER_EXT, rb.colBufs[j] );
			}
			else
			{
				// Create a color texture
				glGenTextures( 1, &rb.colBufs[j] );
				glBindTexture( GL_TEXTURE_2D, rb.colBufs[j] );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bilinear ? GL_LINEAR : GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bilinear ? GL_LINEAR : GL_NEAREST );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D, 0, glFormat, rb.width, rb.height, 0, GL_RGBA, GL_FLOAT, 0x0 );

				// Attach the texture
				glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + j, GL_TEXTURE_2D, rb.colBufs[j], 0 );
			}
		}

		uint32 buffers[] = {	GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT,
								GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
		glDrawBuffers( numColBufs, buffers );
	}

	// Attach depth buffer
	if( depth )
	{
		if( samples > 0 )
		{
			// Create a multisampled renderbuffer
			glGenRenderbuffersEXT( 1, &rb.depthBuf );
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rb.depthBuf );
			glRenderbufferStorageMultisampleEXT( GL_RENDERBUFFER_EXT, rb.samples,
												 GL_DEPTH_COMPONENT, rb.width, rb.height );

			// Attach the renderbuffer
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
										  GL_RENDERBUFFER_EXT, rb.depthBuf );
		}
		else
		{
			// Create a depth texture
			glGenTextures( 1, &rb.depthBuf );
			glBindTexture( GL_TEXTURE_2D, rb.depthBuf );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );
			glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE );
			glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, rb.width, rb.height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0x0 );

			// Attach the texture
			glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, rb.depthBuf, 0 );
		}
	}

	// Check if successful
	uint32 status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT )
	{	
		return RenderBuffer();
	}

	return rb;
}


void RendererBase::destroyRenderBuffer( RenderBuffer &rb )
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	
	if( rb.depthBuf != 0 )
	{
		if( rb.samples > 0 )
		{
			glDeleteRenderbuffersEXT( 1, &rb.depthBuf );
		}
		else
		{
			glDeleteTextures( 1, &rb.depthBuf );
		}
		rb.depthBuf = 0;
	}
		
	for( uint32 i = 0; i < 4; ++i )
	{
		if( rb.samples > 0 )
		{
			if( rb.colBufs[i] != 0 ) glDeleteRenderbuffersEXT( 1, &rb.colBufs[i] );
		}
		else
		{
			if( rb.colBufs[i] != 0 ) glDeleteTextures( 1, &rb.colBufs[i] );
		}
		rb.colBufs[i] = 0;
	}

	if( rb.fbo != 0 )
	{
		glDeleteFramebuffersEXT( 1, &rb.fbo );
		rb.fbo = 0;
	}
}


void RendererBase::setRenderBuffer( RenderBuffer *rb )
{
	_curRendBuf = rb;
	
	if( rb == 0x0 || rb->fbo == 0 )
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		glDrawBuffer( _outputBufferIndex == 1 ? GL_BACK_RIGHT : GL_BACK_LEFT );
		glViewport( _vpX, _vpY, _vpWidth, _vpHeight );
		_fbWidth = _vpWidth + _vpX;
		_fbHeight = _vpHeight + _vpY;
	}
	else
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, rb->fbo );
		glViewport( 0, 0, rb->width, rb->height );
		_fbWidth = rb->width;
		_fbHeight = rb->height;
	}
}


bool RendererBase::getBufferData( RenderBuffer *rb, int bufIndex, int *width, int *height,
								  int *compCount, float *dataBuffer, int bufferSize )
{
	int x, y, w, h;
	
	if( rb == 0x0 )
	{
		if( bufIndex != 32 && bufIndex != 0 ) return false;
		if( width != 0x0 ) *width = _vpWidth;
		if( height != 0x0 ) *height = _vpHeight;
		
		x = _vpX; y = _vpY; w = _vpWidth; h = _vpHeight;

		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		if( bufIndex != 32 ) glReadBuffer( GL_BACK_LEFT );
	}
	else
	{
		if( bufIndex == 32 && rb->depthBuf == 0 ) return false;
		if( bufIndex != 32 )
		{
			if( (unsigned)bufIndex > 3 || rb->colBufs[bufIndex] == 0 ) return false;
		}
		if( width != 0x0 ) *width = rb->width;
		if( height != 0x0 ) *height = rb->height;

		x = 0; y = 0; w = rb->width; h = rb->height;
		
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, rb->fbo );
		if( bufIndex != 32 ) glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + bufIndex );
	}
	
	int comps = (bufIndex == 32 ? 1 : 4);
	if( compCount != 0x0 ) *compCount = comps;
	
	if( dataBuffer == 0x0 ) return true;
	if( bufferSize < w * h * comps * (signed)sizeof( float ) ) return false;
	
	glFinish();
	int format = GL_RGBA;
	if( bufIndex == 32 ) format = GL_DEPTH_COMPONENT;
	glReadPixels( x, y, w, h, format, GL_FLOAT, dataBuffer );
	
	return true;
}


void RendererBase::blitRenderBuffer( RenderBuffer &sourceBuf, RenderBuffer &destBuf )
{
	glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, sourceBuf.fbo );
	glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, destBuf.fbo );
	glViewport( 0, 0, destBuf.width, destBuf.height );

	// TODO:	- Determine if combining depth and color0 would yield any speedup
	//			  (from the fbo_blit spec, it seems unlikely)
	//			- Check that this routine works with multiple color attachments

	if( sourceBuf.depthBuf != 0 && destBuf.depthBuf != 0 )
	{
		// Blit depth buffer
		glReadBuffer( GL_NONE );
		glDrawBuffer( GL_NONE );
		glBlitFramebufferEXT( 0, 0, sourceBuf.width, sourceBuf.height,
							  0, 0, destBuf.width, destBuf.height,
							  GL_DEPTH_BUFFER_BIT, GL_NEAREST );
	}

	for( uint32 i = 0; i < RenderBuffer::MaxColorAttachmentCount; ++i )
	{
		if( sourceBuf.colBufs[i] != 0 && destBuf.colBufs[i] != 0 )
		{
			glReadBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
			glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT + i );
			glBlitFramebufferEXT( 0, 0, sourceBuf.width, sourceBuf.height,
								  0, 0, destBuf.width, destBuf.height,
								  GL_COLOR_BUFFER_BIT, GL_NEAREST );
		}
	}

	// Reset framebuffer binding
    setRenderBuffer( _curRendBuf );
}


uint32 RendererBase::createOccQuery()
{
	uint32 object;
	glGenQueries( 1, &object );
	return object;
}


void RendererBase::destroyOccQuery( uint32 queryId )
{
	glDeleteQueries( 1, &queryId );
}


void RendererBase::beginOccQuery( uint32 queryId )
{
	glBeginQuery( GL_SAMPLES_PASSED, queryId );
}


void RendererBase::endOccQuery( uint32 /*queryId*/ )
{
	glEndQuery( GL_SAMPLES_PASSED );
}


uint32 RendererBase::getOccQueryResult( uint32 queryId )
{
	uint32 samples = 0;
	glGetQueryObjectuiv( queryId, GL_QUERY_RESULT, &samples );
	return samples;
}
