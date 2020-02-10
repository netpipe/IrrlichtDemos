#include <windows.h>
#include <stdlib.h>
#include <irrlicht.h>
#include <irrKlang.h>
#include "World.h"

using namespace std;
using namespace irr;

int main(int argc, char *argv[])
{
    //freopen("log.txt", "wt", stdout);
    //freopen("errlog.txt", "wt", stderr);
    
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
        core::dimension2d<irr::s32>(1024,768),
        32,
        true,
        false,false);
    
    device->setWindowCaption(L"...");
    //device->getFileSystem()->addZipFileArchive("media/graphics.zip");
    device->getFileSystem()->addFolderFileArchive("media/graphics");
    device->getFileSystem()->addFolderFileArchive("media/sfx");
    device->getFileSystem()->addFolderFileArchive("media/music");
    device->getFileSystem()->addFolderFileArchive("media/xml");
    device->getFileSystem()->addFolderFileArchive("media/gui");
    device->getFileSystem()->addFolderFileArchive("media");
    scene::ISceneManager *smgr = device->getSceneManager();
    device->getCursorControl()->setVisible(false);
    srand(device->getTimer()->getTime());
    
    audio::ISoundEngine *sengine = createIrrKlangDevice(audio::ESOD_WIN_MM);
    
    // camera
    scene::ICameraSceneNode *camera = smgr->addCameraSceneNode();
    //scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100.0, 10.0);
    camera->setPosition(core::vector3df(10,5,5));
    camera->setTarget(core::vector3df(0,0,0));
    
    // action!
    World *world = new World(device, sengine);
    
    video::IVideoDriver *driver = device->getVideoDriver();
    ITimer *timer = device->getTimer();
    
    long int currentTime = timer->getTime();
    long int startTime = timer->getTime();
    f32 dt;
    
    //unsigned __int64 currentTime,newTime,timerFreq;
    
    //QueryPerformanceFrequency((LARGE_INTEGER *)&timerFreq);
    //QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);
    
    int lastFPS = -1;
    
    while(device->run())
    {
        if (device->isWindowActive())
        {
            if (timer->isStopped())
            {
                timer->start();
            }
            
            if ( (timer->getTime() - startTime >= 1000.0 / 60.0) && device->isWindowActive())
            {
                
                long int newTime = timer->getTime();
                dt = newTime - currentTime;
                dt *= 0.001;
                currentTime = newTime;
                
                /*
                QueryPerformanceFrequency((LARGE_INTEGER *)&timerFreq);
                QueryPerformanceCounter((LARGE_INTEGER *)&newTime);
                dt = ((double)newTime) / ((double)timerFreq) - ((double)currentTime) / ((double)timerFreq);
                currentTime = newTime;
                */
                
                // update...
                world->update(dt);
                
                startTime = timer->getTime();
            }
            
            int fps = driver->getFPS();
            
            if (lastFPS != fps)
            {
                printf("FPS: %i\n", fps);
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
    
    delete world;
    
    device->drop();
    sengine->drop();
    
    return 0;
}
