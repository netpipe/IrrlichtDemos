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

#include "egTextures.h"
#include "egModules.h"
#include "utImage.h"

#include "utDebug.h"


// *************************************************************************************************
// Class ImageLoader
// *************************************************************************************************

uint32 ImageLoader::nextSmallerPowerOfTwo( uint32 n )
{
	while( (n & (n - 1)) != 0 ) --n;

	return n;
}


uint32 ImageLoader::loadImage( const char *data, uint32 size, bool makePOT,
							   void *&pixels, int &width, int &height, int &comps, bool &hdr )
{
	if( stbi_is_hdr_from_memory( (unsigned char *)data, size ) > 0 ) hdr = true;
	else hdr = false;
	
	void *pixData = 0x0;
	if( hdr )
		pixData = stbi_loadf_from_memory( (unsigned char *)data, size, &width, &height, &comps, 4 );
	else
		pixData = stbi_load_from_memory( (unsigned char *)data, size, &width, &height, &comps, 4 );

	if( pixData == 0x0 ) return 0;

	bool scaled = false;
	comps = 4;

	if( makePOT )
	{
		// Convert to a POT image
		int newWidth = nextSmallerPowerOfTwo( width );
		int newHeight = nextSmallerPowerOfTwo( height );

		if( newWidth != width || newHeight != height )
		{
			scaled = true;
			
			if( hdr ) pixels = new float[newWidth * newHeight * 4];
			else pixels = new unsigned char[newWidth * newHeight * 4];

			for( int y = 0; y < newHeight; ++y )
			{
				int yy = (height - 1) - (int)(y * ((float)height / newHeight) + 0.5f);
				
				for( int x = 0; x < newWidth; ++x )
				{
					int xx = (int)(x * ((float)width / newWidth) + 0.5f);
					
					if( hdr )
						memcpy( (float *)pixels + (y * newWidth + x) * 4,
								(float *)pixData + (yy * width + xx) * 4, 4 * sizeof( float ) );
					else
						memcpy( (unsigned char *)pixels + (y * newWidth + x) * 4,
								(unsigned char *)pixData + (yy * width + xx) * 4, 4 * sizeof( char ) );
				}
			}

			width = newWidth;
			height = newHeight;
		}
	}

	if( !scaled )
	{
		// Flip image vertically
		if( hdr )
		{
			pixels = new float[width * height * 4];
			for( int i = 0; i < height; ++i )
			{
				memcpy( (float *)pixels + i * width * 4,
						(float *)pixData + (height - 1 - i) * width * 4,
						width * 4 * sizeof( float ) );
			}
		}
		else
		{
			pixels = new unsigned char[width * height * 4];
			for( int i = 0; i < height; ++i )
			{
				memcpy( (unsigned char *)pixels + i * width * 4,
						(unsigned char *)pixData + (height - 1 - i) * width * 4,
						width * 4 * sizeof( char ) );
			}
		}
	}

	stbi_image_free( pixData );

	return scaled ? 2 : 1;
}


string ImageLoader::getErrorString()
{
	return stbi_failure_reason();
}


// *************************************************************************************************
// Class Texture2DResource
// *************************************************************************************************

uint32 Texture2DResource::defTexObject = 0;


void Texture2DResource::initializationFunc()
{
	unsigned char defaultTexture[] = 
		{	255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128	};

	// Upload default texture
	defTexObject = Modules::renderer().uploadTexture2D(
		defaultTexture, 4, 4, 24, false, false, true, true, true );
}


void Texture2DResource::releaseFunc()
{
	Modules::renderer().unloadTexture( defTexObject, false );
}


Texture2DResource::Texture2DResource( const string &name, int flags ) :
	Resource( ResourceTypes::Texture2D, name, flags )
{
	initDefault();
}


Texture2DResource::Texture2DResource( const string &name, int flags,
									  uint32 width, uint32 height, bool renderable ) :
	Resource( ResourceTypes::Texture2D, name, flags ), _rendBuf(0x0),
	_width(width), _height(height), _comps(4), _hdr(false)
{	
	if( !renderable )
	{
		_texObject = Modules::renderer().uploadTexture2D( 0x0, _width, _height, _comps, _hdr,
			!(_flags & ResourceFlags::NoTexCompression), !(_flags & ResourceFlags::NoTexMipmaps),
			!(_flags & ResourceFlags::NoTexFiltering), !(_flags & ResourceFlags::NoTexRepeat) );
		
		if( _texObject == 0 ) initDefault();
	}
	else
	{
		_loaded = true;
		_rendBuf = new RenderBuffer();
		*_rendBuf = Modules::renderer().createRenderBuffer(
			width, height, RenderBufferFormats::RGBA8, false, 1, !(_flags & ResourceFlags::NoTexFiltering), 0 ); 
		_texObject = _rendBuf->colBufs[0];
	}
}


