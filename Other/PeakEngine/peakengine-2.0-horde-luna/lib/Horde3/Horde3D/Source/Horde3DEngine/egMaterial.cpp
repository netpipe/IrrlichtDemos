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

#include "egMaterial.h"
#include "egTextures.h"
#include "egCom.h"
#include "egModules.h"
#include "utPlatform.h"
#include "utXMLParser.h"

#include "utDebug.h"


MaterialResource::MaterialResource( const string &name, int flags ) :
	Resource( ResourceTypes::Material, name, flags )
{
	initDefault();	
}


MaterialResource::~MaterialResource()
{
	release();
}


Resource *MaterialResource::clone()
{
	MaterialResource *res = new MaterialResource( "", _flags );

	*res = *this;
	
	return res;
}


void MaterialResource::initDefault()
{
	_shaderRes = 0x0;
	_matLink = 0x0;
	_class = "";
}


void MaterialResource::release()
{
	_shaderRes = 0x0;
	_matLink = 0x0;
	for( uint32 i = 0; i < _texUnits.size(); ++i ) _texUnits[i].texRes = 0x0;

	_texUnits.clear();
	_uniforms.clear();
}


ResHandle MaterialResource::getTexUnit( int unit )
{
	for( size_t i = 0, size = _texUnits.size(); i < size; ++i )
	{
		if( _texUnits[i].unit == unit )
			return _texUnits[i].texRes != 0x0 ? _texUnits[i].texRes->getHandle() : 0;
	}
	
	return 0;
}


bool MaterialResource::setTexUnit( int unit, ResHandle texRes )
{
	Resource *res = 0x0;
	if( texRes != 0 )
	{
		res = Modules::resMan().resolveResHandle( texRes );
		if( res == 0x0 || 
			(res->getType() != ResourceTypes::Texture2D &&
			 res->getType() != ResourceTypes::TextureCube) )
		{
			return false;
		}
	}

	// Try to find existing texture unit
	for( size_t i = 0, size = _texUnits.size(); i < size; ++i )
	{
		if( _texUnits[i].unit == unit )
		{
			if( res != 0x0 )
				_texUnits[i].texRes = res;
			else
				_texUnits.erase( _texUnits.begin() + i );
			return true;
		}
	}

	// Add new texture unit
	TexUnit texUnit;
	texUnit.unit = unit;
	texUnit.texRes = res;
	_texUnits.push_back( texUnit );

	return true;
}


bool MaterialResource::raiseError( const string &msg, int line )
{
	// Reset
	release();
	initDefault();

	if( line < 0 )
		Modules::log().writeError( "Material resource '%s': %s", _name.c_str(), msg.c_str() );
	else
		Modules::log().writeError( "Material resource '%s' in line %i: %s", _name.c_str(), line, msg.c_str() );
	
	return false;
}


