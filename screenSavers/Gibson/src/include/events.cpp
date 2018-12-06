/*
 * events.cpp

    Copyright © 2010 John Serafino
    This file is part of ray3d.

    Ray3d is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Ray3d v0.01 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ray3d If not, see <http://www.gnu.org/licenses/>.
 */

#include "events.h"


// keystates array
bool key[KEY_KEY_CODES_COUNT];

// mouse position
f32 mouseX,mouseY;
// mouse buttons
bool leftMouse,rightMouse,middleMouse;

Receiver rcv;

bool Receiver::OnEvent(const SEvent& event)
{
  if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
	  switch(event.MouseInput.Event)
	  {
	    case EMIE_LMOUSE_PRESSED_DOWN:
	      leftMouse = true;
		  break;

		case EMIE_LMOUSE_LEFT_UP:
		  leftMouse = false;
		  break;

	    case EMIE_RMOUSE_PRESSED_DOWN:
	      rightMouse = true;
		  break;

		case EMIE_RMOUSE_LEFT_UP:
		  rightMouse = false;
		  break;

	    case EMIE_MMOUSE_PRESSED_DOWN:
	      rightMouse = true;
		  break;

		case EMIE_MMOUSE_LEFT_UP:
		  rightMouse = false;
		  break;

      case EMIE_MOUSE_MOVED:
        cursorPos = irrlicht->getCursorControl()->getRelativePosition();
        mouseX = cursorPos.X;
	      mouseY = cursorPos.Y;
	      break;

      default:
	        break;
      }
    }

	  if (event.EventType == EET_KEY_INPUT_EVENT)
	    key[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
  }

// empty keystates
void clearKeys(void)
{
  int i;
  for(i=0; i < KEY_KEY_CODES_COUNT; ++i)
    key[i]=false;
}
