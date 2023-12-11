/***************************************************************
*     This file is a modified file of the "Irrlicht Engine".   *
*							Irrlicht engine					   *
*															   *
**************************************************************** 
*File Created: 05.12.08                                        *
*File updated: 05.12.08                                        *
*Author:	Nikolaus Gebhardt, update by TMyke                 *
*Contens: TriangleSelector code						           *
***************************************************************/
#include "TTriangleSelector.h"
#include "ISceneNode.h"
#include "IMeshBuffer.h"

namespace irr
{
namespace scene
{

//! constructor
TTriangleSelector::TTriangleSelector(ISceneNode* node)
: SceneNode(node)
{
}


//! constructor
TTriangleSelector::TTriangleSelector(IMeshBuffer* buf)
{


		s32 idxCnt = buf->getIndexCount();
		const u16* const indices = buf->getIndices();
		core::triangle3df tri;

		video::S3DVertex2TCoords* vtx = (video::S3DVertex2TCoords*)buf->getVertices();
		for (s32 j=0; j<idxCnt; j+=3)
		{
			Triangles.push_back(core::triangle3df(	vtx[indices[j+0]].Pos,
													vtx[indices[j+1]].Pos,
													vtx[indices[j+2]].Pos));
		}
	SceneNode=NULL;
}


//! constructor
TTriangleSelector::TTriangleSelector(core::aabbox3d<f32> box, ISceneNode* node)
: SceneNode(node)
{
	// TODO
}


//! Gets all triangles.
void TTriangleSelector::getTriangles(core::triangle3df* triangles,
									s32 arraySize, s32& outTriangleCount, 
									const core::matrix4* transform) const
{
	s32 cnt = Triangles.size();
	if (cnt > arraySize)
		cnt = arraySize;

	core::matrix4 mat;

	if (transform)		mat = *transform;

	if (SceneNode)		mat *= SceneNode->getAbsoluteTransformation();

	for (s32 i=0; i<cnt; ++i)
	{
		triangles[i] = Triangles[i];
		mat.transformVect(triangles[i].pointA);
		mat.transformVect(triangles[i].pointB);
		mat.transformVect(triangles[i].pointC);
	}
	outTriangleCount = cnt;
}



//! Gets all triangles which lie within a specific bounding box.
void TTriangleSelector::getTriangles(core::triangle3df* triangles, 
					s32 arraySize, s32& outTriangleCount, 
					const core::aabbox3d<f32>& box,
					const core::matrix4* transform) const
{
	// return all triangles
	getTriangles(triangles, arraySize, outTriangleCount, transform);
}


//! Gets all triangles which have or may have contact with a 3d line.
void TTriangleSelector::getTriangles(core::triangle3df* triangles,
					s32 arraySize, s32& outTriangleCount,
					const core::line3d<f32>& line,
					const core::matrix4* transform) const
{
	// return all triangles
	getTriangles(triangles, arraySize, outTriangleCount, transform);
}


//! Returns amount of all available triangles in this selector
s32 TTriangleSelector::getTriangleCount() const
{
	return Triangles.size();
}



} // end namespace scene
} // end namespace irr

