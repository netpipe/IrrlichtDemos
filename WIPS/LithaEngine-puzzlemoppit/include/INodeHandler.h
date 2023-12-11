
#ifndef I_NODE_HANDLER_H
#define I_NODE_HANDLER_H

#include "IGraphic.h"

// This is used to provide smooth interpolated motion to any Irrlicht scene nodes.
class INodeHandler : public IGraphic
{
public:
	virtual ~INodeHandler(){}
	
	virtual scene::ISceneNode *GetIrrNode() = 0;
};

#endif

