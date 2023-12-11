#include "videoPlayer.h"
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


using namespace irr;



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

IrrlichtDevice *device;
IVideoDriver* driver;
ISceneManager* smgr;

cVideoPlayer* videoPlayer;


class MyEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(const SEvent &event)
    {
        /*
        If the key 'W' or 'S' was left up, we get the position of the scene node,
        and modify the Y coordinate a little bit. So if you press 'W', the node
        moves up, and if you press 'S' it moves down.
        */

        if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
        {
            switch (event.KeyInput.Key)
            {
            case KEY_KEY_A: videoPlayer->play();
                printf("-> Play\n");
                break;

            case KEY_KEY_S: videoPlayer->pause();
                printf("-> Pause\n");
                break;

            case KEY_KEY_D: videoPlayer->stop();
                printf("-> Stop\n");
                break;

                return true;

                 case KEY_KEY_F: videoPlayer->goToSecond(20);
                printf("-> Manual\n");
                break;

                return true;

            }
        }

        return false;
    }
};


//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------
void main_loop(){
    device->run();

        static int i = 0;

        driver->beginScene(true, true, SColor(255,100,101,140));

        // refresh the video
        videoPlayer->refresh();

        // draw the texture in a rectangle
        videoPlayer->drawVideoTexture();

        //smgr->drawAll();

        driver->endScene();

}

int main(int argc, char *argv[])
{
    MyEventReceiver receiver;

#ifdef __EMSCRIPTEN__
    device = createDevice( video::EDT_OGLES2, dimension2d<u32>(640, 480), 32, false, false, false, &receiver);
#else
    device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 32, false, false, false, &receiver);
#endif
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    // initialise the videoPlayer with a filename
    videoPlayer = new cVideoPlayer(driver, device->getTimer(), "123.avi",800,600);
    videoPlayer->setLoop(false);

    // to get the framerate (fps)
    int lastFPS = -1;
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
    while (device->run())
    {
        static int i = 0;

        driver->beginScene(true, true, SColor(255,100,101,140));

        // refresh the video
        videoPlayer->refresh();

        // draw the texture in a rectangle
        videoPlayer->drawVideoTexture();

        //smgr->drawAll();

        driver->endScene();


        // show the framerate (fps)
        int fps = driver->getFPS();
        if (lastFPS != fps)
        {
            wchar_t tmp[1024];
            swprintf(tmp, 1024, L"Video Example - Irrlicht Engine [%ls] fps: %d",
                     driver->getName(), fps);

            device->setWindowCaption(tmp);
            lastFPS = fps;
        }
    }
#endif
    device->drop();

    return 0;
}



//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

