/*
Copyright (C) 2008  Lukas Kropatschek

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

#ifndef _KEYCODE_H_
#define _KEYCODE_H_

#include <string>

//tolua_begin
namespace peak
{

        enum Keycode
        {
        		EK_NONE				= 0x00,
                EK_LBUTTON          = 0x01,  // Left mouse button  
                EK_RBUTTON          = 0x02,  // Right mouse button  
                EK_CANCEL           = 0x03,  // Control-break processing  
                EK_MBUTTON          = 0x04,  // Middle mouse button (three-button mouse)  
                EK_XBUTTON1         = 0x05,  // Windows 2000/XP: X1 mouse button 
                EK_XBUTTON2         = 0x06,  // Windows 2000/XP: X2 mouse button 
                EK_BACK             = 0x08,  // BACKSPACE key  
                EK_TAB              = 0x09,  // TAB key  
                EK_CLEAR            = 0x0C,  // CLEAR key  
                EK_RETURN           = 0x0D,  // ENTER key  
                EK_SHIFT            = 0x10,  // SHIFT key  
                EK_CONTROL          = 0x11,  // CTRL key  
                EK_MENU             = 0x12,  // ALT key  
                EK_PAUSE            = 0x13,  // PAUSE key  
                EK_CAPITAL          = 0x14,  // CAPS LOCK key  
                EK_KANA             = 0x15,  // IME Kana mode 
                EK_HANGUEL          = 0x15,  // IME Hanguel mode (maintained for compatibility use EK_HANGUL) 
                EK_HANGUL           = 0x15,  // IME Hangul mode 
                EK_JUNJA            = 0x17,  // IME Junja mode 
                EK_FINAL            = 0x18,  // IME final mode 
                EK_HANJA            = 0x19,  // IME Hanja mode 
                EK_KANJI            = 0x19,  // IME Kanji mode 
                EK_ESCAPE           = 0x1B,  // ESC key  
                EK_CONVERT          = 0x1C,  // IME convert 
                EK_NONCONVERT       = 0x1D,  // IME nonconvert 
                EK_ACCEPT           = 0x1E,  // IME accept 
                EK_MODECHANGE       = 0x1F,  // IME mode change request 
                EK_SPACE            = 0x20,  // SPACEBAR  
                EK_PRIOR            = 0x21,  // PAGE UP key  
                EK_NEXT             = 0x22,  // PAGE DOWN key  
                EK_END              = 0x23,  // END key  
                EK_HOME             = 0x24,  // HOME key  
                EK_LEFT             = 0x25,  // LEFT ARROW key  
                EK_UP               = 0x26,  // UP ARROW key  
                EK_RIGHT            = 0x27,  // RIGHT ARROW key  
                EK_DOWN             = 0x28,  // DOWN ARROW key  
                EK_SELECT           = 0x29,  // SELECT key  
                EK_PRINT            = 0x2A,  // PRINT key
                EK_EXECUT           = 0x2B,  // EXECUTE key  
                EK_SNAPSHOT         = 0x2C,  // PRINT SCREEN key  
                EK_INSERT           = 0x2D,  // INS key  
                EK_DELETE           = 0x2E,  // DEL key  
                EK_HELP             = 0x2F,  // HELP key  
                EK_KEY_0            = 0x30,  // 0 key  
                EK_KEY_1            = 0x31,  // 1 key  
                EK_KEY_2            = 0x32,  // 2 key  
                EK_KEY_3            = 0x33,  // 3 key  
                EK_KEY_4            = 0x34,  // 4 key  
                EK_KEY_5            = 0x35,  // 5 key  
                EK_KEY_6            = 0x36,  // 6 key  
                EK_KEY_7            = 0x37,  // 7 key  
                EK_KEY_8            = 0x38,  // 8 key  
                EK_KEY_9            = 0x39,  // 9 key  
                EK_KEY_A            = 0x41,  // A key  
                EK_KEY_B            = 0x42,  // B key  
                EK_KEY_C            = 0x43,  // C key  
                EK_KEY_D            = 0x44,  // D key  
                EK_KEY_E            = 0x45,  // E key  
                EK_KEY_F            = 0x46,  // F key  
                EK_KEY_G            = 0x47,  // G key  
                EK_KEY_H            = 0x48,  // H key  
                EK_KEY_I            = 0x49,  // I key  
                EK_KEY_J            = 0x4A,  // J key  
                EK_KEY_K            = 0x4B,  // K key  
                EK_KEY_L            = 0x4C,  // L key  
                EK_KEY_M            = 0x4D,  // M key  
                EK_KEY_N            = 0x4E,  // N key  
                EK_KEY_O            = 0x4F,  // O key  
                EK_KEY_P            = 0x50,  // P key  
                EK_KEY_Q            = 0x51,  // Q key  
                EK_KEY_R            = 0x52,  // R key  
                EK_KEY_S            = 0x53,  // S key  
                EK_KEY_T            = 0x54,  // T key  
                EK_KEY_U            = 0x55,  // U key  
                EK_KEY_V            = 0x56,  // V key  
                EK_KEY_W            = 0x57,  // W key  
                EK_KEY_X            = 0x58,  // X key  
                EK_KEY_Y            = 0x59,  // Y key  
                EK_KEY_Z            = 0x5A,  // Z key  
                EK_LWIN             = 0x5B,  // Left Windows key (Microsoft Natural keyboard)  
                EK_RWIN             = 0x5C,  // Right Windows key (Natural keyboard)  
                EK_APPS             = 0x5D,  //Applications key (Natural keyboard)  
                EK_SLEEP            = 0x5F,  // Computer Sleep key 
                EK_NUMPAD0          = 0x60,  // Numeric keypad 0 key  
                EK_NUMPAD1          = 0x61,  // Numeric keypad 1 key  
                EK_NUMPAD2          = 0x62,  // Numeric keypad 2 key  
                EK_NUMPAD3          = 0x63,  // Numeric keypad 3 key  
                EK_NUMPAD4          = 0x64,  // Numeric keypad 4 key  
                EK_NUMPAD5          = 0x65,  // Numeric keypad 5 key  
                EK_NUMPAD6          = 0x66,  // Numeric keypad 6 key  
                EK_NUMPAD7          = 0x67,  // Numeric keypad 7 key  
                EK_NUMPAD8          = 0x68,  // Numeric keypad 8 key  
                EK_NUMPAD9          = 0x69,  // Numeric keypad 9 key  
                EK_MULTIPLY         = 0x6A,  // Multiply key  
                EK_ADD              = 0x6B,  // Add key  
                EK_SEPARATOR        = 0x6C,  // Separator key  
                EK_SUBTRACT         = 0x6D,  // Subtract key  
                EK_DECIMAL          = 0x6E,  // Decimal key  
                EK_DIVIDE           = 0x6F,  // Divide key  
                EK_F1               = 0x70,  // F1 key  
                EK_F2               = 0x71,  // F2 key  
                EK_F3               = 0x72,  // F3 key  
                EK_F4               = 0x73,  // F4 key  
                EK_F5               = 0x74,  // F5 key  
                EK_F6               = 0x75,  // F6 key  
                EK_F7               = 0x76,  // F7 key  
                EK_F8               = 0x77,  // F8 key  
                EK_F9               = 0x78,  // F9 key  
                EK_F10              = 0x79,  // F10 key  
                EK_F11              = 0x7A,  // F11 key  
                EK_F12              = 0x7B,  // F12 key  
                EK_F13              = 0x7C,  // F13 key  
                EK_F14              = 0x7D,  // F14 key  
                EK_F15              = 0x7E,  // F15 key  
                EK_F16              = 0x7F,  // F16 key  
                EK_F17              = 0x80,  // F17 key  
                EK_F18              = 0x81,  // F18 key  
                EK_F19              = 0x82,  // F19 key  
                EK_F20              = 0x83,  // F20 key  
                EK_F21              = 0x84,  // F21 key  
                EK_F22              = 0x85,  // F22 key  
                EK_F23              = 0x86,  // F23 key  
                EK_F24              = 0x87,  // F24 key  
                EK_NUMLOCK          = 0x90,  // NUM LOCK key  
                EK_SCROLL           = 0x91,  // SCROLL LOCK key  
                EK_LSHIFT           = 0xA0,  // Left SHIFT key 
                EK_RSHIFT           = 0xA1,  // Right SHIFT key 
                EK_LCONTROL         = 0xA2,  // Left CONTROL key 
                EK_RCONTROL         = 0xA3,  // Right CONTROL key 
                EK_LMENU            = 0xA4,  // Left MENU key 
                EK_RMENU            = 0xA5,  // Right MENU key 
                EK_PLUS             = 0xBB,  // Plus KEY   (+)
                EK_COMMA            = 0xBC,  // Comma KEY  (,)
                EK_MINUS            = 0xBD,  // Minus KEY  (-)
                EK_PERIOD           = 0xBE,  // Period KEY (.)
                EK_ATTN             = 0xF6,  // Attn key 
                EK_CRSEL            = 0xF7,  // CrSel key 
                EK_EXSEL            = 0xF8,  // ExSel key 
                EK_EREOF            = 0xF9,  // Erase EOF key 
                EK_PLAY             = 0xFA,  // Play key 
                EK_ZOOM             = 0xFB,  // Zoom key 
                EK_PA1              = 0xFD,  // PA1 key 
                EK_OEM_CLEAR        = 0xFE,   // Clear key 
 
                EK_COUNT  = 0xFF // this is not a key, but the amount of keycodes there are.
        };
 	
 	//tolua_end
 	struct KeyName
 	{
 		Keycode code;
 		const char *name;
 	};
 	extern KeyName keynames[];
 	//tolua_begin
 	
 	/**
 	 * \brief Translates a key name to a key code
 	 */
 	Keycode getKey(std::string name);
}
//tolua_end

#endif
