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

#ifndef _daeLibControllers_H_
#define _daeLibControllers_H_

#include "utXMLParser.h"
#include <string>
#include <vector>
using namespace std;


struct DaeWeight
{
	unsigned int			joint;
	float					weight;
};


typedef vector< DaeWeight > DaeVertWeights;


struct DaeSkin
{
	string						id;
	string						ownerId;
	float						bindShapeMat[16];
	vector< DaeSource >			sources;
	DaeSource					*jointArray;
	DaeSource					*weightArray;
	DaeSource					*bindMatArray;
	vector< DaeVertWeights >	vertWeights;

	
	DaeSkin()
	{
		jointArray = 0x0;
		weightArray = 0x0;
		bindMatArray = 0x0;
	}
	

	DaeSource *findSource( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < sources.size(); ++i )
		{
			if( sources[i].id == id ) return &sources[i];
		}

		return 0x0;
	}

	
	bool parse( const XMLNode &ctrlNode )
	{
		XMLNode node1 = ctrlNode.getChildNode( "skin" );
		if( node1.isEmpty() ) return false;
		
		id = ctrlNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		ownerId = node1.getAttribute( "source", "" );
		removeGate( ownerId );

		// Bind shape matrix
		XMLNode node2 = node1.getChildNode( "bind_shape_matrix" );
		if( node2.isEmpty() ) return false;

		unsigned int pos = 0;
		char *s = (char *)node2.getText();
		if( s == 0x0 ) return false;
		for( int i = 0; i < 16; ++i )
		{
			float f;
			parseFloat( s, pos, f );
			bindShapeMat[i] = f;
		}
		
		// Sources
		int nodeItr2 = 0;
		node2 = node1.getChildNode( "source", nodeItr2 );
		while( !node2.isEmpty() )
		{
			sources.push_back( DaeSource() );
			if( !sources[sources.size() - 1].parse( node2 ) ) sources.pop_back();
			
			node2 = node1.getChildNode( "source", ++nodeItr2 );
		}

		// Joints
		node2 = node1.getChildNode( "joints" );
		if( !node2.isEmpty() )
		{
			int nodeItr3 = 0;
			XMLNode node3 = node2.getChildNode( "input", nodeItr3 );
			while( !node3.isEmpty() )
			{
				if( strcmp( node3.getAttribute( "semantic", "" ), "JOINT" ) == 0 )
				{
					string id = node3.getAttribute( "source", "" );
					removeGate( id );
					jointArray = findSource( id );
				}
				else if( strcmp( node3.getAttribute( "semantic", "" ), "INV_BIND_MATRIX" ) == 0 )
				{
					string sourceId = node3.getAttribute( "source", "" );
					removeGate( sourceId );
					bindMatArray = findSource( sourceId );
				}

				node3 = node2.getChildNode( "input", ++nodeItr3 );
			}
		}
		
		// Vertex weights
		unsigned int jointOffset = 0, weightOffset = 0;
		unsigned int numInputs = 0;
		
		node2 = node1.getChildNode( "vertex_weights" );
		int count = atoi( node2.getAttribute( "count", "0" ) );
		int nodeItr3 = 0;
		XMLNode node3 = node2.getChildNode( "input", nodeItr3 );
		while( !node3.isEmpty() )
		{
			++numInputs;

			if( strcmp( node3.getAttribute( "semantic", "" ), "JOINT" ) == 0 )
			{
				jointOffset = atoi( node3.getAttribute( "offset", "0" ) );
				
				string id = node3.getAttribute( "source", "" );
				removeGate( id );
				DaeSource *vertJointArray = findSource( id );
				if( jointOffset != 0 || vertJointArray->stringArray != jointArray->stringArray )
					log( "Warning: Vertex weight joint array doesn't match skin joint array" );
			}
			else if( strcmp( node3.getAttribute( "semantic", "" ), "WEIGHT" ) == 0 )
			{
				weightOffset = atoi( node3.getAttribute( "offset", "0" ) );
				string id = node3.getAttribute( "source", "" );
				removeGate( id );
				weightArray = findSource( id );
			}
			
			node3 = node2.getChildNode( "input", ++nodeItr3 );
		}

		node3 = node2.getChildNode( "vcount" );
		pos = 0;
		s = (char *)node3.getText();
		if( s == 0x0 ) return false;
		for( int i = 0; i < count; ++i )
		{
			unsigned int ui;
			parseUInt( s, pos, ui );

			DaeVertWeights vertWeight;
			for( unsigned int j = 0; j < ui; ++j )
			{
				vertWeight.push_back( DaeWeight() );
			}

			vertWeights.push_back( vertWeight );
		}

		node3 = node2.getChildNode( "v" );
		pos = 0;
		s = (char *)node3.getText();
		if( s == 0x0 ) return false;
		for( int i = 0; i < count; ++i )
		{
			for( unsigned int j = 0; j < vertWeights[i].size(); ++j )
			{
				for( unsigned int k = 0; k < numInputs; ++k )
				{
					float f;
					parseFloat( s, pos, f );

					if( k == jointOffset ) vertWeights[i][j].joint = (unsigned int)f;
					if( k == weightOffset ) vertWeights[i][j].weight = f;
				}
			}
		}
		
		return true;
	}
};


