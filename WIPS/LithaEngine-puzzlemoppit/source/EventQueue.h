
#include "IEventQueue.h"
#include <deque>

class EventQueue : public IEventQueue
{
	enum QUEUE_ITEM_TYPE
	{
		QIT_EVENT,
		QIT_TIME_WAIT,
		QIT_LOGIC_WAIT
	};
	
	struct QueueItem
	{
		QueueItem(QUEUE_ITEM_TYPE type)
			: type(type), neverFirstInQueue(true)
		{
		}
		
		QUEUE_ITEM_TYPE type;
		
		// used to detect when item becomes first in queue
		bool neverFirstInQueue;
		
		// Time a wait starts.
		// Set when an item becomes the first in the queue.
		f32 waitStartTime;
		
		// things specific to a particular type
		Event event;
		IWaitLogic *waitLogic;
		f32 waitTime;
	};
	
	u32 eventCount;
	std::deque<QueueItem> items;
	
public:
	EventQueue();
	~EventQueue();
	
	void AddEvent(const Event &event);
	
	void AddTimeWait(f32 waitTime);
	
	void AddLogicWait(IWaitLogic *waitLogic);
	
	u32 GetEventsWaiting();
	
	bool IsEmpty();
	
	bool IsEventWaiting(const core::stringc &eventName);
	
	void ScaleTimes(f32 scale);
	
	std::vector<Event *> GetAllEvents();
	
	void Update(f32 dt);
};

