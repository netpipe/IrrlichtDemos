// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#ifndef CGAMEINTERFACE_H_INCLUDED
#define CGAMEINTERFACE_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

#include "CGameTimer.h"


namespace GAME
{

//! forward decl.
class CGame;


//! CGameInterface class
/// everything GUI related is here
class CGameInterface
{
    public:
        //! constructor
        CGameInterface(CGame* Game);

        //! getFont
        gui::IGUIFont* getFont() const { return fontVerdana8; }

        //! updateMeters
        void updateMeters();

        //! setDebugInfo
        void setDebugInfo(core::stringw Info);

    private:
        //! createDebugInfo
        void createDebugInfo();

        //! createMeters
        void createMeters();

        // instances
        CGame* game;
        gui::IGUIEnvironment* guienv;
        gui::IGUIElement* root;
        video::IVideoDriver* driver;
        CGameTimer* timer;

        // vars
        gui::IGUIStaticText* debugInfo;
        gui::IGUIStaticText* FPSMeter;
        gui::IGUIStaticText* elapsedTimeMeter;
        int lastFPS;
        gui::IGUIFont* fontVerdana8;

}; // end class CGameInterface

} // end namespace GAME

#endif // CGAMEINTERFACE_H_INCLUDED
