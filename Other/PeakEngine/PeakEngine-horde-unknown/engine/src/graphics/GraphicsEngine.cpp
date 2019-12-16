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

#include "graphics/GraphicsEngine.h"
#include "core/SettingsManager.h"
#include "core/Logger.h"
#include "core/GameEngine.h"
#include "core/InputManager.h"
#include "graphics/OverlayManager.h"
#include "graphics/CameraSceneNode.h"
#include "graphics/SceneNodeController.h"
#include "graphics/ParticleSystem.h"
#include "menu/MenuManager.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <guichan.hpp>

#include <FreeImagePlus.h>

namespace peak
{
	// TODO: The input code is incredibly ugly and should maybe be rewritten

	// Input data
	int oldx = 0;
	int oldy = 0;
	int mousex = 0;
	int mousey = 0;
	bool keys[300];
	Vector2DI screensize;

	float debugview = 0.0f;

	bool shiftpressed = false;
	bool controlpressed = false;

	bool mousecaptured = false;

	static void GLFWCALL mouseMoveListener(int x, int y)
	{
		if (!mousecaptured) return;

		int dx = x - oldx;
		int dy = y - oldy;

		InputManager * inputmgr = InputManager::get();
		if (MenuManager::get()->getCurrentMenu())
		{
			// Pass mouse movement to menu
			mousex += dx;
			if (mousex < 0) mousex = 0;
			if (mousex >= screensize.x) mousex = screensize.x;
			mousey += dy;
			if (mousey < 0) mousey = 0;
			if (mousey >= screensize.y) mousey = screensize.y;
			MenuManager::get()->injectMouseMoved(mousex, mousey);
		}
		else
		{
			// Pass mouse movement to input manager
			inputmgr->mouseMoved(x - oldx, y - oldy);
		}
		oldx = x;
		oldy = y;
	}
	static void GLFWCALL mouseButtonListener(int button, int state)
	{
		// Capture mouse
		if (!mousecaptured)
		{
			glfwDisable(GLFW_MOUSE_CURSOR);
			mousecaptured = true;
			return;
		}

		InputManager * inputmgr = InputManager::get();
		if (MenuManager::get()->getCurrentMenu())
		{
			// Pass mouse button to menu
			MenuManager::get()->injectMouseButton(button, mousex, mousey, state);
		}
		else
		{
			// Inject button into input manager
			if (button == 0)
			{
				inputmgr->keyboardEvent(EK_LBUTTON, state);
			}
			else if (button == 1)
			{
				inputmgr->keyboardEvent(EK_RBUTTON, state);
			}
			else if (button == 2)
			{
				inputmgr->keyboardEvent(EK_MBUTTON, state);
			}
		}
	}

	static void GLFWCALL keyPressListener(int key, int action)
	{
		InputManager * inputmgr = InputManager::get();
		// Save key state
		if (key > 300) return;
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
			if (key == 'Q')
			{
				debugview = 1.0f - debugview;
				Horde3D::setOption(EngineOptions::DebugViewMode, debugview);
			}
		}
		else
		{
			keys[key] = false;
		}

