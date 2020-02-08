// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "grid_triangle_selector.h"

using namespace irr;

GridTriangleSelector::GridTriangleSelector(const irr::scene::IMesh* mesh, irr::scene::ISceneNode* node, irr::f32 cellSize)
: mSceneNode(node), mCellSize(cellSize)
{
	if ( mCellSize == 0.f )
		mCellSize = 1.f;

    updateMesh(mesh);
}

GridTriangleSelector::GridTriangleSelector::~GridTriangleSelector()
{
}

irr::s32 GridTriangleSelector::getTriangleCount() const
{
	return (irr::s32)mTriangles.size();
}

void GridTriangleSelector::getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
	irr::s32& outTriangleCount, const irr::core::matrix4* transform) const
{
	u32 cnt = mTriangles.size();
	if (cnt > (u32)arraySize)
		cnt = (u32)arraySize;

	core::matrix4 mat;
	if (transform)
		mat = *transform;
	if (mSceneNode)
		mat *= mSceneNode->getAbsoluteTransformation();

	bool isIdentityMatrix = mat.isIdentity();
	if (isIdentityMatrix)
	{
		for (u32 i=0; i<cnt; ++i)
		{
			triangles[i] = mTriangles[i];
		}
	}
	else
	{
		for (u32 i=0; i<cnt; ++i)
		{
			mat.transformVect( triangles[i].pointA, mTriangles[i].pointA );
			mat.transformVect( triangles[i].pointB, mTriangles[i].pointB );
			mat.transformVect( triangles[i].pointC, mTriangles[i].pointC );
		}
	}

	outTriangleCount = (s32)cnt;
}

