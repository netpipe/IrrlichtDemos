#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __TKTERRAINSCENENODE__
#define __TKTERRAINSCENENODE__

class TKTerrainSceneNode : public scene::ISceneNode
{
	IMesh* mesh;
    ISceneManager* smgr;
    video::SMaterial Material;

public:

	TKTerrainSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, stringc heightMapFile);

    virtual ~TKTerrainSceneNode();

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

	IMesh* getMesh();

	static const int MAPSIZE=2;

	float TERRAIN_HEIGHT=0.5;///Mudar para getHeight()
};

#endif
