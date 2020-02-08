// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.
#include "gui_menu_keyboard.h"

#include "../main.h"
#include "../irrlicht_manager.h"
#include "../config.h"

#include <irrlicht.h>
#include <sstream>

#ifdef _IRR_ANDROID_PLATFORM_
#include "android_tools.h"
#endif

using namespace irr;
using namespace gui;

GuiMenuKeyboard::GuiMenuKeyboard(const Config& config)
: GuiDialog(config)
, mOldFocus(NULL)
{
	// We only have one keyboard so far.
	MakeKbUS();
}

GuiMenuKeyboard::~GuiMenuKeyboard()
{
}

bool GuiMenuKeyboard::Load(const char* filename_, bool reloadLast_)
{
	mButtons.clear();

    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
	{
        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

		std::string errorMsg("GuiMenuKeyboard::Load");
		size_t numButtons = 0;
		IGUIElement* button = GetElementByName(root, "key_0", errorMsg);
		while ( button )
		{
			std::map<size_t, VKeyDesc>::const_iterator it = mKeys.find(numButtons);
			if ( it != mKeys.end() )
			{
				button->setText(it->second.mText.c_str());
			}

			mButtons[button] = numButtons;
			++numButtons;

			std::ostringstream strstream;
			strstream << "key_" << numButtons;
			button = GetElementByName(root, strstream.str(), errorMsg);
		}
	}

	return ok;
}

void GuiMenuKeyboard::Show()
{
	mOldFocus = NULL;
	gui::IGUIEnvironment* env = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getGUIEnvironment();
	if ( env )
		mOldFocus = env->getFocus();

	GuiDialog::Show();

	if ( env )
		env->setFocus(mOldFocus);
}

void GuiMenuKeyboard::Hide()
{
	mOldFocus = NULL;	// Just to make sure we don't keep pointers around to elements which might be invalid some day
	GuiDialog::Hide();
}

