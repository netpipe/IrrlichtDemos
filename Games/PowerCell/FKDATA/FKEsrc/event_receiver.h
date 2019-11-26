#include "menu_events.h"
#include "button_events.h"
#include "combo_events.h"
#include "scrollbar_events.h"
#include "listbox_events.h"
#include "mouse_events.h"
#include "viewports.h"                              // viewports functions
/*
   To get all the events sent by the GUI Elements, we need to create an event
   receiver. This one is really simple. If an event occurs, it checks the id
   of the caller and the event type, and starts an action based on these values.
   For example, if a menu item with id 100 was selected, if opens a file-open-dialog.
*/
class MyEventReceiver : public IEventReceiver
{
public:
    virtual bool OnEvent(const SEvent& event)
    {
        // use the viewport class
        refresh=true;

        if (event.EventType == EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_SPACE)
        {
            if (event.KeyInput.PressedDown)
            {
                viewp.moveviews=true;
                viewp.Camera[0]->setInputReceiverEnabled(false);
            }
            else
            {
                viewp.moveviews=false;
            }
            return true;
        }
        if (event.EventType == EET_KEY_INPUT_EVENT &&
                event.KeyInput.Key == KEY_TAB)
        {
            if (event.KeyInput.PressedDown)
            {
                tab1->setVisible(!tab1->isVisible());
                selList->setVisible(!selList->isVisible());
                seltitle->setVisible(!seltitle->isVisible());
                coords->setVisible(!coords->isVisible());
                if (viewp.viewrightspace==180) viewp.viewrightspace=0;
                else viewp.viewrightspace=180;
                if (tab1->isVisible()) vptopright->move(core::position2d< s32 >(-180,0));
                else vptopright->move(core::position2d< s32 >(180,0));
                if (tab1->isVisible()) vpbottomright->move(core::position2d< s32 >(-180,0));
                else vpbottomright->move(core::position2d< s32 >(180,0));
            }
            return true;
        }
        if (event.EventType == EET_GUI_EVENT)
        {
            s32 id = event.GUIEvent.Caller->getID();
            IGUIEnvironment* env = Device->getGUIEnvironment();

            switch (event.GUIEvent.EventType)
            {
            case EGET_MENU_ITEM_SELECTED:
            {
                menuEvents(event);
                break;
            }
            case EGET_FILE_SELECTED:
            {
                // load the model file, selected in the file open dialog
                irr::gui::IGUIFileOpenDialog* dialog =
                    (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                loadModel(core::stringc(dialog->getFileName()).c_str());
            }

            case EGET_SCROLL_BAR_CHANGED:
                scrollbarEvents(event);
                break;
            case EGET_COMBO_BOX_CHANGED:
                comboEvents(event);
                break;
            case  EGET_LISTBOX_CHANGED:
                listBoxEvents(event);
                break;
            case EGET_LISTBOX_SELECTED_AGAIN:
                switch (id)
                {
                case 890 :
                    bool value=fileRequest.doubleClick();
                    if (value)
                    {
                        loadModel(fileRequest.getFilename().c_str());
                        loading=false;
                    }
                    break;
                }
                break;
            case EGET_BUTTON_CLICKED:
                buttonEvents(event);
                break;
            case EGET_ELEMENT_CLOSED:
            {
                switch (id)
                {
                case 1202 :
                    loading=false;
                    printf ("The windows as been closed!!!");
                    break;
                }
            }
            break;
            }
            return false;
        }
        if ( event.EventType == EET_MOUSE_INPUT_EVENT )
        {
            mouseEvents(event);
        }
        return false;
    }
};
