// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GRID_TRIANGLE_SELECTOR_H
#define GRID_TRIANGLE_SELECTOR_H

#include <irrlicht.h>
#include <vector>

//! Triangles in a 2d-grid.
//! 2D grid is using the X,Z coordinates of the triangles.
class GridTriangleSelector : public irr::scene::ITriangleSelector
{
public:

	GridTriangleSelector(const irr::scene::IMesh* mesh, irr::scene::ISceneNode* node, irr::f32 cellSize);

	virtual ~GridTriangleSelector();

	//! Get amount of all available triangles in this selector
	virtual irr::s32 getTriangleCount() const;

	//! Gets all triangle in the selector
	virtual void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::matrix4* transform=0) const;

	//! Gets all triangles within a specific bounding box.
	virtual void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::aabbox3d<irr::f32>& box,
		const irr::core::matrix4* transform=0) const;

	//! Gets the triangles for one associated node which have or may have contact with a 3d line.
	// NOTE: not optimized
	virtual void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::line3d<irr::f32>& line,
		const irr::core::matrix4* transform=0) const;


	void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::matrix4* transform=0,
		bool useNodeTransform=true,
		irr::core::array<irr::scene::SCollisionTriangleRange>* outTriangleInfo=0) const {};

	 void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::line3d<irr::f32>& line,
		const irr::core::matrix4* transform=0, bool useNodeTransform=true,
		irr::core::array<irr::scene::SCollisionTriangleRange>* outTriangleInfo=0) const {};


	 void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::aabbox3d<irr::f32>& box,
		const irr::core::matrix4* transform=0);


	 void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
		irr::s32& outTriangleCount, const irr::core::aabbox3d<irr::f32>& box,
		const irr::core::matrix4* transform=0, bool useNodeTransform=true,
		irr::core::array<irr::scene::SCollisionTriangleRange>* outTriangleInfo=0) const{};

//	virtual void getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
//		irr::s32& outTriangleCount, const irr::core::aabbox3d<irr::f32>& box,
//		const irr::core::matrix4* transform=0, bool useNodeTransform=true,
//		irr::core::array<irr::scene::SCollisionTriangleRange>* outTriangleInfo=0) const {};



	//! Return scenenode for the selector
	virtual irr::scene::ISceneNode* getSceneNodeForTriangle(irr::u32 triangleIndex) const
	{
		return mSceneNode;
	}

	//! Get number of TriangleSelectors that are part of this one
	virtual irr::u32 getSelectorCount() const
	{
		return 1;
	}

	//! Get TriangleSelector based on index based on getSelectorCount
	virtual irr::scene::ITriangleSelector* getSelector(irr::u32 index)
	{
		return this;
	}

	//! Get TriangleSelector based on index based on getSelectorCount
	virtual const irr::scene::ITriangleSelector* getSelector(irr::u32 index) const
	{
		return this;
	}

protected:
	void updateMesh(const irr::scene::IMesh* mesh);
	void addMeshBuffer(const irr::scene::IMeshBuffer* meshBuffer);
	void addTriangle(const irr::core::triangle3df& triangle);

	// For the given position return the x and y grid coordinates.
	void getGridCoordinates(const irr::core::vector2df& position, irr::s32& x, irr::s32& y, bool clip=true) const;

	// Return the rectangle making up the cell for the given grid coordinates
	irr::core::rectf getCellRect(irr::s32 x, irr::s32 y) const;

	// For the given triangle return a rectf enclosing the XZ coordinates of the triangle
	irr::core::rectf getTriangleRectXZ(const irr::core::triangle3df& triangle) const;

	// Check if the triangle and the rectangle are intersecting when only regarding the X,Z coordinates of the triangle
	bool isIntersectingXZ(const irr::core::rectf& rect, const irr::core::triangle3df& triangle) const;

	// Check if the given point is inside the triangle regarding only it's XZ coordinates
	bool isPointInsideXZ(const irr::core::vector2df& point, const irr::core::triangle3df& triangle) const;

private:
    struct Cell
    {
		irr::core::array<irr::u32> mTriangleIndices;
    };

    irr::scene::ISceneNode* mSceneNode;
    irr::f32 mCellSize;
    irr::core::dimension2du mGridDimension;
    irr::core::vector2df mGridStart;
    irr::core::vector2df mGridEnd;
    irr::core::array<Cell> mGrid;

    irr::core::array<irr::core::triangle3df> mTriangles;
    mutable irr::core::array<bool> mUsed;
};

#endif // GRID_TRIANGLE_SELECTOR_H
