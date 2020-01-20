
#ifndef I_UPDATER_H
#define I_UPDATER_H

#include "litha_internal.h"

class IUpdatable;

class IUpdater : public virtual IReferenceCounted
{
public:
	~IUpdater(){}
	
	// Get the virtual time of this updater.
	// This virtual time is the same as the virtual time of the parent IUpdatable that owns this updater.
	virtual f32 GetVirtualTime() const = 0;
	
	// Return the last delta time...
	// If this is called from within a child updatable's Update method, this will be the *current* delta time.
	virtual f32 GetLastDeltaTime() const = 0;
	
	// Add something that must be updated by this updater.
	// When the thing is updated depends on what owns this updater.
	// keepRef - should the task keep a reference to the updatable?
	virtual void AddUpdatable(IUpdatable *updatable, bool keepRef = true) = 0;
	
	virtual void RemoveUpdatable(IUpdatable *updatable) = 0;
	
	virtual void RemoveAllUpdatables() = 0;
	
	// Recurses through tree (each IUpdatable having an IUpdater)
	virtual void RemoveAllUpdatablesRecursive() = 0;
	
	// Call InitUpdateTime on all updatables.
	virtual void InitAllUpdateTimes() = 0;
	
	// Update this collection of updatables!
	// This is used internally by IUpdatable.
	virtual void UpdateAllUpdatables(f32 virtualTime, f32 dt) = 0;
	
	// These pause/resume methods should only be used internally by IUpdatable.
	// Unlike in IPausable, no ref counting is done.
	virtual void PauseAllUpdatables() = 0;
	virtual void ResumeAllUpdatables() = 0;
};

#endif

