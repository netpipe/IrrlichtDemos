
#ifndef __I_BATCHED_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __I_BATCHED_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "IBillboardSceneNode.h"

namespace irr
{
namespace scene
{
class IBatchedBillboardSceneNode;
}
}

#include "IEmptyBillboardSceneNode.h"

namespace irr
{
namespace scene
{


class IBatchedBillboardSceneNode : public ISceneNode
{
public:

	//! Constructor
	IBatchedBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes=0, u32 count=0, core::array<IEmptyBillboardSceneNode*> *impostorsOut=0,
        s32 id=-1, const core::vector3df& position = core::vector3df(0,0,0))
		: ISceneNode(parent, mgr, id, position) {}


	virtual IEmptyBillboardSceneNode* addBillboard(IBillboardSceneNode* node) =0;

	virtual void removeBillboard(IEmptyBillboardSceneNode* node) = 0;
	virtual void removeBillboard(IEmptyBillboardSceneNode* node,bool noDrop) = 0;
	virtual void removeBillboard(IBillboardSceneNode* node) = 0;

	virtual void updateBillboardSize(const IEmptyBillboardSceneNode* const node, const core::dimension2d<f32>& size) const = 0;
	virtual void updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & overallColor) = 0;
	virtual void updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & topColor, const video::SColor & bottomColor) = 0;
    virtual void updateBillboardScale(IEmptyBillboardSceneNode* node,const core::vector3df& scale) = 0;
    virtual void updateBillboardRotation(IEmptyBillboardSceneNode* node,const core::vector3df& rotation) = 0;
    virtual void updateBillboardPosition(IEmptyBillboardSceneNode* node,const core::vector3df& newpos) = 0;
    virtual void updateBillboardAbsoluteTransformation(IEmptyBillboardSceneNode* node,core::matrix4 AbsoluteTransformation) = 0;
    virtual video::SMaterial& getMaterial(u32 num) = 0;
    virtual u32 getMaterialCount() const = 0;

    virtual bool isPassingNormals() const = 0;
    virtual void setPassingNormals(bool passing) = 0;
    virtual f32 getLoDAngle() const = 0;
    virtual void setLoDAngle(f32 angle) = 0;
	virtual u32 getCount() = 0;
};

}
}

#endif
