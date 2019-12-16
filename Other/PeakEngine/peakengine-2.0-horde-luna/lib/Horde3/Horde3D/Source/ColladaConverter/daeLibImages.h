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

#ifndef _daeLibImages_H_
#define _daeLibImages_H_

#include "utXMLParser.h"
#include <string>
#include <vector>
using namespace std;


struct DaeImage
{
	string		id;
	string		name;
	string		fileName;


	bool parse( const XMLNode &imageNode )
	{
		id = imageNode.getAttribute( "id", "" );
		if( id == "" ) return false;
		name = imageNode.getAttribute( "name", "" );
		
		if( !imageNode.getChildNode( "init_from" ).isEmpty() &&
			imageNode.getChildNode( "init_from" ).getText() != 0x0 )
			fileName = imageNode.getChildNode( "init_from" ).getText();
		else
			return false;

		// Extract filename from path
		fileName = extractFileName( decodeURL( fileName ), true );
		
		return true;
	}
};


struct DaeLibImages
{
	vector< DaeImage * >	images;


	~DaeLibImages()
	{
		for( unsigned int i = 0; i < images.size(); ++i ) delete images[i];
	}


	DaeImage *findImage( const string &id )
	{
		if( id == "" ) return 0x0;
		
		for( unsigned int i = 0; i < images.size(); ++i )
		{
			if( images[i]->id == id ) return images[i];
		}

		return 0x0;
	}

	
	bool parse( const XMLNode &rootNode )
	{
		XMLNode node1 = rootNode.getChildNode( "library_images" );
		if( node1.isEmpty() ) return true;

		int nodeItr2 = 0;
		XMLNode node2 = node1.getChildNode( "image", nodeItr2 );
		while( !node2.isEmpty() )
		{
			DaeImage *image = new DaeImage();
			if( image->parse( node2 ) ) images.push_back( image );
			else delete image;

			node2 = node1.getChildNode( "image", ++nodeItr2 );
		}
		
		return true;
	}
};

#endif // _daeLibImages_H_
