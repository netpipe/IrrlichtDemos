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

#include "egShader.h"
#include "egModules.h"
#include "utXMLParser.h"
#include "utPlatform.h"
#include <sstream>

#include "utDebug.h"


// *************************************************************************************************
// CodeResource
// *************************************************************************************************

CodeResource::CodeResource( const string &name, int flags ) :
	Resource( ResourceTypes::Code, name, flags )
{
	initDefault();
}


CodeResource::~CodeResource()
{
	release();
}


Resource *CodeResource::clone()
{
	CodeResource *res = new CodeResource( "", _flags );

	*res = *this;
	
	return res;
}


void CodeResource::initDefault()
{
	_code = "";
}


void CodeResource::release()
{
}


bool CodeResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;

	_code.reserve( size );
	_code = data;

	// Compile shaders that require this code block
	updateShaders();

	return true;
}


void CodeResource::updateShaders()
{
	for( uint32 i = 0; i < Modules::resMan().getResources().size(); ++i )
	{
		Resource *res = Modules::resMan().getResources()[i];

		if( res != 0x0 && res->getType() == ResourceTypes::Shader )
		{
			ShaderResource *shaderRes = (ShaderResource *)res;
			
			// Mark shaders using this code as uncompiled
			for( uint32 j = 0; j < shaderRes->getContexts().size(); ++j )
			{
				ShaderContext &sc = shaderRes->getContexts()[j];

				for( uint32 k = 0; k < sc.vertShaderFracts.size(); ++k )
				{
					if( sc.vertShaderFracts[k].refCodeRes == this ) sc.compiled = false;
				}
				for( uint32 k = 0; k < sc.fragShaderFracts.size(); ++k )
				{
					if( sc.fragShaderFracts[k].refCodeRes == this ) sc.compiled = false;
				}
			}
			
			shaderRes->compileShaders();
		}
	}
}


// *************************************************************************************************
// ShaderResource
// *************************************************************************************************

string ShaderResource::_vertPreamble = "";
string ShaderResource::_fragPreamble = "";


ShaderResource::ShaderResource( const string &name, int flags ) :
	Resource( ResourceTypes::Shader, name, flags )
{
	initDefault();
}


ShaderResource::~ShaderResource()
{
	release();
}


void ShaderResource::initDefault()
{
}


void ShaderResource::release()
{
	for( uint32 i = 0; i < _contexts.size(); ++i )
	{
		Modules::renderer().unloadShader( _contexts[i].shaderObject );
	}

	_contexts.clear();
}


bool ShaderResource::raiseError( const string &msg, int line )
{
	// Reset
	release();
	initDefault();

	if( line < 0 )
		Modules::log().writeError( "Shader resource '%s': %s", _name.c_str(), msg.c_str() );
	else
		Modules::log().writeError( "Shader resource '%s' in line %i: %s", _name.c_str(), line, msg.c_str() );
	
	return false;
}


bool ShaderResource::parseCode( XMLNode &node, vector< ShaderCodeFract > &codeFracts )
{
	int nodeItr1 = 0;
	XMLNode node1 = node.getChildNode( nodeItr1 );
	while( !node1.isEmpty() && node1.getName() != 0x0 )
	{
		if( strcmp( node1.getName(), "DefCode" ) == 0 )
		{
			ShaderCodeFract codeFract;
			// Find CDATA
			for( int i = 0; i < node1.nClear(); ++i )
			{
				if( strcmp( node1.getClear( i ).lpszOpenTag, "<![CDATA[" ) == 0 )
				{
					codeFract.code = node1.getClear().lpszValue;
					break;
				}
			}
			codeFracts.push_back( codeFract );
		}
		else if( strcmp( node1.getName(), "InsCode" ) == 0 )
		{
			if( node1.getAttribute( "code" ) == 0x0 ) return false;
		
			ShaderCodeFract codeFract;
			
			uint32 id = Modules::resMan().addResource(
				ResourceTypes::Code, node1.getAttribute( "code" ), 0, false );

			Resource *res = Modules::resMan().resolveResHandle( id );
			if( res != 0x0 )
			{	
				codeFract.refCodeRes = (CodeResource *)res;
				codeFracts.push_back( codeFract );
			}
		}

		node1 = node.getChildNode( ++nodeItr1 );
	}

	return true;
}


