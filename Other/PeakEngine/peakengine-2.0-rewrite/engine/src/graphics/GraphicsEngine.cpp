/*
Copyright (C) 2008, 2009  Lukas Kropatschek, Mathias Gottschlag

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
#include "graphics/SceneNode.h"
#include "core/SettingsManager.h"
#include "core/Logger.h"
#include "core/GameEngine.h"

#include <Horde3D.h>
#include <Horde3DUtils.h>
#include <GL/glfw.h>
#include <cstdlib>
#include <guichan.hpp>

#include <FreeImagePlus.h>

namespace peak
{
	
	void (*keycb)(Keycode key, bool pressed) = 0;
	
	static void GLFWCALL keyPressListener(int key, int action)
	{
		if (!keycb) return;
		// Pass key to input manager
		// TODO
		if ((key >= 'A') && (key <= 'Z'))
		{
			keycb((Keycode)(EK_KEY_A + key - 'A'), action);
		}
		else if ((key >= '0') && (key <= '9'))
		{
			keycb((Keycode)(EK_KEY_0 + key - '0'), action);
		}
		else
		{
			switch (key)
			{
				case GLFW_KEY_SPACE:
					keycb(EK_SPACE, action);
					break;
				case GLFW_KEY_BACKSPACE:
					keycb(EK_BACK, action);
					break;
				case GLFW_KEY_ESC:
					keycb(EK_ESCAPE, action);
					break;
				case GLFW_KEY_DEL:
					keycb(EK_DELETE, action);
					break;
				case GLFW_KEY_LSHIFT:
					keycb(EK_LSHIFT, action);
					break;
				case GLFW_KEY_RSHIFT:
					keycb(EK_RSHIFT, action);
					break;
				case GLFW_KEY_UP:
					keycb(EK_UP, action);
					break;
				case GLFW_KEY_DOWN:
					keycb(EK_DOWN, action);
					break;
				case GLFW_KEY_LEFT:
					keycb(EK_LEFT, action);
					break;
				case GLFW_KEY_RIGHT:
					keycb(EK_RIGHT, action);
					break;
				case GLFW_KEY_PAGEUP:
					keycb(EK_PRIOR, action);
					break;
				case GLFW_KEY_PAGEDOWN:
					keycb(EK_NEXT, action);
					break;
				case GLFW_KEY_HOME:
					keycb(EK_HOME, action);
					break;
				case GLFW_KEY_END:
					keycb(EK_END, action);
					break;
				// TODO
			};
		}
	}
	
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
		FreeImage_Initialise();

		// Create window
		Vector2DI size = conf->getVector2DI("window.resolution");
		bool fullscreen = conf->getBool("window.fullscreen");
		glfwInit();
		if(!glfwOpenWindow(size.x, size.y, 8, 8, 8, 8, 24, 8, fullscreen?GLFW_FULLSCREEN:GLFW_WINDOW))
		{
			LERROR("Could not open window!\n");
			glfwTerminate();
			return false;
		}
		glfwSetWindowSize(size.x, size.y);
		caption = conf->getString("window.title");
		glfwSetWindowTitle(caption.c_str());
		glfwSwapInterval(0);
		glfwSetKeyCallback(keyPressListener);

		// Initialize Horde3D
		if (!Horde3D::init())
		{
			LERROR("Could not initialize Horde3D!\n");
			glfwTerminate();
			return false;
		}
		Horde3D::resize(0, 0, size.x, size.y);
		// Set resource paths
		Horde3DUtils::setResourcePath(ResourceTypes::SceneGraph, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Geometry, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Animation, "models");
		Horde3DUtils::setResourcePath(ResourceTypes::Material, "materials");
		Horde3DUtils::setResourcePath(ResourceTypes::Code, "shaders");
		Horde3DUtils::setResourcePath(ResourceTypes::Shader, "shaders");
		Horde3DUtils::setResourcePath(ResourceTypes::Texture2D, "textures");
		Horde3DUtils::setResourcePath(ResourceTypes::TextureCube, "textures");
		Horde3DUtils::setResourcePath(ResourceTypes::Effect, "effects");
		Horde3DUtils::setResourcePath(ResourceTypes::Pipeline, "pipelines");

		// Load pipeline and create camera
		ResHandle pipeline = Horde3D::addResource(ResourceTypes::Pipeline, conf->getString("device.pipeline").c_str(), 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		camera = Horde3D::addCameraNode(RootNode, "Camera", pipeline);
		Horde3D::setNodeTransform(camera, 6, 10, -6, -45, 180-45, 0, 1, 1, 1 );
		cameranode = new SceneNode;
		cameranode->create(camera);
		
		// Test model
		ResHandle modelres = Horde3D::addResource(ResourceTypes::SceneGraph, "tank_low2.scene.xml", 0);
		Horde3DUtils::loadResourcesFromDisk(GameEngine::get()->getRootDirectory().c_str());
		NodeHandle model = Horde3D::addNodes(RootNode, modelres);

		NodeHandle light = Horde3D::addLightNode( RootNode, "Light1", 0, "LIGHTING", "SHADOWMAP" );
		// Set light position and radius
		Horde3D::setNodeTransform( light, 0, 20, 0, -90, 0, 0, 1, 1, 1 );
		Horde3D::setNodeParamf( light, LightNodeParams::Radius, 50.0f );
		
		timesincefps = 0;

		return true;
	}

	bool GraphicsEngine::shutdown(void)
	{
		Horde3D::release();
		glfwTerminate();
		return true;
	}

	bool GraphicsEngine::renderFrame(float msecs)
	{
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
		
		Horde3D::render(camera);
		saveMessages();

		glfwSwapBuffers();

		return glfwGetWindowParam(GLFW_OPENED);
	}

	void GraphicsEngine::saveMessages(void)
	{
		Horde3DUtils::dumpMessages();
	}
	
	SceneNode *GraphicsEngine::getDefaultCamera(void)
	{
		return cameranode;
	}
	
	void GraphicsEngine::setKeyCallback(void (*cb)(Keycode key, bool pressed))
	{
		keycb = cb;
	}
}
