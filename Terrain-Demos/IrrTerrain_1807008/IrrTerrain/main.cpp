#include <irrlicht.h>
#include "CTerrainPager.h"

#pragma comment(lib, "Irrlicht.lib")

using namespace irr;


void createTilesForImage(c8 * ImageFileName, c8 * TileName, irr::video::IVideoDriver * Driver, bool IsHeightMap)
{
	/* Les tiles sont créés et nommés dans l'ordre suivant :

		1 - 2 - 3 - 4
		|   |   |   |
		5 - 6 - 7 - 8
		|   |   |   |
		9 -10 -11 -12
		|   |   |   |
		13 -14 -15 -16
	*/


	u16 TileSize = 256;
	u16 TileSize2 = TileSize;
	core::dimension2du dim;

	// si c'est une heightmap on forme des tiles de 2^n+1 (257)
	if(IsHeightMap)
	{
		//TileSize = 256;
		//TileSize2 = TileSize;
		TileSize2++;
		dim.Height = TileSize2;
		dim.Width = TileSize2;
	}
	else
	{
		TileSize = 256;
		TileSize2 = TileSize;
		dim.Height = TileSize;
		dim.Width = TileSize;
	}



	video::IImage * image = Driver->createImageFromFile(ImageFileName);
	s32 * data = new s32[dim.Height * dim.Width * 3];
	video::IImage * tile  = Driver->createImageFromData(video::ECF_R8G8B8, dim, data);

	int n = 1;
	int xx = 0;
	int yy = 0;
	for(int j=0; j<4; j++)
	{
		for(int p=0; p<4; p++)
		{
			for(int y=0; y<=TileSize2; y++)
			{
				for(int x=0; x<=TileSize2; x++)
				{
					tile->setPixel(x, y, image->getPixel(x+xx,y+yy));
				}
			}

			//core::stringc str = L"Media2/map";
			core::stringc str(TileName);
			str += n;
			str +=  ".png";
			n++;


			Driver->writeImageToFile(tile, str.c_str(), 0 );
			if(IsHeightMap)
				xx+=TileSize;
			else
				xx+=TileSize-1;
		}
		xx=0;
		if(IsHeightMap)
			yy+=TileSize;
		else
			yy+=TileSize-1;
	}



	image->drop();
	tile->drop();
}







int main(int argc, char **argv)
{
	// irrlicht parametres
	irr::SIrrlichtCreationParameters param;
	param.DriverType = video::EDT_OPENGL;
	param.WindowSize = core::dimension2du(800,600);
	param.Fullscreen = false;
	param.Vsync = false;
	param.Bits = 16;
	param.AntiAlias = false;
	IrrlichtDevice * device = createDeviceEx(param);

    // get irr pointers
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager * smgr = device->getSceneManager();
    gui::IGUIEnvironment * guienv = device->getGUIEnvironment();

	// add camera fps
	scene::ICameraSceneNode * cam =  smgr->addCameraSceneNodeFPS(0,100,1.0f);
	cam->setFarValue(200000.0f);
	cam->setNearValue(0.1f);
	cam->setPosition(core::vector3df(8000,50,8000));
	device->getCursorControl()->setVisible(false);

	// split la heightmap et la colormap
	createTilesForImage("Media2/bigmap.png", "Media2/map", driver, true);
	createTilesForImage("Media2/colormap.jpg", "Media2/color", driver, false);

	/*
		colormap and heightmap tiles organisation

		1 - 2 - 3 - 4
		|   |   |   |
		5 - 6 - 7 - 8
		|   |   |   |
		9 -10 -11 -12
		|   |   |   |
		13 -14 -15 -16

	*/


	c8 * listeMap[] = { "Media2/map13.png",
						"Media2/map14.png",
						"Media2/map15.png",
						"Media2/map16.png",
						"Media2/map9.png",
						"Media2/map10.png",
						"Media2/map11.png",
						"Media2/map12.png",
						"Media2/map5.png",
						"Media2/map6.png",
						"Media2/map7.png",
						"Media2/map8.png",
						"Media2/map1.png",
						"Media2/map2.png",
						"Media2/map3.png",
						"Media2/map4.png"};

	c8 * colorMap[] = { "Media2/color13.png",
						"Media2/color14.png",
						"Media2/color15.png",
						"Media2/color16.png",
						"Media2/color9.png",
						"Media2/color10.png",
						"Media2/color11.png",
						"Media2/color12.png",
						"Media2/color5.png",
						"Media2/color6.png",
						"Media2/color7.png",
						"Media2/color8.png",
						"Media2/color1.png",
						"Media2/color2.png",
						"Media2/color3.png",
						"Media2/color4.png"};


	// Terrain pager parameters
	STerrainParameters param2;
	param2.MapSize = 256;							// size of terrain-1 => 256 is ideal for quality/speed
	param2.Dimension = core::dimension2di(4,4);		// tiles organisation (4x4 square)
	param2.LodTextureScale = 100.0f;				// detail texture scale
	param2.MapListFileName = listeMap;				// heightmaps list
	param2.ColorMapListFileName = colorMap;			// colormap list
	param2.Position = core::vector3df(0,0,0);		// first terrain position
	param2.Scale = core::vector3df(10,10,10);		// terrain scale
	param2.Quality = CTerrain::ETQ_MEDIUM;			// terrain quality
	param2.DistanceMaxRender = 180.0f;				// max render distance, ideal tweek between 100-250
	param2.DistanceLoad = param2.MapSize * 1.38f * param2.Scale.X;		// +38% of map size
	param2.DistanceUnload = param2.DistanceLoad * 1.05f;	// +5% of load distance
	param2.Fog = true;								// use fog
	param2.Debug = true;							// show bounding box for terrain tiles

	// create Terrain pager
	CTerrainPager * terrPag = new  CTerrainPager(param2, smgr->getRootSceneNode(), smgr, 0);

	// set fog
	f32 minFog = 400;
	f32 maxFog = param2.DistanceMaxRender * param2.Scale.X * 0.85f;
	video::SColor fogColor = video::SColor(0,150,150,255);
	driver->setFog(fogColor, irr::video::EFT_FOG_LINEAR, minFog, maxFog);


    // loop application
	int lastFPS = -1;
    while(device->run())
    {
		driver->beginScene(true, true, fogColor);
			smgr->drawAll();
            guienv->drawAll();
         driver->endScene();

		// show fps in title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
		  core::stringw str = L"fps:";
		  str += fps;
		  lastFPS = fps;
		  device->setWindowCaption(str.c_str() );
		}
	}

    // clear device
    device->drop();

    return 0;
}



