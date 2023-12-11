

#ifndef __I_HW_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __I_HW_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "IBatchedBillboardSceneNode.h"

namespace irr
{
namespace scene
{


class IHardwareBillboardSceneNode : public IBatchedBillboardSceneNode
{
public:

	//! Constructor
	IHardwareBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes=0, u32 count=0, core::array<IEmptyBillboardSceneNode*> *impostorsOut=0,
        s32 id=-1, const core::vector3df& position = core::vector3df(0,0,0))
		: IBatchedBillboardSceneNodeSceneNode(parent, mgr, nodes, count, impostorsOut, id, position) {}
};

}
}

#endif
