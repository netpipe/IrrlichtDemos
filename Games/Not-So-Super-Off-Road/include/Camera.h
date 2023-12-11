#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "GameObject.h"

class Camera : public GameObject
{
private:
	ISceneNode *follow;
	
public:
	
	void followNode(ISceneNode *n)
	{
		follow = n;
	}
	
	virtual void update()
	{
		node->setPosition(follow->getPosition() + vector3df(0,275,-275));
		static_cast<ICameraSceneNode*>(node)->setTarget(follow->getPosition());
	}
};

#endif
