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

#ifndef _egMaterial_H_
#define _egMaterial_H_

#include "egPrerequisites.h"
#include "egResource.h"
#include "egShader.h"


struct MaterialResParams
{
	enum List
	{
		Class = 400,
		Link,
		Shader,
		TexUnit_0,
		TexUnit_1,
		TexUnit_2,
		TexUnit_3,
		TexUnit_4,
		TexUnit_5,
		TexUnit_6,
		TexUnit_7,
		TexUnit_8,
		TexUnit_9,
		TexUnit_10,
		TexUnit_11
	};	
};


struct TexUnit
{
	uint32		unit;
	PResource	texRes;
	

	TexUnit(): unit( 0 ) {}
};


struct Uniform
{
	string			name;
	float			values[4];	


	Uniform()
	{
		values[0] = 0; values[1] = 0; values[2] = 0; values[3] = 0;
	}
};


class MaterialResource : public Resource
{
private:

	string							_class;
	PShaderResource					_shaderRes;
	vector< TexUnit >				_texUnits;
	vector< Uniform >				_uniforms;
	SmartResPtr< MaterialResource >	_matLink;	

	ResHandle getTexUnit( int unit );
	bool setTexUnit( int unit, ResHandle texRes );
	bool raiseError( const string &msg, int line = -1 );

public:

	static Resource *factoryFunc( const string &name, int flags )
		{ return new MaterialResource( name, flags ); }
	
	MaterialResource( const string &name, int flags );
	~MaterialResource();
	Resource *clone();
	
	void initDefault();
	void release();
	bool load( const char *data, int size );
	bool setUniform( const string &name, float a, float b, float c, float d );
	bool isOfClass( const string &theClass );

	int getParami( int param );
	bool setParami( int param, int value );
	const char *getParamstr( int param );
	bool setParamstr( int param, const char *value );

	friend class ResourceManager;
	friend class Renderer;
	friend class MeshNode;
};

typedef SmartResPtr< MaterialResource > PMaterialResource;

#endif // _egMaterial_H_
