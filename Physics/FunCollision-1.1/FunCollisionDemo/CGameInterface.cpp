// FunCollisionDemo -- Demo application for "FunCollision" -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or main.cpp

#include <irrlicht.h>
using namespace irr;

#include "CGameInterface.h"
#include "CGame.h"


namespace GAME
{

//! constructor
CGameInterface::CGameInterface(CGame* Game)
{
    // instances
    game = Game;
    guienv = game->getGUIEnv();
    root = guienv->getRootGUIElement();
    driver = game->getDriver();
    timer = game->getTimer();

    // load skin
    guienv->loadGUI("../media/gui/skin.gui", NULL);
    gui::IGUISkin* skin = guienv->getSkin();

    // load font
    fontVerdana8 = guienv->getFont("../media/gui/verdana8.fnt");
    if (!fontVerdana8) fontVerdana8 = guienv->getBuiltInFont();

    skin->setFont(fontVerdana8, gui::EGDF_DEFAULT); // set as base font

    // misc
    createDebugInfo();
    createMeters();
}

//! createDebugInfo
void CGameInterface::createDebugInfo()
{
    core::dimension2d<u32> screen = driver->getScreenSize();
    debugInfo = guienv->addStaticText(L"", core::rect<s32>(10, 50, 410, screen.Height - 30), false, true, root, -1, false);
}

//! setDebugInfo
void CGameInterface::setDebugInfo(core::stringw Info)
{
    debugInfo->setText(Info.c_str());
}

//! createMeters
void CGameInterface::createMeters()
{
    FPSMeter = guienv->addStaticText(L"", core::rect<s32>(10, 10, 410, 30), false, false, root, -1, false);
    elapsedTimeMeter = guienv->addStaticText(L"", core::rect<s32>(10, 30, 210, 50), false, false, root, -1, false);

    lastFPS = -1;
}

//! updateMeters
void CGameInterface::updateMeters()
{
    int FPS = driver->getFPS();

    // if FPS has changed
    if (lastFPS != FPS)
    {
        core::stringw strFPS = L"FPS: ";
        strFPS += FPS;
        FPSMeter->setText(strFPS.c_str());

        lastFPS = FPS;
    }

    core::stringw strElapsedTime = L"Elapsed time: ";
    strElapsedTime += timer->getElapsedTime();
    elapsedTimeMeter->setText(strElapsedTime.c_str());
}

} // end namespace GAME