		// Release mouse if control and shift are pressed
		if (mousecaptured && action && (keys[GLFW_KEY_LSHIFT] || keys[GLFW_KEY_RSHIFT])
			&& (keys[GLFW_KEY_LCTRL] || keys[GLFW_KEY_RCTRL])
			&& (keys[GLFW_KEY_LALT] || keys[GLFW_KEY_RALT]))
		{
			glfwEnable(GLFW_MOUSE_CURSOR);
			mousecaptured = false;
		}
		if (MenuManager::get()->getCurrentMenu())
		{
			// Convert special key for menu system
			switch (key)
			{
				case GLFW_KEY_BACKSPACE:
					MenuManager::get()->injectKey(gcn::Key::BACKSPACE, action);
					break;
				case GLFW_KEY_ESC:
					MenuManager::get()->injectKey(gcn::Key::ESCAPE, action);
					break;
				case GLFW_KEY_DEL:
					MenuManager::get()->injectKey(gcn::Key::DELETE, action);
					break;
				case GLFW_KEY_LSHIFT:
					MenuManager::get()->injectKey(gcn::Key::LEFT_SHIFT, action);
					shiftpressed = action;
					break;
				case GLFW_KEY_RSHIFT:
					MenuManager::get()->injectKey(gcn::Key::RIGHT_SHIFT, action);
					shiftpressed = action;
					break;
				case GLFW_KEY_UP:
					MenuManager::get()->injectKey(gcn::Key::UP, action);
					break;
				case GLFW_KEY_DOWN:
					MenuManager::get()->injectKey(gcn::Key::DOWN, action);
					break;
				case GLFW_KEY_LEFT:
					MenuManager::get()->injectKey(gcn::Key::LEFT, action);
					break;
				case GLFW_KEY_RIGHT:
					MenuManager::get()->injectKey(gcn::Key::RIGHT, action);
					break;
				case GLFW_KEY_PAGEUP:
					MenuManager::get()->injectKey(gcn::Key::PAGE_UP, action);
					break;
				case GLFW_KEY_PAGEDOWN:
					MenuManager::get()->injectKey(gcn::Key::PAGE_DOWN, action);
					break;
				case GLFW_KEY_HOME:
					MenuManager::get()->injectKey(gcn::Key::HOME, action);
					break;
				case GLFW_KEY_END:
					MenuManager::get()->injectKey(gcn::Key::END, action);
					break;
				// TODO
			};
		}
		else
		{
			// Pass key to input manager
			// TODO
			if ((key >= 'A') && (key <= 'Z'))
			{
				inputmgr->keyboardEvent((Keycode)(EK_KEY_A + key - 'A'), action);
			}
			else if ((key >= '0') && (key <= '9'))
			{
				inputmgr->keyboardEvent((Keycode)(EK_KEY_0 + key - '0'), action);
			}
			else
			{
				switch (key)
				{
					case GLFW_KEY_SPACE:
						inputmgr->keyboardEvent(EK_SPACE, action);
						break;
					case GLFW_KEY_BACKSPACE:
						inputmgr->keyboardEvent(EK_BACK, action);
						break;
					case GLFW_KEY_ESC:
						inputmgr->keyboardEvent(EK_ESCAPE, action);
						break;
					case GLFW_KEY_DEL:
						inputmgr->keyboardEvent(EK_DELETE, action);
						break;
					case GLFW_KEY_LSHIFT:
						inputmgr->keyboardEvent(EK_LSHIFT, action);
						break;
					case GLFW_KEY_RSHIFT:
						inputmgr->keyboardEvent(EK_RSHIFT, action);
						break;
					case GLFW_KEY_UP:
						inputmgr->keyboardEvent(EK_UP, action);
						break;
					case GLFW_KEY_DOWN:
						inputmgr->keyboardEvent(EK_DOWN, action);
						break;
					case GLFW_KEY_LEFT:
						inputmgr->keyboardEvent(EK_LEFT, action);
						break;
					case GLFW_KEY_RIGHT:
						inputmgr->keyboardEvent(EK_RIGHT, action);
						break;
					case GLFW_KEY_PAGEUP:
						inputmgr->keyboardEvent(EK_PRIOR, action);
						break;
					case GLFW_KEY_PAGEDOWN:
						inputmgr->keyboardEvent(EK_NEXT, action);
						break;
					case GLFW_KEY_HOME:
						inputmgr->keyboardEvent(EK_HOME, action);
						break;
					case GLFW_KEY_END:
						inputmgr->keyboardEvent(EK_END, action);
						break;
					// TODO
				};
			}
		}
	}

	static void GLFWCALL charListener(int character, int action)
	{
		if (MenuManager::get()->getCurrentMenu())
		{
			MenuManager::get()->injectKey(character, action);
		}
	}

	GraphicsEngine::GraphicsEngine()
	{
		lastfps = 0;
		time = 0;
	}

	GraphicsEngine* GraphicsEngine::get(void)
	{
		static GraphicsEngine graphics;
		return &graphics;
	}

	bool GraphicsEngine::init(void)
	{
		SettingsManager *conf = SettingsManager::get();
		FreeImage_Initialise();

		// Create window
		// FIXME: glfwOpenWindow should be called here
		Vector2DI size = conf->getVector2DI("window.resolution");
		screensize = size;
		bool fullscreen = conf->getBool("window.fullscreen");
		//#ifdef WIN32
		glfwInit();
		if(!glfwOpenWindow(size.x, size.y, 8, 8, 8, 8, 24, 8, fullscreen?GLFW_FULLSCREEN:GLFW_WINDOW))
		{
			LERROR("Could not open window!\n");
			glfwTerminate();
			return false;
		}
		//#endif
		// TODO: Fullscreen
		glfwSetWindowSize(size.x, size.y);
		caption = conf->getString("window.title");
		glfwSetWindowTitle(caption.c_str());
		glfwSwapInterval(0);

		// Initialize Horde3D
		if (!Horde3D::init())
		{
			LERROR("Could not initialize Horde3D!\n");
			glfwTerminate();
			return false;
		}
		Horde3D::setupViewport(0, 0, size.x, size.y, true);
		glfwSetMousePosCallback(mouseMoveListener);
		glfwSetMouseButtonCallback(mouseButtonListener);
		glfwSetKeyCallback(keyPressListener);
		glfwSetCharCallback(charListener);
		//glfwDisable(GLFW_MOUSE_CURSOR);
		glfwGetMousePos(&oldx, &oldy);
		for (int i = 0; i < 300; i++)
		{
			keys[i] = 0;
		}
		// Set resource paths
		/*Horde3DUtils::setResourcePath(ResourceTypes::SceneGraph, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Geometry, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Animation, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Material, "materials");
		Horde3DUtils::setResourcePath(ResourceTypes::Code, "shaders");
		Horde3DUtils::setResourcePath(ResourceTypes::Shader, "shaders");
		Horde3DUtils::setResourcePath(ResourceTypes::Texture2D, "textures");
		Horde3DUtils::setResourcePath(ResourceTypes::TextureCube, "textures");
		Horde3DUtils::setResourcePath(ResourceTypes::Effect, "effects");
		Horde3DUtils::setResourcePath(ResourceTypes::Pipeline, "pipelines");*/

		// Load pipeline and create camera
		ResHandle pipeline = Horde3D::addResource(ResourceTypes::Pipeline, conf->getString("device.pipeline").c_str(), 0);
		postprocessmaterial = Horde3D::addResource(ResourceTypes::Material, "materials/light.material.xml", 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		stdcam = new CameraSceneNode;

		OverlayManager::get()->init();

		timesincefps = 0;

		return true;
	}

	bool GraphicsEngine::shutdown(void)
	{
		OverlayManager::get()->shutdown();
		Horde3D::release();
		glfwTerminate();
		return true;
	}

	bool GraphicsEngine::renderFrame(float msecs)
	{
		SceneNodeController::doAllWork(msecs);
		ParticleSystem::doAllWork(msecs);

		// Overlays
		Horde3D::clearOverlays();
		OverlayManager::get()->render();

		// Get FPS
		int fps = (int)(1000.0f/msecs);
		timesincefps += msecs;
		if ((lastfps != fps) && (timesincefps > 500) && (msecs != 0))
		{
			lastfps = fps;
			char title[40];
			sprintf(title, "FPS: %d\n", fps);
			glfwSetWindowTitle(title);
			timesincefps = 0;
		}

		// Render
		time += msecs / 1000;
		Horde3D::setMaterialUniform(postprocessmaterial, "time", time, time, time, time);
		stdcam->render();
		MenuManager::get()->render();
		glfwSwapBuffers();

		return glfwGetWindowParam(GLFW_OPENED);
	}

	CameraSceneNode *GraphicsEngine::getStandardCamera(void)
	{
		return stdcam;
	}

	void GraphicsEngine::saveMessages(void)
	{
		Horde3DUtils::dumpMessages();
	}

	Vector2DI GraphicsEngine::getScreensize(void)
	{
		return screensize;
	}
}
