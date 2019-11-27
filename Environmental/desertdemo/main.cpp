
#include <irrlicht.h>
//#include <irrKlang.h>
#include <iostream>

#include "irrExt/CGrassPatchSceneNode.h"
#include "irrExt/CWindGenerator.h"
#include "irrExt/CLensFlareSceneNode.h"
#include "irrExt/CCloudSceneNode.h"
#include "irrExt/CRendererPostProc.h"
#include "irrExt/CEffectPostProc.h"
#include "irrExt/CTransitionPostProc.h"
#include "irrExt/CSplitPostProc.h"
#include "irrExt/CLensFlarePostProc.h"
#include "irrExt/CWaterPostProc.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace io;
//using namespace irrklang;

void weaponFire();
IrrlichtDevice* device = 0;

#include "player.h"

Player *player;
#include "event.h"

video::ITexture *tex1;
video::ITexture *tex2;

scene::IWindGenerator *wind;
//ISoundEngine* soundEngine;

scene::ISceneManager* smgr = 0;

int main()
{
    IrrlichtDevice* deviceNULL = createDevice(video::EDT_NULL);
    irr::io::IXMLReader* xml  = deviceNULL->getFileSystem()->createXMLReader("config.xml");

    if(xml == 0)
        std::cout << "Problem reading the configuration file.\n";

    int screenWidth = 0, screenHeight  = 0, fullscreen = 0,
        vsync      = 0, antiAlias   = 0, stencilBuffer = 0, shader = 1, clouds = 21, grass = 1,
        cloudseed = 20, grassdensity = 600, grassdistance = 6000;
    while(xml && xml->read())
    {
        if(core::stringw("video") == xml->getNodeName())
        {
            screenWidth  = xml->getAttributeValueAsInt(L"screenWidth");
            screenHeight  = xml->getAttributeValueAsInt(L"screenHeight");
            fullscreen = xml->getAttributeValueAsInt(L"fullscreen");
            vsync = xml->getAttributeValueAsInt(L"vsync");
            antiAlias = xml->getAttributeValueAsInt(L"antiAliasing");
            stencilBuffer = xml->getAttributeValueAsInt(L"stencilBuffer");
        }

        if(core::stringw("detail") == xml->getNodeName())
        {
            shader  = xml->getAttributeValueAsInt(L"shader");
            clouds  = xml->getAttributeValueAsInt(L"clouds");
            cloudseed  = xml->getAttributeValueAsInt(L"cloudseed");
            grass  = xml->getAttributeValueAsInt(L"grass");
            grassdensity  = xml->getAttributeValueAsInt(L"grassdensity");
            grassdistance  = xml->getAttributeValueAsInt(L"grassdistance");
        }
    }

    deviceNULL->closeDevice();

//	soundEngine = createIrrKlangDevice();
//	if (!soundEngine)
//	{
		printf("Could not startup sound engine.\n");
//		return 0;
//	}

	QEventReceiver receiver;

    SIrrlichtCreationParameters deviceParameter;
    deviceParameter.WindowSize =  core::dimension2di(screenWidth, screenHeight);
    deviceParameter.DriverType = video::EDT_OPENGL;
    deviceParameter.Fullscreen = fullscreen;
    deviceParameter.Vsync = vsync;
    deviceParameter.Bits = 32;
    deviceParameter.AntiAlias = antiAlias;
    deviceParameter.Stencilbuffer = stencilBuffer;
    deviceParameter.EventReceiver = &receiver;

	device = createDeviceEx(deviceParameter);

    if(!device)
        return 1;

    video::IVideoDriver* driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();

    driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	scene::ILightSceneNode *light = smgr->addLightSceneNode(0, core::vector3df(-60.0f,10000.0f,8.0f),
                            video::SColor(0,248, 130, 0));
    light->setLightType(video::ELT_DIRECTIONAL);
    light->enableCastShadow(true);

    scene::ISceneNodeAnimator* sunCycle = smgr->createFlyCircleAnimator(core::vector3df(100.0f,0.0f,100.0f), 15000.0f, 0.000006f, core::vector3df(0.f, 1.f, 1.f), 0.4f);
		light->addAnimator(sunCycle);
		sunCycle->drop();

    scene::ISceneNodeAnimator* sunRotation = smgr->createRotationAnimator(core::vector3df(0.0f,0.004f,0.05f));
		light->addAnimator(sunRotation);
		sunRotation->drop();

	smgr->setAmbientLight(video::SColor(255,200,200,200));
	driver->setFog(video::SColor(0, 248, 130, 0), video::EFT_FOG_LINEAR, 500.0f, 25000.0f, 0.3f, false);
/*
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 1.20f, -1, 0, 0, true);

	camera->setPosition(core::vector3df(4000.0f, 2000.0f, 6000.0f));
	camera->setFarValue(20000.0f);*/

	// create wind...
	wind = scene::createWindGenerator(2.0f, 2.0f);

	device->getCursorControl()->setVisible(false);

	// add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("data/heightmap.tga");

	if (!terrain)
		return 1;

	terrain->setScale(core::vector3df(100.0f, 16.0f, 100.0f));
	terrain->setPosition(core::vector3df(0,0,0));

	video::IImage* heightMap  = driver->createImageFromFile("data/heightmap.tga");
	video::IImage* textureMap = driver->createImageFromFile("data/grasscol.tga");
	video::IImage* grassMap   = driver->createImageFromFile("data/grassmap.tga");

	terrain->setMaterialTexture(0, driver->getTexture("data/texture.tga"));
	terrain->setMaterialTexture(1, driver->getTexture("data/detailmap.tga"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);

	terrain->scaleTexture(35.0f, 0.0f);

	scene::ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
	selector->drop();
/*
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,100,60),
		core::vector3df(0,-5,0),
		core::vector3df(0,50,0));
	camera->addAnimator(anim);
	anim->drop();*/

	scene::CLensFlareSceneNode* flare = new scene::CLensFlareSceneNode(light, smgr, 666);
	if(flare)
		flare->getMaterial(0).setTexture(0, driver->getTexture("data/flares.jpg"));
    flare->setMaterialFlag(video::EMF_LIGHTING, false);

    smgr->addSkyDomeSceneNode(driver->getTexture("data/skydome.tga"),16,16,1.0f,1.4f);

    int lastFPS = -1;
    if(grass == 1)
    {
        int gw = 24, gh = 24;
        scene::CGrassPatchSceneNode *grass[gw*gh];

        tex1 = driver->getTexture("data/grass.tga");
        tex2 = driver->getTexture("data/grass2.bmp");
        driver->makeColorKeyTexture(tex2, core::position2d<s32>(0,0));

        //now we add some grass nodes
        for (int x=0; x<gw; ++x)
            for (int z=0; z<gh; ++z)
            {
                grass[x*gh + z] = new scene::CGrassPatchSceneNode(terrain, smgr, -1, core::vector3d<s32>(x,0,z), "grass", heightMap, textureMap, grassMap, wind);
                grass[x*gh + z]->setMaterialFlag(video::EMF_LIGHTING, false);
                grass[x*gh + z]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                grass[x*gh + z]->getMaterial(0).TextureLayer[0].TextureWrap = video::ETC_CLAMP;
                grass[x*gh + z]->getMaterial(0).MaterialTypeParam = 0.5f;
                grass[x*gh + z]->setMaterialTexture(0, tex1);
                grass[x*gh + z]->setMaterialFlag(video::EMF_FOG_ENABLE, true);
                grass[x*gh + z]->drop();
            }

        for(int n=0; n<gw*gh; ++n)
        {
            grass[n]->setDrawDistance(grass[n]->getDrawDistance()+grassdistance);
            grass[n]->setMaxDensity(grass[n]->getMaxDensity() + grassdensity);
        }
    }

    if(clouds == 1)
    {
        scene::CCloudSceneNode* clouds = new scene::CCloudSceneNode(
                smgr->getRootSceneNode(), smgr,
                    device->getTimer(), 666, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1,1,1));

        video::ITexture * txture = driver->getTexture("data/cloud.tga");
        srand(1111);
        clouds->setLOD(4);
        clouds->setMaxDepth(4);
        clouds->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        srand(rand());
        clouds->makeRandomCloud(cloudseed);
        clouds->setMaterialTexture( 0, txture );
        clouds->setPosition(core::vector3df(0,400,0));

        scene::ISceneNodeAnimator* cloudsCycle = smgr->createFlyCircleAnimator(core::vector3df(100.0f,0.0f,100.0f), 15000.0f, 0.000006f, core::vector3df(0.f, 1.f, 1.f), 0.4f);
            clouds->addAnimator(cloudsCycle);
            cloudsCycle->drop();
    }

/*
	scene::ISceneNode* sun = smgr->addBillboardSceneNode(light, core::dimension2d<f32>(2000, 2000));
	sun->setMaterialFlag(video::EMF_LIGHTING, false);
	sun->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	sun->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	sun->setMaterialTexture(0, driver->getTexture("data/sun.bmp"));

    CEffectPostProc* ppBloom;
    IPostProc* ppRenderer;
    CEffectPostProc* ppDirect8X;
    CEffectPostProc* ppDirect4X;
    CEffectPostProc* ppDirect2X;
    CEffectPostProc* ppDirect1X;
    if(shader == 1)
    {
        ppRenderer = new CRendererPostProc( smgr, core::dimension2du( 1024, 512 ), true, true, video::SColor( 255, 100, 101, 140 ) );
        ppBloom = new CEffectPostProc( ppRenderer, core::dimension2du( 1024, 512 ), PP_BLOOM, 4.0f, 0.005f, 1.0f);
        ppDirect8X = new CEffectPostProc(ppBloom, core::dimension2du( 8192, 4096 ), PP_DIRECT);
        ppDirect4X = new CEffectPostProc(ppDirect8X, core::dimension2du( 4096, 2048 ), PP_DIRECT);
        ppDirect2X = new CEffectPostProc(ppDirect4X, core::dimension2du( 2048, 1024 ), PP_DIRECT);
        ppDirect1X = new CEffectPostProc(ppDirect2X, core::dimension2du( 1024, 512 ), PP_DIRECT);
    }
*/
//	ISound* windSFX = soundEngine->play3D("data/sound/desert_wind.ogg",
//	                               vec3df(0,0,0), true, false, true);
 //   soundEngine->play2D("data/sound/atmo.ogg", true);
	//if(windSFX)
	//	windSFX->setMinDistance(5.0f);
	float posOnCircle = 0;
	const float radius = 5;

	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	gui::IGUIFont* font2 = device->getGUIEnvironment()->getFont("data/fonts/font.bmp");

    player = new Player();
    player->create (device, selector );

	while(device->run())
    if (device->isWindowActive())
    {
		driver->beginScene(true, true, video::SColor(0,50,50,200) );
        if(shader == 1)
//            ppBloom->render(NULL);
//        else
           smgr->drawAll();
		core::stringw ammoStr = L"A:";
//		ammoStr += player->ammo;
//		ammoStr += " / H:";
//		ammoStr += player->health;
//		ammoStr += " / S:";
//		ammoStr += player->shield;
		if(font2)
//        font2->draw(ammoStr.c_str(),
     //                       core::rect<s32>(driver->getScreenSize().Width-200,driver->getScreenSize().Height-60,20,60),
     //                       video::SColor(128,255,255,255));
		env->drawAll();

		driver->endScene();

		posOnCircle += 0.04f;
		irr::core::vector3df pos3d(radius * cosf(posOnCircle), 0,
					  radius * sinf(posOnCircle * 0.5f));
//		soundEngine->setListenerPosition(vec3df(0,0,0), vec3df(0,0,1));
//		if(windSFX)
//			windSFX->setPosition(pos3d);

	    if(receiver.keyPressed(irr::KEY_RIGHT) || receiver.keyPressed(irr::KEY_LEFT)
	    || receiver.keyPressed(irr::KEY_DOWN) || receiver.keyPressed(irr::KEY_UP))
	    {
//	        if(!soundEngine->isCurrentlyPlaying("data/sound/sand_step.wav"))
 //               soundEngine->play2D("data/sound/sand_step.wav", false, false, false, ESM_NO_STREAMING);
	    }

	    if(receiver.keyPressed(irr::KEY_KEY_R) && player->ammo == 0)
	    {
            player->ammo = 18;
 //           soundEngine->play2D("data/sound/reload.wav", false, false, false, ESM_NO_STREAMING);
        }

		// display frames per second in window title
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"irrDesertDemo! [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;
			str += ", Tri:";
			str += s32(driver->getPrimitiveCountDrawn());

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

//    if(windSFX)
 //       windSFX->drop();

	heightMap->drop();
	textureMap->drop();
	grassMap->drop();

	device->drop();
//	soundEngine->drop();

    if(shader == 1)
    {
//        delete ppRenderer;
//w        delete ppBloom;
//        delete ppDirect8X;
   //     delete ppDirect4X;
   //     delete ppDirect2X;
   //     delete ppDirect1X;
    }
	delete player;

    return 0;
}

void weaponFire()
{
	scene::ICameraSceneNode* camera = smgr->getActiveCamera();
	if(!camera)
		return;

	core::vector3df start = camera->getPosition()-core::vector3df(10.0f,10.0f,0.0f);

	core::vector3df end = (camera->getTarget() - start);
	end.normalize();
	start += end*20.0f;

	end = start + (end * camera->getFarValue());

	core::triangle3df triangle;
	core::line3d<f32> line(start, end);

	scene::ISceneNode* node = 0;
	node = smgr->addBillboardSceneNode( 0,core::dimension2d<f32>(10,10), start);

    player->setAnim("pow");
    player->Anim[1].next += player->Anim[1].delta;

	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setMaterialTexture(0, device->getVideoDriver()->getTexture("data/fireball.bmp"));
	node->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	f32 length = (f32)(end - start).getLength();
	const f32 speed = 4.8f;
	u32 time = (u32)(length / speed);

	scene::ISceneNodeAnimator* anim = 0;

	anim = smgr->createFlyStraightAnimator(start, end, time);
	node->addAnimator(anim);
	anim->drop();

	anim = smgr->createDeleteAnimator(time);
	node->addAnimator(anim);
	anim->drop();

//	soundEngine->play2D("data/sound/plasma.wav", false, false, false, ESM_NO_STREAMING);
}
