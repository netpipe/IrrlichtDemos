/*********************************************************************
Copyright (c) 2020-2021 TheMrCerebro
http://themrcerebro.com

irrTiled - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <irrlicht.h>

//======================
#include "../irrTiled.h"
//======================

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

//
int main()
{
    IrrlichtDevice* device = createDevice(EDT_OPENGL);
    device->setWindowCaption(L"irrTiled v1.1 - by TheMrCerebro");

    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();

    smgr->setAmbientLight(SColorf(1, 1, 1, 1));

    ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 20.0f, 0.1f, -1, 0, 0, true);
    cam->setPosition(vector3df(-832, 0, 128));

    //=======================================================
    irrTiled* tld = new irrTiled("media/3dbasic.tmx",device);
	//=======================================================

	// Extracts data from all tile sets.
	for (u32 i = 0; i < tld->tileset.size(); ++i)
	{
		TILESET ts = tld->tileset[i];

		// Height and width of the texture.
		s32 texW = ts.img.imageWidth;
		s32 texH = ts.img.imageHeight;

		// Extracts data from all layers.
		for (u32 j = 0; j < tld->layer.size(); ++j)
		{
			LAYER lyr = tld->layer[j];

			// The array "lyr.data" contains all the ids of the tiles, in this way it can be used to identify
			// in an image the position of the section to be drawn.
			for (u32 d = 0; d < lyr.data.size(); ++d)
			{
				s32 id = lyr.data[d] - ts.firstgid;
				if (id >= 0)
				{
					// Tile position.
					s32 x = (d % lyr.width) * ts.tileWidth;
					s32 y = (d / lyr.width) * ts.tileHeight;

					// As you will see, I add a negative sign at the "x" coordinate.This is because if you position
					// anything in a 3D space with the coordinates taken directly from the map, it would be positioned
					// in reverse.In 2D environments it does not happen but in 3D ¿?.
					IMeshSceneNode* mesh = smgr->addCubeSceneNode(ts.tileWidth, 0, -1, vector3df(-x, 0.0f, y));
					mesh->setMaterialTexture(0, ts.img.tex);
					mesh->setMaterialFlag(EMF_BILINEAR_FILTER, false);
					mesh->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

					// Modify the position and size of the texture to fit a 3D environment.
					f32 tx = float(id % (texW /ts.tileWidth)) / (texW / ts.tileWidth);
					f32 ty = float(id / (texW / ts.tileWidth)) / (texH / ts.tileHeight);
					f32 sx = float(ts.tileWidth) / float(texW);
					f32 sy = float(ts.tileHeight) / float(texH);

					// Apply a matrix with all the texture data to the mesh.
					matrix4 mat;
					mat.setTextureTranslate(tx, ty);
					mat.setTextureScale(sx, sy);
					mesh->getMaterial(0).TextureLayer[0].setTextureMatrix(mat);
				}
			}
		}
	}

	// Create the floor and ceiling.
    IMesh* plane = smgr->addHillPlaneMesh("", dimension2df(1000, 1000), dimension2du(10,10),0,1);

    IMeshSceneNode* floor = smgr->addMeshSceneNode(plane);
    floor->setPosition(vector3df(0, -64, 0));
    floor->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    floor->getMaterial(0).AmbientColor = SColor(255, 130, 130, 130);

    IMeshSceneNode* ceiling = smgr->addMeshSceneNode(plane);
    ceiling->setPosition(vector3df(0, 64, 0));
    ceiling->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
    ceiling->getMaterial(0).AmbientColor = SColor(255, 195, 195, 195);

    while(device->run())
    {
        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}
