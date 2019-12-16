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

#ifndef _daeLibGeometries_H_
#define _daeLibGeometries_H_

#include "utXMLParser.h"
#include "daeCommon.h"
#include "utils.h"
#include "utMath.h"
#include <string>
#include <vector>
using namespace std;


struct DaeVSource
{
	string		id;
	string		posSourceId;
	DaeSource	*posSource;


	bool parse( const XMLNode &verticesNode )
	{
		id = verticesNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		
		int nodeItr1 = 0;
		XMLNode node1 = verticesNode.getChildNode( "input", nodeItr1 );
		while( !node1.isEmpty() )
		{
			if( strcmp( node1.getAttribute( "semantic", "" ), "POSITION" ) == 0 )
			{
				posSourceId = node1.getAttribute( "source", "" );
				removeGate( posSourceId );
				return true;
			}
			
			node1 = verticesNode.getChildNode( "input", ++nodeItr1 );
		}
		
		return false;
	}
};


struct IndexEntry
{
	unsigned int				posIndex;		// Required
	int							normIndex;		// Optional, can be -1
	int							texIndex[4];	// Optional, can be -1


	IndexEntry()
	{
		normIndex = -1;
		texIndex[0] = -1; texIndex[1] = -1; texIndex[2] = -1; texIndex[3] = -1;
	}
};


struct DaeTriGroup
{
	string						vSourceId;
	DaeVSource					*vSource;
	string						normSourceId;
	DaeSource					*normSource;
	string						texSourceId[4];
	DaeSource					*texSource[4];
	string						matId;
	vector< IndexEntry >		indices;


	DaeTriGroup()
	{
		vSource = 0x0;
		normSource = 0x0;
		texSource[0] = 0x0; texSource[1] = 0x0; texSource[2] = 0x0; texSource[3] = 0x0;
	}


	bool parse( const XMLNode &trianglesNode )
	{
		int vertexOffset = 0, normOffset = -1, texCoordOffset[4] = { -1, -1, -1, -1 };
		unsigned int numInputs = 0;
		
		// Find the base mapping channel with the lowest set-id
		int baseChannel = 999999;
		int nodeItr1 = 0;
		XMLNode node1 = trianglesNode.getChildNode( "input", nodeItr1 );
		while( !node1.isEmpty() )
		{
			if( strcmp( node1.getAttribute( "semantic", "" ), "TEXCOORD" ) == 0 )
			{
				if( node1.getAttribute( "set" ) != 0x0 )
				{
					if( atoi( node1.getAttribute( "set" ) ) < baseChannel )
						baseChannel = atoi( node1.getAttribute( "set" ) );
				}
				else
				{
					baseChannel = 0;
					break;
				}
			}
			node1 = trianglesNode.getChildNode( "input", ++nodeItr1 );
		}
		
		// Parse data
		nodeItr1 = 0;
		node1 = trianglesNode.getChildNode( "input", nodeItr1 );
		while( !node1.isEmpty() )
		{
			++numInputs;
			
			if( strcmp( node1.getAttribute( "semantic", "" ), "VERTEX" ) == 0 )
			{
				vertexOffset = atoi( node1.getAttribute( "offset", "0" ) );
				vSourceId = node1.getAttribute( "source", "" );
				removeGate( vSourceId );
			}
			if( strcmp( node1.getAttribute( "semantic", "" ), "TEXCOORD" ) == 0 )
			{
				int set = -1;
				if( node1.getAttribute( "set" ) == 0x0 )
					set = 0;
				else
					set = atoi( node1.getAttribute( "set" ) );
				set -= baseChannel;
				
				if( set >= 0 && set < 4 )
				{
					texCoordOffset[set] = atoi( node1.getAttribute( "offset", "0" ) );
					texSourceId[set] = node1.getAttribute( "source", "" );
					removeGate( texSourceId[set] );
				}
			}
			if( strcmp( node1.getAttribute( "semantic", "" ), "NORMAL" ) == 0 )
			{
				normOffset = atoi( node1.getAttribute( "offset", "0" ) );
				normSourceId = node1.getAttribute( "source", "" );
				removeGate( normSourceId );
			}
			
			node1 = trianglesNode.getChildNode( "input", ++nodeItr1 );
		}
		
		if( vSourceId == "" ) return false;

		matId = trianglesNode.getAttribute( "material", "" );
		
		int count = atoi( trianglesNode.getAttribute( "count", "0" ) ) * 3;
		node1 = trianglesNode.getChildNode( "p" );
		
		unsigned int pos = 0;
		char *s = (char *)node1.getText();
		if( s == 0x0 ) return false;
		for( int i = 0; i < count; ++i )
		{
			IndexEntry indexEntry;
			
			for( int j = 0; j < (int)numInputs; ++j )
			{
				unsigned int ui;
				parseUInt( s, pos, ui );
				
				// No else-if since offset sharing is possible
				if( j == vertexOffset )
					indexEntry.posIndex = ui;
				if( j == normOffset )
					indexEntry.normIndex = (int)ui;
				if( j == texCoordOffset[0] )	
					indexEntry.texIndex[0] = (int)ui;
				if( j == texCoordOffset[1] )	
					indexEntry.texIndex[1] = (int)ui;
				if( j == texCoordOffset[2] )	
					indexEntry.texIndex[2] = (int)ui;
				if( j == texCoordOffset[3] )	
					indexEntry.texIndex[3] = (int)ui;
			}

			indices.push_back( indexEntry );
		}

		return true;
	}


