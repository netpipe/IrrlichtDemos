#include <stdlib.h>
#include <irrlicht.h>
#include <irrKlang.h>
#include "launcher/DeviceSettings.h"
#include "platforms.h"
#include "player.h"
#include "state.h"

using namespace irr;
using namespace audio;

IrrlichtDevice *device;
ISoundEngine *sengine;
scene::ISceneManager *smgr;
video::IVideoDriver *driver;
gui::IGUIEnvironment *guienv;
ITimer *timer;

core::vector3df cameraStartPos = core::vector3df(0,10.0,-16.0);

int main()
{
    //freopen("log.txt", "wt", stdout);
    //freopen("errlog.txt", "wt", stderr);
    
    DeviceSettings ds = getSettings();
    
    video::E_DRIVER_TYPE driver_type;
    if (ds.driver == 0)
    {
        driver_type = video::EDT_DIRECT3D8;
    }
    else if (ds.driver == 1)
    {
        driver_type = video::EDT_OPENGL;
    }
    else
    {
        driver_type = video::EDT_BURNINGSVIDEO;
    }
    device = createDevice(driver_type,
        core::dimension2d<irr::s32>(ds.width,ds.height),
        ds.bits,
        ds.fullscreen ? true:false,
        true,false,0);
    
    if (!device)
    {
        deviceSettingsErrorMsg("A video device could not be created. Please try adjusting the game settings to \
use a different video driver or screen mode.");
        return 0;
    }
    
    core::stringw windowCaption = L"Purple Skies - Work In Progress";
    device->getCursorControl()->setVisible(false);
    
    sengine = createIrrKlangDevice(ESOD_WIN_MM);
    if (!sengine) printf("Error initialising sound.\n");
    
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
    guienv = device->getGUIEnvironment();
    timer = device->getTimer();
    
    device->getFileSystem()->addFolderFileArchive("media");
    
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
    //scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(NULL, 0.0, 20.0);
    camera->setPosition(cameraStartPos);
    
	scene::ILightSceneNode *light = smgr->addLightSceneNode(camera, core::vector3df(0,50.0,0),
        video::SColorf(0.3, 0.3, 0.3), 20.0f);
	video::SLight &slight = light->getLightData();
	slight.AmbientColor = video::SColorf(0.4,0.4,0.4);
	slight.SpecularColor = video::SColorf(0.0,0.0,0.0);
    
    u32 time_start = timer->getTime();
    double dt;
    
    srand(time_start);
    
    startScreen();
    
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
                
                // updates
                if (state == IN_GAME)
                {
                    updatePlatforms(dt);
                    updatePlayer(dt);
                }
                
                driver->beginScene(true, true, video::SColor(0,180,197,255));
                smgr->drawAll();
                guienv->drawAll();
                driver->endScene();
                
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
    
    device->drop();
    sengine->drop();
    return 0;
}
