
#ifndef __CTILENODE_H
#define __CTILENODE_H

#include <irrlicht.h>
#include <ode/ode.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

namespace irr { namespace scene {

struct CTileNodeInfo
{
    dSpaceID dSpace;
    int sin_x;
    int randomness;
};

// perhaps do away with this struct UNLESS using function for terrain instead of random
struct CTileNodeSidePoint
{
    //s32 a, b;
    f32 height;
};

class CTileNode : public scene::ISceneNode
{
    ISceneManager *smgr;

	aabbox3d<f32> box;
	S3DVertex *vertices;
	u16 *indices;
	SMaterial material;
	u32 vcount, tricount, i, j;
	CTileNodeSidePoint *sides[4];

	// user options
	f32 maxHeight;
	f32 scale;

	// custom info structure (type of terrain etc)
	CTileNodeInfo info;

	// extra geometry
	array <dTriMeshDataID> dTriMeshData;
	array <dGeomID> dGeom;
	array <vector3df> dGeomRelPosition; // each geom must have one (even if 0,0,0)
	array <dVector3 *> dVertices;
	array <int *> dIndices;

public:

    CTileNode(ISceneNode *parent, ISceneManager *smgr, s32 id, f32 tileWidth, u32 width, vector3df pos,
    CTileNode **tiles, CTileNodeInfo);

    ~CTileNode();

    f32 getHeight(f32, f32);
    void addGeom(s32, vector3df);

    virtual void setPosition(const core::vector3df& newpos)
    {
        RelativeTranslation = newpos;
        updateAbsolutePosition();

        // set all ODE geoms with their relative positions
        for (i = 0; i < dGeom.size(); i ++)
        {
            vector3df pos = dGeomRelPosition[i];
            dGeomSetPosition(dGeom[i], newpos.X+pos.X, newpos.Y+pos.Y, newpos.Z+pos.Z);
        }
    }

	virtual void OnRegisterSceneNode()
	{
        updateAbsolutePosition();
		if (IsVisible) SceneManager->registerNodeForRendering(this);
		ISceneNode::OnRegisterSceneNode();
	}

	virtual void render()
	{
		IVideoDriver *driver = SceneManager->getVideoDriver();
		driver->setMaterial(material);
		driver->setTransform(ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(vertices, vcount, indices, tricount);
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return box;
	}
	virtual s32 getMaterialCount()
	{
		return 1;
	}
	virtual SMaterial& getMaterial(s32 i)
	{
		return material;
	}
};

};
};

#endif
