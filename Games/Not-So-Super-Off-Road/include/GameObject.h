#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace scene;
using namespace io;

class GameObject
{
protected:
	IMesh *mesh;
	ISceneNode *node;
	u32 id;
	static u32 nr_of_objects;
	
public:
	
	GameObject()
	{
		id = nr_of_objects;
		nr_of_objects++;
	}
	virtual ~GameObject()
	{
	}
	
	void setSceneNode(ISceneNode *n)
	{
		node = n;
	}
	
	IMesh *getMesh() const
	{
		return mesh;
	}
	
	ISceneNode *getSceneNode() const
	{
		return node;
	}
	
	u32 getID()
	{
		return id;
	}
	
	virtual void update()
	{
	}
};

#endif