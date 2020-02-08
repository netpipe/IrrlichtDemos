// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_KEYBOARD_H
#define GUI_MENU_KEYBOARD_H

#include "../gui_dialog.h"
#include <Keycodes.h>
#include <IEventReceiver.h>
#include <map>
#include <vector>
#include <string>

struct VKeyDesc
{
	VKeyDesc() : mChar(0), mKeyCode(irr::KEY_KEY_CODES_COUNT)
	{
	}

	VKeyDesc(const wchar_t* text, wchar_t character, irr::EKEY_CODE code)
	: mText(text), mChar(character), mKeyCode(code)
	{
	}

	std::wstring mText;			// To display
	wchar_t mChar;				// Used in an editbox
	irr::EKEY_CODE mKeyCode;	// Occasionally needed by the editbox
};

// Virtual keyboard to click in keys with mouse or touch-device
class GuiMenuKeyboard : public GuiDialog
{
public:
    GuiMenuKeyboard(const Config& config);
    ~GuiMenuKeyboard();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();
    virtual void Hide();

    bool OnEvent(const irr::SEvent &event);

protected:
    void MakeKbUS();

private:
	std::map<size_t, VKeyDesc> mKeys;
	std::map<irr::gui::IGUIElement*, size_t> mButtons;
	irr::gui::IGUIElement* mOldFocus;	// Should be the editbox which receives the input
};

#endif // GUI_MENU_KEYBOARD_H
