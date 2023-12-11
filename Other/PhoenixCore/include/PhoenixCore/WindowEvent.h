/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHWINDOWEVENT_H__
#define __PHWINDOWEVENT_H__

#include "config.h"
#include "Vector2d.h"
#include "Keys.h"

namespace phoenix{

//! Window Events Types
enum E_WINDOWEVENT_TYPE{
	WET_KEY,				//!< Keyboard events ( int_data = key, bool_data = state ).
	WET_CHAR,				//!< Keyboard character event ( int_data = char ).
	WET_MOUSE_DOWN,			//!< Mouse click down ( vector_data = position ).
	WET_MOUSE_UP,			//!< Mouse click released ( vector_data = position ).
	WET_MOUSE_POSITION,		//!< Mouse position change ( vector_data = position ).
	WET_MOUSE_WHEEL,		//!< Mouse wheel change ( int_data = position ).
	WET_CLOSE,				//!< Window close event.
	WET_UPDATE,				//!< Window update event.
	WET_RESIZE,				//!< Window Resize Event ( vector_data = new size ).
	WET_MOVE				//!< Window moving event ( bool_data = 0, stopped moving; bool_data = 1 started moving ). This event doesn't always fire ( See _GLFW_WM_MOVE_HACK ).
};

//! Window Event Data
struct WindowEvent{ 
	E_WINDOWEVENT_TYPE type;
	int int_data; //!< used for keys, etc.
	bool bool_data; //!< used for keystates. false for released, true for pressed
	Vector2d vector_data; //!< used for mouse position, window size, etc.
};

}//namespace phoenix

#endif //__PHWINDOWEVENT_H__