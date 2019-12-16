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

#ifndef _egTextures_H_
#define _egTextures_H_

#include "egPrerequisites.h"
#include "egResource.h"

struct RenderBuffer;


class ImageLoader
{
private:

	static uint32 nextSmallerPowerOfTwo( uint32 n );

public:

	static uint32 loadImage( const char *data, uint32 size, bool makePOT,
							 void *&pixels, int &width, int &height, int &comps, bool &hdr );
	static string getErrorString();
};


struct TextureResParams
{
	enum List
	{
		PixelData = 700,
		Width,
		Height,
		Comps,
		HDR
	};
};

class Texture2DResource : public Resource
{
protected:
	
	int				_width, _height, _comps;
	bool			_hdr;
	uint32			_texObject;
	RenderBuffer	*_rendBuf;	// Used when texture is renderable

	bool raiseError( const string &msg );

public:
	
	static uint32 defTexObject;

	static void initializationFunc();
	static void releaseFunc();
	static Resource *factoryFunc( const string &name, int flags )
		{ return new Texture2DResource( name, flags ); }
	
	Texture2DResource( const string &name, int flags );
	Texture2DResource( const string &name, int flags,
					   uint32 width, uint32 height, bool renderable );
	~Texture2DResource();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );
	bool updateData( int param, const void *data, int size );
	float *downloadImageData();

	int getParami( int param );

	uint32 getWidth() const { return _width; }
	uint32 getHeight() const { return _height; } 
	uint32 getComps() const { return _comps; }
	bool isHDR() const { return _hdr; }
	uint32 getTexObject() { return _texObject; }
	RenderBuffer *getRenderBuffer()  { return _rendBuf; }

	friend class ResourceManager;
};

typedef SmartResPtr< Texture2DResource > PTexture2DResource;


class TextureCubeResource : public Resource
{
protected:
	
	int		_width, _height, _comps;
	bool	_hdr;
	uint32	_texObject;

	bool raiseError( const string &msg );

public:
	
	static uint32 defTexObject;

	static void initializationFunc();
	static void releaseFunc();
	static Resource *factoryFunc( const string &name, int flags )
		{ return new TextureCubeResource( name, flags ); }
	
	TextureCubeResource( const string &name, int flags );
	~TextureCubeResource();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );

	int getParami( int param );

	uint32 getTexObject() { return _texObject; }

	friend class ResourceManager;
};

typedef SmartResPtr< TextureCubeResource > PTextureCubeResource;

#endif // _egTextures_H_
