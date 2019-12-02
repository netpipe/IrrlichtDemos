#ifndef __C_GRID_SCENE_NODE_H__
#define __C_GRID_SCENE_NODE_H__

#include "irrlicht.h"

using namespace irr;
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;

//! Grid scene node
/*! If you need a grid on the XY or ZY axis, simply rotate this node by 90 
degrees in the appropiate axis.
This node creates an XZ grid by default, which should be fine for normal use.
Axis Lines are a default Red and Blue for the X and Z axis respectively.

Thanks goes to MasterGod for helping to clean up the code and for a few
bug fixes.
*/
class CGridSceneNode : public ISceneNode
{
public:
	//! Constructor
	CGridSceneNode(ISceneNode* parent_rootSceneNode, ISceneManager* smgr, s32 id = -1, 
		u32 spacing = 8, u32 size = 1024, SColor gridcolor = SColor(255,128,128,128), u32 accentlineoffset = 8, 
		SColor accentgridcolor = SColor(255,192,192,192), bool axislinestate = false)	: ISceneNode(parent_rootSceneNode, smgr, id), 
		m_spacing(spacing), m_size(size), 
		m_gridcolor(gridcolor), m_accentlineoffset(accentlineoffset), 
		m_accentgridcolor(accentgridcolor), m_AxisLineState(axislinestate),
		m_XLineColor(SColor(255,255,0,0)), m_ZLineColor(SColor(255,0,0,255))
	{
		// Set the material
		Material.Wireframe = false;
		Material.Lighting = false;
		Material.Thickness = 1;
		Material.FogEnable = false;

		/* We divide this here to save some calculations and speed up rendering time
		   (Keep in mind that from now on, m_size is actually half the total size.
		   This means that we'll need to multiply the size by 2 whenever we give
		   the size back to the user) */
		m_size /= 2;

		// Create our box, it is the size of the grid exactly, plus 1 in the Y axis
		Box = aabbox3df(-f32(m_size),-1.0f,-f32(m_size),f32(m_size),1.0f,f32(m_size));

		// Set the default culling state to Frustum Box
		AutomaticCullingState = EAC_FRUSTUM_BOX;
	}

	//! Will create a copy of this scenenode and it's settings
	virtual CGridSceneNode* Clone(ISceneNode* newParent = 0, ISceneManager* newSceneManager = 0);

	//! Pre-Register stuff
	virtual void OnRegisterSceneNode();

	//! Render our grid using 3D lines
	virtual void render();

	//! Returns our bounding box
	virtual const aabbox3d<f32>& getBoundingBox() const;

	//! Returns the total number of materials, in this case, only 1
	virtual u32 getMaterialCount();

	//! Returns the only material
	virtual SMaterial& getMaterial(u32 i);

	//! Returns the Spacing of the grid
	u32 GetSpacing();

	//! Returns the total size of the grid
	u32 GetSize();

	//! Returns the Grid Color
	SColor GetGridColor();

	//! Returns the offset of the accent lines
	u32 GetAccentlineOffset();

	//! Returns the Accent Line Color
	SColor GetAccentlineColor();

	//! Returns the Active State of the Axis Lines
	bool AreAxisLineActive();

	//! Returns the Color of the "X" axis lines
	SColor GetAxisLineXColor();

	//! Returns the Color of the "Z" axis lines
	SColor GetAxisLineZColor();

	//! Sets Spacing
	void SetSpacing(u32 newspacing);

	//! Sets Size
	void SetSize(u32 newsize);

	//! Sets the general grid color
	void SetGridColor(SColor newcolor);

	//! Sets the offset for the accent lines
	void SetAccentlineOffset(u32 newoffset);

	//! Sets the color of the accent lines
	void SetAccentlineColor(SColor newcolor);

	//! Sets the Active State of the Axis Lines
	void SetAxisLineActive(bool active);

	//! Sets the Color of the "X" axis lines
	void SetAxisLineXColor(SColor XLine);
	
	//! Sets the Color of the "Z" axis lines
	void SetAxisLineZColor(SColor ZLine);

	//! Allows for setting a complete new material
	void SetMaterial(SMaterial newMaterial);

private:
	aabbox3df Box;
	SMaterial Material;
	u32 m_spacing;
	u32 m_size;
	SColor m_gridcolor;
	SColor m_accentgridcolor;
	u32 m_accentlineoffset;
	bool m_AxisLineState;
	SColor m_XLineColor;
	SColor m_ZLineColor;
};

#endif // __C_GRID_SCENE_NODE_H__
