
/*
Copyright (C) 2008   Mathias Gottschlag

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

#include "InputReceiver.h"

#include <gtkmm.h>

InputReceiver::InputReceiver()
{
	mx = -1;
	my = -1;
	state = 0;
}
InputReceiver::~InputReceiver()
{
}

void InputReceiver::injectMouseMovement(int x, int y, unsigned int state)
{
	this->state = state;
	int dx = x - mx;
	if (mx == -1) dx = 0;
	int dy = y - my;
	if (my == -1) dy = 0;
	mx = x;
	my = y;
	onMouseMoved(x, y, dx, dy);
}

bool InputReceiver::getMouseButton(int button)
{
	if (button == 1)
	{
		return state & GDK_BUTTON1_MASK;
	}
	else if (button == 2)
	{
		return state & GDK_BUTTON2_MASK;
	}
	else if (button == 3)
	{
		return state & GDK_BUTTON3_MASK;
	}
	else if (button == 4)
	{
		return state & GDK_BUTTON4_MASK;
	}
	else if (button == 5)
	{
		return state & GDK_BUTTON5_MASK;
	}
	else
	{
		return false;
	}
}

