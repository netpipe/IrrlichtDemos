/*
Copyright (C) 2008  Mathias Gottschlag

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

#include "graphic/SkyBox.h"
#include "graphic/GraphicsEngine.h"
#include "core/FileSystem.h"
#include "core/Logger.h"

#include <irrlicht.h>

namespace peak
{
	SkyBox::SkyBox(std::string top, std::string bottom, std::string left, std::string right,
		std::string front, std::string back, SceneNode *parent)
	{
		GraphicsEngine *engine = GraphicsEngine::get();
		std::string topfilename = FileSystem::get()->getFileExists(std::string("materials/") + top);
		std::string bottomfilename = FileSystem::get()->getFileExists(std::string("materials/") + bottom);
		std::string leftfilename = FileSystem::get()->getFileExists(std::string("materials/") + left);
		std::string rightfilename = FileSystem::get()->getFileExists(std::string("materials/") + right);
		std::string frontfilename = FileSystem::get()->getFileExists(std::string("materials/") + front);
		std::string backfilename = FileSystem::get()->getFileExists(std::string("materials/") + back);
		node = engine->getSceneManager()->addSkyBoxSceneNode(engine->getDriver()->getTexture(topfilename.c_str()),
			engine->getDriver()->getTexture(bottomfilename.c_str()),
			engine->getDriver()->getTexture(leftfilename.c_str()),
			engine->getDriver()->getTexture(rightfilename.c_str()),
			engine->getDriver()->getTexture(frontfilename.c_str()),
			engine->getDriver()->getTexture(backfilename.c_str()));
		node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	}
	SkyBox::~SkyBox()
	{
	}
}