	Vec3f getPos( int posIndex )
	{
		Vec3f v;
		
		// Assume the float buffer has at least 3 values per element
		DaeSource *source = vSource->posSource;
		v.x = source->floatArray[posIndex * source->elemsPerEntry + 0];
		v.y = source->floatArray[posIndex * source->elemsPerEntry + 1];
		v.z = source->floatArray[posIndex * source->elemsPerEntry + 2];
		
		return v;
	}

	Vec3f getNormal( int normIndex )
	{
		Vec3f v;
		
		DaeSource *source = normSource;
		if( source != 0x0 && normIndex >= 0 )
		{
			// Assume the float buffer has at least 3 values per element
			v.x = source->floatArray[normIndex * source->elemsPerEntry + 0];
			v.y = source->floatArray[normIndex * source->elemsPerEntry + 1];
			v.z = source->floatArray[normIndex * source->elemsPerEntry + 2];
		}
		
		return v;
	}

	Vec3f getTexCoords( int texIndex, unsigned int set )
	{
		Vec3f v;
		
		if( set < 4 )
		{
			DaeSource *source = texSource[set];
			if( source != 0x0 && texIndex >= 0 )
			{
				// Assume the float buffer has at least 2 values per element
				v.x = source->floatArray[texIndex * source->elemsPerEntry + 0];
				v.y = source->floatArray[texIndex * source->elemsPerEntry + 1];
				
				if( source->elemsPerEntry >= 3 )
					v.z = source->floatArray[texIndex * source->elemsPerEntry + 2];
			}
		}
		
		return v;
	}
};


struct DaeGeometry
{
	string					id;
	string					name;
	vector< DaeSource >		sources;
	vector< DaeVSource >	vsources;
	vector< DaeTriGroup >	triGroups;


	DaeSource *findSource( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < sources.size(); ++i )
		{
			if( sources[i].id == id ) return &sources[i];
		}

