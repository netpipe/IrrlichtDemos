#pragma once
#include "GameObject.h"

class GOList
{
public:
	GOList(void);
	GOList(GameObject*);
	~GOList(void);
	void InsertNode(GameObject*);
	void RemoveNode(int index);
	GameObject* RemoveNode(GameObject*);
	GameObject* GetNode(int index);
	int GetSize();
	
private:
	GameObject* start;
	GameObject* end;
	GameObject* prev;
	GameObject* temp;
	int size;
};
