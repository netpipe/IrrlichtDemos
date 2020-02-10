
#ifndef __INFO_H
#define __INFO_H

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

struct Info
{
    // irrlicht setup
    IrrlichtDevice *set()
    {
        this->device = createDeviceEx(Options.Screen);
        device->setWindowCaption(L"Test");
        driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        guienv = device->getGUIEnvironment();
        timer = device->getTimer();
        cursor = device->getCursorControl();
        fs = device->getFileSystem();

        levelEditing = false;
        return device;
    }

    // irrlicht pointers
    IrrlichtDevice *device;
    IVideoDriver *driver;
    ISceneManager *smgr;
    IGUIEnvironment *guienv;
    ITimer *timer;
    ICursorControl *cursor;
    IFileSystem *fs;

    // hmm
    bool levelEditing;

    // options. data from this can be saved to file.
    struct Options
    {
        Options()
        {
            Screen.AntiAlias = false;
            Screen.Bits = 32;
            Screen.DriverType = EDT_OPENGL;
            //Screen.DriverType = EDT_SOFTWARE2;
            Screen.Fullscreen = false;
            Screen.Stencilbuffer = true; // shadows...
            Screen.Vsync = false;
            Screen.WindowSize = dimension2d <s32> (640,480);
            Screen.WindowSize = dimension2d <s32> (1024, 768);
        }

        // data
        SIrrlichtCreationParameters Screen;

    } Options;

};

#endif
