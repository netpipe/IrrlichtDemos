/*
    events.h

    Created on: Mar 17, 2010

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

#ifndef EVENTS_H_
#define EVENTS_H_
#include "globals.h"

// keystates array
extern bool key[KEY_KEY_CODES_COUNT];

// mouse position
extern f32 mouseX,mouseY;
// mouse buttons
extern bool leftMouse,rightMouse,middleMouse;

class Receiver : public IEventReceiver
{
public:
  virtual bool OnEvent(const SEvent& event);


private:

  vector2d<f32> cursorPos;

};

extern Receiver rcv;

// empty keystates
void clearKeys(void);


#endif /* EVENTS_H_ */
