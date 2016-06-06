// terrainrender v0.0.2, 26.09.2003, irrlicht licence
// a hack from Nikos CGeometryCreator
// if you want to help, please contact
// kortyburns or knightoflight, irrlicht-forummembers


#ifndef _TERRAIN_H
#define _TERRAIN_H
#include <fstream>
#include <iostream>
#include <irrlicht.h>
#include "IAnimatedMesh.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;



const int mapsizex=256;
const int mapsizez=256;

class terrain
{
	public:
		int getHeight(int X, int Z);

		void loadRawMap(char *name );

		//Scaled for better performance, from Kortyburns
		IAnimatedMesh* renderterrainscaled(const c8* name,
			const core::dimension2d<f32>& tileSize,
			video::SMaterial* material,
			const core::dimension2d<f32>& textureRepeatCount,
			int heightstretch, int scale);

		// The terraincode from version 0.0.1
		IAnimatedMesh* renderterrain(const c8* name,
			const core::dimension2d<f32>& tileSize,
			video::SMaterial* material,
			const core::dimension2d<f32>& textureRepeatCount,
			int heightstretch);

		// A texture is used to color the vertices
	//	IAnimatedMesh* terrain::renderterrainwithvertexcolors(const c8* name,
	//		video::ITexture* texture,
	//		const core::dimension2d<f32>& tileSize,
	//		video::SMaterial* material,
	//		const core::dimension2d<f32>& textureRepeatCount,
	//		int heightstretch);

	private:
		unsigned char heightmap[mapsizex][mapsizez];

};

#endif
