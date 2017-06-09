#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKSKYSCENENODE__
#define __TKSKYSCENENODE__

class TKSkySceneNode : public ISceneNode
{
public:
    TKSkySceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);
    virtual ~TKSkySceneNode();
    virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);

private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;

    SMaterial Material;

    vector3df originalPos;
};

#endif
