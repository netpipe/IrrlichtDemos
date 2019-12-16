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

#ifndef _daeCommon_H_
#define _daeCommon_H_

#include "utXMLParser.h"
#include "utils.h"
#include <string>
#include <vector>
using namespace std;


struct DaeSource
{
	string				id;
	vector< float >		floatArray;
	vector< string >	stringArray;
	unsigned int		elemsPerEntry;


	bool parse( const XMLNode &sourceNode )
	{
		bool isFloatArray = true;
		elemsPerEntry = 1;
		
		// Check accessor
		XMLNode node1 = sourceNode.getChildNode( "technique_common" );
		if( node1.isEmpty() ) return false;
		XMLNode node2 = node1.getChildNode( "accessor" );
		if( node2.isEmpty() ) return false;
		
		int numEntries = atoi( node2.getAttribute( "count", "0" ) );
		if( numEntries == 0 ) return false;
		
		id = sourceNode.getAttribute( "id", "" );
		if( id == "" ) return false;

		node1 = sourceNode.getChildNode( "float_array" );
		if( node1.isEmpty() )
		{	
			node1 = sourceNode.getChildNode( "Name_array" );
			if( node1.isEmpty() ) node1 = sourceNode.getChildNode( "IDREF_array" );
			if( node1.isEmpty() ) return false;
			isFloatArray = false;
		}
		
		int count = atoi( node1.getAttribute( "count", "0" ) );
		elemsPerEntry = count / numEntries;

		// Parse data
		unsigned int pos = 0;
		char *s = (char *)node1.getText();
		if( s == 0x0 ) return false;
		for( int i = 0; i < count; ++i )
		{
			if( isFloatArray )
			{
				float f;
				parseFloat( s, pos, f );
				floatArray.push_back( f );
			}
			else
			{
				string name;
				parseString( s, pos, name );
				stringArray.push_back( name );
			}
		}

		return true;
	}
};

#endif // _daeCommon_H_
