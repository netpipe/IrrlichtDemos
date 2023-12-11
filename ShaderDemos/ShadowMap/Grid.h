#include "main.h"

#ifndef __CGRID__
#define __CGRID__

class CGrid : public ISceneNode
{
public:

	//! Constructor
	CGrid(ISceneNode* parent, ISceneManager* smgr, s32 id = -1, 
		u32 spacing = 8, u32 size = 1024, video::SColor gridcolor = SColor(255,128,128,128),
		u32 accentlineoffset = 8, video::SColor accentgridcolor = SColor(255,192,192,192),
		bool axislinestate = false);

	//! Pre-Register stuff
	virtual void OnRegisterSceneNode();

	//! Render our grid using 3D lines stored inside the internal meshbuffer
	virtual void render();

	//! Returns our bounding box
	virtual const aabbox3d<f32>& getBoundingBox() const;

	//! Returns the total number of materials, in this case, only 1
	virtual u32 getMaterialCount();

	//! Returns the only material
	virtual SMaterial& getMaterial(u32 i);

	//! Will cause the grid scene node to rebuild it's grid
	void RegenerateGrid();

private:

	u32 m_spacing;
	u32 m_size;
	SColor m_gridcolor;
	SColor m_accentgridcolor;
	u32 m_accentlineoffset;
	bool m_AxisLineState;
	SColor m_XLineColor;
	SColor m_ZLineColor;

	SMeshBuffer Buffer;

};

#endif