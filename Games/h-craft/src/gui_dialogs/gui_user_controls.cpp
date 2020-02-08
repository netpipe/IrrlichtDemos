// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_user_controls.h"
#include "../gui_ids.h"
#include "../input_device_manager.h"
#include "../controller.h"
#include "../device_mouse.h"
#include "../device_joystick.h"
#include "../main.h"

using namespace irr;
using namespace gui;


GuiUserControls::GuiUserControls(gui::IGUIEnvironment* env_)
: mEnvironment(env_)
, wndRectUserControls(50,50,590,470)
{
    wndUserControls = NULL;
}

GuiUserControls::~GuiUserControls()
{
}

void GuiUserControls::ClearWndUserControls()
{
    if ( !wndUserControls )
        return;

    const core::list<IGUIElement*>&childrenOrig = wndUserControls->getChildren();
    core::list<IGUIElement*>childrenCopy;

    IGUIButton* closeButton = wndUserControls->getCloseButton();
    IGUIButton* maxButton = wndUserControls->getMaximizeButton();
    IGUIButton* minButton = wndUserControls->getMinimizeButton();

    // ugly HACK, but core::list just sucks
    core::list<IGUIElement*>::ConstIterator it = childrenOrig.begin();
    for (; it != childrenOrig.end(); ++it)
    {
        if (    *it != closeButton
            &&  *it != maxButton
            &&  *it != minButton )
        childrenCopy.push_back(*it);
    }

    core::list<IGUIElement*>::Iterator itCopy = childrenCopy.begin();
    for (; itCopy != childrenCopy.end(); ++itCopy)
    {
        wndUserControls->removeChild(*itCopy);
    }
}

void GuiUserControls::ShowUserControlTabs()
{
    if ( !wndUserControls )
        return;
    ClearWndUserControls();

	// create tab control and tabs
	core::rect<s32> tabRect(2,20,wndRectUserControls.getWidth()-4, wndRectUserControls.getHeight()-22);
	IGUITabControl* tab = mEnvironment->addTabControl(tabRect, wndUserControls , true, true);

    // devices
    IGUITab* tabDevices = tab->addTab(L"Devices");

    s32 leftText = 10;
    s32 rightText = leftText + 200;
    s32 leftScroll = rightText + 10;
    s32 rightScroll = leftScroll + 200;
    s32 top = 10;

    DeviceMouse * mouse = APP.GetInputDeviceManager()->GetMouse();
    if ( mouse )
    {
        mEnvironment->addStaticText(L"Mouse changerate", core::rect<s32>(leftText, top, rightText, top+15), false, false, tabDevices);

        IGUIScrollBar * sb = mEnvironment->addScrollBar(true, core::rect<s32>(leftScroll,top,rightScroll,top+15), tabDevices, SCROLL_MOUSE_MAX_CHANGE);
        top += 20;
        sb->setMax(10);
        sb->setPos((int)(mouse->mSettings.mMaxChange*100.f));
    }

    for ( size_t i = 0; i < APP.GetInputDeviceManager()->GetNumJoysticks(); ++i )
    {
        const IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(i);

        mEnvironment->addStaticText(L"Joystick dead area", core::rect<s32>(leftText, top, rightText, top+15), false, false, tabDevices);
        IGUIScrollBar * sb = mEnvironment->addScrollBar(true, core::rect<s32>(leftScroll,top,rightScroll,top+15), tabDevices, SCROLL_JS_DEAD_AREA+i);
        top += 20;
        sb->setMax(100);
        sb->setPos( (int)(((float)joystick->mSettings.mDeadArea/(float)MAX_JOYSTICK_AREA)*100.f) );

        mEnvironment->addStaticText(L"Joystick max area", core::rect<s32>(leftText, top, rightText, top+15), false, false, tabDevices);
        sb = mEnvironment->addScrollBar(true, core::rect<s32>(leftScroll,top,rightScroll,top+15), tabDevices, SCROLL_JS_MAX_AREA+i);
        top += 20;
        sb->setMax(100);
        sb->setPos( (int)(((float)joystick->mSettings.mMaxArea/(float)MAX_JOYSTICK_AREA)*100.f) );

        mEnvironment->addStaticText(L"calibrate joystick", core::rect<s32>(leftText, top, rightText, top+15), false, false, tabDevices);
        mEnvironment->addButton(core::rect<s32>(leftScroll,top,rightScroll,top+15), tabDevices, BTN_CALIBRATE_JOYSTICK+i, L"calibrate");
        top += 20;
    }
}

