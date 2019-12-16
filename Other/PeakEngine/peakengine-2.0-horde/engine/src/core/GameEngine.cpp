/*
Copyright (C) 2009  Mathias Gottschlag

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

#include "core/GameEngine.h"
#include "core/FileSystem.h"
#include "core/SettingsManager.h"
#include "core/Logger.h"
#include "network/BroadcastClient.h"
#include "core/Game.h"
#include "graphics/GraphicsEngine.h"

#include <enet/enet.h>
#include <GL/glfw.h>
#include <unistd.h>
namespace peak
{
	GameEngine *GameEngine::get(void)
	{
		static GameEngine engine;
		return &engine;
	}

	bool GameEngine::run(std::string root)
	{
		// Needed for timer
		glfwInit();

		rootdir = root;
		// Load settings
		Logger::get()->setFile("peakengine.log");
		LINFO("PeakEngine starting.\n");
		FileSystem::get()->setRoot(root);
		if (!SettingsManager::get()->load())
		{
			LERROR("Could not load settings.\n");
		}

		if (enet_initialize())
		{
			LERROR("Could not initialize network.\n");
			return false;
		}

		GraphicsEngine::get()->init();

		// Test initialization
		if (!Game::get()->init())
		{
			Game::get()->init(Address("127.0.0.1"));
		}
		else
		{
			// Create some entities
			printf("Test entity.\n");
			Entity *test1 = Game::get()->createEntity("cube", 0);
			//test1 = Game::get()->createEntity("cube", 0);
		}

		// Main loop
		float waittime = 0.02;
		float lastframetime = 0.02;
		float currenttime = glfwGetTime();
		while (1)
		{
			Game::get()->doWork(20);
			if (!GraphicsEngine::get()->renderFrame(lastframetime * 1000))
				break;
			while (glfwGetTime() < currenttime + waittime)
				usleep(1000);
			float lastframe = currenttime;
			currenttime = glfwGetTime();
			waittime -= currenttime - lastframe;
			while (waittime < 0.0)
				waittime += 0.02;
			lastframetime = currenttime - lastframe;
			printf("Frame time: %f\n", lastframetime);
		}

		// Shut down rest of the engine
		if (!SettingsManager::get()->save())
		{
			LERROR("Could not save settings.\n");
		}

		enet_deinitialize();

		return true;
	}

	void GameEngine::stopGame(void)
	{
		stopengine = true;
	}

	void GameEngine::setRootDirectory(std::string dir)
	{
		rootdir = dir;
		FileSystem::get()->setRoot(dir);
	}
	std::string GameEngine::getRootDirectory(void)
	{
		return rootdir;
	}

	GameEngine::GameEngine()
	{
		stopengine = false;
	}
}

