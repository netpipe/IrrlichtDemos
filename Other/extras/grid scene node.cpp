#ifndef __C_GRID_SCENE_NODE_H__
#define __C_GRID_SCENE_NODE_H__
 
#include "irrlicht.h"
 
//! Grid scene node 
/*! If you need a grid on the XY or ZY axis, simply rotate this node by 90
degrees in the appropiate axis.
This node creates an XZ grid by default, which should be fine for normal use.
Axis Lines are a default Red and Blue for the X and Z axis respectively.
 
Please note that the internal meshbuffer used for the grid has a max size of 65535 indices.
 
Code is based on the class from here: http://irrlicht.sourceforge.net/forum/viewtopic.php?f=9&t=24938
Thanks goes to MasterGod for helping to clean up the code and for a few bug fixes.
 
Additional thanks to:
JP for optimising the rendering.
Vins for fixing a nasty crash bug and optimising memory usage.
*/
 
class CGridSceneNode : public irr::scene::ISceneNode
{
public:

    enum EAlign
	{
		CENTER,
		MIN_X_MIN_Y
	};

	struct SGrid
	{
		friend class CGridSceneNode;

		SGrid();
		~SGrid();

		SGrid(const SGrid& other);
		SGrid& operator=(const SGrid& other);

		void setVisible(bool visible);
		bool isVisible() const;

		//! If size/spacing doesn't have a modulo of 0 we need to align the grid inside the given size
		void setAlignment(EAlign align);

		//! return grid alignment inside space
		EAlign getAlignment() const;

		//! Sets Spacing
		void setSpacing(irr::f32 newspacing);

		//! Returns the Spacing of the grid
		irr::f32 getSpacing() const;

		//! Sets size
		void setSize(const irr::core::dimension2df& newsize);

		//! Returns the total size of the grid
		const irr::core::dimension2df& getSize() const;

		//! lines are drawn with some offset
		void setOffset(const irr::core::vector3df& offset);

		//! get line offset
		const irr::core::vector3df& getOffset() const;
 
		//! Sets the general grid color
		void setGridColor(irr::video::SColor newcolor);

		//! Returns the grid color
		irr::video::SColor getGridColor() const;

		//! Set the maximal distance the grid can have to the camera to still be rendered
		void setMaxRenderDistance(irr::f32 dist);

		//! Get the maximal distance the grid can have to the camera to still be rendered
		irr::f32 getMaxRenderDistance() const;

		//! Set a new material
		void setMaterial(const irr::video::SMaterial& newMaterial);
		const irr::video::SMaterial& getMaterial() const;
		irr::video::SMaterial& getMaterial();

		// get all grid-lines (tranformed when a matrix is passed as well)
		void getGridLines(irr::core::array<irr::core::line2df> &lines, const irr::core::matrix4* transformation=0) const;

		const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

	protected:
		//! rebuild the grid
		void regenerateMesh(const CGridSceneNode* const gridNode);
		void setDefaultMaterial();

		// render this grid 
		void render(irr::video::IVideoDriver* driver);

		bool canUseGridLine(irr::f32 pos, bool axisX, const CGridSceneNode* const gridNode);

		// First and last line of a grid are for each axis usually a bit inside the grid unless the 
		// grid-size divides exactly by the Spacing. This gives the corners of that inner grid.
		void calcGridInside(irr::core::vector2df& leftTop, irr::core::vector2df& rightBottom) const;

		bool IsVisible;
		EAlign Alignment;
		irr::f32 Spacing;
		irr::core::dimension2df Size;
		irr::core::vector3df Offset;
		irr::video::SColor GridColor;
		irr::f32 MaxRenderDist;			// grid not rendered when distance of the gridplane to the camera is larger
		bool GridDirty;					// grid settings have been changed and all grids need to regenerate the mesh.
		bool BoundingBoxDirty;	// boundingbox was changed, CGridSceneNode needs to recalculate it's boundingbox
		irr::scene::IDynamicMeshBuffer * MeshBuffer;
	};

    CGridSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* smgr, irr::s32 id = -1, irr::u32 numGrids=1);
	~CGridSceneNode();
 
    //! Will create a copy of this scenenode and it's settings
    virtual CGridSceneNode* clone(irr::scene::ISceneNode* newParent = 0, irr::scene::ISceneManager* newSceneManager = 0);
 
    //! Pre-Register stuff
    virtual void OnRegisterSceneNode();
 
    //! Render our grid using 3D lines stored inside the internal meshbuffer
    virtual void render();
 
    //! Returns our bounding box
    virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
 
    //! Returns the total number of materials (identical to number of grids)
    virtual irr::u32 getMaterialCount() const;
 
    //! Returns the only material
    virtual irr::video::SMaterial& getMaterial(irr::u32 i);

	// ! get the number of grids in this node
	irr::u32 getNumberOfGrids() const;

	//! add a new grid and return it's index
	irr::u32 addGrid();

	//! remove an existing grid
	void removeGrid(irr::u32 index);

	//! access a grid
	//*  Note: reference no longer valid when grids are added/removed	*/
	SGrid& getGrid(irr::u32 index);

	//! access a grid
	//*  Note: reference no longer valid when grids are added/removed	*/
	const SGrid& getGrid(irr::u32 index) const;

	//! Sets the size for all grids
	void setGridsSize(const irr::core::dimension2df& newsize);

	//! Get the size used in setGridsSize
	//! Note that individual grids can overwrite that size
	const irr::core::dimension2df& getGridsSize() const;

	//! For debugging: show the selection buffer on next render
	void showSelectionOnce()
	{
		ShowSelectionOnce = true;
	}

protected:
	bool hasDirtyBoundingBox() const;
	bool hasDirtyGrid() const;
	void regenerateMeshes();
	void rebuildBoundingBox();
	void rebuildSelectionMesh();

	// automatic enabling/disabling of grids based on distance of the plane to the camera
	void setGridVisibilityByDistToCamera();

private:
	irr::scene::SMesh* SelectionMesh;
	irr::scene::IDynamicMeshBuffer * SelectionMeshBuffer;
	bool ShowSelectionOnce;
	irr::core::aabbox3d<irr::f32> BoundingBox;
	irr::core::dimension2df GridsSize;
	irr::core::array<SGrid> Grids;
};

#endif // __C_GRID_SCENE_NODE_H__
 