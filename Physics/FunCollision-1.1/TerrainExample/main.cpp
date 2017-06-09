/* -------------------------------------------------------------------------------
    Terrain example for FunCollision

    This tutorial shows how to set up FunCollision, shows the basics of creating
    a camera entity, and loading a terrain object.

    This is a modification of the Irrlicht terrain rendering example, what you
    might already know. I've modified those parts only that were neccessary or
    more convinient to handle this way.
-------------------------------------------------------------------------------- */

#include <iostream>

#include <irrlicht.h>
using namespace irr;

#include "FunCollision.h"
using namespace COLL;

#include "CGameTimer.h"
#include "CGameInput.h"
#include "CGameCamera.h"


/*
The start of the main function starts like in most other example. We ask the user
for the desired renderer and start it up. This time with the advanced parameter handling.
*/
int main()
{
	// let user select driver type

	video::E_DRIVER_TYPE driverType = video::EDT_DIRECT3D9;

	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;

	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 1;
	}

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.DriverType=driverType;
	params.WindowSize=core::dimension2d<u32>(800, 600);
	IrrlichtDevice* device = createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.


	/*
	First, we add standard stuff to the scene: A nice irrlicht engine
	logo, a small help text, a user controlled camera, and we disable
	the mouse cursor.
	*/

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

    // Load skin and font
    env->loadGUI("../media/gui/skin.gui", NULL);
    gui::IGUISkin* skin = env->getSkin();

    gui::IGUIFont* fontVerdana8 = env->getFont("../media/gui/verdana8.fnt");
    if (!fontVerdana8) fontVerdana8 = env->getBuiltInFont();

    skin->setFont(fontVerdana8, gui::EGDF_DEFAULT); // set as base font

    // Create timer
    CGameTimer *timer = new CGameTimer(device);

    // Create and set our event receiver
    CGameInput* input = new CGameInput(device);
    device->setEventReceiver(input);

    // Create the collision manager -- Don't forget to call "drop()" when you don't need it anymore!
    ICollManager* cmgr = createCollisionManager(device);
    // Setting the base friction to 0.98f will result in a space-like movement
    cmgr->setBaseFriction(0.98f);

	// Create our camera
	CGameCamera* camera = new CGameCamera(device, cmgr, input, core::vector3df(2700 * 2, 255 * 2, 2600 * 2), 100);

	// Add some help text
	env->addStaticText(
		L"[WASD] Move\n[E] Wireframe\n[R] Detail map\n[T] Skybox/skydome\n[ESC] Exit",
		core::rect<s32>(10,520,200,590), true, true, 0, -1, true);

	// Disable mouse cursor
	device->getCursorControl()->setVisible(false);

	/*
	Here comes the terrain renderer scene node: We add it just like any
	other scene node to the scene using
	ISceneManager::addTerrainSceneNode(). The only parameter we use is a
	file name to the heightmap we use. A heightmap is simply a gray scale
	texture. The terrain renderer loads it and creates the 3D terrain from
	it.

	To make the terrain look more big, we change the scale factor of
	it to (40, 4.4, 40). Because we don't have any dynamic lights in the
	scene, we switch off the lighting, and we set the file
	terrain-texture.jpg as texture for the terrain and detailmap3.jpg as
	second texture, called detail map. At last, we set the scale values for
	the texture: The first texture will be repeated only one time over the
	whole terrain, and the second one (detail map) 20 times.
	*/

	// Add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
		"../media/objects/terrain-heightmap.bmp",
		0,					                // parent node
		-1,					                // node id
		core::vector3df(0.f, 0.f, 0.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(40.f, 4.4f, 40.f),	// scale
		video::SColor(255, 255, 255, 255),	// vertexColor
		5,					                // maxLOD
		scene::ETPS_17,				        // patchSize
		4					                // smoothFactor
		);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0,
			driver->getTexture("../media/objects/terrain-texture.jpg"));
	terrain->setMaterialTexture(1,
			driver->getTexture("../media/objects/detail02.png"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 30.0f);

	/* Just like in Irrlicht, first we create a triangle selector for the terrain.
       This can be done through the collision manager. We can use the terrain scene
       node directly as the data source for out selector. Ususally we won't need
       all the triangles of the tarrain, so we set the LODLevel to 2.
       You'll have to call the "build()" command after adding the terrain. */
    ICollTerrainSelector* terrainSelector = cmgr->addTerrainSelector(terrain, 2);
    terrainSelector->build();


    /* After creating the selector, we'll need an object as the representation
       of our terrain. To make it more easier to understand: a selector is like
       a mesh, an object is like a scene node.

       We're going to bind the terrain selector and the terrain scene node to
       our object. */
	cmgr->addObject(terrainSelector, terrain);


	/*
	To make the user be able to switch between normal and wireframe mode,
	we create an instance of the event reciever from above and let Irrlicht
	know about it. In addition, we add the skybox which we already used in
	lots of Irrlicht examples and a skydome, which is shown mutually
	exclusive with the skybox by pressing 'S'.
	*/
	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/objects/irrlicht2_up.jpg"),
		driver->getTexture("../media/objects/irrlicht2_dn.jpg"),
		driver->getTexture("../media/objects/irrlicht2_lf.jpg"),
		driver->getTexture("../media/objects/irrlicht2_rt.jpg"),
		driver->getTexture("../media/objects/irrlicht2_ft.jpg"),
		driver->getTexture("../media/objects/irrlicht2_bk.jpg"));
	scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("../media/objects/skydome.jpg"),16,8,0.95f,2.0f);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    skybox->setVisible(true);
    skydome->setVisible(false);

    bool showBox = true;

	/*
	That's it, draw everything.
	*/

    // main loop
	int lastFPS = -1;
    bool quit = false;

	while (device->run() && !quit)
	{
	    // do we have to restart the timer?
	    timer->checkTimer();

	    // do we have to restart the collision manager?
	    cmgr->checkTimeing();

		if (device->isWindowActive())
		{
		    input->setLockMouse(true);  // lock mouse to the center of the screen
		    timer->updateTimer();       // update the timer

            // 3D stuff
			driver->beginScene(true, true, video::SColor(255, 50, 150, 255));

            cmgr->update(timer->getElapsedTime());
            camera->update();

            smgr->drawAll();

            // 2D drawing
            video::SMaterial unlit;
            unlit.Lighting = false;
            unlit.FogEnable = false;
            driver->setMaterial(unlit);
            driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

            env->drawAll();
            // \2D drawing

			driver->endScene();
			// \3D stuff

		    if (input->getKeyReleased(irr::KEY_ESCAPE)) quit = true;
		    if (input->getKeyReleased(irr::KEY_KEY_E)) // switch wire frame mode
		    {
				terrain->setMaterialFlag(video::EMF_WIREFRAME, !terrain->getMaterial(0).Wireframe);
		    }
		    if (input->getKeyReleased(irr::KEY_KEY_R)) // toggle detail map
		    {
				terrain->setMaterialType(terrain->getMaterial(0).MaterialType == video::EMT_SOLID ?
                    video::EMT_DETAIL_MAP : video::EMT_SOLID);
		    }
		    if (input->getKeyReleased(irr::KEY_KEY_T)) // toggle skies
		    {
				showBox = !showBox;
				skybox->setVisible(showBox);
				skydome->setVisible(!showBox);
		    }

            // display frames per second in window title
            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
                core::stringw str = L"Terrain Example [FunCollision v";
                str += FUNCOLLISION_VERSION_MAJOR;
                str += ".";
                str += FUNCOLLISION_VERSION_MINOR;
                str += " build: ";
                str += FUNCOLLISION_VERSION_BUILD;
                str += "] FPS: ";
                str += fps;

                device->setWindowCaption(str.c_str());
                lastFPS = fps;
            }

		    // update input
		    input->update();

            // thread delay (uncomment it if you want to lock framerate to refresh rate)
            // timer->threadDelay();
		}
		else
		{
		    // if the window is inactive, we should pause the device and release the mouse
		    device->yield();
		    input->setLockMouse(false);
		}
	}

    // Get rid of trash:
    if (camera) delete camera;
    if (input) delete input;
    if (timer) delete timer;
    if (cmgr) cmgr->drop();     // dont't forget to call ->drop() once you don't need the manager!
	if (device) device->drop();

	return 0;
}

/*
Now you know how to use terrain in Irrlicht.
**/