void GuiUserControls::ShowWaitForUserAxis()
{
    if ( !wndUserControls )
        return;
    ClearWndUserControls();
}

void GuiUserControls::ShowWaitForUserButton()
{
    if ( !wndUserControls )
        return;
    ClearWndUserControls();
}

void GuiUserControls::CreateUserControls()
{
    if ( !mEnvironment )
        return;

	// remove tool box if already there
	IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_WND_USER_CONTROLS, true);
	if (e)
	{
        e->remove();
        wndUserControls = NULL;
    }

	// create the window
	wndUserControls = mEnvironment->addWindow(wndRectUserControls, false, L"User Controls", 0, GUI_WND_USER_CONTROLS);

    ShowUserControlTabs();
}

bool GuiUserControls::OnEvent(const SEvent &event)
{
    if ( !mEnvironment || !wndUserControls )
        return false;

    switch ( event.EventType )
    {
        case EET_GUI_EVENT:
        {
            s32 id = event.GUIEvent.Caller->getID();

            switch(event.GUIEvent.EventType)
            {
                case EGET_BUTTON_CLICKED:
                {
                    switch(id)
                    {
                        case BTN_CALIBRATE_JOYSTICK:
                        case BTN_CALIBRATE_JOYSTICK + 1:
                        case BTN_CALIBRATE_JOYSTICK + 2:
                        case BTN_CALIBRATE_JOYSTICK + 3:
                        {
                            IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(id-BTN_CALIBRATE_JOYSTICK);
                            if ( joystick )
                            {
                                joystick->CalibrateCenter();
                            }
                        }
                        return true;
                    }
                }
                break;
                case EGET_SCROLL_BAR_CHANGED:
                {
                    IGUIScrollBar * sb = static_cast<IGUIScrollBar*>(event.GUIEvent.Caller);
                    if ( !sb )
                        break;
                    int pos = sb->getPos();
                    switch ( id )
                    {
                        case SCROLL_MOUSE_MAX_CHANGE:
                        {
                            DeviceMouse* mouse = APP.GetInputDeviceManager()->GetMouse();
                            if ( !mouse )
                                break;
                            mouse->mSettings.mMaxChange = (float)pos / 100.f;
                        }
                        break;

                        case SCROLL_JS_DEAD_AREA:
                        case SCROLL_JS_DEAD_AREA+1:
                        case SCROLL_JS_DEAD_AREA+2:
                        case SCROLL_JS_DEAD_AREA+3:
                        {
                            IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(id-SCROLL_JS_DEAD_AREA);
                            if ( !joystick )
                                break;
                            joystick->mSettings.mDeadArea = (int)((float)MAX_JOYSTICK_AREA*(float)pos/100.f);
                        }
                        break;
                        case SCROLL_JS_MAX_AREA:
                        case SCROLL_JS_MAX_AREA+1:
                        case SCROLL_JS_MAX_AREA+2:
                        case SCROLL_JS_MAX_AREA+3:
                        {
                            IDeviceJoystick* joystick = APP.GetInputDeviceManager()->GetJoystick(id-SCROLL_JS_DEAD_AREA);
                            if ( !joystick )
                                break;
                            joystick->mSettings.mMaxArea = (int)((float)MAX_JOYSTICK_AREA*(float)pos/100.f);
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
        default:
        break;
    }

    return false;
}