bool MaterialResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	if( data[size - 1] != '\0' )
	{	
		return raiseError( "Data block not NULL-terminated" );
	}

	// Parse material
	XMLResults res;
	XMLNode rootNode = XMLNode::parseString( data, "Material", &res );
	if( res.error != eXMLErrorNone )
	{
		return raiseError( XMLNode::getError( res.error ), res.nLine );
	}

	// Class
    _class = rootNode.getAttribute( "class", "" );

	// Link
	if( strcmp( rootNode.getAttribute( "link", "" ), "" ) != 0 )
	{
		uint32 mat = Modules::resMan().addResource(
			ResourceTypes::Material, rootNode.getAttribute( "link" ), 0, false );
		_matLink = (MaterialResource *)Modules::resMan().resolveResHandle( mat );
	}
    
    // Shader
	XMLNode node1 = rootNode.getChildNode( "Shader" );
	if( !node1.isEmpty() )
	{
		if( node1.getAttribute( "source" ) == 0x0 ) return raiseError( "Missing Shader attribute 'source'" );
			
		uint32 shader = Modules::resMan().addResource(
				ResourceTypes::Shader, node1.getAttribute( "source" ), 0, false );
		_shaderRes = (ShaderResource *)Modules::resMan().resolveResHandle( shader );
	}

	// Texture units
	int nodeItr1 = 0;
	node1 = rootNode.getChildNode( "TexUnit", nodeItr1 );
	while( !node1.isEmpty() )
	{
		if( node1.getAttribute( "unit" ) == 0x0 ) return raiseError( "Missing TexUnit attribute 'unit'" );
		if( node1.getAttribute( "map" ) == 0x0 ) return raiseError( "Missing TexUnit attribute 'map'" );
		
		TexUnit texUnit;
		texUnit.unit = atoi( node1.getAttribute( "unit" ) );

		uint32 texMap;
		uint32 flags = 0;

		if( _stricmp( node1.getAttribute( "allowPOTConversion", "true" ), "false" ) == 0 ||
			_stricmp( node1.getAttribute( "allowPOTConversion", "1" ), "0" ) == 0 )
			flags |= ResourceFlags::NoTexPOTConversion;
		
		if( _stricmp( node1.getAttribute( "allowCompression", "true" ), "false" ) == 0 ||
			_stricmp( node1.getAttribute( "allowCompression", "1" ), "0" ) == 0 )
			flags |= ResourceFlags::NoTexCompression;

		if( _stricmp( node1.getAttribute( "mipmaps", "true" ), "false" ) == 0 ||
			_stricmp( node1.getAttribute( "mipmaps", "1" ), "0" ) == 0 )
			flags |= ResourceFlags::NoTexMipmaps;
		
		if( _stricmp( node1.getAttribute( "bilinear", "true" ), "false" ) == 0 ||
			_stricmp( node1.getAttribute( "bilinear", "1" ), "0" ) == 0 )
			flags |= ResourceFlags::NoTexFiltering;

		if( _stricmp( node1.getAttribute( "repeatMode", "true" ), "false" ) == 0 ||
			_stricmp( node1.getAttribute( "repeatMode", "1" ), "0" ) == 0 )
			flags |= ResourceFlags::NoTexRepeat;
	
		if( _stricmp( node1.getAttribute( "type", "2D" ), "CUBE" ) == 0 )
		{
			texMap = Modules::resMan().addResource( ResourceTypes::TextureCube, 
						node1.getAttribute( "map" ), flags, false );
		}
		else
		{
			texMap = Modules::resMan().addResource( ResourceTypes::Texture2D,
						node1.getAttribute( "map" ), flags, false );
		}

		texUnit.texRes = Modules::resMan().resolveResHandle( texMap );
		
		_texUnits.push_back( texUnit );
		
		node1 = rootNode.getChildNode( "TexUnit", ++nodeItr1 );
	}
		
	// Vector uniforms
	nodeItr1 = 0;
	node1 = rootNode.getChildNode( "Uniform", nodeItr1 );
	while( !node1.isEmpty() )
	{
		if( node1.getAttribute( "name" ) == 0x0 ) return raiseError( "Missing Uniform attribute 'name'" );

		Uniform uniform;
		uniform.name = node1.getAttribute( "name" );

		uniform.values[0] = (float)atof( node1.getAttribute( "a", "0" ) );
		uniform.values[1] = (float)atof( node1.getAttribute( "b", "0" ) );
		uniform.values[2] = (float)atof( node1.getAttribute( "c", "0" ) );
		uniform.values[3] = (float)atof( node1.getAttribute( "d", "0" ) );

		_uniforms.push_back( uniform );

		node1 = rootNode.getChildNode( "Uniform", ++nodeItr1 );
	}
	
	return true;
}


bool MaterialResource::setUniform( const string &name, float a, float b, float c, float d )
{
	for( uint32 i = 0; i < _uniforms.size(); ++i )
	{
		if( _uniforms[i].name == name )
		{
			_uniforms[i].values[0] = a;
			_uniforms[i].values[1] = b;
			_uniforms[i].values[2] = c;
			_uniforms[i].values[3] = d;
			return true;
		}
	}

	return false;
}


