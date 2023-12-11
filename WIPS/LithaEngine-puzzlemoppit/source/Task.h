
#ifndef TASK_H
#define TASK_H

#include "litha_internal.h"
#include "IUpdatable.h"
#include "IEngine.h"

// A task is basically a top level IUpdatable, with a few special methods.
// Tasks are owned by Kernel.
// Tasks are all created internally, at the same time (engine start), so that their virtual times
// are in sync. (all start at time=0 on engine start)
// So task's virtual time is the same as Engine time.
class Task : public virtual IReferenceCounted, public IUpdatable
{
	IEngine *engine;
	f32 lastTime;
	
	// Time step params.
	// Set by Engine with SetSteppingParams.
	bool fixedTimeStep;
	f32 timeStep;
	
protected:
	// Pause and resume are not available for the top level tasks, since they mustn't get out of sync.
	void OnPause()	{ASSERT(false);}
	void OnResume()	{ASSERT(false);}
	
public:
	Task()
	{
		engine = GetEngine();
		SetSteppingParams(false);
	}
	
	virtual ~Task(){}
	
	// called by Engine after creating a task
	// For a non fixed time step, timeStep is the *minimum* timestep.
	// (there's no real need for unlimited FPS rendering is there? We don't want to
	// be rendering at 1000 FPS on later hardware)
	// So for a fixed timestep, all updates are done and none are skipped.
	// For a variable timestep, some updates may be skipped if there is not enough time.
	void SetSteppingParams(bool fixedTimeStep, f32 timeStep = 0.01)
	{
		this->fixedTimeStep = fixedTimeStep;
		this->timeStep = timeStep;
	}
	
	// Task virtual time is synchronised with engine time.
	f32 GetVirtualTime() { return engine->GetEngineTime(); }
	
	// Since a task cannot be paused, its updatables will always be updated.
	virtual void Update(f32 dt)
	{
		IUpdatable::Update(dt);
	}
	
	// Gets the interpolation from this task's previous Update to the current point in time.
	// Note: this is only useful if this task has a fixed time step
	// This is used by engine.GetLogicInterpolationAlpha()
	f32 GetInterpolationAmount()
	{
		return min(1.f, (engine->GetEngineTime() - lastTime) / timeStep);
	}
	
	// Called by Kernel after task has been added to Kernel.
	void InitUpdateTime()
	{
		IUpdatable::InitUpdateTime();
		
		// Zero the next dt
		lastTime = engine->GetEngineTime();
	}
	
private:
	
	// This PotentialUpdate stuff is a bit hackish, but it does the job.
	// It should really be moved out into a separate class.
	
	friend class Kernel;
	
	// Potential update.
	// This is called as often as possible.
	// If the alotted timestep has passed, or if this task is not at a fixed time step,
	// then Update() is called.
	void PotentialUpdate()
	{
		f32 currentTime = engine->GetEngineTime();
		
		if (fixedTimeStep)
		{
			while ( (currentTime - lastTime) > timeStep )
			{
				Update(timeStep);
				lastTime += timeStep;
			}
		}
		else // Variable timestep
		{
			f32 deltaTime = currentTime - lastTime;
			
			// Does the time that has passed exceed the minimum timestep?
			if (deltaTime >= timeStep)
			{
				Update(deltaTime);
				lastTime = currentTime;
			}
		}
	}
};

#endif
