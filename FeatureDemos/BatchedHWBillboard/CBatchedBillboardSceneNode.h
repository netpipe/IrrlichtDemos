#ifndef __C_BATCHED_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __C_BATCHED_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "IBatchedBillboardSceneNode.h"
#include "SMeshBuffer.h"
#include "ICameraSceneNode.h"

namespace irr
{
namespace scene
{


class CBatchedBillboardSceneNode : public IBatchedBillboardSceneNode
{
protected:
    core::array<IEmptyBillboardSceneNode*> impostors;
    core::array<core::vector3df> impostorTranslations;
    core::array<core::vector3df> impostorScales;
	core::array<core::dimension2df> impostorSizes;
    core::array<IBillboardSceneNode*> billboards;
    core::array<video::S3DVertex> vertices;
    core::array<u16> indices;
    core::array<f32> maxEdges[3];
    u16 maxEdgeIndices[3];
    core::array<f32> minEdges[3];
    u16 minEdgeIndices[3];

    core::array<video::SMaterial> billMaterial;

    bool PassingNormals;
    f32 cosCutoff;
    core::vector3df lastCamView;
public:
    CBatchedBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes=0, u32 count=0, core::array<IEmptyBillboardSceneNode*> *impostorsOut=0,  s32 id=-1,
		const core::vector3df& position = core::vector3df(0,0,0));
    ~CBatchedBillboardSceneNode();

	virtual IEmptyBillboardSceneNode* addBillboard(IBillboardSceneNode* node);

	virtual void removeBillboard(IEmptyBillboardSceneNode* node);
	virtual void removeBillboard(IEmptyBillboardSceneNode* node,bool noDrop);
	virtual void removeBillboard(IBillboardSceneNode* node);

	virtual void updateBillboardSize(const IEmptyBillboardSceneNode* const node, const core::dimension2d<f32>& size) const;
	virtual void updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & overallColor);
	virtual void updateBillboardColor(IEmptyBillboardSceneNode* node, const video::SColor & topColor, const video::SColor & bottomColor);
    virtual void updateBillboardScale(IEmptyBillboardSceneNode* node,const core::vector3df& scale);
    virtual void updateBillboardRotation(IEmptyBillboardSceneNode* node,const core::vector3df& rotation);
    virtual void updateBillboardPosition(IEmptyBillboardSceneNode* node,const core::vector3df& newpos);
    virtual void updateBillboardAbsoluteTransformation(IEmptyBillboardSceneNode* node,core::matrix4 AbsoluteTransformation);
    virtual video::SMaterial& getMaterial(u32 num);
    virtual u32 getMaterialCount() const;

    virtual bool isPassingNormals() const;
    virtual void setPassingNormals(bool passing);
    virtual f32 getLoDAngle() const;
    virtual void setLoDAngle(f32 angle);
	virtual u32 getCount();

	virtual void render();
	virtual const core::aabbox3d<float>& getBoundingBox() const {return core::aabbox3df(minEdges[0][minEdgeIndices[0]],minEdges[1][minEdgeIndices[1]],minEdges[2][minEdgeIndices[2]],maxEdges[0][maxEdgeIndices[0]],maxEdges[1][maxEdgeIndices[1]],maxEdges[2][maxEdgeIndices[2]]);}
	virtual void OnRegisterSceneNode();
};

}
}

#endif
