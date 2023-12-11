// Shifting Tiled Terrain Demo
// autor: Michal vantner
// date: 29.04.2007

#include <irrlicht.h>
using namespace irr;

#include "MCamera.h"
#include "MInput.h"
#include "ShTlTerrainSceneNode.h"
#include "SomeFunctions.h"

int main()
{
    MyEventReceiver receiver;
    IrrlichtDevice *device;
    video::E_DRIVER_TYPE drivertype = video::EDT_SOFTWARE;
    bool fulscreen = false;

    // setup

    device = createDevice(drivertype, core::dimension2d<s32>(500, 300), 32, fulscreen, false, false, 0);

    video::IVideoDriver* driver = device->getVideoDriver();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	ITimer* timer = device->getTimer();

    gui::IGUIFont* font = guienv->getFont("media/cityblueprint.bmp");
    guienv->getSkin()->setFont(font);

    device->setWindowCaption(L"Tiled Terrain Demo Setup");

    guienv->addStaticText(L"TILED TERRAIN DEMO", core::rect<s32>(20,20,250,40), false);
    guienv->addStaticText(L"created: 26.04.2007", core::rect<s32>(20,40,250,60), false);
    guienv->addStaticText(L"author: arras", core::rect<s32>(20,60,250,80), false);

    gui::IGUIButton *drtbutton = guienv->addButton(core::rect<s32>(170,70,330,100), 0, 101, L"OpenGl");
    gui::IGUIButton *fscrbutton = guienv->addButton(core::rect<s32>(170,110,330,140), 0, 101, L"WINDOW");
    gui::IGUIButton *contbutton = guienv->addButton(core::rect<s32>(170,150,330,180), 0, 101, L"CONTINUE");
    gui::IGUIButton *extbutton = guienv->addButton(core::rect<s32>(170,190,330,220), 0, 101, L"EXIT");
    guienv->addImage(driver->getTexture("media/irrlichtlogo.jpg"), core::position2d<int>(390,250));

    bool exit = true;

    drivertype = video::EDT_OPENGL;

    u32 pause = 0;

    while(device->run())
    {
        if(timer->getRealTime() > pause) pause = 0;

        if(!pause)
        {
            if(drtbutton->isPressed())
            {
                pause = timer->getRealTime() + 300;

                if(drivertype == video::EDT_OPENGL)
                {
                    drivertype = video::EDT_DIRECT3D9;
                    drtbutton->setText(L"DirectX 9");
                }
                else
                {
                    drivertype = video::EDT_OPENGL;
                    drtbutton->setText(L"OpenGl");
                }
            }

            if(fscrbutton->isPressed())
            {
                pause = timer->getRealTime() + 500;

                if(fulscreen)
                {
                    fulscreen = false;
                    fscrbutton->setText(L"WINDOW");
                }
                else
                {
                    fulscreen = true;
                    fscrbutton->setText(L"FULLSCREEN");
                }

            }
        }

        if(contbutton->isPressed())
        {
            exit = false;
            device->closeDevice();
        }

        if(extbutton->isPressed())
        {
            exit = true;
            device->closeDevice();
        }

        driver->beginScene(true, true, video::SColor(255,100,120,100));
		guienv->drawAll();
		driver->endScene();
    }
    device->drop();

    if(exit) return 0;




    // demo

    device = createDevice(drivertype, core::dimension2d<s32>(800, 600), 32, fulscreen, false, false, &receiver);

    device->setWindowCaption(L"Tiled Terrain Demo");

    driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	font = guienv->getFont("media/cityblueprint.bmp");
	guienv->getSkin()->setFont(font);

	timer = device->getTimer();

	// set atributes of terrain
	s32 terrainWidth = 2000;
	s32 terrainHeight = 2000;
	s32 meshSize = 100;
	f32 tileSize = 1;
	core::vector3df terrainPos(0.0f, 0.0f, 0.0f);

	// setup camera
	MCameraFPS camera(smgr);
	camera.setNearValue(0.1f);
	camera.setFarValue(tileSize*meshSize/2);
	camera.setPosition(core::vector3df(terrainWidth*tileSize/2, 0, terrainHeight*tileSize/2) + terrainPos);

	// fog
	driver->setFog(video::SColor(255,100,101,140), true, tileSize*meshSize/4, tileSize*(meshSize-4)/2, 0.05f);

	// light
	scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(0,0,0),
		video::SColorf(255, 255, 255, 255), 1000.0f);
	video::SLight ldata = light->getLightData();
    ldata.AmbientColor = video::SColorf(0.2,0.2,0.2);
	ldata.DiffuseColor = video::SColorf(1,1,1);
	ldata.Type = video::ELT_DIRECTIONAL;
	ldata.Position = core::vector3df(-10,5,-5);
	light->setLightData(ldata);

	// create test scene node
	scene::IAnimatedMesh* testmesh = smgr->getMesh("media/test.3ds");
    scene::IAnimatedMeshSceneNode* test = smgr->addAnimatedMeshSceneNode( testmesh );
    test->setMaterialFlag(video::EMF_LIGHTING, false);
    test->setMaterialFlag(video::EMF_FOG_ENABLE, true);

    // create terrain scene node
    ShTlTerrainSceneNode* terrain = new ShTlTerrainSceneNode(smgr, terrainWidth, terrainHeight, tileSize, meshSize);
	terrain->drop();

	// setup terrain node
	terrain->setMaterialFlag(video::EMF_LIGHTING, true);
	//terrain->setMaterialFlag(video::EMF_WIREFRAME, true);
	//terrain->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    terrain->setMaterialFlag(video::EMF_FOG_ENABLE, true);

    terrain->setPosition(terrainPos);

    // set terrain to render around camera
	terrain->follow(camera.getNode());

    gui::IGUIStaticText *text[10];
    text[0] = guienv->addStaticText(L"RANDOMIZING TERRAIN", core::rect<s32>(250,140,400,160), false);
	text[1] = guienv->addStaticText(L"...please wait", core::rect<s32>(440,140,700,160), false);

    driver->beginScene(true, true, video::SColor(255,100,101,140));
	guienv->drawAll();
	driver->endScene();

	// randomize terrain
	makeRandomHills(terrain, 20000, 20);
	makeRandomHills(terrain, 100000, 5);

	text[1]->setText(L"done");
	text[2] = guienv->addStaticText(L"CALCULATING NORMALS", core::rect<s32>(250,160,430,180), false);
	text[3] = guienv->addStaticText(L"...please wait", core::rect<s32>(440,160,700,180), false);

	driver->beginScene(true, true, video::SColor(255,100,101,140));
	guienv->drawAll();
	driver->endScene();

	// smooth normals
	terrain->smoothNormals();

	text[3]->setText(L"done");
	text[4] = guienv->addStaticText(L"RANDOMIZING COLOR", core::rect<s32>(250,180,430,200), false);
	text[5] = guienv->addStaticText(L"...please wait", core::rect<s32>(440,180,700,200), false);

	driver->beginScene(true, true, video::SColor(255,100,101,140));
	guienv->drawAll();
	driver->endScene();

	// randomize color
	randomizeColor(terrain);

	text[5]->setText(L"done");
	text[6] = guienv->addStaticText(L"RANDOMIZING TEXTURE", core::rect<s32>(250,200,430,220), false);
	text[7] = guienv->addStaticText(L"...please wait", core::rect<s32>(440,200,700,220), false);

	driver->beginScene(true, true, video::SColor(255,100,101,140));
	guienv->drawAll();
	driver->endScene();

    // randomize texture UV coords
    randomizeUV(terrain);

	// set detail texture
	//irr::video::ITexture texto;
	//texto
	terrain->setMaterialTexture(0, driver->getTexture("media/dirt_x_43.bmp"));
	text[7]->setText(L"done");

	driver->beginScene(true, true, video::SColor(255,100,101,140));
	guienv->drawAll();
	driver->endScene();

    // movement speed per second
    f32 speed = 10.0f * tileSize;
    f32 turn = 0.1f;

	// time
	f32 time = 0;
	f32 oldtime;
	// time between frames
	f32 deltaT;

	wchar_t tmp[1024];

	bool help = false;

	guienv->clear();

	while(device->run())
    {
        driver->beginScene(true, true, video::SColor(255,100,101,140));

        // calculate time between frames
        oldtime = time;
        time = (f32)timer->getTime() / 1000.0f;
        deltaT = time - oldtime;

        // camera movement control
        if(key.pressed(KEY_KEY_W)) camera.moveForward(speed * deltaT);
        if(key.pressed(KEY_KEY_S)) camera.moveBack(speed * deltaT);
        if(key.pressed(KEY_KEY_D)) camera.moveRight(speed * deltaT);
        if(key.pressed(KEY_KEY_A)) camera.moveLeft(speed * deltaT);

        // mouse directional control
        camera.turnRight(50 * (device->getCursorControl()->getRelativePosition().X - 0.5f));
        camera.turnUp(50 * (device->getCursorControl()->getRelativePosition().Y - 0.5f));
        device->getCursorControl()->setPosition(0.5f, 0.5f);

        // camera height control
        if (mouse.wheel < 10) mouse.wheel = 10;
        camera.setHeight(  mouse.wheel/10 + terrain->getHeight(camera.getPosition())  );

        // intersection with line test
        if(mouse.left)
        {
            core::vector3df v = camera.getDirection();
            v = v * camera.getFarValue();

            v = v + camera.getPosition();
            core::line3d<f32> line = core::line3d<f32>(camera.getPosition(), v);
            core::vector3df intersection;

            if(terrain->getIntersectionWithLine(line, intersection) )
                test->setPosition(intersection);
        }

        if(key.pressed(KEY_KEY_H)) help = true;
        else help = false;

        if(key.pressed_once(KEY_KEY_B))
        {
            scene::E_DEBUG_SCENE_TYPE dtype =(scene::E_DEBUG_SCENE_TYPE) terrain->isDebugDataVisible();

            switch(dtype)
            {
                case scene::EDS_OFF: terrain->setDebugDataVisible(scene::EDS_BBOX); break;
                case scene::EDS_BBOX: terrain->setDebugDataVisible(scene::EDS_BBOX_BUFFERS); break;
                case scene::EDS_BBOX_BUFFERS: terrain->setDebugDataVisible(scene::EDS_NORMALS); break;
                case scene::EDS_NORMALS: terrain->setDebugDataVisible(scene::EDS_MESH_WIRE_OVERLAY); break;
                case scene::EDS_MESH_WIRE_OVERLAY: terrain->setDebugDataVisible(scene::EDS_FULL); break;
                default: terrain->setDebugDataVisible(scene::EDS_OFF); break;
            }
        }

		smgr->drawAll();
		guienv->drawAll();

		s32 fps = driver->getFPS();
		swprintf(tmp, 1024, L"frames per second: %i", fps);
        font->draw(tmp, core::rect<s32>( 10, 10, 400, 20 ), video::SColor(255,255,255,255));

        swprintf(tmp, 1024, L"sectors rendered: %i", terrain->getSectorsRendered());
        font->draw(tmp, core::rect<s32>( 10, 30, 400, 40 ), video::SColor(255,255,255,255));

        swprintf(tmp, 1024, L"out of total %i", terrain->getSectorCount());
        font->draw(tmp, core::rect<s32>( 150, 30, 400, 40 ), video::SColor(255,255,255,255));

        swprintf(tmp, 1024, L"camera position: %i, %i, %i,", (s32)camera.getPosition().X, (s32)camera.getPosition().Y, (s32)camera.getPosition().Z);
        font->draw(tmp, core::rect<s32>( 10, 50, 400, 60 ), video::SColor(255,255,255,255));

        swprintf(tmp, 1024, L"camera heading: %i", (s32)camera.getHeading());
        font->draw(tmp, core::rect<s32>( 10, 70, 400, 80 ), video::SColor(255,255,255,255));

        //u32 prm = driver->getPrimitiveCountDrawn();
        //swprintf(tmp, 1024, L"primitives: %i", prm );
        //font->draw(tmp, core::rect<s32>( 10, 90, 300, 100 ), video::SColor(255,255,255,255));

        if(help)
        {
            swprintf(tmp, 1024, L"W,A,S,D  -move camera");
            font->draw(tmp, core::rect<s32>( 10, 110, 400, 120 ), video::SColor(255,255,255,255));

            swprintf(tmp, 1024, L"mouse  -turn camera");
            font->draw(tmp, core::rect<s32>( 10, 130, 400, 140 ), video::SColor(255,255,255,255));

            swprintf(tmp, 1024, L"mouse wheel  -hover camera");
            font->draw(tmp, core::rect<s32>( 10, 150, 400, 160 ), video::SColor(255,255,255,255));

            swprintf(tmp, 1024, L"left mouse button  -test collision with line");
            font->draw(tmp, core::rect<s32>( 10, 170, 400, 180 ), video::SColor(255,255,255,255));

            swprintf(tmp, 1024, L"B  -switch different debug modes");
            font->draw(tmp, core::rect<s32>( 10, 190, 400, 200 ), video::SColor(255,255,255,255));

            swprintf(tmp, 1024, L"Alt F4  -exit");
            font->draw(tmp, core::rect<s32>( 10, 210, 400, 220 ), video::SColor(255,255,255,255));
        }
        else
        {
            swprintf(tmp, 1024, L"press H for help");
            font->draw(tmp, core::rect<s32>( 10, 110, 400, 120 ), video::SColor(255,255,255,255));
        }
        device->sleep(8,0);

		driver->endScene();
    }
    device->drop();

    return 0;
}
