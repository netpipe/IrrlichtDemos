#include "WPManager.h"

WPManager::WPManager(void)
{

}

WPManager::~WPManager(void)
{

}

//Insert a Waypoint* into the list
void WPManager::InsertNode(Waypoint* n)
{
	data.push_back(n);
}

//Remove a node from the list by index	
void WPManager::RemoveNode(int index)
{
	data.erase(index);
}
	
Waypoint* WPManager::GetNode(int index)
{
	return data[index];
}
	
int WPManager::GetSize()
{
	return data.size();
}