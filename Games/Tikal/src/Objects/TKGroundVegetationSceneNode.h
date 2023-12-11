#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifndef __TKGROUNDVEGETATIONSCENENODE__
#define __TKGROUNDVEGETATIONSCENENODE__

class TKGroundVegetationSceneNode : public ISceneNode
{
public:
    TKGroundVegetationSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, int maxDistance);
    virtual ~TKGroundVegetationSceneNode();
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

    static const bool DETAIL_HIGH=false;
};

#endif
