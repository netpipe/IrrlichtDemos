
#ifndef I_EVENT_QUEUE_H
#define I_EVENT_QUEUE_H

#include "litha_internal.h"
#include "IUpdatable.h"

class IEventQueue;
class Event;

class IWaitLogic : public virtual IReferenceCounted
{
public:
	virtual ~IWaitLogic(){}
	
	// Should return true when the wait is allowed to finish.
	// Will be dropped shortly after returning true.
	// eventQueue - the event queue that contains the wait that this logic is for
	// waitStartTime - time that this wait started (in the event queue's virtual time)
	virtual bool IsWaitOver(IEventQueue *eventQueue, f32 waitStartTime) = 0;
};

// Event queue must be added to an IUpdater to function.
class IEventQueue : public virtual IReferenceCounted, public IUpdatable
{
public:
	virtual ~IEventQueue(){}
	
	// Add event to the queue.
	// If no waits are present, this event will be sent immediately, and then removed from the queue.
	// (using IEngine.QueueEvent with time delay 0 - so on the next kernel tick)
	// If waits are present, event will be sent when they finish.
	virtual void AddEvent(const Event &event) = 0;
	
	// Add a time wait to the queue.
	// This causes a delay of the given waitTime before the next item is processed.
	virtual void AddTimeWait(f32 waitTime) = 0;
	
	// Add a logic wait to the queue.
	// This delays processing of the next item in the queue until the IWaitLogic.IsWaitOver method returns true.
	// (IsWaitOver is queried every update of the EventQueue)
	virtual void AddLogicWait(IWaitLogic *waitLogic) = 0;
	
	// Return the number of events waiting in the queue.
	// (ignoring any time or logic waits etc)
	virtual u32 GetEventsWaiting() = 0;
	
	// Returns whether there is anything in the queue.
	// Note this includes waits.
	// An empty queue has no events and no waits.
	virtual bool IsEmpty() = 0;
	
	// Is there an event of this specific type waiting?
	virtual bool IsEventWaiting(const core::stringc &eventName) = 0;
	
	// scale any time delays remaining. useful to run the queue at a different speed.
	virtual void ScaleTimes(f32 scale) = 0;
	
	// Get access to all events present in the queue.
	// So they can be modified... why ever would you want to do that!?
	// All Event pointers are invalidated with the next event queue update!
	virtual std::vector<Event *> GetAllEvents() = 0;
};

#endif

