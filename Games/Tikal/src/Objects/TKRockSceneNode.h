#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifndef __TKROCKSCENENODE__
#define __TKROCKSCENENODE__

class TKRockSceneNode : public ISceneNode
{
public:
    TKRockSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, int maxDistance);
    virtual ~TKRockSceneNode();
    virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);

private:
    ISceneManager* smgr;

    IAnimatedMesh* mesh;

    SMaterial Material;

    f32 MaxDistance;

    vector3df originalPos;
};

#endif

