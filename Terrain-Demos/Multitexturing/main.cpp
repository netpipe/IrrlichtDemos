#include <irrlicht.h>
using namespace irr;

#include "CMultiTexturingManager.h"

int main(int argc, char** argv)
{
    //Set up device and receiver
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640,480), 32,false,false,false);

    video::IVideoDriver *driver = device->getVideoDriver();
    scene::ISceneManager *smgr = device->getSceneManager();

    //Create the camera
    SKeyMap keyMap[8];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0,100,0.6,-1,keyMap,8);
    camera->setPosition(core::vector3df(1300,250,0));
    camera->setTarget(core::vector3df(600,0,600));
    camera->setNearValue(0.01);

	scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("Data/textures/skydome.jpg"),16,8,0.95f,2.0f);
    skydome->setRotation(core::vector3df(0,-100,0));
    CMultiTexturingManager *texture_manager = new CMultiTexturingManager(smgr);

    //Create a terrain like in the terrain sample
    scene::ITerrainSceneNode *terrain = smgr->addTerrainSceneNode(
		"Data/textures/splatting_test.png",
		0,					// parent node
		-1,					// node id
		core::vector3df(0.f, 2.f, 0.f),		// position
		core::vector3df(0.f, 0.f, 0.f),		// rotation
		core::vector3df(10,3,10),	// scale
		video::SColor ( 255, 255, 255, 255 ),	// vertexColor
		5,					// maxLOD
		scene::ETPS_17,				// patchSize
		3					// smoothFactor
		);
    terrain->setMaterialFlag(video::EMF_LIGHTING,false);

	terrain->scaleTexture(1.0f, 20.0f);

    terrain->setScale(core::vector3df(5,2,5));

    //Add it to the manager
    texture_manager->addNode(terrain);

    //Set the passes
    //First Pass, with Sand, Gras and Rock
    texture_manager->addPass(terrain,
        driver->getTexture("Data/textures/splat_1.tga"),
        driver->getTexture("Data/textures/Rock.jpg"),
        driver->getTexture("Data/textures/Gras.jpg"),
        driver->getTexture("Data/textures/Sand.jpg"));

    //Second Pass with Snow, Mud and the Vulcano's base texture
    texture_manager->addPass(terrain,
        driver->getTexture("Data/textures/splat_2.tga"),
        driver->getTexture("Data/textures/Snow.jpg"),
        driver->getTexture("Data/textures/Mud.jpg"),
        driver->getTexture("Data/textures/Ashes.jpg"));

    //Third Pass. the base
    texture_manager->addPass(terrain,
        driver->getTexture("Data/textures/splat_3.tga"),
        0,
        0,
        driver->getTexture("Data/textures/Pebbles.jpg"));

    //Fourth Pass: the Lightmap
    texture_manager->addPass(terrain,
        driver->getTexture("Data/textures/lm_terrain.tga"),
        driver->getTexture("Data/textures/black.jpg"),
        0,
        0);

    //Fith Pass: Lava, drawn above the Lightmap to make it "glow"
    CMultiTexturingManager::STexturePass *anim_lava_pass = texture_manager->addPass(terrain,
        driver->getTexture("Data/textures/splat_4.tga"),
        driver->getTexture("Data/textures/lava_1.jpg"),
        0,
        0);

    //FPS measurement
    int lastFPS = -1;
    while(device->run())
    {

        //Draw the scene
        driver->beginScene(true, true, video::SColor(50,50,50,50));
        smgr->drawAll();
        texture_manager->drawAll();

        //Dirty Workaround to make the lava animated :P
        core::stringc name = "Data/textures/lava_";
        name+=1+int(floor((device->getTimer()->getRealTime()/40)%29));
        name+=".jpg";
        anim_lava_pass->red_texture = driver->getTexture(name.c_str());

        driver->endScene();


        //Display the FPS
        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"Texture Splatting - Move with Mouse and WASD/Arrow Keys - FPS:";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }

    }
    //Cleanup
    device->drop();
    delete texture_manager;

    return 0;
}
