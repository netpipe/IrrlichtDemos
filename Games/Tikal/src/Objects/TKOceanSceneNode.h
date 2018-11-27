#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKOCEANSCENENODE__
#define __TKOCEANSCENENODE__

class TKOceanSceneNode : public ISceneNode
{
public:
    TKOceanSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id);
    virtual ~TKOceanSceneNode();
    virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);
	void setHeight(f32 height);

private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;

    SMaterial Material;

    vector3df originalPos;

    static const int MAPSIZE=2;
};

#endif