bool MaterialResource::isOfClass( const string &theClass )
{
	static string theClass2;
	
	if( theClass != "" )
	{
		if( theClass[0]	!= '~' )
		{
			if( _class.find( theClass, 0 ) != 0 ) return false;
			if( _class.length() > theClass.length() && _class[theClass.length()] != '.' ) return false;
		}
		else	// Not operator
		{
			theClass2 = theClass.substr( 1, theClass.length() - 1);
			
			if( _class.find( theClass2, 0 ) == 0 )
			{
				if( _class.length() == theClass2.length() )
				{
					return false;
				}
				else
				{
					if( _class[theClass2.length()] == '.' ) return false;
				}
			}
		}
	}
	else
	{
		// Special name which is hidden when drawing objects of "all classes"
		if( _class == "_DEBUG_" ) return false;
	}

	return true;
}


int MaterialResource::getParami( int param )
{
	switch( param )
	{
	case MaterialResParams::Link:
		return _matLink != 0x0 ? _matLink->getHandle() : 0;		
	case MaterialResParams::Shader:
		return _shaderRes != 0x0 ? _shaderRes->getHandle() : 0;
	case MaterialResParams::TexUnit_0:
		return getTexUnit( 0 );
	case MaterialResParams::TexUnit_1:
		return getTexUnit( 1 );
	case MaterialResParams::TexUnit_2:
		return getTexUnit( 2 );
	case MaterialResParams::TexUnit_3:
		return getTexUnit( 3 );
	case MaterialResParams::TexUnit_4:
		return getTexUnit( 4 );
	case MaterialResParams::TexUnit_5:
		return getTexUnit( 5 );
	case MaterialResParams::TexUnit_6:
		return getTexUnit( 6 );
	case MaterialResParams::TexUnit_7:
		return getTexUnit( 7 );
	case MaterialResParams::TexUnit_8:
		return getTexUnit( 8 );
	case MaterialResParams::TexUnit_9:
		return getTexUnit( 9 );
	case MaterialResParams::TexUnit_10:
		return getTexUnit( 10 );
	case MaterialResParams::TexUnit_11:
		return getTexUnit( 11 );
	default:
		return Resource::getParami( param );
	}
}


bool MaterialResource::setParami( int param, int value )
{
	switch( param )
	{
	case MaterialResParams::Link:
		{
			Resource *res = Modules::resMan().resolveResHandle( value );
			if( value != 0 && res == 0x0 ) return false;
			if( res != 0x0 && res->getType() != ResourceTypes::Material ) return false;

			_matLink = (MaterialResource *)res;
			return true;
		}
	case MaterialResParams::Shader:
		{
			Resource *res = Modules::resMan().resolveResHandle( value );
			if( value != 0 && res == 0x0 ) return false;
			if( res != 0x0 && res->getType() != ResourceTypes::Shader ) return false;

			_shaderRes = (ShaderResource *)res;
			return true;
		}
	case MaterialResParams::TexUnit_0:
		return setTexUnit( 0, value );
	case MaterialResParams::TexUnit_1:
		return setTexUnit( 1, value );
	case MaterialResParams::TexUnit_2:
		return setTexUnit( 2, value );
	case MaterialResParams::TexUnit_3:
		return setTexUnit( 3, value );
	case MaterialResParams::TexUnit_4:
		return setTexUnit( 4, value );
	case MaterialResParams::TexUnit_5:
		return setTexUnit( 5, value );
	case MaterialResParams::TexUnit_6:
		return setTexUnit( 6, value );
	case MaterialResParams::TexUnit_7:
		return setTexUnit( 7, value );
	case MaterialResParams::TexUnit_8:
		return setTexUnit( 8, value );
	case MaterialResParams::TexUnit_9:
		return setTexUnit( 9, value );
	case MaterialResParams::TexUnit_10:
		return setTexUnit( 10, value );
	case MaterialResParams::TexUnit_11:
		return setTexUnit( 11, value );
	default:
		return Resource::setParami( param, value );
	}
}


const char *MaterialResource::getParamstr( int param )
{
	switch( param )
	{
	case MaterialResParams::Class:
		return _class.c_str();
	default:
		return Resource::getParamstr( param );
	}
}


bool MaterialResource::setParamstr( int param, const char *value )
{	
	switch( param )
	{
	case MaterialResParams::Class:
		_class = value;
		return true;
	default:
		return Resource::setParamstr( param, value );
	}
}