Texture2DResource::~Texture2DResource()
{
	release();
}


void Texture2DResource::initDefault()
{
	_width = 0; _height = 0; _comps = 0; _hdr = false;
	_texObject = defTexObject; _rendBuf = 0x0;
}


void Texture2DResource::release()
{
	if( _rendBuf != 0x0 )
	{
		// In this case _texObject is just points to the render buffer
		Modules::renderer().destroyRenderBuffer( *_rendBuf );
		delete _rendBuf; _rendBuf = 0x0;
	}
	else if( _texObject != 0 && _texObject != defTexObject )
	{
		Modules::renderer().unloadTexture( _texObject, false );
	}

	_texObject = 0;
}


bool Texture2DResource::raiseError( const string &msg )
{
	// Reset
	release();
	initDefault();

	Modules::log().writeError( "Texture2D resource '%s': %s", _name.c_str(), msg.c_str() );
	
	return false;
}


bool Texture2DResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	
	if( !Modules::config().loadTextures )
		return true;

	bool makePOT = !Modules::renderer().supportsNPOTTextures();
	if( _flags & ResourceFlags::NoTexPOTConversion ) makePOT = false;
	
	void *pixels;
	uint32 res = ImageLoader::loadImage( data, size, makePOT, pixels, _width, _height, _comps, _hdr );
	if( res == 0 )
		return raiseError( "Invalid image format (" + ImageLoader::getErrorString() + ")" );
	else if( res == 2 )
		Modules::log().writeWarning( "Texture2D resource '%s': Texture size was changed to match POT", _name.c_str() );

	// Check color depth
	if( _comps != 3 && _comps != 4 )
		return raiseError( "Invalid color depth" );
	
	// Upload texture
	_texObject = Modules::renderer().uploadTexture2D( pixels, _width, _height, _comps, _hdr,
		!(_flags & ResourceFlags::NoTexCompression), !(_flags & ResourceFlags::NoTexMipmaps),
		!(_flags & ResourceFlags::NoTexFiltering), !(_flags & ResourceFlags::NoTexRepeat) );
	if( _texObject == 0 )
		return raiseError( "Failed to upload texture map" );

	operator delete[]( pixels );	// Delete without calling destructor

	return true;
}


bool Texture2DResource::updateData( int param, const void *data, int size )
{
	if( Resource::updateData( param, data, size ) ) return true;
	
	if( param == TextureResParams::PixelData )
	{
		if( _texObject == 0 ) return false;
		if( size != _width * _height * _comps ) return false;

		Modules::renderer().updateTexture2D( (unsigned char *)data, _width, _height, _comps,
			!(_flags & ResourceFlags::NoTexFiltering), _texObject );

		return true;
	}

	return false;
}


float *Texture2DResource::downloadImageData()
{
	int width, height;
	return Modules::renderer().downloadTexture2DData( _texObject, &width, &height );
}


int Texture2DResource::getParami( int param )
{
	switch( param )
	{
	case TextureResParams::Width:
		return _width;
	case TextureResParams::Height:
		return _height;
	case TextureResParams::Comps:
		return _comps;
	case TextureResParams::HDR:
		return _hdr ? 1 : 0;
	default:
		return Resource::getParami( param );
	}
}


// *************************************************************************************************
// Class TextureCubeResource
// *************************************************************************************************

uint32 TextureCubeResource::defTexObject = 0;


void TextureCubeResource::initializationFunc()
{
	unsigned char defaultTexture[] = 
		{	255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128,
			255,192,128, 255,192,128, 255,192,128, 255,192,128	};

	// Upload default texture
	defTexObject = Modules::renderer().uploadTextureCube(
		defaultTexture, 4, 4, 24, false, 0, false, true, true );
	for( uint32 j = 1; j < 6; j++ ) 
	{
		Modules::renderer().uploadTextureCube(
			defaultTexture, 4, 4, 24, false, j, false, true, true, defTexObject );
	}
}


void TextureCubeResource::releaseFunc()
{
	Modules::renderer().unloadTexture( defTexObject, true );
}


TextureCubeResource::TextureCubeResource( const string &name, int flags ) :
	Resource( ResourceTypes::TextureCube, name, flags )
{
	initDefault();
}


TextureCubeResource::~TextureCubeResource()
{
	release();
}


void TextureCubeResource::initDefault()
{
	_width = 0; _height = 0; _comps = 0; _hdr = false;
	_texObject = defTexObject;
}


void TextureCubeResource::release()
{
	if( _texObject != 0 && _texObject != defTexObject )
		Modules::renderer().unloadTexture( _texObject, true );

	_texObject = 0;
}


