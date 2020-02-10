
#ifndef __IRRINFO_H
#define __IRRINFO_H

#include <irrlicht.h>
#include <vector>

class Entity;

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
 * this is the main game state structure
 * it's named wrong ^_^
 * originally it served a different purpose
 */
struct irrInfo
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
        return device;
    }

    // irrlicht pointers
    IrrlichtDevice *device;
    IVideoDriver *driver;
    ISceneManager *smgr;
    IGUIEnvironment *guienv;
    ITimer *timer;
    ICursorControl *cursor;

    // options. data from this can be saved to file.
    struct Options
    {
        Options()
        {
            Screen.AntiAlias = false;
            Screen.Bits = 32;
            Screen.DriverType = EDT_OPENGL;
            Screen.Fullscreen = false;//false;
            Screen.Stencilbuffer = false; // shadows...
            Screen.Vsync = false;
            Screen.WindowSize = dimension2d <u32> (800, 600);
        }

        // data
        SIrrlichtCreationParameters Screen;

        void Save(char *filename)
        {
        }

    } Options;

    void centreMouse()
    {
        cursor->setPosition(400,400);
    }

    // game state info

    // constructor
    irrInfo()
    {
        gameRunning = false;
        gamePaused = false;
    }

    // flags
    bool gameRunning;
    bool gamePaused;

    // pointers
    dWorldID dWorld;
    dSpaceID dSpace;
    std::vector <Entity *> *entities;

    // game state info (OLD structure, should not be a sub structure)
    struct GlobalGameState
    {
        bool endLevel;

        GlobalGameState()
        {
            endLevel = false;
        }
        // level termination
        void requestEndLevel() { endLevel = true; }
        bool endLevelCheck()
        {
            if (endLevel)
            {
                endLevel = false;
                return true;
            }
            else
            {
                return false;
            }
        }
    } GlobalGameState;

};

#endif
