// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_game.h"
#include "../globals.h"

using namespace irr;
using namespace gui;

GuiGame::GuiGame(irr::gui::IGUIEnvironment* env_)
: mEnvironment(env_)
, mStaticGameTime(NULL)
, mStaticTrackTime(NULL)
, mStaticBestTrackTime(NULL)
{
}

GuiGame::~GuiGame()
{
}

void GuiGame::CreateGameGui()
{
    IGUIElement* root = mEnvironment->getRootGUIElement();
    if ( !mStaticGameTime )
    {
        core::rect<s32> rectTime(5, 50, 100, 80);
        mStaticGameTime = mEnvironment->addStaticText (L"TIME       ", rectTime, /*border*/false, /*wordWrap*/false, /*parent*/0, /*id*/-1, /*fillBackground*/true);
    }
    else
    {
        mStaticGameTime->setVisible(true);
    }

    if ( !mStaticTrackTime )
    {
        core::rect<s32> rectTrackTime(5, 82, 100, 100);
        mStaticTrackTime = mEnvironment->addStaticText (L"track     ", rectTrackTime, /*border*/false, /*wordWrap*/false, /*parent*/root, /*id*/-1, /*fillBackground*/true);
    }
    else
    {
        mStaticTrackTime->setVisible(true);
    }

    if ( !mStaticBestTrackTime )
    {
        core::rect<s32> rectBestTime(5, 102, 100, 120);
        mStaticBestTrackTime = mEnvironment->addStaticText (L"best    ", rectBestTime, /*border*/false, /*wordWrap*/false, /*parent*/root, /*id*/-1, /*fillBackground*/true);
    }
    else
    {
        mStaticBestTrackTime ->setVisible(true);
    }
}

void GuiGame::HideGameGui()
{
    if ( mStaticGameTime)
    {
        mStaticGameTime->setVisible(false);
//        mStaticGameTime->remove();
//        mStaticGameTime = NULL;
    }

    if ( mStaticTrackTime )
    {
        mStaticTrackTime->setVisible(false);
//        mStaticTrackTime->remove();
//        mStaticTrackTime = NULL;
    }

    if ( mStaticBestTrackTime )
    {
        mStaticBestTrackTime->setVisible(false);
//        mStaticBestTrackTime->remove();
//        mStaticBestTrackTime = NULL;
    }
}

void GuiGame::SetGameTime(u32 time_)
{
    if ( !mStaticGameTime)
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];
    swprintf_hc(editText, editSize, L"TIME %.2f", (float)time_ / 1000.f);
    mStaticGameTime->setText(editText);
}

void GuiGame::SetTrackTime(u32 time_)
{
    if ( !mStaticTrackTime)
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];
    swprintf_hc(editText, editSize, L"track %.2f", (float)time_ / 1000.f);
    mStaticTrackTime->setText(editText);
}

void GuiGame::SetBestTrackTime(u32 time_)
{
    if ( !mStaticBestTrackTime)
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];
    swprintf_hc(editText, editSize, L"best %.2f", (float)time_ / 1000.f);
    mStaticBestTrackTime->setText(editText);
}

bool GuiGame::OnEvent(const SEvent &event)
{
    return false;
}
