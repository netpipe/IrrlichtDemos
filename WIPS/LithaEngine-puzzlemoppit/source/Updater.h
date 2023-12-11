
#ifndef UPDATER_H
#define UPDATER_H

#include "IUpdater.h"
#include "IUpdatable.h"
#include <vector>

struct UpdatableInfo
{
	IUpdatable *ptr;
	bool keptRef;
};

class Updater : public IUpdater
{
	std::vector<UpdatableInfo> updatables;
	f32 virtualTime;
	f32 lastDeltaTime;
	bool updatablesArePaused;
	
public:
	Updater()
	{
		updatablesArePaused = false;
		virtualTime = 0.f;
		lastDeltaTime = 0.f;
	}
	
	~Updater()
	{
		RemoveAllUpdatables();
	}
	
	void InitAllUpdateTimes()
	{
		for (u32 i = 0; i < updatables.size(); i ++)
			updatables[i].ptr->InitUpdateTime();
	}
	
	void UpdateAllUpdatables(f32 virtualTime, f32 dt)
	{
		// Cache the parent's virtual time
		this->virtualTime = virtualTime;
		
		// Cache delta time
		lastDeltaTime = dt;
		
		for (u32 i = 0; i < updatables.size(); i ++)
			updatables[i].ptr->Update(dt);
	}
	
	f32 GetVirtualTime() const
	{
		return virtualTime;
	}
	
	f32 GetLastDeltaTime() const
	{
		return lastDeltaTime;
	}
	
	void AddUpdatable(IUpdatable *updatable, bool keepRef = true)
	{
		if (!updatable)
			return;
		
		// check not already present
		for (u32 i = 0; i < updatables.size(); i ++)
		{
			if (updatables[i].ptr == updatable)
				return;
		}
		
		if (keepRef)
			updatable->grab();
		
		UpdatableInfo info = {updatable, keepRef};
		updatables.push_back(info);
		
		// maybe pause
		if (updatablesArePaused)
			updatable->Pause();
	}
	
	void RemoveUpdatable(IUpdatable *updatable)
	{
		for (u32 i = 0; i < updatables.size(); i ++)
		{
			if (updatables[i].ptr == updatable)
			{
				bool keptRef = updatables[i].keptRef;
				
				updatables.erase(updatables.begin()+i);
				
				// did we grab it?
				if (keptRef)
					updatable->drop();
				
				break;
			}
		}
	}
	
	void RemoveAllUpdatables()
	{
		while (updatables.size())
			RemoveUpdatable(updatables.front().ptr);
	}
	
	void RemoveAllUpdatablesRecursive()
	{
		for (u32 i = 0; i < updatables.size(); i ++)
			updatables[i].ptr->GetUpdater().RemoveAllUpdatablesRecursive();
		
		RemoveAllUpdatables();
	}
	
	void PauseAllUpdatables()
	{
		updatablesArePaused = true;
		
		for (u32 i = 0; i < updatables.size(); i ++)
			updatables[i].ptr->Pause();
	}
	
	void ResumeAllUpdatables()
	{
		updatablesArePaused = false;
		
		for (u32 i = 0; i < updatables.size(); i ++)
			updatables[i].ptr->Resume();
	}
};

#endif

