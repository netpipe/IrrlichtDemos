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

// #define PROFILE_MAIN_LOOP

#include "core/GameEngine.h"
#include "core/Script.h"
#include "core/FileSystem.h"
#include "core/SettingsManager.h"
#include "core/InputManager.h"
#include "core/Game.h"
#include "core/Logger.h"
#include "core/Timer.h"
#include "network/BroadcastClient.h"
#include "graphic/GraphicsEngine.h"
#include "physics/World.h"
#include "physics/Body.h"
#include "sound/SoundEngine.h"

#include <enet/enet.h>
#include <ode/ode.h>
 #include <unistd.h>

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
int gettimeofday(struct timeval* tv, void *timezone)
{
	union {
		long long ns100;
		FILETIME ft;
	} now;

	GetSystemTimeAsFileTime (&now.ft);
	tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
	tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);
	return (0);
}
#endif

namespace peak
{
	GameEngine *GameEngine::get(void)
	{
		static GameEngine engine;
		return &engine;
	}

	bool GameEngine::run(std::string root)
	{
		rootdir = root;
		// Load settings
		Logger::get()->setFile("peakengine.log");
		LINFO("PeakEngine starting.\n");
		FileSystem::get()->setRoot(root);
		if (!SettingsManager::get()->load())
		{
			LERROR("Could not load settings.\n");
		}
		InputManager::get()->init();
		SoundEngine::get()->init();

		if (enet_initialize())
		{
			LERROR("Could not initialize network.\n");
			return false;
		}
		GraphicsEngine::get()->init();
		dInitODE();
		// Run startup script
		Script script;
		std::string filename = root + "/" + SettingsManager::get()->getString("scripts.startup");
		if (!script.run(filename))
		{
			LERROR("Could not execute startup script.\n");
			return false;
		}

		// Main loop
		bool multithreaded = false;
		if (multithreaded)
		{
			LERROR("Multithreading not yet supported!\n");
		}
		else
		{
			gettimeofday(&lastframe, 0);
			float lastframetime = 0;
			#ifdef PROFILE_MAIN_LOOP
			Timer timer;
			timer.start();
			#endif
			while (!stopengine)
			{
				// Compute time needed for the last frame
				timeval currtime;
				gettimeofday(&currtime, 0);
				float frametime = currtime.tv_usec - lastframe.tv_usec;
				if (frametime < 0) frametime += 1000000;
				frametime /= 1000;
				lastframe = currtime;

				if (lastframetime > 1000)
				{
					printf("Time: %f\n", lastframetime);
				}
				// Update physics (fixed time step!)
				lastframetime += frametime;
				while (lastframetime > 5)
				{
					Body::updateAll();
					World::doAllWork(5);
					lastframetime -= 5;
				}
				#ifdef PROFILE_MAIN_LOOP
				float physics = timer.getTime();
				timer.reset();
				#endif
				// Update game/entities
				Timer::doAllWork();
				Game::get()->doWork(frametime);
				#ifdef PROFILE_MAIN_LOOP
				float game = timer.getTime();
				timer.reset();
				#endif

				BroadcastClient::doAllWork(frametime);
				if (!GraphicsEngine::get()->renderFrame(frametime))
				{
					stopengine = true;
				}
				SoundEngine::get()->doWork(frametime);
				#ifdef PROFILE_MAIN_LOOP
				float gfx = timer.getTime();
				timer.reset();
				printf("%f/%f/%f\n", physics, game, gfx);
				#endif
				usleep(1000);
			}
		}
		//InputManager::get()->shutdown();
		Game::get()->shutdown();

		// Run shutdown script
		filename = root + "/" + SettingsManager::get()->getString("scripts.shutdown");
		if (!script.run(filename))
		{
			LERROR("Could not execute shutdown script.\n");
			return false;
		}
		SoundEngine::get()->shutdown();
		GraphicsEngine::get()->shutdown();
		InputManager::get()->shutdown();
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