void GridTriangleSelector::getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
	irr::s32& outTriangleCount, const irr::core::aabbox3d<irr::f32>& box, const irr::core::matrix4* transform) const
{
	core::matrix4 mat(core::matrix4::EM4CONST_NOTHING);
	core::aabbox3df tBox(box);

	if (mSceneNode)
	{
		mSceneNode->getAbsoluteTransformation().getInverse(mat);
		mat.transformBoxEx(tBox);
	}
	if (transform)
		mat = *transform;
	else
		mat.makeIdentity();
	if (mSceneNode)
		mat *= mSceneNode->getAbsoluteTransformation();
	bool isIdentityMatrix = mat.isIdentity();

	// get grid-cells hit by box
	core::rectf boxRect(box.MinEdge.X, box.MinEdge.Z, box.MaxEdge.X, box.MaxEdge.Z);
	irr::s32 startX, startY, endX, endY;
	getGridCoordinates(boxRect.UpperLeftCorner, startX, startY);
	getGridCoordinates(boxRect.LowerRightCorner, endX, endY);

	outTriangleCount = 0;

	if ( !mGrid.empty() )
	{
		memset((void*)mUsed.pointer(), 0, sizeof(bool)*mUsed.size());

		if (isIdentityMatrix)
		{
			for ( s32 y=startY; y<=endY; ++y)
			{
				for ( s32 x=startX; x<=endX; ++x)
				{
					const irr::core::array<irr::u32>& triangleIndices = mGrid[y*mGridDimension.Width+x].mTriangleIndices;
					for ( size_t i=0; i < triangleIndices.size(); ++i )
					{
						u32 index = triangleIndices[i];
						if ( !mUsed[index] )
						{
							mUsed[index] = true;
							core::triangle3df triangle(mTriangles[index]);
	//						if ( isIntersectingXZ(boxRect, triangle) )	// kicks out a few more triangles, but doing that too slow so far
							core::rectf triangleRect(getTriangleRectXZ(triangle));
							if ( triangleRect.isRectCollided(boxRect) )
							{
								triangles[outTriangleCount] = triangle;

								++outTriangleCount;
								if (outTriangleCount == arraySize)
								{
									return;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			for ( s32 y=startY; y<=endY; ++y)
			{
				for ( s32 x=startX; x<=endX; ++x)
				{
					const irr::core::array<irr::u32>& triangleIndices = mGrid[y*mGridDimension.Width+x].mTriangleIndices;
					for ( size_t i=0; i < triangleIndices.size(); ++i )
					{
						u32 index = triangleIndices[i];
						if ( !mUsed[index] )
						{
							mUsed[index] = true;
							core::triangle3df triangle(mTriangles[index]);
	//						if ( isIntersectingXZ(boxRect, triangle) )	// kicks out a few more triangles, but doing that too slow so far
							core::rectf triangleRect(getTriangleRectXZ(triangle));
							if ( triangleRect.isRectCollided(boxRect) )
							{
								mat.transformVect(triangles[outTriangleCount].pointA, triangle.pointA);
								mat.transformVect(triangles[outTriangleCount].pointB, triangle.pointB);
								mat.transformVect(triangles[outTriangleCount].pointC, triangle.pointC);

								++outTriangleCount;
								if (outTriangleCount == arraySize)
								{
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	// Could use normal triangles as fallback, but probably better failing completely so users notice things have gone wrong
}

void GridTriangleSelector::getTriangles(irr::core::triangle3df* triangles, irr::s32 arraySize,
	irr::s32& outTriangleCount, const irr::core::line3d<irr::f32>& line,const irr::core::matrix4* transform) const
{
	// TODO: no optimization for that so far

	irr::core::aabbox3d<irr::f32> box(line.start);
	box.addInternalPoint(line.end);
irr::core::array<irr::scene::SCollisionTriangleRange>* outTriangleInfo;

	getTriangles(triangles, arraySize, outTriangleCount, box, transform,true,outTriangleInfo);

//		getTriangles(triangles, arraySize, outTriangleCount,box, transform);
}

void GridTriangleSelector::updateMesh(const irr::scene::IMesh* mesh)
{
    mGrid.clear();
    mTriangles.clear();
    mUsed.clear();

	const irr::core::aabbox3df& gridSize = mesh->getBoundingBox();
    mGridStart.X = gridSize.MinEdge.X;
    mGridStart.Y = gridSize.MinEdge.Z;
    mGridEnd.X = gridSize.MaxEdge.X;
    mGridEnd.Y = gridSize.MaxEdge.Z;

    mGridDimension.Width = (irr::u32)ceil((mGridEnd.X-mGridStart.X) / mCellSize);
    mGridDimension.Height = (irr::u32)ceil((mGridEnd.Y-mGridStart.Y) / mCellSize);
    irr::core::vector2df extent(mGridDimension.Width*mCellSize, mGridDimension.Height*mCellSize);
    extent *= 0.5f;
    irr::core::vector2df gridCenter((mGridStart+mGridEnd)*0.5f);
    mGridStart = gridCenter - extent;
    mGridEnd = gridCenter + extent;

    irr::u32 gridLen = mGridDimension.Width*mGridDimension.Height;
    mGrid.reallocate(gridLen);
    for ( u32 i=0; i< gridLen; ++i)
		mGrid.push_back(Cell());	// set_used does not initialize so we can't use that (yeah... dangerous and confusing stuff...)

    // Check how many primitives we can expect
    u32 indexCount = 0;
    for ( u32 i=0; i<mesh->getMeshBufferCount(); ++i )
    {
		indexCount += mesh->getMeshBuffer(i)->getIndexCount();
	}

    irr::u32 numTrianglesReserved = indexCount/3;
    if (numTrianglesReserved > 0)
    {
		mTriangles.reallocate(numTrianglesReserved);
		mUsed.reallocate(numTrianglesReserved);
	}

    for ( u32 i=0; i<mesh->getMeshBufferCount(); ++i )
    {
		addMeshBuffer(mesh->getMeshBuffer(i));
	}
}

void GridTriangleSelector::addMeshBuffer(const irr::scene::IMeshBuffer* meshBuffer)
{
	u32 indexCount = meshBuffer->getIndexCount();
	const u16 * indices = meshBuffer->getIndices();
	switch(meshBuffer->getVertexType())
	{
		case video::EVT_STANDARD:
		{
			video::S3DVertex* vertices = (video::S3DVertex*)meshBuffer->getVertices();
			for ( u32 i=0; i<indexCount; i += 3 )
			{
				addTriangle(core::triangle3df(vertices[indices[i]].Pos, vertices[indices[i+1]].Pos, vertices[indices[i+2]].Pos));
			}
		}
		break;
		case video::EVT_2TCOORDS:
		{
			video::S3DVertex2TCoords* vertices = (video::S3DVertex2TCoords*)meshBuffer->getVertices();
			for ( u32 i=0; i<indexCount; i += 3 )
			{
				addTriangle(core::triangle3df(vertices[indices[i]].Pos, vertices[indices[i+1]].Pos, vertices[indices[i+2]].Pos));
			}
		}
		break;
		case video::EVT_TANGENTS:
		{
			video::S3DVertexTangents* vertices = (video::S3DVertexTangents*)meshBuffer->getVertices();
			for ( u32 i=0; i<indexCount; i += 3 )
			{
				addTriangle(core::triangle3df(vertices[indices[i]].Pos, vertices[indices[i+1]].Pos, vertices[indices[i+2]].Pos));
			}
		}
		break;
	}
}

void GridTriangleSelector::addTriangle(const irr::core::triangle3df& triangle)
{
	mTriangles.push_back(triangle);
	mUsed.push_back(false);
	u32 index = mTriangles.size()-1;

	if ( !mGrid.empty() )
	{
		irr::core::rectf rect = getTriangleRectXZ(triangle);
		irr::s32 startX, startY, endX, endY;
		getGridCoordinates(rect.UpperLeftCorner, startX, startY);
		getGridCoordinates(rect.LowerRightCorner, endX, endY);
		for ( s32 y=startY; y<=endY; ++y)
		{
			for ( s32 x=startX; x<=endX; ++x)
			{
				irr::core::rectf cellRect = GridTriangleSelector::getCellRect(x, y);
				if ( isIntersectingXZ(cellRect, triangle) )
				{
					Cell& cell = mGrid[y*mGridDimension.Width+x];
					cell.mTriangleIndices.push_back(index);
				}
			}
		}
	}
}

void GridTriangleSelector::getGridCoordinates(const irr::core::vector2df& position, irr::s32& x, irr::s32& y, bool clip) const
{
	x = (s32)((position.X - mGridStart.X) / mCellSize);
	y = (s32)((position.Y - mGridStart.Y) / mCellSize);

	if (clip)
	{
		if( x >= (s32)mGridDimension.Width )
			x = (s32)mGridDimension.Width-1;
		if ( x < 0 )
			x = 0;
		if( y >= (s32)mGridDimension.Height )
			y = (s32)mGridDimension.Height-1;
		if ( y < 0 )
			y = 0;
	}
}

irr::core::rectf GridTriangleSelector::getCellRect(irr::s32 x, irr::s32 y) const
{
	irr::core::rectf result;
	result.UpperLeftCorner.X = mGridStart.X + mCellSize*x;
	result.UpperLeftCorner.Y = mGridStart.Y + mCellSize*y;
	result.LowerRightCorner.X = result.UpperLeftCorner.X + mCellSize;
	result.LowerRightCorner.Y = result.UpperLeftCorner.Y + mCellSize;
	return result;
}

irr::core::rectf GridTriangleSelector::getTriangleRectXZ(const irr::core::triangle3df& triangle) const
{
	irr::core::rectf result;
	result.UpperLeftCorner.X  = core::min_(triangle.pointA.X, triangle.pointB.X, triangle.pointC.X);
	result.UpperLeftCorner.Y  = core::min_(triangle.pointA.Z, triangle.pointB.Z, triangle.pointC.Z);
	result.LowerRightCorner.X = core::max_(triangle.pointA.X, triangle.pointB.X, triangle.pointC.X);
	result.LowerRightCorner.Y = core::max_(triangle.pointA.Z, triangle.pointB.Z, triangle.pointC.Z);

	return result;
}

bool GridTriangleSelector::isIntersectingXZ(const irr::core::rectf& rect, const irr::core::triangle3df& triangle) const
{
	if ( rect.isPointInside(core::vector2df(triangle.pointA.X, triangle.pointA.Z)) ||
		 rect.isPointInside(core::vector2df(triangle.pointB.X, triangle.pointB.Z)) ||
		 rect.isPointInside(core::vector2df(triangle.pointC.X, triangle.pointC.Z)) ||
		 isPointInsideXZ(rect.getCenter(), triangle) )
		 return true;

	// Yeah, that's slow. Could be precalculated. Or at least rect line re-used.
	// There is even another algorithm out there which is faster for axis aligned rectangles, but have no time right now get through that.
	core::line2df ab(triangle.pointA.X, triangle.pointA.Z, triangle.pointB.X, triangle.pointB.Z);
	core::line2df bc(triangle.pointB.X, triangle.pointB.Z, triangle.pointC.X, triangle.pointC.Z);
	core::line2df ca(triangle.pointC.X, triangle.pointC.Z, triangle.pointA.X, triangle.pointA.Z);

	core::line2df r0(rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X, rect.UpperLeftCorner.Y);
	core::line2df r1(rect.LowerRightCorner.X, rect.UpperLeftCorner.Y, rect.LowerRightCorner.X, rect.LowerRightCorner.Y);
	core::line2df r2(rect.LowerRightCorner.X, rect.LowerRightCorner.Y, rect.UpperLeftCorner.X, rect.LowerRightCorner.Y);
	core::line2df r3(rect.UpperLeftCorner.X, rect.LowerRightCorner.Y, rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y);

	core::vector2df dummyOut;
	return 	ab.intersectWith(r0, dummyOut, true) || ab.intersectWith(r1, dummyOut, true) ||
			ab.intersectWith(r2, dummyOut, true) || ab.intersectWith(r3, dummyOut, true) ||
			bc.intersectWith(r0, dummyOut, true) || bc.intersectWith(r1, dummyOut, true) ||
			bc.intersectWith(r2, dummyOut, true) || bc.intersectWith(r3, dummyOut, true) ||
			ca.intersectWith(r0, dummyOut, true) || ca.intersectWith(r1, dummyOut, true) ||
			ca.intersectWith(r2, dummyOut, true) || ca.intersectWith(r3, dummyOut, true);
}

bool GridTriangleSelector::isPointInsideXZ(const irr::core::vector2df& point, const irr::core::triangle3df& triangle) const
{
	core::vector2df v0(triangle.pointC.X - triangle.pointA.X, triangle.pointC.Z - triangle.pointA.Z);
	core::vector2df v1(triangle.pointB.X - triangle.pointA.X, triangle.pointB.Z - triangle.pointA.Z);
	core::vector2df v2(point.X - triangle.pointA.X, point.Y - triangle.pointA.Z);

	f64 dot00 = v0.dotProduct(v0);
	f64 dot01 = v0.dotProduct(v1);
	f64 dot02 = v0.dotProduct(v2);
	f64 dot11 = v1.dotProduct(v1);
	f64 dot12 = v1.dotProduct(v2);

	// Compute barycentric coordinates
	f64 denom = dot00 * dot11 - dot01 * dot01;
	if ( denom == 0 )
		return false;
	f64 u = (dot11 * dot02 - dot01 * dot12) / denom;
	f64 v = (dot00 * dot12 - dot01 * dot02) / denom;

	const f64 epsilon = core::ROUNDING_ERROR_f32;	// don't use f64 rounding error, errors tend to get larger
	return (u >= -epsilon) && (v >= -epsilon) && (u + v <= 1.0+epsilon);
}
