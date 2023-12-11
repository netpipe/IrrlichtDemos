#ifndef __C_HW_BILLBOARD_SCENE_NODE_H_INCLUDED__
#define __C_HW_BILLBOARD_SCENE_NODE_H_INCLUDED__

#include "CBatchedBillboardSceneNode.h"
#include "IShaderConstantSetCallBack.h"

namespace irr
{
namespace scene
{


class CHardwareBillboardSceneNode : public CBatchedBillboardSceneNode, video::IShaderConstantSetCallBack
{
    video::SMaterial material;
public:
    CHardwareBillboardSceneNode(ISceneNode* parent, ISceneManager* mgr,
        IBillboardSceneNode** nodes=0, u32 count=0, core::array<IEmptyBillboardSceneNode*> *impostorsOut=0,  s32 id=-1,
		const core::vector3df& position = core::vector3df(0,0,0));
    ~CHardwareBillboardSceneNode();

	virtual IEmptyBillboardSceneNode* addBillboard(IBillboardSceneNode* node);

	virtual void render();
	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);
    virtual video::SMaterial& getMaterial(u32 num) {return material;}
    virtual u32 getMaterialCount() {return 1;}
};

}
}

#endif
