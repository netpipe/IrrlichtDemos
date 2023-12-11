#include "GOList.h"
#include "GameObject.h"
#include "Player.h"
#include "Projectile.h"

GOList::GOList(void)
{
	size = 0;
	start = new GameObject();
	end = start;
}

GOList::GOList(GameObject* n)
{
	size = 1;
	start = new GameObject();
	start->next = n;
	end = n;
}

GOList::~GOList(void)
{
	delete start;
	delete end;
	delete prev;
	delete temp;
}

//Insert a GameObject* into the list
void GOList::InsertNode(GameObject* n)
{
	if(size == 0 ){
		start->next = n;
		end = n;
		size= size+1;
	}else{
		end->next = n;
		end = n;
		size= size+1;
	}
}

//Remove a node from the list by index
void GOList::RemoveNode(int index)
{
	//size--;
}

//Remove a node from the list by GameObject*.
//Objects are check against each other with unique id
GameObject* GOList::RemoveNode(GameObject* n)
{
	prev = start;
	temp = start->next;
	for(int i = 1;i <=GetSize();i++)
	{
		if(temp->getID() == n->getID())
		{
			prev->next = temp->next;
			if(end->getID() == temp->getID())
			{
				end = prev;
			}
			prev = 0;
			size = size-1;
			return temp;
		}else
		{
			//iterate 1 step
			prev = temp;
			temp = temp->next;
		}
	}
	temp = 0;
	prev = 0;
	return 0;
}

GameObject* GOList::GetNode(int index)
{
	GameObject* c = start->next;
	if(index > 0 && index <= GetSize())
	{
 		for(int i = 1; i <= GetSize(); i++)
		{
			if(i == index)
			{
				return c;
			}
			else
			{
				c = c->next;
			}
		}
	}
//	return false;
}

int GOList::GetSize()
{
	return size;
}
