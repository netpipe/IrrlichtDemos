
#include "litha_internal.h"
#include <vector>

class IEngine;
class Task;

class Kernel : public IReferenceCounted
{
	IEngine *engine;
	IrrlichtDevice *device;
	
	std::vector<Task *> tasks;
	
	bool readyToExit;
	
public:
	Kernel();
	~Kernel();
	
	// only add a task once, this does no checks.
	// tasks should only be added at engine start, as they must all be in sync.
	void AddTask(Task *task);
	
	void Run();
	void Exit();
};

