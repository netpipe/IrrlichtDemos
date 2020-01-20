
#include "LogicTask.h"

LogicTask::LogicTask()
{
}

void LogicTask::Update(f32 dt)
{
	// World is updated here.
	// (it is an updatable added to this task's Updater)
	Task::Update(dt);
	
	// Logic task currently does very little other than update world.
	// Don't remove it though. It could be useful.
}