bool ShaderResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	if( data[size - 1] != '\0' )
	{	
		return raiseError( "Data block not NULL-terminated" );
	}
	
	// Parse shader
	XMLResults res;
	XMLNode rootNode = XMLNode::parseString( data, "Shader", &res );
	if( res.error != eXMLErrorNone )
	{
		return raiseError( XMLNode::getError( res.error ), res.nLine );
	}
	
	int nodeItr1 = 0;
	XMLNode node1 = rootNode.getChildNode( "Context", nodeItr1 );
	while( !node1.isEmpty() )
	{
		if( node1.getAttribute( "id" ) == 0x0 ) return raiseError( "Missing Context attribute 'id'" );
		
		ShaderContext sc;

		sc.id = node1.getAttribute( "id" );
		
		// Config
		XMLNode node2 = node1.getChildNode( "RenderConfig" );
		if( !node2.isEmpty() )
		{
			if( _stricmp( node2.getAttribute( "writeDepth", "true" ), "false" ) == 0 ||
				_stricmp( node2.getAttribute( "writeDepth", "1" ), "0" ) == 0 )
				sc.writeDepth = false;
			else
				sc.writeDepth = true;

			if( _stricmp( node2.getAttribute( "blendMode", "REPLACE" ), "BLEND" ) == 0 )
				sc.blendMode = BlendModes::Blend;
			else if( _stricmp( node2.getAttribute( "blendMode", "REPLACE" ), "ADD" ) == 0 )
				sc.blendMode = BlendModes::Add;
			else if( _stricmp( node2.getAttribute( "blendMode", "REPLACE" ), "ADD_BLENDED" ) == 0 )
				sc.blendMode = BlendModes::AddBlended;
			else if( _stricmp( node2.getAttribute( "blendMode", "REPLACE" ), "MULT" ) == 0 )
				sc.blendMode = BlendModes::Mult;
			else
				sc.blendMode = BlendModes::Replace;
		}
		
		// Code 
		node2 = node1.getChildNode( "VertexShader" );
		if( node2.isEmpty() ) return raiseError( "Missing VertexShader node in Context '" + sc.id + "'" );
		if( !parseCode( node2, sc.vertShaderFracts ) ) return raiseError( "Error in VertexShader node of Context '" + sc.id + "'" );

		node2 = node1.getChildNode( "FragmentShader" );
		if( node2.isEmpty() ) return raiseError( "Missing VertexShader node in Context '" + sc.id + "'" );
		if( !parseCode( node2, sc.fragShaderFracts ) ) return raiseError( "Error in FragmentShader node of Context '" + sc.id + "'" );

		_contexts.push_back( sc );
		
		node1 = rootNode.getChildNode( "Context", ++nodeItr1 );
	}

	compileShaders();
	
	return true;
}


void ShaderResource::compileShaders()
{
	for( uint32 i = 0; i < _contexts.size(); ++i )
	{
		ShaderContext &sc = _contexts[i];

		if( !sc.compiled )
		{
			// Check if all referenced code blocks are available
			bool allAvailbale = true;
			for( uint32 j = 0; j < sc.vertShaderFracts.size(); ++j )
			{
				if( sc.vertShaderFracts[j].refCodeRes != 0x0 && !sc.vertShaderFracts[j].refCodeRes->isLoaded() )
				{
					allAvailbale = false;
					break;
				}
			}
			if( !allAvailbale ) continue;
			
			for( uint32 j = 0; j < sc.fragShaderFracts.size(); ++j )
			{
				if( sc.fragShaderFracts[j].refCodeRes != 0x0 && !sc.fragShaderFracts[j].refCodeRes->isLoaded() )
				{
					allAvailbale = false;
					break;
				}
			}
			if( !allAvailbale ) continue;
						
			// Assemble shader
			stringstream vertCode, fragCode;
			
			vertCode << "#line 0\n" << _vertPreamble << "\n";
			for( uint32 j = 0; j < sc.vertShaderFracts.size(); ++j )
			{
				vertCode << "#line " << (j + 1) * 1000 << "\n";
				
				if( !sc.vertShaderFracts[j].refCodeRes != 0x0 )
					vertCode << sc.vertShaderFracts[j].code;
				else
					vertCode << sc.vertShaderFracts[j].refCodeRes->getCode();

				vertCode << "\n";
			}
			
			fragCode << "#line 0\n" << _fragPreamble << "\n";
			for( uint32 j = 0; j < sc.fragShaderFracts.size(); ++j )
			{
				fragCode << "#line " << (j + 1) * 1000 << "\n";
				
				if( !sc.fragShaderFracts[j].refCodeRes != 0x0 )
					fragCode << sc.fragShaderFracts[j].code;
				else
					fragCode << sc.fragShaderFracts[j].refCodeRes->getCode();

				fragCode << "\n";
			}
			
			// Compile shader
			Modules::log().writeInfo( "Shader resource '%s': Compiling shader context '%s'", _name.c_str(), sc.id.c_str() );
			if( sc.shaderObject != 0 )
			{
				Modules::renderer().unloadShader( sc.shaderObject );
				sc.shaderObject = 0;
			}
			
			if( !Modules::renderer().uploadShader( vertCode.str().c_str(), fragCode.str().c_str(), sc ) )
				Modules::log().writeError( "Shader resource '%s': Failed to compile shader context '%s'", _name.c_str(), sc.id.c_str() );

			if( Modules::renderer().getShaderLog() != "" )
				Modules::log().writeInfo( "Shader resource '%s': ShaderLog: %s", _name.c_str(), Modules::renderer().getShaderLog().c_str() );

			sc.compiled = true;
		}
	}	
}
