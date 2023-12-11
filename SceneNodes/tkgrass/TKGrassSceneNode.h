#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKGRASSSCENENODE__
#define __TKGRASSSCENENODE__

class TKGrassSceneNode : public ISceneNode
{
public:
    TKGrassSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id, s32 material, int maxDistance);
    virtual ~TKGrassSceneNode();
    virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual u32 getMaterialCount() const;
	virtual video::SMaterial& getMaterial(u32 i);


private:
    int idGrass;

    ISceneManager* smgr;

    IAnimatedMesh* mesh;//GLSL Billboard. You can also use the BillboardSceneNode... but you lose the render control (LOD)

    SMaterial Material;

    f32 MaxDistance;

    vector3df originalPos;
};

#endif
