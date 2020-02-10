#include <stdlib.h>
#include <irrlicht.h>
//#include <irrKlang.h>
#include "event.h"
#include "rabbit.h"
#include "platforms.h"
#include "carrots.h"
#include "text.h"
#include "misc.h"
#include "launcher/DeviceSettings.h" // the launcher

using namespace irr;
//using namespace audio;

IrrlichtDevice *device;
//ISoundEngine *sengine;
scene::ISceneManager *smgr;
video::IVideoDriver *driver;
gui::IGUIEnvironment *guienv;
ITimer *timer;

core::vector3df firstCameraPosition = core::vector3df(0,14.0,-20.0);
core::vector3df firstCameraTarget = core::vector3df(0,0,0);

bool OPEN_WEBPAGE_ON_EXIT = 0;

void preloadStuff()
{
    timer->stop();
    //gui::IGUIStaticText *loadText = staticText(L"Loading...", screenHW(),screenHH(), 4, "media/bigfont.xml");
    driver->beginScene(true, true, video::SColor(0,255,255,120));
    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();

    smgr->getMesh("media/plat0.obj");
    smgr->getMesh("media/plat1.obj");
    smgr->getMesh("media/plat2.obj");
    loadFonts();
    driver->getTexture("media/demo.png");

    //loadText->remove();
    timer->start();
}

//#define IS_IGL

int main()
{
    DeviceSettings ds;
    ds.width = 800;
    ds.height = 600;
    ds.bits = 32;
    ds.fullscreen = false;
    video::E_DRIVER_TYPE driver_type = video::EDT_OPENGL;


    device = createDevice(driver_type,
        core::dimension2d<irr::u32>(ds.width,ds.height),
        ds.bits,
        ds.fullscreen ? true:false,
        true,false,0);

    if (!device)
    {
    //    deviceSettingsErrorMsg("A video device could not be created. Please try adjusting the game settings to \
//use a different video driver or screen mode.");
        return 0;
    }

    core::stringw windowCaption = L"Run Rabbit RUN!";
    device->getCursorControl()->setVisible(false);

//    sengine = createIrrKlangDevice(ESOD_WIN_MM);//ESOD_AUTO_DETECT, 0);
//    if (!sengine) printf("Error initialising sound.\n");

    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    guienv = device->getGUIEnvironment();
    timer = device->getTimer();

    device->getFileSystem()->addFolderFileArchive("media");

    //scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(NULL, 0,0);

    camera->setFOV(M_PI/2.0);

	scene::ILightSceneNode *light = smgr->addLightSceneNode(camera, core::vector3df(-100,1000,0),
        video::SColorf(0.5f, 0.5f, 0.5f, 0.5f), 400.0f);
	video::SLight &slight = light->getLightData();
	slight.AmbientColor = video::SColorf(0.8,0.8,0.8,0.8);

    preloadStuff();

    startScreen();

    u32 time_start = timer->getTime();
    double dt;

    srand(time_start);

    int lastFPS = -1;

    while(device->run())
    {
        if (device->isWindowActive())
        {
            if (timer->isStopped())
            {
                timer->start();
            }
            if ( (timer->getTime() - time_start >= 1000.0 / 60.0) && device->isWindowActive())
            {
                dt = (double)(timer->getTime() - time_start) / 1000.0f;

                if (state != IN_DEMOSCREEN)
                {
                    createPlatformUpdate(timer->getTime()); // platform generator
                    updatePlatforms(dt);
                    rotateCarrotIcon();
                }

                if (state == IN_GAME)
                {
                    updatePlatformDifficulty(dt);
                    //updateRabbit(dt);

                    int steps = 10;
                    for (int i = 0; i < steps; i ++)
                    {
                        updateRabbit(dt/(double)steps);
                    }

                    updateCarrots();

                    if (rabbitDied())// && state != IN_ENDSCREEN)
                    {
                        removeRabbit();
                        removePlatforms();
                        endScreen();
                        printf("END GAME %i\n", rand());
                    }
                }

                updateTexts(dt);

                driver->beginScene(true, true, video::SColor(0,255,255,120));
                smgr->drawAll();
                guienv->drawAll();
                driver->endScene();
device->sleep(30);
                time_start = timer->getTime();
            }

            int fps = driver->getFPS();

            if (lastFPS != fps)
            {
                core::stringw str = windowCaption;
                str += " - [";
                str += driver->getName();
                str += "] FPS:";
                str += fps;
                device->setWindowCaption(str.c_str());
                lastFPS = fps;
            }
        }
        else
        {
            if (!timer->isStopped())
            {
                timer->stop();
            }
        }
    }

    stopMusic();
    device->drop();
//    sengine->drop();

    if (OPEN_WEBPAGE_ON_EXIT)
    {
        //deviceSettingsOpenWebpage("");
    }

    return 0;
}
