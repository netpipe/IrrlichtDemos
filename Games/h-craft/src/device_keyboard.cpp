// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "device_keyboard.h"
#include "main.h"
#include "string_table.h"
#include <irrlicht.h>

using namespace irr;


DeviceKeyboard::DeviceKeyboard() : IInputDevice()
{
    Reset();
}

DeviceKeyboard::~DeviceKeyboard()
{
}

std::string DeviceKeyboard::GetTypeName() const
{
    return std::string("KEYBOARD"); // TODO: i18n
}

std::string DeviceKeyboard::GetDeviceName() const
{
    return GetTypeName();
}

InputDeviceType DeviceKeyboard::GetType() const
{
    return IDP_KEYBOARD;
}

void DeviceKeyboard::Update()
{
    // nothing to do - keys are set from external event receiver
}

int DeviceKeyboard::GetNumAxes() const
{
    return 0;
}

int DeviceKeyboard::GetNumButtons() const
{
    return NUM_KEYS_KEYBOARD;
}

std::string DeviceKeyboard::GetNameAxis(int axis_) const
{
    return std::string("");
}

std::string DeviceKeyboard::GetNameButton(int button_) const
{
    StringTable* st = APP.GetStringTable();

    irr::core::stringw strw(L"unknown");

    if ( st )
    {
        switch ( button_ )
        {
        case KEY_CANCEL:    strw = st->Get("Control-break"); break;
        case KEY_BACK:      strw = st->Get("BACKSPACE"); break;
        case KEY_TAB:       strw = st->Get("TAB"); break;
        case KEY_CLEAR:     strw = st->Get("CLEAR"); break;
        case KEY_RETURN:    strw = st->Get("ENTER"); break;
        case KEY_SHIFT:     strw = st->Get("SHIFT"); break;
        case KEY_CONTROL:   strw = st->Get("CTRL"); break;
        case KEY_MENU:      strw = st->Get("ALT"); break;
        case KEY_PAUSE:     strw = st->Get("PAUSE"); break;
        case KEY_CAPITAL:   strw = st->Get("CAPS LOCK"); break;
        case KEY_ESCAPE:    strw = st->Get("ESC key"); break;
        case KEY_SPACE:     strw = st->Get("SPACEBAR"); break;
        case KEY_PRIOR:     strw = st->Get("PAGE UP"); break;
        case KEY_NEXT:      strw = st->Get("PAGE DOWN"); break;
        case KEY_END:       strw = st->Get("END"); break;
        case KEY_HOME:      strw = st->Get("HOME"); break;
        case KEY_LEFT:      strw = st->Get("LEFT ARROW"); break;
        case KEY_UP:        strw = st->Get("UP ARROW"); break;
        case KEY_RIGHT:     strw = st->Get("RIGHT ARROW"); break;
        case KEY_DOWN:      strw = st->Get("DOWN ARROW"); break;
        case KEY_SELECT:    strw = st->Get("SELECT"); break;
        case KEY_PRINT:     strw = st->Get("PRINT"); break;
        case KEY_EXECUT:    strw = st->Get("EXECUTE"); break;
        case KEY_SNAPSHOT:  strw = st->Get("PRINT SCREEN"); break;
        case KEY_INSERT:    strw = st->Get("INS"); break;
        case KEY_DELETE:    strw = st->Get("DEL"); break;
        case KEY_HELP:      strw = st->Get("HELP"); break;
        case KEY_KEY_0:     strw = st->Get("0"); break;
        case KEY_KEY_1:     strw = st->Get("1"); break;
        case KEY_KEY_2:     strw = st->Get("2"); break;
        case KEY_KEY_3:     strw = st->Get("3"); break;
        case KEY_KEY_4:     strw = st->Get("4"); break;
        case KEY_KEY_5:     strw = st->Get("5"); break;
        case KEY_KEY_6:     strw = st->Get("6"); break;
        case KEY_KEY_7:     strw = st->Get("7"); break;
        case KEY_KEY_8:     strw = st->Get("8"); break;
        case KEY_KEY_9:     strw = st->Get("9"); break;
        case KEY_KEY_A:     strw = st->Get("A"); break;
        case KEY_KEY_B:     strw = st->Get("B"); break;
        case KEY_KEY_C:     strw = st->Get("C"); break;
        case KEY_KEY_D:     strw = st->Get("D"); break;
        case KEY_KEY_E:     strw = st->Get("E"); break;
        case KEY_KEY_F:     strw = st->Get("F"); break;
        case KEY_KEY_G:     strw = st->Get("G"); break;
        case KEY_KEY_H:     strw = st->Get("H"); break;
        case KEY_KEY_I:     strw = st->Get("I"); break;
        case KEY_KEY_J:     strw = st->Get("J"); break;
        case KEY_KEY_K:     strw = st->Get("K"); break;
        case KEY_KEY_L:     strw = st->Get("L"); break;
        case KEY_KEY_M:     strw = st->Get("M"); break;
        case KEY_KEY_N:     strw = st->Get("N"); break;
        case KEY_KEY_O:     strw = st->Get("O"); break;
        case KEY_KEY_P:     strw = st->Get("P"); break;
        case KEY_KEY_Q:     strw = st->Get("Q"); break;
        case KEY_KEY_R:     strw = st->Get("R"); break;
        case KEY_KEY_S:     strw = st->Get("S"); break;
        case KEY_KEY_T:     strw = st->Get("T"); break;
        case KEY_KEY_U:     strw = st->Get("U"); break;
        case KEY_KEY_V:     strw = st->Get("V"); break;
        case KEY_KEY_W:     strw = st->Get("W"); break;
        case KEY_KEY_X:     strw = st->Get("X"); break;
        case KEY_KEY_Y:     strw = st->Get("Y"); break;
        case KEY_KEY_Z:     strw = st->Get("Z"); break;
        case KEY_LWIN:      strw = st->Get("Left Windows"); break;
        case KEY_RWIN:      strw = st->Get("Right Windows"); break;
        case KEY_APPS:      strw = st->Get("Applications"); break;
        case KEY_SLEEP:     strw = st->Get("Sleep"); break;
        case KEY_NUMPAD0:   strw = st->Get("Num 0"); break;
        case KEY_NUMPAD1:   strw = st->Get("Num 1"); break;
        case KEY_NUMPAD2:   strw = st->Get("Num 2"); break;
        case KEY_NUMPAD3:   strw = st->Get("Num 3"); break;
        case KEY_NUMPAD4:   strw = st->Get("Num 4"); break;
        case KEY_NUMPAD5:   strw = st->Get("Num 5"); break;
        case KEY_NUMPAD6:   strw = st->Get("Num 6"); break;
        case KEY_NUMPAD7:   strw = st->Get("Num 7"); break;
        case KEY_NUMPAD8:   strw = st->Get("Num 8"); break;
        case KEY_NUMPAD9:   strw = st->Get("Num 9"); break;
        case KEY_MULTIPLY:  strw = st->Get("Multiply"); break;
        case KEY_ADD:       strw = st->Get("Add"); break;
        case KEY_SEPARATOR: strw = st->Get("Separator"); break;
        case KEY_SUBTRACT:  strw = st->Get("Subtract"); break;
        case KEY_DECIMAL:   strw = st->Get("Decimal"); break;
        case KEY_DIVIDE:    strw = st->Get("Divide"); break;
        case KEY_F1:        strw = st->Get("F1"); break;
        case KEY_F2:        strw = st->Get("F2"); break;
        case KEY_F3:        strw = st->Get("F3"); break;
        case KEY_F4:        strw = st->Get("F4"); break;
        case KEY_F5:        strw = st->Get("F5"); break;
        case KEY_F6:        strw = st->Get("F6"); break;
        case KEY_F7:        strw = st->Get("F7"); break;
        case KEY_F8:        strw = st->Get("F8"); break;
        case KEY_F9:        strw = st->Get("F9"); break;
        case KEY_F10:       strw = st->Get("F10"); break;
        case KEY_F11:       strw = st->Get("F11"); break;
        case KEY_F12:       strw = st->Get("F12"); break;
        case KEY_NUMLOCK:   strw = st->Get("NUM LOCK"); break;
        case KEY_SCROLL:    strw = st->Get("SCROLL LOCK"); break;
        case KEY_LSHIFT:    strw = st->Get("Left SHIFT"); break;
        case KEY_RSHIFT:    strw = st->Get("Right SHIFT"); break;
        case KEY_LCONTROL:  strw = st->Get("Left CONTROL"); break;
        case KEY_RCONTROL:  strw = st->Get("Right CONTROL"); break;
        case KEY_LMENU:     strw = st->Get("Left MENU"); break;
        case KEY_RMENU:     strw = st->Get("Right MENU"); break;
        case KEY_COMMA:     strw = st->Get(","); break;
        case KEY_PLUS:      strw = st->Get("+"); break;
        case KEY_MINUS:     strw = st->Get("-"); break;
        case KEY_PERIOD:    strw = st->Get("."); break;
        case KEY_ATTN:      strw = st->Get("Attn"); break;
        case KEY_CRSEL:     strw = st->Get("CrSel"); break;
        case KEY_EXSEL:     strw = st->Get("ExSel"); break;
        case KEY_EREOF:     strw = st->Get("Erase EOF"); break;
        case KEY_PLAY:      strw = st->Get("Play"); break;
        case KEY_ZOOM:      strw = st->Get("Zoom"); break;
        case KEY_PA1:       strw = st->Get("PA1"); break;
        case KEY_OEM_CLEAR: strw = st->Get("Clear"); break;
        default:
            strw = st->Get("unknown key"); break;
        }
    }

    std::wstring wstr(strw.c_str());
    std::string str(wstr.begin(), wstr.end());
    return str;
}