struct DaeMorph
{
	string					id;
	string					ownerId;
	vector< DaeSource >		sources;
	DaeSource				*targetArray;
	DaeSource				*weightArray;


	DaeMorph()
	{
		targetArray = 0x0;
		weightArray = 0x0;
	}
		

	DaeSource *findSource( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < sources.size(); ++i )
		{
			if( sources[i].id == id ) return &sources[i];
		}

		return 0x0;
	}

	
	bool parse( const XMLNode &ctrlNode )
	{
		XMLNode node1 = ctrlNode.getChildNode( "morph" );
		if( node1.isEmpty() ) return false;

		id = ctrlNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		ownerId = node1.getAttribute( "source", "" );
		removeGate( ownerId );
		
		// Sources
		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "source", nodeItr2 );
		while( !node2.isEmpty() )
		{
			sources.push_back( DaeSource() );
			if( !sources[sources.size() - 1].parse( node2 ) ) sources.pop_back();
			
			node2 = node1.getChildNode( "source", ++nodeItr2 );
		}

		// Targets
		node2 = node1.getChildNode( "targets" );
		if( !node2.isEmpty() )
		{
			int nodeItr3 = 0;
			XMLNode node3 = node2.getChildNode( "input", nodeItr3 );
			while( !node3.isEmpty() )
			{
				if( strcmp( node3.getAttribute( "semantic", "" ), "MORPH_TARGET" ) == 0 )
				{
					string sourceId = node3.getAttribute( "source", "" );
					removeGate( sourceId );
					targetArray = findSource( sourceId );
				}
				else if( strcmp( node3.getAttribute( "semantic", "" ), "MORPH_WEIGHT" ) == 0 )
				{
					string sourceId = node3.getAttribute( "source", "" );
					removeGate( sourceId );
					weightArray = findSource( sourceId );
				}

				node3 = node2.getChildNode( "input", ++nodeItr3 );
			}
		}

		return true;
	}
};


struct DaeLibControllers
{
	vector< DaeSkin * >		skinControllers;
	vector< DaeMorph * >	morphControllers;


	~DaeLibControllers()
	{
		for( unsigned int i = 0; i < skinControllers.size(); ++i ) delete skinControllers[i];
		for( unsigned int i = 0; i < morphControllers.size(); ++i ) delete morphControllers[i];
	}

	
	DaeSkin *findSkin( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < skinControllers.size(); ++i )
		{
			if( skinControllers[i]->id == id ) return skinControllers[i];
		}
		
		return 0x0;
	}


	DaeMorph *findMorph( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < morphControllers.size(); ++i )
		{
			if( morphControllers[i]->id == id ) return morphControllers[i];
		}
		
		return 0x0;
	}
	
	
	bool parse( const XMLNode &rootNode )
	{
		XMLNode node1 = rootNode.getChildNode( "library_controllers" );
		if( node1.isEmpty() ) return true;

		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "controller", nodeItr2 );
		while( !node2.isEmpty() )
		{
			// Skin
			XMLNode node3 = node2.getChildNode( "skin" );
			if( !node3.isEmpty() )
			{
				DaeSkin *skin = new DaeSkin();
				if( skin->parse( node2 ) ) skinControllers.push_back( skin );
				else delete skin;
			}

			// Morph
			node3 = node2.getChildNode( "morph" );
			if( !node3.isEmpty() )
			{
				DaeMorph *morph = new DaeMorph();
				if( morph->parse( node2 ) ) morphControllers.push_back( morph );
				else delete morph;
			}

			node2 = node1.getChildNode( "controller", ++nodeItr2 );
		}
		
		return true;
	}
};

#endif // _daeLibControllers_H_
