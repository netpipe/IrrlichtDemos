/*
Copyright (C) 2008   Lukas Kropatschek

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "core/Keycode.h"

namespace peak
{
 	KeyName keynames[] =
 	{
		{EK_LBUTTON, "LBUTTON"},
		{EK_RBUTTON, "RBUTTON"},
		{EK_CANCEL, "CANCEL"},
		{EK_MBUTTON, "MBUTTON"},
		{EK_XBUTTON1, "XBUTTON1"},
		{EK_XBUTTON2, "XBUTTON2"},
		{EK_BACK, "BACK"},
		{EK_TAB, "TAB"},
		{EK_CLEAR, "CLEAR"},
		{EK_RETURN, "RETURN"},
		{EK_SHIFT, "SHIFT"},
		{EK_CONTROL, "CONTROL"},
		{EK_MENU, "MENU"},
		{EK_PAUSE, "PAUSE"},
		{EK_CAPITAL, "CAPITAL"},
		{EK_KANA, "KANA"},
		{EK_HANGUEL, "HANGUEL"},
		{EK_HANGUL, "HANGUL"},
		{EK_JUNJA, "JUNJA"},
		{EK_FINAL, "FINAL"},
		{EK_HANJA, "HANJA"},
		{EK_KANJI, "KANJI"},
		{EK_ESCAPE, "ESCAPE"},
		{EK_CONVERT, "CONVERT"},
		{EK_NONCONVERT, "NONCONVERT"},
		{EK_ACCEPT, "ACCEPT"},
		{EK_MODECHANGE, "MODECHANGE"},
		{EK_SPACE, "SPACE"},
		{EK_PRIOR, "PRIOR"},
		{EK_NEXT, "NEXT"},
		{EK_END, "END"},
		{EK_HOME, "HOME"},
		{EK_LEFT, "LEFT"},
		{EK_UP, "UP"},
		{EK_RIGHT, "RIGHT"},
		{EK_DOWN, "DOWN"},
		{EK_SELECT, "SELECT"},
		{EK_PRINT, "PRINT"},
		{EK_EXECUT, "EXECUT"},
		{EK_SNAPSHOT, "SNAPSHOT"},
		{EK_INSERT, "INSERT"},
		{EK_DELETE, "DELETE"},
		{EK_HELP, "HELP"},
		{EK_KEY_0, "KEY_0"},
		{EK_KEY_1, "KEY_1"},
		{EK_KEY_2, "KEY_2"},
		{EK_KEY_3, "KEY_3"},
		{EK_KEY_4, "KEY_4"},
		{EK_KEY_5, "KEY_5"},
		{EK_KEY_6, "KEY_6"},
		{EK_KEY_7, "KEY_7"},
		{EK_KEY_8, "KEY_8"},
		{EK_KEY_9, "KEY_9"},
		{EK_KEY_A, "KEY_A"},
		{EK_KEY_B, "KEY_B"},
		{EK_KEY_C, "KEY_C"},
		{EK_KEY_D, "KEY_D"},
		{EK_KEY_E, "KEY_E"},
		{EK_KEY_F, "KEY_F"},
		{EK_KEY_G, "KEY_G"},
		{EK_KEY_H, "KEY_H"},
		{EK_KEY_I, "KEY_I"},
		{EK_KEY_J, "KEY_J"},
		{EK_KEY_K, "KEY_K"},
		{EK_KEY_L, "KEY_L"},
		{EK_KEY_M, "KEY_M"},
		{EK_KEY_N, "KEY_N"},
		{EK_KEY_O, "KEY_O"},
		{EK_KEY_P, "KEY_P"},
		{EK_KEY_Q, "KEY_Q"},
		{EK_KEY_R, "KEY_R"},
		{EK_KEY_S, "KEY_S"},
		{EK_KEY_T, "KEY_T"},
		{EK_KEY_U, "KEY_U"},
		{EK_KEY_V, "KEY_V"},
		{EK_KEY_W, "KEY_W"},
		{EK_KEY_X, "KEY_X"},
		{EK_KEY_Y, "KEY_Y"},
		{EK_KEY_Z, "KEY_Z"},
		{EK_LWIN, "LWIN"},
		{EK_RWIN, "RWIN"},
		{EK_APPS, "APPS"},
		{EK_SLEEP, "SLEEP"},
		{EK_NUMPAD0, "NUMPAD0"},
		{EK_NUMPAD1, "NUMPAD1"},
		{EK_NUMPAD2, "NUMPAD2"},
		{EK_NUMPAD3, "NUMPAD3"},
		{EK_NUMPAD4, "NUMPAD4"},
		{EK_NUMPAD5, "NUMPAD5"},
		{EK_NUMPAD6, "NUMPAD6"},
		{EK_NUMPAD7, "NUMPAD7"},
		{EK_NUMPAD8, "NUMPAD8"},
		{EK_NUMPAD9, "NUMPAD9"},
		{EK_MULTIPLY, "MULTIPLY"},
		{EK_ADD, "ADD"},
		{EK_SEPARATOR, "SEPARATOR"},
		{EK_SUBTRACT, "SUBTRACT"},
		{EK_DECIMAL, "DECIMAL"},
		{EK_DIVIDE, "DIVIDE"},
		{EK_F1, "F1"},
		{EK_F2, "F2"},
		{EK_F3, "F3"},
		{EK_F4, "F4"},
		{EK_F5, "F5"},
		{EK_F6, "F6"},
		{EK_F7, "F7"},
		{EK_F8, "F8"},
		{EK_F9, "F9"},
		{EK_F10, "F10"},
		{EK_F11, "F11"},
		{EK_F12, "F12"},
		{EK_F13, "F13"},
		{EK_F14, "F14"},
		{EK_F15, "F15"},
		{EK_F16, "F16"},
		{EK_F17, "F17"},
		{EK_F18, "F18"},
		{EK_F19, "F19"},
		{EK_F20, "F20"},
		{EK_F21, "F21"},
		{EK_F22, "F22"},
		{EK_F23, "F23"},
		{EK_F24, "F24"},
		{EK_NUMLOCK, "NUMLOCK"},
		{EK_SCROLL, "SCROLL"},
		{EK_LSHIFT, "LSHIFT"},
		{EK_RSHIFT, "RSHIFT"},
		{EK_LCONTROL, "LCONTROL"},
		{EK_RCONTROL, "RCONTROL"},
		{EK_LMENU, "LMENU"},
		{EK_RMENU, "RMENU"},
		{EK_PLUS, "PLUS"},
		{EK_COMMA, "COMMA"},
		{EK_MINUS, "MINUS"},
		{EK_PERIOD, "PERIOD"},
		{EK_ATTN, "ATTN"},
		{EK_CRSEL, "CRSEL"},
		{EK_EXSEL, "EXSEL"},
		{EK_EREOF, "EREOF"},
		{EK_PLAY, "PLAY"},
		{EK_ZOOM, "ZOOM"},
		{EK_PA1, "PA1"},
		{EK_OEM_CLEAR, "OEM_CLEAR"},
		{EK_NONE, ""}
 	};
 	
 	Keycode getKey(std::string name)
 	{
 		int i = 0;
 		while (keynames[i].code != EK_NONE)
 		{
 			if (name == keynames[i].name)
 			{
  				return keynames[i].code;
 			}
 			i++;
 		}
 		return (Keycode)0;
 	}
}

