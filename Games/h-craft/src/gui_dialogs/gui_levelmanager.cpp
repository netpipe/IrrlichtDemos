// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_levelmanager.h"
#include "../gui_ids.h"
#include "../level.h"
#include "../main.h"

using namespace irr;
using namespace gui;


GuiLevelManager::GuiLevelManager(irr::gui::IGUIEnvironment* env_)
: mEnvironment(env_)
, mListLevels(NULL)
{
}

GuiLevelManager::~GuiLevelManager()
{
}

void GuiLevelManager::CloseLevelSelection()
{
	IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_WND_LEVEL_SELECTION, true);
	if (e)
	{
        e->remove();
        mListLevels = NULL;
    }
}

void GuiLevelManager::ShowLevelSelection()
{
    CloseLevelSelection();

	// create the window
	core::rect<s32> wndRect(140,50,500,400);
	IGUIWindow* wnd = mEnvironment->addWindow(wndRect, false, L"Levels", 0, GUI_WND_LEVEL_SELECTION);
	if ( !wnd )
        return;

    core::rect<s32> levelsRect(20,30,340,330);
    mListLevels = mEnvironment->addListBox(levelsRect, wnd, GUI_LIST_LEVELS );
    for ( int i=0; i < APP.GetLevelManager()->GetNumLevels(); ++i )
    {
        const LevelSettings &levelSettings = APP.GetLevelManager()->GetLevel(i);
        mListLevels->addItem( levelSettings.mName.c_str() );
    }
}

bool GuiLevelManager::OnEvent(const SEvent &event)
{
    switch ( event.EventType )
    {
        case EET_GUI_EVENT:
        {
            s32 id = event.GUIEvent.Caller->getID();
            switch(event.GUIEvent.EventType)
            {
                case EGET_LISTBOX_CHANGED:
                case EGET_LISTBOX_SELECTED_AGAIN:
                {
                    switch(id)
                    {
                        case GUI_LIST_LEVELS:
                        {
                            int selected = mListLevels->getSelected();
                            if ( selected < 0 )
                                break;
                            std::wstring name(mListLevels->getListItem(selected));
                            APP.GetLevelManager()->LoadLevel(name);
                            CloseLevelSelection();
                            return true;
                        }
                        break;

                        default:
                        break;
                    }
                }
                break;

                default:
                break;
            }
        }
        break;

        default:
        break;
    }
    return false;
}