bool DeviceKeyboard::IsAnyButtonDown(int & button_) const
{
    if ( !IsEnabled() )
        return false;

    for ( int i=0; i < NUM_KEYS_KEYBOARD; ++i )
    {
        if ( mKeyState[i] )
        {
            button_ = i;
            return true;
        }
    }

    return false;
}

float DeviceKeyboard::IsAnyAxisMoved(int & axis_) const
{
    return 0.f;
}

bool DeviceKeyboard::IsButtonDown(int button_) const
{
    if ( !IsEnabled() )
        return false;

    if ( button_ >= NUM_KEYS_KEYBOARD )
        return false;
    return mKeyState[button_] ? true : false;
}

float DeviceKeyboard::GetAxisValue(int axis_) const
{
    return 0.f;
}

void DeviceKeyboard::Reset()
{
    for ( int i=0; i < NUM_KEYS_KEYBOARD; ++i )
        mKeyState[i] = 0;
}

void DeviceKeyboard::SetKeyDown(int key_)
{
    if ( key_ >= 0 && key_ < NUM_KEYS_KEYBOARD )
        mKeyState[key_] = 1;
}

void DeviceKeyboard::SetKeyUp(int key_)
{
    if ( key_ >= 0 && key_ < NUM_KEYS_KEYBOARD )
        mKeyState[key_] = 0;
}