		return 0x0;
	}


	DaeVSource *findVSource( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < vsources.size(); ++i )
		{
			if( vsources[i].id == id ) return &vsources[i];
		}

		return 0x0;
	}


	bool parse( const XMLNode &geometryNode )
	{
		XMLNode node1 = geometryNode.getChildNode( "mesh" );
		if( node1.isEmpty() ) return false;

		id = geometryNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		
		name = geometryNode.getAttribute( "name", "" );

		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "source", nodeItr2 );
		while( !node2.isEmpty() )
		{
			sources.push_back( DaeSource() );
			if( !sources[sources.size() - 1].parse( node2 ) ) sources.pop_back();
			
			node2 = node1.getChildNode( "source", ++nodeItr2 );
		}

		nodeItr2 = 0;
		node2 = node1.getChildNode( "vertices", nodeItr2 );
		while( !node2.isEmpty() )
		{
			vsources.push_back( DaeVSource() );
			if( vsources[vsources.size() - 1].parse( node2 ) )
			{
				vsources[vsources.size() - 1].posSource = findSource( vsources[vsources.size() - 1].posSourceId );
				if( vsources[vsources.size() - 1].posSource == 0x0 ) vsources.pop_back();
			}
			else vsources.pop_back();
			
			node2 = node1.getChildNode( "vertices", ++nodeItr2 );
		}

		nodeItr2 = 0;
		node2 = node1.getChildNode( "triangles", nodeItr2 );
		while( !node2.isEmpty() )
		{
			triGroups.push_back( DaeTriGroup() );
			if( triGroups[triGroups.size() - 1].parse( node2 ) )
			{
				DaeTriGroup &triGroup = triGroups[triGroups.size() - 1];
				
				triGroup.vSource = findVSource( triGroup.vSourceId );
				triGroup.normSource = findSource( triGroup.normSourceId );
				triGroup.texSource[0] = findSource( triGroup.texSourceId[0] );
				triGroup.texSource[1] = findSource( triGroup.texSourceId[1] );
				triGroup.texSource[2] = findSource( triGroup.texSourceId[2] );
				triGroup.texSource[3] = findSource( triGroup.texSourceId[3] );

				if( triGroup.vSource == 0x0 )
				{
					log( "Warning: Mesh '" + id + "' has no vertex coordinates and is ignored" );
					triGroups.pop_back();
				}
			}
			else triGroups.pop_back();
			
			node2 = node1.getChildNode( "triangles", ++nodeItr2 );
		}

		if( !node1.getChildNode( "polygons" ).isEmpty() )
			log( "Warning: Ingnoring non-triangle data in mesh '" + id + "'" );
		if( !node1.getChildNode( "polylist" ).isEmpty() )
			log( "Warning: Ingnoring non-triangle data in mesh '" + id + "'" );
		if( !node1.getChildNode( "trifans" ).isEmpty() )
			log( "Warning: Ingnoring non-triangle data in mesh '" + id + "'" );
		if( !node1.getChildNode( "tristrips" ).isEmpty() )
			log( "Warning: Ingnoring non-triangle data in mesh '" + id + "'" );

		return true;
	}


	Vec3f getPos( int posIndex )
	{
		Vec3f v;
		
		// Assume the float buffer has at least 3 values per element
		DaeSource *source = vsources[0].posSource;
		v.x = source->floatArray[posIndex * source->elemsPerEntry + 0];
		v.y = source->floatArray[posIndex * source->elemsPerEntry + 1];
		v.z = source->floatArray[posIndex * source->elemsPerEntry + 2];
		
		return v;
	}
};


struct DaeLibGeometries
{
public:

	vector< DaeGeometry* >	geometries;

	
	~DaeLibGeometries()
	{
		for( unsigned int i = 0; i < geometries.size(); ++i ) delete geometries[i];
	}
	

	DaeGeometry *findGeometry( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < geometries.size(); ++i )
		{
			if( geometries[i]->id == id ) return geometries[i];
		}

		return 0x0;
	}
	
	
	bool parse( const XMLNode &rootNode )
	{
		XMLNode node1 = rootNode.getChildNode( "library_geometries" );
		if( node1.isEmpty() ) return true;

		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "geometry", nodeItr2 );
		while( !node2.isEmpty() )
		{
			DaeGeometry *geometry = new DaeGeometry();
			if( geometry->parse( node2 ) ) geometries.push_back( geometry );
			else delete geometry;

			node2 = node1.getChildNode( "geometry", ++nodeItr2 );
		}
		
		return true;
	}
};

#endif // _daeLibGeometries_H_
