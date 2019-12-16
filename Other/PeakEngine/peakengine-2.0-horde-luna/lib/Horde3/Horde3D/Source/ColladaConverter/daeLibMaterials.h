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

#ifndef _daeLibMaterials_H_
#define _daeLibMaterials_H_

#include "utXMLParser.h"
#include "daeLibEffects.h"
#include <string>
#include <vector>
using namespace std;



struct DaeMaterial
{
	string		id;
	string		name;
	string		effectId;
	DaeEffect	*effect;
	bool		used;
	

	DaeMaterial()
	{
		used = false;
		effect = 0x0;
	}
	

	bool parse( const XMLNode &matNode )
	{
		id = matNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		name = matNode.getAttribute( "name", "" );
		
		XMLNode node1 = matNode.getChildNode( "instance_effect" );
		if( !node1.isEmpty() )
		{
			effectId = node1.getAttribute( "url", "" );
			removeGate( effectId );
		}
		
		return true;
	}
};


struct DaeLibMaterials
{
	vector< DaeMaterial * >	materials;


	~DaeLibMaterials()
	{
		for( unsigned int i = 0; i < materials.size(); ++i ) delete materials[i];
	}


	DaeMaterial *findMaterial( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < materials.size(); ++i )
		{
			if( materials[i]->id == id ) return materials[i];
		}

		return 0x0;
	}


	bool parse( const XMLNode &rootNode )
	{
		XMLNode node1 = rootNode.getChildNode( "library_materials" );
		if( node1.isEmpty() ) return true;

		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "material", nodeItr2 );
		while( !node2.isEmpty() )
		{
			DaeMaterial *material = new DaeMaterial();
			if( material->parse( node2 ) ) materials.push_back( material );
			else delete material;

			node2 = node1.getChildNode( "material", ++nodeItr2 );
		}
		
		return true;
	}
};

#endif // _daeLibMaterials_H_
