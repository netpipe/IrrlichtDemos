#ifndef CWATERMESH_H_HG_INCLUDE
#define CWATERMESH_H_HG_INCLUDE

#include <irrlicht.h>

class CWaterMesh
{
public:
	CWaterMesh(	irr::core::dimension2d<irr::f32> tileSize,
				irr::core::dimension2d<irr::u32> tileCount);

	~CWaterMesh();

	irr::scene::IMesh* getMesh() {return mesh;}

	irr::video::S3DVertex& getVertex(irr::u32 x, irr::u32 y);

	irr::core::dimension2d<irr::u32>& getTileCount() {return tileCnt;}

	irr::core::dimension2d<irr::f32>& getTileSize() {return tileSze;}

	void setDirty();

	void recalculateNormals();

	void recalculateBoundingBox();

private:
	irr::scene::SMesh* mesh;
	irr::scene::SMeshBuffer* meshBuffer;
	irr::core::dimension2d<irr::u32> tileCnt;
	irr::core::dimension2d<irr::f32> tileSze;
};

#endif