/*
Copyright (C) 2008   Lukas Kropatschek, Mathias Gottschlag

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

#include "graphic/GraphicsEngine.h"
#include "core/SettingsManager.h"
#include "graphic/irr/InputReceiver.h"
#include "graphic/OverlayManager.h"
#include "graphic/CameraSceneNode.h"
#include "graphic/SceneNodeController.h"

#include <irrlicht.h>

#include <cstdlib>

using namespace irr;

namespace peak
{

	GraphicsEngine::GraphicsEngine()
	{
		lastfps = 0;
	}

	GraphicsEngine* GraphicsEngine::get(void)
	{
		static GraphicsEngine graphics;
		return &graphics;
	}

	bool GraphicsEngine::init(void)
	{
		SettingsManager *conf = SettingsManager::get();

		irr::SIrrlichtCreationParameters param;

		param.AntiAlias = conf->getBool("device.antialias");
		param.Bits = conf->getInt("device.bits");
		param.Fullscreen = conf->getBool("window.fullscreen");
		param.Stencilbuffer = conf->getBool("device.stencil");
		param.Vsync = conf->getBool("device.vsync");
		irr::core::dimension2d<int> ws;
		ws.Height = conf->getVector2DI("window.resolution").y;
		ws.Width = conf->getVector2DI("window.resolution").x;
		param.WindowSize = ws;
		std::string drv = conf->getString("device.driver");
		if (drv == "OpenGL" || drv == "opengl" || drv == "OpenGl"
				|| drv == "OPENGL")
		{
			param.DriverType = irr::video::EDT_OPENGL;
		}
		if (drv == "Direct3D8" || drv == "direct3d8" ||
				drv == "Direct3d8" || drv == "d3d8" || drv == "DIRECT3D8")
		{
			param.DriverType = irr::video::EDT_DIRECT3D8;
		}
		if (drv == "Direct3D9" || drv == "direct3d9" ||
				drv == "Direct3d9" || drv == "d3d9" || drv == "DIRECT3D9")
		{
			param.DriverType = irr::video::EDT_DIRECT3D9;
		}
		if (drv == "Software" || drv == "software" ||
				drv == "SOFTWARE")
		{
			param.DriverType = irr::video::EDT_DIRECT3D8;
		}

		device = createDeviceEx(param);
		if (!device) return false;
		caption = conf->getWString("window.title");
		device->setWindowCaption(caption.c_str());

		driver = device->getVideoDriver();
		scenemgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();
		device->setEventReceiver(InputReceiver::get());

		//scene::ICameraSceneNode *camera = scenemgr->addCameraSceneNode();
		//camera->setPosition(core::vector3df(0, 0, -30));
		stdcam = new CameraSceneNode();
		stdcam->setPosition(Vector3D(0, 0, -30));

		scenemgr->setAmbientLight(video::SColor(128, 128, 128, 255));

		return OverlayManager::get()->init();
	}

	bool GraphicsEngine::shutdown(void)
	{
		delete stdcam;
		device->drop();
		return false;
	}

	bool GraphicsEngine::renderFrame(float msecs)
	{
		SceneNodeController::doAllWork(msecs);
		CameraSceneNode::updateAllCameras();
		driver->beginScene(true, true, video::SColor(255,0,0,0));
		scenemgr->drawAll();

		// Reset transformation for 2D drawing
		core::matrix4 worldmatrix;
		worldmatrix.setTranslation(core::vector3df(-0.5, -0.5, 0));
		driver->setTransform(video::ETS_WORLD, worldmatrix);
		core::matrix4 viewmatrix;
		viewmatrix.buildProjectionMatrixOrthoLH(1, -1, 1, -1);

		driver->setTransform(video::ETS_PROJECTION, viewmatrix);
		driver->setTransform(video::ETS_VIEW, core::matrix4());
		OverlayManager::get()->render();

		guienv->drawAll();
		driver->endScene();

		// Get FPS
		int fps = driver->getFPS();
		if (lastfps != fps)
		{
			core::stringw str = caption.c_str();
			str += L", FPS: ";
			str += fps;
			str += ", Tris: ";
			str += driver->getPrimitiveCountDrawn();

			device->setWindowCaption(str.c_str());
			lastfps = fps;
		}

		return device->run();
	}

	CameraSceneNode *GraphicsEngine::getStandardCamera(void)
	{
		return stdcam;
	}

	irr::scene::ISceneManager *GraphicsEngine::getSceneManager(void)
	{
		return scenemgr;
	}

	irr::video::IVideoDriver *GraphicsEngine::getDriver(void)
	{
		return driver;
	}

	irr::gui::IGUIEnvironment *GraphicsEngine::getGUI(void)
	{
		return guienv;
	}

	irr::gui::ICursorControl *GraphicsEngine::getCursorControl(void)
	{
		return device->getCursorControl();
	}
}