bool GuiMenuKeyboard::OnEvent(const irr::SEvent &event)
{
	using namespace irr;
	using namespace gui;

	switch ( event.EventType )
	{
    	case EET_KEY_INPUT_EVENT:
    	{
			if ( !event.KeyInput.PressedDown )
            {
                switch(event.KeyInput.Key)
                {
                    case KEY_RETURN:
					{
#ifdef _IRR_ANDROID_PLATFORM_
						if ( GetConfig().GetVirtualKeyboard() == 0 )
							hc1::android::setSoftInputVisibility(APP.GetAndroidApp(), false);
						else if ( GetConfig().GetVirtualKeyboard() == 1 )
#endif
						if ( IsVisible() )
							Hide();
					}
					break;
					default:
					break;
				}
			}
		}
		break;
        case EET_GUI_EVENT:
        {
            gui::IGUIEnvironment* env = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getGUIEnvironment();
            if ( !env )
                break;
            switch((int)event.GUIEvent.EventType)
            {
                case EGET_ELEMENT_FOCUS_LOST:
                {
					if ( event.GUIEvent.Caller->getType() == irr::gui::EGUIET_EDIT_BOX )
					{
#ifdef _IRR_ANDROID_PLATFORM_
						if ( GetConfig().GetVirtualKeyboard() == 0 )
							hc1::android::setSoftInputVisibility(APP.GetAndroidApp(), false);
						else if ( GetConfig().GetVirtualKeyboard() == 1 )
#endif

						if ( IsVisible() )
						{
							std::map<irr::gui::IGUIElement*, size_t>::const_iterator it = mButtons.find(event.GUIEvent.Element);	// Element has the new focus
							if ( it != mButtons.end() )
							{
								mOldFocus = env->getFocus();
								break;
							}

							Hide();
						}
					}
                }
                break;
                case EGET_ELEMENT_FOCUSED:
                    if ( event.GUIEvent.Caller->getID() >= 0 )
					{
						if ( event.GUIEvent.Caller->getType() == irr::gui::EGUIET_EDIT_BOX )
						{
#ifdef _IRR_ANDROID_PLATFORM_
							if ( GetConfig().GetVirtualKeyboard() == 0 )
								hc1::android::setSoftInputVisibility(APP.GetAndroidApp(), true);
							else if ( GetConfig().GetVirtualKeyboard() == 1 )
#endif

							if ( !IsVisible() )
								Show();
						}
					}
                break;
                case EGET_BUTTON_CLICKED:
                {
                	// Note we get all button events here - not just those from this dialog. That's why we can get buttons here which are not found.
                	std::map<irr::gui::IGUIElement*, size_t>::const_iterator itButton = mButtons.find(event.GUIEvent.Caller);
                	if ( itButton == mButtons.end() )
						break;

					size_t index = itButton->second;
					std::map<size_t, VKeyDesc>::const_iterator it = mKeys.find(index);
					if ( it != mKeys.end() )
					{
						env->setFocus(mOldFocus);

						const VKeyDesc& desc = it->second;
						APP.GetIrrlichtManager()->SendKeyEvent( desc.mKeyCode, true, desc.mChar);
						APP.GetIrrlichtManager()->SendKeyEvent( desc.mKeyCode, false, desc.mChar);
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

void GuiMenuKeyboard::MakeKbUS()
{
	using namespace irr;
	mKeys[0] = VKeyDesc(L"!", L'!', KEY_KEY_1);
	mKeys[1] = VKeyDesc(L"?", L'?', KEY_OEM_2);
	mKeys[2] = VKeyDesc(L"-", L'-', KEY_MINUS);
	mKeys[3] = VKeyDesc(L"1", L'1', KEY_KEY_1);
	mKeys[4] = VKeyDesc(L"2", L'2', KEY_KEY_2);
	mKeys[5] = VKeyDesc(L"3", L'3', KEY_KEY_3);
	mKeys[6] = VKeyDesc(L"4", L'4', KEY_KEY_4);
	mKeys[7] = VKeyDesc(L"5", L'5', KEY_KEY_5);
	mKeys[8] = VKeyDesc(L" ", L' ', KEY_SPACE);
	mKeys[9] = VKeyDesc(L"6", L'6', KEY_KEY_6);
	mKeys[10] =VKeyDesc(L"7", L'7', KEY_KEY_7);
	mKeys[11] =VKeyDesc(L"8", L'8', KEY_KEY_8);
	mKeys[12] =VKeyDesc(L"9", L'9', KEY_KEY_9);
	mKeys[13] =VKeyDesc(L"0", L'0', KEY_KEY_0);
	mKeys[14] =VKeyDesc(L"BACK", 0x08, KEY_BACK);
	mKeys[15] =VKeyDesc(L"Q", L'Q', KEY_KEY_Q);
	mKeys[16] =VKeyDesc(L"W", L'W', KEY_KEY_W);
	mKeys[17] =VKeyDesc(L"E", L'E', KEY_KEY_E);
	mKeys[18] =VKeyDesc(L"r", L'r', KEY_KEY_R);
	mKeys[19] =VKeyDesc(L"T", L'T', KEY_KEY_T);
	mKeys[20] =VKeyDesc(L"Y", L'Y', KEY_KEY_Y);
	mKeys[21] =VKeyDesc(L"U", L'U', KEY_KEY_U);
	mKeys[22] =VKeyDesc(L"i", L'i', KEY_KEY_I);
	mKeys[23] =VKeyDesc(L"O", L'O', KEY_KEY_O);
	mKeys[24] =VKeyDesc(L"P", L'P', KEY_KEY_P);
	mKeys[25] =VKeyDesc(L"@", L'@', KEY_KEY_Q);
	mKeys[26] =VKeyDesc(L"A", L'A', KEY_KEY_A);
	mKeys[27] =VKeyDesc(L"S", L'S', KEY_KEY_S);
	mKeys[28] =VKeyDesc(L"D", L'D', KEY_KEY_D);
	mKeys[29] =VKeyDesc(L"F", L'F', KEY_KEY_F);
	mKeys[30] =VKeyDesc(L"G", L'G', KEY_KEY_G);
	mKeys[31] =VKeyDesc(L"H", L'H', KEY_KEY_H);
	mKeys[32] =VKeyDesc(L"J", L'J', KEY_KEY_J);
	mKeys[33] =VKeyDesc(L"K", L'K', KEY_KEY_K);
	mKeys[34] =VKeyDesc(L"L", L'L', KEY_KEY_L);
	mKeys[35] =VKeyDesc(L"#", L'#', KEY_OEM_2);
	mKeys[36] =VKeyDesc(L"", 0, KEY_LEFT);
	mKeys[37] =VKeyDesc(L"", 0, KEY_RIGHT);
	mKeys[38] =VKeyDesc(L"Z", L'Z', KEY_KEY_Z);
	mKeys[39] =VKeyDesc(L"X", L'X', KEY_KEY_X);
	mKeys[40] =VKeyDesc(L"C", L'C', KEY_KEY_C);
	mKeys[41] =VKeyDesc(L"V", L'V', KEY_KEY_V);
	mKeys[42] =VKeyDesc(L"B", L'B', KEY_KEY_B);
	mKeys[43] =VKeyDesc(L"N", L'N', KEY_KEY_N);
	mKeys[44] =VKeyDesc(L"M", L'M', KEY_KEY_M);
	mKeys[45] =VKeyDesc(L"enter", 0x13, KEY_RETURN);
}
