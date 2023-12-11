/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHOENIXKEYS__
#define __PHOENIXKEYS__

#include "config.h"

namespace phoenix
{

    //! Mouse button definitions
    /*!
    	Defines mouse buttons for window events
    	\sa EventReceiver
    */
    enum MouseButton
    {
        PHK_MB_LEFT     =   0,
        PHK_MB_RIGHT	=   1,
        PHK_MB_MIDDLE   =   2
    };

    //! Keyboard key definitions
    /*!
    	Defines keyboard keys for window events.
    	\sa EventReceiver
    */
    enum Key
    {
        PHK_A					='A',
        PHK_B					='B',
        PHK_C					='C',
        PHK_D					='D',
        PHK_E					='E',
        PHK_F					='F',
        PHK_G					='G',
        PHK_H					='H',
        PHK_I					='I',
        PHK_J					='J',
        PHK_K					='K',
        PHK_L					='L',
        PHK_M					='M',
        PHK_N					='N',
        PHK_O					='O',
        PHK_P					='P',
        PHK_Q					='Q',
        PHK_R					='R',
        PHK_S					='S',
        PHK_T					='T',
        PHK_U					='U',
        PHK_V					='V',
        PHK_W					='W',
        PHK_X					='X',
        PHK_Y					='Y',
        PHK_Z					='Z',
        PHK_0					='0',
        PHK_1					='1',
        PHK_2					='2',
        PHK_3					='3',
        PHK_4					='4',
        PHK_5					='5',
        PHK_6					='6',
        PHK_7					='7',
        PHK_8					='8',
        PHK_9					='9',
        PHK_BACKTICK            ='`',
		PHK_TILDE				='~',
		PHK_BANG				='!',
        PHK_RIGHTBRACKET        ='[',
        PHK_LEFTBRACKET         =']',
        PHK_SEMICOLON           =';',
        PHK_QUOTE               ='\'',
        PHK_BACKSLASH           ='\\',
		PHK_PIPE				='|',
        PHK_EQUAL               ='=',
		PHK_PLUS				='+',
        PHK_MINUS               ='-',
        PHK_SPACE				=32,
        PHK_ESC					=257,
        PHK_F1					=258,
        PHK_F2					=259,
        PHK_F3					=260,
        PHK_F4					=261,
        PHK_F5					=262,
        PHK_F6					=263,
        PHK_F7					=264,
        PHK_F8					=265,
        PHK_F9					=266,
        PHK_F10					=267,
        PHK_F11					=268,
        PHK_F12					=269,
        PHK_F13					=270,
        PHK_F14					=271,
        PHK_F15					=272,
        PHK_F16					=273,
        PHK_F17					=274,
        PHK_F18					=275,
        PHK_F19					=276,
        PHK_F20					=277,
        PHK_F21					=278,
        PHK_F22					=279,
        PHK_F23					=280,
        PHK_F24					=281,
        PHK_F25					=282,
        PHK_UP					=283,
        PHK_DOWN				=284,
        PHK_LEFT				=285,
        PHK_RIGHT				=286,
        PHK_LSHIFT				=287,
        PHK_RSHIFT				=288,
        PHK_LCTRL				=289,
        PHK_RCTRL				=290,
        PHK_LALT				=291,
        PHK_RALT				=292,
        PHK_TAB					=293,
        PHK_ENTER				=294,
        PHK_BACKSPACE			=295,
        PHK_INSERT				=296,
        PHK_DEL					=297,
        PHK_PAGEUP				=298,
        PHK_PAGEDOWN			=299,
        PHK_HOME				=300,
        PHK_END					=301,
        PHK_KP_0				=302,
        PHK_KP_1				=303,
        PHK_KP_2				=304,
        PHK_KP_3				=305,
        PHK_KP_4				=306,
        PHK_KP_5				=307,
        PHK_KP_6				=308,
        PHK_KP_7				=309,
        PHK_KP_8				=310,
        PHK_KP_9				=311,
        PHK_KP_ADD				=315,
        PHK_KP_SUBTRACT			=314,
        PHK_KP_DIVIDE			=312,
        PHK_KP_MULTIPLY			=313,
        PHK_KP_DECIMAL			=316,
        PHK_KP_EQUAL			=317,
        PHK_KP_ENTER			=318,
        PHK_a					='a',
        PHK_b					='b',
        PHK_c					='c',
        PHK_d					='d',
        PHK_e					='e',
        PHK_f					='f',
        PHK_g					='g',
        PHK_h					='h',
        PHK_i					='i',
        PHK_j					='j',
        PHK_k					='k',
        PHK_l					='l',
        PHK_m					='m',
        PHK_n					='n',
        PHK_o					='o',
        PHK_p					='p',
        PHK_q					='q',
        PHK_r					='r',
        PHK_s					='s',
        PHK_t					='t',
        PHK_u					='u',
        PHK_v					='v',
        PHK_w					='w',
        PHK_x					='x',
        PHK_y					='y',
        PHK_z					='z',
 
    };

} //namespace phoenix

#endif //__PHOENIXKEYS__
