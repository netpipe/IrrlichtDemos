#include "Scene_Interface.h"

Scene_Interface::Scene_Interface(void)
{
	smgr = 0;
	CurrentNode = 0;
}

Scene_Interface::~Scene_Interface(void)
{
}

void		Scene_Interface::setSceneManager(scene::ISceneManager *mgr)
{
	smgr = mgr;
}

void		Scene_Interface::setCamera(scene::ICameraSceneNode *cam)
{
	camera = cam;
}

bool		Scene_Interface::setCurrentNode(const u32 node)
{
	if (!smgr)
		return false;
	
	CurrentNode = smgr->getSceneNodeFromId(node);

	if (CurrentNode)
		return true;

	return false;
}
int			Scene_Interface::getCurrentNode()
{
	if (CurrentNode)
	{
		return CurrentNode->getID();
	}

	return -1;
}

vector3		Scene_Interface::getPosition()
{
	if (CurrentNode)
	{
		vector3 v = CurrentNode->getPosition();
		return v;
	}
	else return vector3();
}

vector3		Scene_Interface::getRotation()
{
	if (CurrentNode)
	{
		vector3 v = CurrentNode->getRotation();
		return v;
	}
	else return vector3();
}

vector3		Scene_Interface::getScale()
{
	if (CurrentNode)
	{
		vector3 v = CurrentNode->getScale();
		return v;
	}
	else return vector3();
}

bool		Scene_Interface::setPosition(vector3 *value)
{
	if (CurrentNode)
	{
		CurrentNode->setPosition(core::vector3df(value->X, value->Y, value->Z));
		return true;
	}
	return false;
}

bool		Scene_Interface::setRotation(vector3 *value)
{
	if (CurrentNode)
	{
		CurrentNode->setRotation(core::vector3df(value->X, value->Y, value->Z));
		return true;
	}
	return false;
}

bool		Scene_Interface::setScale   (vector3 *value)
{
	if (CurrentNode)
	{
		CurrentNode->setScale(core::vector3df(value->X, value->Y, value->Z));
		return true;
	}
	return false;
}


bool		Scene_Interface::setFrameLoop(const s32 begin, const s32 end)
{
	if (CurrentNode)
	{
		scene::IAnimatedMeshSceneNode *node = (scene::IAnimatedMeshSceneNode*)CurrentNode;
		node->setFrameLoop(begin, end);
		return true;
	}
	return false;
}

bool		Scene_Interface::setFrameType(const u32 type)
{
	if (CurrentNode)
	{
		scene::IAnimatedMeshSceneNode *node = (scene::IAnimatedMeshSceneNode*)CurrentNode;
		node->setMD2Animation((scene::EMD2_ANIMATION_TYPE)type);
		return true;
	}
	return false;
}
s32			Scene_Interface::getFrameNr()
{
	if (CurrentNode)
	{
		scene::IAnimatedMeshSceneNode *node = (scene::IAnimatedMeshSceneNode*)CurrentNode;
		return node->getFrameNr();
	}
	return -1;
}

vector3		Scene_Interface::getCameraPosition()
{
	if (camera)
	{
		vector3 v = camera->getPosition();
		return v;
	}
	else return vector3();
}
vector3		Scene_Interface::getCameraRotation()
{
	if (camera)
	{
		vector3 v = camera->getRotation();
		return v;
	}
	else return vector3();
}

vector3		Scene_Interface::getCameraTarget()
{
	if (camera)
	{
		vector3 v = camera->getTarget();
		return v;
	}
	else return vector3();
}

bool		Scene_Interface::setCameraTarget(vector3 *value)
{
	if (camera)
	{
		camera->setTarget(core::vector3df(value->X, value->Y, value->Z));
		return true;
	}
	else return false;
}