// *************************************************************************************************
//
// Horde3D Terrain Extension
// --------------------------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz and Volker Wiendl
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

#include "utPlatform.h"

#include "terrain.h"
#include "egModules.h"


namespace Horde3DTerrain
{
	const char *getExtensionName()
	{
		return "Terrain";
	}
	
	
	bool initExtension()
	{
		Modules::sceneMan().registerType( SNT_TerrainNode, "Terrain",
			TerrainNode::parsingFunc, TerrainNode::factoryFunc, TerrainNode::renderFunc );

		// Upload default shader used for debug view
		Modules::renderer().uploadShader(
			vsTerrainDebugView, fsTerrainDebugView, TerrainNode::debugViewShader );
		
		return true;
	}

	
	void releaseExtension()
	{
	}

	
	string safeStr( const char *str )
	{
		if( str != 0x0 ) return str;
		else return "";
	}

	
	DLLEXP NodeHandle addTerrainNode( NodeHandle parent, const char *name, ResHandle heightMapRes,
									  ResHandle materialRes )
	{
		SceneNode *parentNode = Modules::sceneMan().resolveNodeHandle( parent );
		if( parentNode == 0x0 ) return 0;
		
		Resource *hmapRes = Modules::resMan().resolveResHandle( heightMapRes );
		if( hmapRes == 0x0 || hmapRes->getType() != ResourceTypes::Texture2D ) return 0;
		
		Resource *matRes =  Modules::resMan().resolveResHandle( materialRes );
		if( matRes == 0x0 || matRes->getType() != ResourceTypes::Material ) return 0;
		
		Modules::log().writeInfo( "Adding Terrain node '%s'", safeStr( name ).c_str() );
		
		TerrainNodeTpl tpl( safeStr( name ), (Texture2DResource *)hmapRes, (MaterialResource *)matRes );
		SceneNode *sn = Modules::sceneMan().findType( SNT_TerrainNode )->factoryFunc( tpl );
		return Modules::sceneMan().addNode( sn, *parentNode );
	}

	
	DLLEXP ResHandle createGeometryResource( NodeHandle node, const char *name, float meshQuality )
	{	
		SceneNode *sn = Modules::sceneMan().resolveNodeHandle( node );
		if( sn != 0x0 && sn->getType() == SNT_TerrainNode )
			return ((TerrainNode *)sn)->createGeometryResource( safeStr( name ), 1.0f / meshQuality );
		else
			return 0;
	}
}
