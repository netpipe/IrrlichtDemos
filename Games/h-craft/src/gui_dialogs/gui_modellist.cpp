// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_modellist.h"

#ifdef HC1_ENABLE_EDITOR

#include "../gui_ids.h"
#include "../node_manager.h"
#include "../main.h"
#include "gui_editor.h"

using namespace irr;
using namespace gui;


GuiModelList::GuiModelList(irr::gui::IGUIEnvironment* env_, GuiEditor * editor_)
: mEnvironment(env_)
, mListModels(NULL)
, mEditor(editor_)
{
}

GuiModelList::~GuiModelList()
{
}

void GuiModelList::Close()
{
	IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_WND_MODEL_LIST, true);
	if (e)
	{
        e->remove();
        mListModels = NULL;
    }
}

void GuiModelList::Show()
{
    Close();

	// create the window
	core::rect<s32> wndRect(140,50,500,400);
	IGUIWindow* wnd = mEnvironment->addWindow(wndRect, false, L"Models", 0, GUI_WND_MODEL_LIST);
	if ( !wnd )
        return;

    core::rect<s32> levelsRect(20,30,340,330);
    mListModels = mEnvironment->addListBox(levelsRect, wnd, GUI_LIST_MODELS );
    for ( unsigned int i=0; i < APP.GetNodeManager()->GetNumNodes(); ++i )
    {
        NodeSettingsUnknown* node = APP.GetNodeManager()->GetNode(i);
        if ( node )
        {
            std::wstring wstr( node->mName.begin(), node->mName.end() );
            mListModels->addItem( wstr.c_str() );
        }
    }
}

bool GuiModelList::OnEvent(const SEvent &event)
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
                        case GUI_LIST_MODELS:
                        {
                            int selected = mListModels->getSelected();
                            if ( selected < 0 )
                                break;
                            std::wstring name(mListModels->getListItem(selected));
                            if ( mEditor )
                            {
                                // this is more of a hack - use an own eventreceiver class if you need it once more
                                mEditor->OnModelSelected(name);
                            }
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

#endif // HC1_ENABLE_EDITOR
