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

/*	Title: Horde3D Terrain Extension */

#pragma once

#include "Horde3D.h"

#ifndef DLL
#	if defined( WIN32 ) || defined( _WINDOWS )
#		define DLL extern "C" __declspec( dllimport )
#	else
#		define DLL extern "C"
#	endif
#endif


/*	Topic: Introduction
		Some words about the Terrain Extension.
	
	The Terrain Extension extends Horde3D with the capability to render large landscapes. A special
	level of detail algorithm adapts the resolution of the terrain mesh so that near regions get
	more details than remote ones. The algorithm also considers the geometric complexity of the
	terrain to increase the resoultion solely where this is really required. This makes the
	rendering fast and provides a high quality with a minimum of popping artifacts.
	
	A height map is used to define the altitude of the terrain. The height map is a usual texture
	map that encodes 16 bit height information in two channels. The red channel of the texture
	contains the coarse height, while the green channel encodes finer graduations. The encoding of
	the information is usually done with an appropriate tool. If you just want to use 8 bit
	height information, you can simply copy the greyscale image to the red channel of the height
	map and leave the green channel black.
	
	To install the extension, copy the Extensions directory to the path where the Horde3D SDK resides,
	so that the two directories are on the same level in the hierarchy.
	In Visual Studio, add the extension and sample projects to the Horde3D solution. Then add the extension
	project to the project dependencies of the Horde3D Engine and the Horde3D Engine to the dependencies of
	the Terrain Sample. After that, include 'Terrain/extension.h' in 'egExtensions.cpp' of the engine and add
	'#pragma comment( lib, "Extension_Terrain.lib" )' to link against the terrain extension (under Windows).
	Finally, add the following line to ExtensionManager::installExtensions to register the extension:

	- installExtension( Horde3DTerrain::getExtensionName, Horde3DTerrain::initExtension, Horde3DTerrain::releaseExtension );

	The extension is then part of the Horde3D DLL and can be used with the Horde3DTerrain.h header file.

	The extension defines the uniform *terBlockParams* and the attribute *terHeight* that can be used
	in a shader to render the terrain. To see how this is working in detail, have a look at the included
	sample shader.
*/


/*
	Constants: Predefined constants
	SNT_TerrainNode	- Type identifier of Terrain scene node
*/
const int SNT_TerrainNode = 100;


struct TerrainNodeParams
{
	/*	Enum: TerrainNodeParams
			The available Terrain node parameters.
		
		HeightMapRes	- Height map texture; must be square and a power of two [type: ResHandle, write-only]
		MaterialRes		- Material resource used for rendering the terrain [type: ResHandle]
		MeshQuality		- Constant controlling the overall resolution of the terrain mesh (default: 50.0) [type: float]
		SkirtHeight		- Height of the skirts used to hide cracks (default: 0.1) [type: float]
		BlockSize		- Size of a terrain block that is drawn in a single render call; must be 2^n+1 (default: 17) [type: int]
	*/
	enum List
	{
		HeightMapRes = 10000,
		MaterialRes,
		MeshQuality,
		SkirtHeight,
		BlockSize
	};
};


namespace Horde3DTerrain
{
	/* 	Function: addTerrainNode
			Adds a Terrain node to the scene.
		
		This function creates a new Terrain node and attaches it to the specified parent node.
		
		Parameters:
			parent		 - handle to parent node to which the new node will be attached
			name		 - name of the node
			heightMapRes - handle to a Texture2D resource that contains the terrain height information (must be square and POT) 
			materialRes  - handle to the Material resource used for rendering the terrain

		Returns:
			 handle to the created node or 0 in case of failure
	*/
	DLL NodeHandle addTerrainNode( NodeHandle parent, const char *name, ResHandle heightMapRes, ResHandle materialRes );
	

	/* 	Function: createGeometryResource
			Creates a Geometry resource from a specified Terrain node.
				
		This function creates a new Geometry resource that contains the vertex data of the specified Terrain node.
		To reduce the amount of data, it is possible to specify a quality value which controls the overall resolution
		of the terrain mesh. The algorithm will automatically create a higher resoultion in regions where the
		geometrical complexity is higher and optimize the vertex count for flat regions.
		
		Parameters:
			node		- handle to terrain node that will be accessed
			resName		- name of the Geometry resource that shall be created
			meshQuality - constant controlling the overall mesh resolution
			
		Returns:
			 handle to the created Geometry resource or 0 in case of failure
	*/
	DLL ResHandle createGeometryResource( NodeHandle node, const char *resName, float meshQuality );
};
