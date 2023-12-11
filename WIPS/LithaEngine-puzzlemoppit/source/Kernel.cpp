
#include "Kernel.h"
#include "Task.h"
#include "IEngine.h"

Kernel::Kernel()
{
	engine = GetEngine();
	device = engine->GetIrrlichtDevice();
	
	readyToExit = false;
}

Kernel::~Kernel()
{
	for (u32 i = 0; i < tasks.size(); i ++)
		tasks[i]->drop();
}

void Kernel::AddTask(Task *task)
{
	task->grab();
	task->InitUpdateTime();
	tasks.push_back(task);
}

void Kernel::Run()
{
	// device->run ticks the Irrlicht timer
	// (which is used by GetEngineTime())
	while (tasks.size() && device->run())
	{
#ifdef __APPLE__
		// We don't do this on full screen on Mac is it causes a freeze... (much like that Linux problem I had once...)
		if (!device->isFullscreen())
		{
#endif
		// Pause engine if window is not active.
		// Note, in future, if networking support is added, then
		// we should *NOT* pause engine ever while in a multiplayer game.
		if (device->isWindowActive() && device->isWindowFocused() && !device->isWindowMinimized())
		{
			if (engine->IsPaused())
				engine->Resume();
		}
		else
		{
			if (!engine->IsPaused())
				engine->Pause();
			
			// Sleep a bit if window is inactive
			device->sleep(100);
		}
#ifdef __APPLE__
		}
#endif
		
		// If engine is not paused, then we update tasks!
		if (!engine->IsPaused())
		{
			for (u32 i = 0; i < tasks.size(); i ++)
			{
				// A potential task update...
				// (PotentialUpdate determines whether or not enough time has passed
				// to perform an actual update).
				tasks[i]->PotentialUpdate();
			}
			
			// Also process queued events
			engine->ProcessEventQueue();
		}
		
		// Sleep a little to avoid hogging the CPU totally...
		// Ideally though we'd calculate the exact time we can sleep for.
		// (i.e. exact time until the next PotentialUpdate will actually Update)
		device->sleep(5);
		
		// Now all updates are finished can process exit request.
		if (readyToExit)
			device->closeDevice();
	}
}

void Kernel::Exit()
{
	// Exit is delayed.
	// Don't want to close Irrlicht devices until all tasks are finished updating etc.
	readyToExit = true;
}




