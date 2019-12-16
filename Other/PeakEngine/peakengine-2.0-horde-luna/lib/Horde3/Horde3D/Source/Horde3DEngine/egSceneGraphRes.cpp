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

#include "egSceneGraphRes.h"
#include "egModules.h"
#include "utXMLParser.h"
#include "utPlatform.h"

#include "utDebug.h"


SceneGraphResource::SceneGraphResource( const string &name, int flags ) :
	Resource( ResourceTypes::SceneGraph, name, flags )
{
	initDefault();
}


SceneGraphResource::~SceneGraphResource()
{
	release();
}


void SceneGraphResource::initDefault()
{
	// Create default root node
	_rootNode = new GroupNodeTpl( _name );
}


void SceneGraphResource::release()
{
	delete _rootNode; _rootNode = 0x0;
}


void SceneGraphResource::parseBaseAttributes( XMLNode &xmlNode, SceneNodeTpl &nodeTpl )
{
	nodeTpl.name = xmlNode.getAttribute( "name", "" );
	nodeTpl.trans.x = (float)atof( xmlNode.getAttribute( "tx", "0" ) );
	nodeTpl.trans.y = (float)atof( xmlNode.getAttribute( "ty", "0" ) );
	nodeTpl.trans.z = (float)atof( xmlNode.getAttribute( "tz", "0" ) );
	nodeTpl.rot.x = (float)atof( xmlNode.getAttribute( "rx", "0" ) );
	nodeTpl.rot.y = (float)atof( xmlNode.getAttribute( "ry", "0" ) );
	nodeTpl.rot.z = (float)atof( xmlNode.getAttribute( "rz", "0" ) );
	nodeTpl.scale.x = (float)atof( xmlNode.getAttribute( "sx", "1" ) );
	nodeTpl.scale.y = (float)atof( xmlNode.getAttribute( "sy", "1" ) );
	nodeTpl.scale.z = (float)atof( xmlNode.getAttribute( "sz", "1" ) );

	XMLNode node1 = xmlNode.getChildNode( "Attachment" );
	if( !node1.isEmpty() )
	{
		char *s = node1.createXMLString();
		nodeTpl.attachmentString = s;
		delete[] s;
	}
}


void SceneGraphResource::parseNode( XMLNode &xmlNode, SceneNodeTpl *parentTpl )
{
	SceneNodeTpl *nodeTpl = 0x0;

	if( xmlNode.getName() != 0x0 )	// Ignore clear tags like DOCTYPE
	{
		if( strcmp( xmlNode.getName(), "Reference" ) == 0 )
		{
			if( strcmp( xmlNode.getAttribute( "sceneGraph", "" ), "" ) != 0 )
			{
				Resource *res = Modules::resMan().resolveResHandle( Modules::resMan().addResource(
					ResourceTypes::SceneGraph, xmlNode.getAttribute( "sceneGraph" ), 0, false ) );
				if (res != 0x0 ) nodeTpl = new ReferenceNodeTpl( "", (SceneGraphResource *)res );
			}
		}
		else
		{
			NodeRegEntry *entry = Modules::sceneMan().findType( xmlNode.getName() );
			if( entry != 0x0 )
			{
				map< string, string > attribs;
				
				// Parse custom attributes
				for( int i = 0; i < xmlNode.nAttribute(); ++i )
				{
					if( strcmp( xmlNode.getAttribute( i ).lpszName, "name" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "tx" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "ty" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "tz" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "rx" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "ry" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "rz" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "sx" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "sy" ) != 0 &&
						strcmp( xmlNode.getAttribute( i ).lpszName, "sz" ) != 0 )
					{
						attribs[xmlNode.getAttribute( i ).lpszName] = xmlNode.getAttribute( i ).lpszValue;
					}
				}

				// Call function pointer
				nodeTpl = (*entry->parsingFunc)( attribs );
			}
		}
		
		if( nodeTpl != 0x0 )
		{
			// Parse base attributes
			parseBaseAttributes( xmlNode, *nodeTpl );
			
			// Add to parent
			if( parentTpl != 0x0 )
			{
				parentTpl->children.push_back( nodeTpl );
			}
			else
			{	
				delete _rootNode;	// Delete default root
				_rootNode = nodeTpl;
			}
		}
		else if( strcmp( xmlNode.getName(), "Attachment" ) != 0 )
		{
			Modules::log().writeWarning( "SceneGraph resource '%s': Unknown node type or missing attribute for '%s'",
										 _name.c_str(), xmlNode.getName() );
			return;
		}
	}
	
	// Parse children
	int nodeItr1 = 0;
	XMLNode xmlNode1 = xmlNode.getChildNode( nodeItr1 );
	while( !xmlNode1.isEmpty() )
	{	
		if( xmlNode1.getName() == 0x0 || strcmp( xmlNode1.getName(), "Attachment" ) != 0 )			
			parseNode( xmlNode1, nodeTpl );

		xmlNode1 = xmlNode.getChildNode( ++nodeItr1 );
	}
}


bool SceneGraphResource::load( const char *data, int size )
{
	if( !Resource::load( data, size ) ) return false;
	if( data[size - 1] != '\0' )
	{	
		Modules::log().writeError( "SceneGraph resource '%s': Data block not NULL-terminated", _name.c_str() );
		return false;
	}

	// Parse XML file
	XMLNode rootNode = XMLNode::parseString( data );

	// Parse scene nodes and load resources
	if( !rootNode.isEmpty() )
	{	
		parseNode( rootNode, 0x0 );
	}
	else
	{
		return false;
	}

	return true;
}
