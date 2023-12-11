#include "irrlicht.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

/*==============================================================================
  Receiver class
==============================================================================*/
class MyEventReceiver : public IEventReceiver
{
   bool KeyIsDown[KEY_KEY_CODES_COUNT];
   bool LEFTBUTTONCLICKED;
   bool RIGHTBUTTONCLICKED;

public:
    virtual bool OnEvent(const SEvent& event)
    {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        if(event.EventType == EET_MOUSE_INPUT_EVENT)
        {
         if     (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN) LEFTBUTTONCLICKED = true;
         else if(event.MouseInput.Event==EMIE_LMOUSE_LEFT_UP     ) LEFTBUTTONCLICKED = false;
         else if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) RIGHTBUTTONCLICKED = true;
         else if(event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP     ) RIGHTBUTTONCLICKED = false;
        }

        return false;
    }

    virtual bool IsKeyDown(EKEY_CODE keyCode) const { return KeyIsDown[keyCode]; }
    virtual bool IsLMBDown() const { return LEFTBUTTONCLICKED; }
    virtual bool IsRMBDown() const { return RIGHTBUTTONCLICKED; }

    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
          KeyIsDown[i] = false;

        LEFTBUTTONCLICKED = RIGHTBUTTONCLICKED = false;
    }
};
/*==============================================================================
  Raise or lower terrain (selected vertice)
==============================================================================*/
void RaiseTerrainVertex(ITerrainSceneNode* terrain, s32 index, f32 step, bool up)
{
   IMesh* pMesh = terrain->getMesh();

   u32 b;
   for (b=0; b<pMesh->getMeshBufferCount(); ++b)
   {
      IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(b);
      // skip mesh buffers that are not the right type
      if (pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue;

      video::S3DVertex2TCoords* pVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices();

      pVertices[index].Pos.Y += (up) ? step : -step;
   }

   // force terrain render buffer to reload
   terrain->setPosition(terrain->getPosition());
}

/*==============================================================================
  Save file
==============================================================================*/
void save (IVideoDriver* driver, ITerrainSceneNode* terrain)
{
	const core::dimension2d<u32> &dim = core::dimension2du(256,256);
   video::IImage *img = driver->createImage(ECF_R8G8B8, dim);

   u32 VertexCount = terrain->getMesh()->getMeshBuffer(0)->getVertexCount();
   S3DVertex2TCoords* verts = (S3DVertex2TCoords*)terrain->getMesh()->getMeshBuffer(0)->getVertices();

   for (u32 i=0; i<VertexCount; i++)
   {
     S3DVertex2TCoords* vertex = verts + i;
     u8 y = (u8)vertex->Pos.Y;
     img->setPixel((u32)vertex->Pos.X, (u32)vertex->Pos.Z, video::SColor(0, y,y,y));
   }

   driver->writeImageToFile(img, "heightmap.bmp", 0);
   img->drop();
}

/*==============================================================================
  Draw a grid showing which vertices will be raised
==============================================================================*/
void drawSelectionGrid(ITerrainSceneNode* terrain, s32 xStart, s32 zStart, int size)
{
	IVideoDriver* driver = terrain->getSceneManager()->getVideoDriver();
	if (size < 1) return;
	core::array<S3DVertex> v;
	core::array<u16> idx;
	for (int x=0;x<size;++x)
	{
		for (int z=0;z<size;++z)
		{
			//USe our +0.5 trick for hte rounding issue
			S3DVertex vert;
			vert.Pos = vector3df(xStart-1.0f*(size/2.0f)+x, terrain->getHeight(xStart-1.0f*(size/2.0f)+x, zStart-1.0f*(size/2)+z)+0.1f, zStart-1.0f*(size/2.0f)+z);
			vert.Color = SColor(255, 0, 255, 255);
			v.push_back(vert);
			idx.push_back(x*size+z);
		}
	}

	SMaterial mat;
	mat.BackfaceCulling = false;
	mat.Lighting = false;
	driver->setMaterial(mat);
	core::matrix4 matr = terrain->getAbsoluteTransformation();
	driver->setTransform(video::ETS_WORLD, matr);
	driver->drawVertexPrimitiveList(&v[0], v.size(), &idx[0], idx.size(), EVT_STANDARD, EPT_POINTS);
}



void explode(ITerrainSceneNode* terrain, s32 indexMid, u8 nbBarrels, s32 size)
{
  //printf("desired impact (%.2f,%.2f)\n", X, Z);

#define CRATOR
#ifdef CRATOR
  //elevation around impact. This array can be stored as raw image:
  static const char eai[25]={
    0,  1,  1,  1, 0,
    1, -1, -1, -1, 1,
    1, -1, -2, -1, 1, // <- impact point vertex will be lowered by -2 units
    1, -1, -1, -1, 1,
    0,  1,  1,  1, 0
  };
#else
    // squares
    static const char eai[25]={
    -1,  -1,  -1,  -1, -1,
    -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, // <- impact point vertex will be lowered by -2 units
    -1, -1, -1, -1, -1,
    -1,  -1,  -1,  -1,-1
  };
#endif

/*  //OldCode

    video::S3DVertex2TCoords* verts = map->getTerrainVertices();

  for (u8 n=0; n<_nbBarrels; n++)
  {
    //arty shells are always deviated
    u16 d;
    d = rand()%2000;
    f32 nX = (f32)((rand()%2) == 0) ? d : -d;
    nX += X;

    d = rand()%2000;
    f32 nZ = (f32)((rand()%2) == 0) ? d : -d;
    nZ += Z;

    //printf("barrel %d (%.2f,%.2f)\n", n, nX,nZ);

    //nearest vertice on terrain
    s32 x = (s32)(nX / map->header->terrainScaleX);
    s32 z = (s32)(nZ / map->header->terrainScaleZ);

    //from the array above its origin is 2 verts left top of impact
    //so always drop arty at least 2 vertices from borders
    x -= 2;
    z -= 2;

    //apply the crater transformation
    int cpt = 0;
    for (u8 v=0; v<5; v++)
      for (u8 u=0; u<5; u++)
      {
        s32 index = (x+u) * map->header->terrainPixels + (z+v);
        verts[index].Pos.Y += eai[cpt++];
      }

    // force terrain render buffer to reload
    terrain->setPosition(terrain->getPosition());
  */

  //ok lets get terrain vertices

  for (u8 n=0; n<nbBarrels; n++)
  {

    //apply the crater transformation
    int cpt = 0;
    for (u8 v=0; v<5; v++)
      for (u8 u=0; u<5; u++)
      {
        s32 index = indexMid - 2 * size + v*size - 2 + u;
		RaiseTerrainVertex(terrain, index, eai[cpt++], true);
      }

    // force terrain render buffer to reload
    //terrain->setPosition(terrain->getPosition());

    //emit particles...

  }
  //shaking...
  //play sound...
}




int main()
{
    MyEventReceiver er;
	IrrlichtDevice *device =
		createDevice( video::EDT_OPENGL, dimension2d<u32>(1280, 960), 16, false, false, false,&er);

	//Our window title
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	//Get the needed managers so we don't need to do device->something->next all the time
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	device->getLogger()->setLogLevel(ELL_INFORMATION);
	device->getLogger()->log("Log level now set to ELL_INFORMATION");

	//This skybox will make stuff look pretty
	ISceneNode* skyBox = 0;

	skyBox = smgr->addSkyBoxSceneNode(
			driver->getTexture("../../media/irrlicht2_up.jpg"),
			driver->getTexture("../../media/irrlicht2_dn.jpg"),
			driver->getTexture("../../media/irrlicht2_lf.jpg"),
			driver->getTexture("../../media/irrlicht2_rt.jpg"),
			driver->getTexture("../../media/irrlicht2_ft.jpg"),
			driver->getTexture("../../media/irrlicht2_bk.jpg"));
    //skyBox = smgr->addSkyDomeSceneNode(driver->getTexture("sky.jpg"));

	// Terrain
	ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("heightmap.bmp");
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, driver->getTexture("../../media/Grass.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("../../media/detailmap3.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(5.0f, 10.0f);
  //  terrain->setScale(vector3df(5,2,5));

	ITriangleSelector* terrainSelector = smgr->createTerrainTriangleSelector(terrain, 0);

	ISceneNode* cube = smgr->addCubeSceneNode(1);

	// Camera
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, .05f);
	cam->setPosition(core::vector3df(-100,100,100));
	cam->setTarget(core::vector3df(0,0,0));

	// Arrow
	ISceneNode* arrow = smgr->addAnimatedMeshSceneNode(smgr->addArrowMesh("arrow",
			SColor(255, 255, 0, 0), SColor(255, 0, 255, 0)), NULL);
	arrow->setMaterialFlag(video::EMF_LIGHTING, false);
	arrow->setRotation(vector3df(0,0,180));

	// GUI
	//IGUIFont* font = guienv->getFont("battlefield.bmp");
	//guienv->getSkin()->setFont(font);

	IGUIStaticText* txt[2];
	txt[0] = guienv->addStaticText(L"Left/Right Mouse Button : raise/lower\nF4/F5 : increase/decrease step\nW : switch wireframe\nS : save heightmap",
				core::rect<s32>(10,10,600,120), false, true, 0, -1, false);
	txt[0]->setOverrideColor(video::SColor(255,150,100,60));
	txt[1] = guienv->addStaticText(L"", core::rect<s32>(10,80,600,2000), false, true, 0, -1, false);
	txt[1]->setOverrideColor(video::SColor(255,0,255,0));


	ITimer* irrTimer = device->getTimer();
	u32 then = 0, then30 = 0;
	f32 step = 2.f;
	bool wireframe = true;
	int lastFPS=0;

	//While our device is still up and running
	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		if (er.IsKeyDown(irr::KEY_ESCAPE)) device->closeDevice();
		if (er.IsKeyDown(irr::KEY_F4))
		{step += 1.f;}
		else if (er.IsKeyDown(irr::KEY_F5) && step > 0)
		{step -= 1.f;}
		if (er.IsKeyDown(irr::KEY_KEY_W))
		{
			wireframe = !wireframe;
			terrain->setMaterialFlag(video::EMF_WIREFRAME, wireframe);
		}
		if(er.IsKeyDown(irr::KEY_KEY_S))
			save (driver, terrain);

		// move the arrow to the nearest vertex ...
		const position2di clickPosition = device->getCursorControl()->getPosition();
		const line3d<float> ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(clickPosition, cam);
		vector3df pos;
		triangle3df Tri;
		ISceneNode* node;

		if (smgr->getSceneCollisionManager()->getCollisionPoint(ray, terrainSelector, pos, Tri, node))
		{
			//arrow->setPosition(pos);
			static const s32 scale = 1; // terrain is scaled 1x
			static const s32 size = 256; // heightmap is 256X256 pixels
			f32 x = (f32)(pos.X / scale);
			f32 z = (f32)(pos.Z / scale);
			s32 index = (s32)(x + 0.5) * size + (s32)(z+0.5);  //Adding 0.5 to X will properly round the position so as to get the nearest index and not the one nearest 0,0

        if (er.IsLMBDown() || er.IsRMBDown()  )
		{
			//RaiseTerrainVertex(terrain, index, step, er->leftMouseDown());
				explode(terrain, index, 1, size);

		}


			x *= scale;
			z *= scale;

			drawSelectionGrid(terrain, (s32)(x + 0.5), (s32)(z+0.5), 2);
			//arrow->setPosition(vector3df((f32)(s32)(x+0.5), terrain->getHeight(x, z)+1 , (f32)(s32)(z+0.5)));  //Double cast to snap to nearest vertex (Note the +0.5) - Not needed for height
		}

		char c[26];
		sprintf(c,"elevation step : %.0f units", step);
        txt[1]->setText(stringw(c).c_str());

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		int fps = driver->getFPS();
        if (lastFPS != fps)
        {
                core::stringw str = L"[";
                str += driver->getName();
                str += "] FPS:";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
        }
	}

	device->drop();

	return 0;
}

