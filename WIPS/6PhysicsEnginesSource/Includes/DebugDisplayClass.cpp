/*
Copyright (c) 2008 Nils Daumann

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "DebugDisplayClass.h"

DebugDisplayClass::DebugDisplayClass(IrrlichtDevice *device_)
{
	device = device_;

	statictext = device->getGUIEnvironment()->addStaticText(NULL, core::rect<s32>(0,0,500,500));

	lastfps = 0;

	screenshotnum = 0;

	setDebugInt("Screenshots: ", &screenshotnum);
}

void DebugDisplayClass::updateDebugDisplay()
{
	currentfps = device->getVideoDriver()->getFPS();
	if(lastfps != currentfps)
	{
		core::stringw str = L"FPS: ";
		str += currentfps;
		str += "\n";
		str += "Triangles: ";
		str += device->getVideoDriver()->getPrimitiveCountDrawn();
		str += "\n";
		str += debugname;
		str += *debugint;
		statictext->setText(str.c_str());
	}
}

void DebugDisplayClass::setDebugInt(const c8 *name, u32 *debug)
{
	debugname = name;
	debugint = debug;
}

void DebugDisplayClass::takeScreenshot()
{
	core::stringc str = "Sceenshot_";
	str += screenshotnum;
	str += ".jpg";

	screenshotnum += 1;

	video::IImage *screenshot = device->getVideoDriver()->createScreenShot();
	device->getVideoDriver()->writeImageToFile(screenshot,str.c_str());
}
