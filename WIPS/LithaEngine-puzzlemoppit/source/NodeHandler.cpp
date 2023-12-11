
#include "NodeHandler.h"

void NodeHandler::ReceiveRenderPosition(core::vector3df pos)
{
	irrNode->setPosition(pos);
}

void NodeHandler::ReceiveRenderRotation(core::vector3df rot)
{
	irrNode->setRotation(rot);
}