bool TextureCubeResource::raiseError( const string &msg )
{
	// Reset
	release();
	initDefault();

	Modules::log().writeError( "TextureCube resource '%s': %s", _name.c_str(), msg.c_str() );
	
	return false;
}


bool TextureCubeResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	
	if( !Modules::config().loadTextures )
		return true;
	
	void *pixels;
	if( !ImageLoader::loadImage( data, size, false, pixels, _width, _height, _comps, _hdr ) )
		return raiseError( "Invalid image format (" + ImageLoader::getErrorString() + ")" );

	// Check color depth
	if( _comps != 3 && _comps != 4 )
		return raiseError( "Invalid color depth" );

	// Check image size
	if( (_height != 16		&& _height != 32	&& _height != 64	&& _height != 128	&&
		 _height != 256		&& _height != 512	&& _height != 1024	&& _height != 2048	&&
		 _height != 4096	&& _height != 8192)	|| (_width * 4 != _height * 3) )
	{
		return raiseError( "Invalid cubemap texture size" );
	}

	uint32	faceWidth = _width / 3, faceHeight = _height / 4, faceComps = _comps;
	void	*cubeFace = 0x0;

	if( _hdr ) cubeFace = new float[faceWidth * faceHeight * faceComps];
	else cubeFace = new unsigned char[faceWidth * faceHeight * faceComps];
	
#define GETPIX( x, y ) (unsigned char *)pixels + ((y) * _width + x) * _comps
#define GETPIXF( x, y ) (float *)pixels + ((y) * _width + x) * _comps
	
	// Extract and upload cube map faces
	for( uint32 i = 0; i < 6; ++i ) 
	{
		void *cp = cubeFace;
		
		for( uint32 y = 0; y < faceHeight; ++y )
		{
			for( uint32 x = 0; x < faceWidth; ++x )
			{
				void *src = pixels;

				switch( i )
				{
				case 0:
					if( _hdr ) src = GETPIXF( 2 * faceWidth + x, _height - (faceHeight + y + 1) );
					else src = GETPIX( 2 * faceWidth + x, _height - (faceHeight + y + 1) );
					break;
				case 1:
					if( _hdr ) src = GETPIXF( x, _height - (faceHeight + y + 1) );
					else src = GETPIX( x, _height - (faceHeight + y + 1) );
					break;
				case 2:
					if( _hdr ) src = GETPIXF( _width - (2 * faceWidth - x), _height - (y + 1) );
					else src = GETPIX( _width - (2 * faceWidth - x), _height - (y + 1) );
					break;
				case 3:
					if( _hdr ) src = GETPIXF( _width - (2 * faceWidth - x), _height - (2 * faceHeight + y + 1) );
					else src = GETPIX( _width - (2 * faceWidth - x), _height - (2 * faceHeight + y + 1) );
					break;
				case 4:
					if( _hdr ) src = GETPIXF( faceWidth + x, _height - (faceHeight + y + 1) );
					else src = GETPIX( faceWidth + x, _height - (faceHeight + y + 1) );
					break;
				case 5:
					if( _hdr ) src = GETPIXF( 2 * faceWidth - (x + 1), y );
					else src = GETPIX( 2 * faceWidth - (x + 1), y );
					break;
				}
				
				if( _hdr )
				{
					memcpy( cp, src, faceComps * sizeof( float ) );
					src = (float *)src + faceComps;
					cp = (float *)cp + faceComps;
				}
				else
				{
					memcpy( cp, src, faceComps * sizeof( char ) );
					src = (unsigned char *)src + faceComps;
					cp = (unsigned char *)cp + faceComps;
				}
			}
		}

		if( i == 0 )
		{
			_texObject = Modules::renderer().uploadTextureCube( cubeFace, faceWidth, faceHeight, _comps,
				_hdr, i, !(_flags & ResourceFlags::NoTexCompression), !(_flags & ResourceFlags::NoTexMipmaps),
				!(_flags & ResourceFlags::NoTexFiltering) );
		}
		else
		{
			Modules::renderer().uploadTextureCube( cubeFace, faceWidth, faceHeight, _comps, _hdr, i,
				!(_flags & ResourceFlags::NoTexCompression), !(_flags & ResourceFlags::NoTexMipmaps),
				!(_flags & ResourceFlags::NoTexFiltering), _texObject );
		}
	}
	
	operator delete[]( cubeFace );	// Delete without calling destructor
	operator delete[]( pixels );

	return true;
}


int TextureCubeResource::getParami( int param )
{
	switch( param )
	{
	case TextureResParams::Width:
		return _width;
	case TextureResParams::Height:
		return _height;
	case TextureResParams::Comps:
		return _comps;
	case TextureResParams::HDR:
		return _hdr ? 1 : 0;
	default:
		return Resource::getParami( param );
	}
}
