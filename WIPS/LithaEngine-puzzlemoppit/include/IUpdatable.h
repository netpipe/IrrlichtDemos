
#ifndef I_UPDATABLE_H
#define I_UPDATABLE_H

#include "litha_internal.h"
#include "IPausable.h"
#include "IEngine.h"
#include "IUpdater.h"
#include "Event.h"

class IUpdatable : public virtual IReferenceCounted, public virtual IPausable
{
	IEngine *engine;
	IUpdater *updater;
	
	// these are in engine time
	f32 startTime;
	f32 pauseTime;
	
	// the virtual time
	f32 virtualTime;
	f32 lastDeltaTime;
	
	// timed events
	std::vector<Event> timedEvents;
	
protected:

	virtual void OnPause()
	{
		pauseTime = engine->GetEngineTime();
		GetUpdater().PauseAllUpdatables();
	}
	
	virtual void OnResume()
	{
		// account for the length of time we were paused for
		startTime += engine->GetEngineTime() - pauseTime;
		GetUpdater().ResumeAllUpdatables();
	}
	
public:
	IUpdatable()
	{
		engine = GetEngine();
		updater = engine->CreateUpdater();
		
		startTime = engine->GetEngineTime();
		virtualTime = 0.f;
		lastDeltaTime = 0.f;
	}
	
	virtual ~IUpdatable()
	{
		updater->drop();
		updater = NULL;
	}
	
	f32 GetVirtualTime() const
	{
		return virtualTime;
	}
	
	f32 GetLastDeltaTime() const
	{
		return lastDeltaTime;
	}
	
	// Used in a similar manner to IEngine.QueueEvent.
	// Key difference: This event's delay will be paused when this Updatable is paused.
	void TimedEvent(const Event &event, f32 delay)
	{
		timedEvents.push_back(event);
		timedEvents.back()["___PostTime___"] = virtualTime + delay;
	}
	
	IUpdater &GetUpdater() { ASSERT(updater); return *updater; }
	
	// Zero any time deltas.
	// This should be used after the app has frozen for a while (i.e. from loading media)
	// in order to prevent a sudden jump forwards in time.
	virtual void InitUpdateTime()
	{
		// This resets any time delta.
		// I.e. causes next call to Update to result in the same virtualTime
		// as the last call to Update.
		f32 lastEngineTime = startTime + virtualTime;
		f32 engineTimeDelta = engine->GetEngineTime() - lastEngineTime;
		startTime += engineTimeDelta;
		
		// Recurse
		GetUpdater().InitAllUpdateTimes();
	}
	
	// Update the updatable! This is called internally by an Updater.
	// An IUpdatable is not passed its parent Updater's virtual time, since the
	// IUpdatable has its own virtual time (being pausable).
	virtual void Update(f32 dt)
	{
		if (IsPaused())
			return;
		
		// update virtual time
		virtualTime = engine->GetEngineTime() - startTime;
		
		// cache delta time
		lastDeltaTime = dt;
		
		// We call GetVirtualTime here, since GetVirtualTime is overidable
		// and so may return different from virtualTime set above.
		GetUpdater().UpdateAllUpdatables(GetVirtualTime(), dt);
		
		// Handle timed events
		for (u32 i = 0; i < timedEvents.size(); i ++)
		{
			if (timedEvents[i]["___PostTime___"].To<f32>() <= virtualTime)
			{
				// Queue it with no delay.
				// So will be sent by Kernel right after all Updatables have finished updating.
				engine->QueueEvent(timedEvents[i]);
				timedEvents.erase(timedEvents.begin()+i);
				i --;
			}
		}
	}
};

#endif

