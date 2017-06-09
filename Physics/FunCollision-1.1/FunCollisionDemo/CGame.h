// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#ifndef CGAME_H_INCLUDED
#define CGAME_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "FunCollision.h"
using namespace COLL;

#include "CGameTimer.h"
#include "CGameInterface.h"
#include "CGameInput.h"
#include "CGameCamera.h"


namespace GAME
{

//! CGame class
class CGame
{
    public:
        //! constructor
        CGame();

        //! destructor
        ~CGame();

        //! init
        int init();

        //! run
        /// main loop
        int run();

        //! get functions
        inline IrrlichtDevice* getDevice() const { return device; }
        inline video::IVideoDriver* getDriver() const { return driver; }
        inline scene::ISceneManager* getSmgr() const { return smgr; }
        inline gui::IGUIEnvironment* getGUIEnv() const { return guienv; }
        inline ICollManager* getCmgr() const { return cmgr; }
        inline CGameTimer* getTimer() const { return timer; }
        inline CGameInterface* getIFace() const { return iface; }
        inline CGameInput* getInput() const { return input; }
        inline CGameCamera* getCamera() const { return camera; }

    private:
        // instances
        IrrlichtDevice* device;
        video::IVideoDriver* driver;
        scene::ISceneManager* smgr;
        gui::IGUIEnvironment* guienv;

        ICollManager* cmgr;
        CGameTimer* timer;
        CGameInterface* iface;
        CGameInput* input;
        CGameCamera* camera;

}; // end class CGame

} // end namespace GAME

#endif // CGAME_H_INCLUDED
