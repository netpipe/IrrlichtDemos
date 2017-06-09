/* -------------------------------------------------------------------------------
    Quake3 map example for FunCollision

    This tutorial shows how to set up FunCollision, shows the basics of creating
    a entities, and loading a level object.

    This is a modification of the Irrlicht collision example, what you might
    already know. I've modified those parts only that were neccessary or
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


int main()
{
	// ----- Let user select driver type
	video::E_DRIVER_TYPE driverType;

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
		default: return 0;
	}

	// ----- Create device
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(800, 600), 32, false);

	if (device == 0) return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

    // ----- Load skin and font
    env->loadGUI("../media/gui/skin.gui", NULL);
    gui::IGUISkin* skin = env->getSkin();

    gui::IGUIFont* fontVerdana8 = env->getFont("../media/gui/verdana8.fnt");
    if (!fontVerdana8) fontVerdana8 = env->getBuiltInFont();

    skin->setFont(fontVerdana8, gui::EGDF_DEFAULT); // set as base font

    /* ----- Create our timer, event receiver the collision manager and our camera.
       Note that you can use your own timer and event receiver classes instead of
       these I've provided. It's just more convinient for me to set the scene up
       this way. Plus, they also have some nice features you can benefit from. */
    // Create timer
    CGameTimer *timer = new CGameTimer(device);

    // Create and set our event receiver
    CGameInput* input = new CGameInput(device);
    device->setEventReceiver(input);

    // Create the collision manager -- Don't forget to call "drop()" on it when you don't need it anymore!
    // We also set up a normal gravity for this scene, which is 0.01f
    ICollManager* cmgr = createCollisionManager(device, 0.01f);

	// Create our camera
	CGameCamera* camera = new CGameCamera(device, cmgr, input,
        core::vector3df(50, 70, -60),   // set it's position
        core::vector3df(30, 50, 30),    // set it's shape
        core::vector3df(0, 30, 0));     // set the "head" position


    // ----- Load up the level:
	device->getFileSystem()->addZipFileArchive("../media/pk3/map-20kdm2.pk3");

	scene::IAnimatedMesh* q3levelmesh = smgr->getMesh("20kdm2.bsp");
	scene::IMeshSceneNode* q3node = 0;

	if (q3levelmesh) q3node = smgr->addOctTreeSceneNode(q3levelmesh->getMesh(0));

	/* Now, for the quake3 map, we are to create an octree triangle selector.
	   The octree triangle selector was created specially for levels and
	   large meshes. It's realtively fast, and you can control it's detail
	   level by setting the minimum amount of triangles to store together.

	   After creating it, we'll also add a collision object as it's
	   physical representation. */
    ICollOctreeSelector* q3selector = NULL;

    if (q3node)
    {
        q3node->setPosition(core::vector3df(-1350,-130,-1400));

        q3selector = cmgr->addOctreeSelector();
        q3selector->addMesh(q3node->getMesh());
        q3selector->build(128); // don't forget to call "build"!

        cmgr->addObject(q3selector, q3node);
    }

	// ----- Now I create three animated characters which we can pick, a dynamic light for
	// lighting them, and a billboard for drawing where we found an intersection.

	// First, let's get rid of the mouse cursor.  We'll use a billboard to show
	// what we're looking at.
	device->getCursorControl()->setVisible(false);

	// ----- Add the billboard.
	scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	bill->setMaterialTexture(0, driver->getTexture("../media/gui/aim.png"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));

	// ----- Add models. These X files uses skeletal animation, but without skinning.
	scene::IAnimatedMeshSceneNode* dwarf1 = 0, *dwarf2 = 0, *dwarf3 = 0;

	dwarf1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../media/objects/dwarf.x"));
	dwarf1->setRotation(core::vector3df(0,-90,0));
	dwarf1->setAnimationSpeed(20.f);

	dwarf2 = (scene::IAnimatedMeshSceneNode*)dwarf1->clone();
	dwarf3 = (scene::IAnimatedMeshSceneNode*)dwarf1->clone();

    /* For the dwarves we're going to create a sampler (shape) and share between them,
       because they have the very same shape anyway. We'll set the origin a little lower,
       so that their feet touches the ground. */
	ICollSphereSampler* dwarfSampler = cmgr->addSphereSampler(core::vector3df(35), core::vector3df(0, -30, 0));

	// Okay, but a sampler won't do anything unless it's attached to an entity. See how it goes:
	cmgr->addEntity(core::vector3df(-150,-66, -40), dwarfSampler, dwarf1);
	cmgr->addEntity(core::vector3df(-150,-66, -80), dwarfSampler, dwarf2);
	cmgr->addEntity(core::vector3df(-150,-66, -120), dwarfSampler, dwarf3);


	// ----- Add a light, so that the unselected nodes aren't completely dark.
	smgr->addLightSceneNode(0, core::vector3df(-60,100,400), video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);


    // ----- Main loop
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

            // ----- Update collision system
            cmgr->update(timer->getElapsedTime());

            // ----- Update camera
            camera->update();

            // ----- Deal with our aim node:
            // If the the ray hits an entity, except our character, highlight it
            SCollContact rayContact;
            core::vector3df camPos = camera->getCamNode()->getAbsolutePosition();
            core::vector3df camTarget = camera->getCamNode()->getTarget() - camPos;
            bool hit = cmgr->rayIntersect(camPos, camTarget, rayContact, 3000, 20, camera->getCamEntity());

            dwarf1->setMaterialFlag(video::EMF_LIGHTING, true);
            dwarf2->setMaterialFlag(video::EMF_LIGHTING, true);
            dwarf3->setMaterialFlag(video::EMF_LIGHTING, true);

            if (hit)
            {
                // Make it visible and place it where the impact was
                bill->setVisible(true);
                bill->setPosition(rayContact.impact);

                // Highlight it if we've hit an entity
                if (rayContact.entity) rayContact.entity->getNode()->setMaterialFlag(video::EMF_LIGHTING, false);
            }
            else bill->setVisible(false);

            smgr->drawAll();

            // 2D drawing
            video::SMaterial unlit;
            unlit.Lighting = false;
            unlit.FogEnable = false;
            driver->setMaterial(unlit);
            driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);

            // Draw the raycast result
            if (hit)
            {
                driver->draw3DTriangle(rayContact.tri.getTri(), video::SColor(255, 255, 255, 0));
                driver->draw3DLine(rayContact.impact, rayContact.impact + (rayContact.normal * 20), video::SColor(255, 0, 0, 255));

                if (rayContact.entity) driver->draw3DBox(rayContact.entity->getBBoxWS(), video::SColor(255, 255, 255, 255));
            }

            env->drawAll();
            // \2D drawing

			driver->endScene();
			// \3D stuff

		    if (input->getKeyReleased(irr::KEY_ESCAPE)) quit = true;


            // display frames per second in window title
            int fps = driver->getFPS();
            if (lastFPS != fps)
            {
                core::stringw str = L"Q3Map Example [FunCollision v";
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

    // Now get rid of the trash:
    if (camera) delete camera;
    if (input) delete input;
    if (timer) delete timer;
    if (cmgr) cmgr->drop(); // Once you don't need it, drop the collision manager!
	if (device) device->drop();

	return 0;
}

/*
**/
