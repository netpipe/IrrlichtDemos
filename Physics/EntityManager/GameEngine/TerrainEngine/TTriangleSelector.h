/***************************************************************
*     This file is a modified file of the "Irrlicht Engine".   *
*							Irrlicht engine					   *
*															   *
**************************************************************** 
*File Created: 05.12.08                                        *
*File updated: 05.12.08                                        *
*Author:	Nikolaus Gebhardt, update by TMyke                 *
*Contens: TriangleSelector header					           *
***************************************************************/
#ifndef __T_TRIANGLE_SELECTOR_H_INCLUDED__
#define __T_TRIANGLE_SELECTOR_H_INCLUDED__

#include "ITriangleSelector.h"
#include "IMesh.h"
#include "irrArray.h"

namespace irr
{
namespace scene
{

class ISceneNode;

//! Stupid triangle selector without optimization
class TTriangleSelector : public ITriangleSelector
{
public:

	//! Constructs a selector based on a mesh
	TTriangleSelector(ISceneNode* node);

	//! Constructs a selector based on a mesh
	TTriangleSelector(IMeshBuffer *mbuffer);

	//! Constructs a selector based on a bounding box
	TTriangleSelector(core::aabbox3d<f32> box, ISceneNode* node);

	//! Gets all triangles.
	void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount, 
		const core::matrix4* transform=0) const;

	//! Gets all triangles which lie within a specific bounding box.
	void getTriangles(core::triangle3df* triangles, s32 arraySize, s32& outTriangleCount, 
		const core::aabbox3d<f32>& box, const core::matrix4* transform=0) const;

	//! Gets all triangles which have or may have contact with a 3d line.
	virtual void getTriangles(core::triangle3df* triangles, s32 arraySize,
		s32& outTriangleCount, const core::line3d<f32>& line, 
		const core::matrix4* transform=0) const;

	//! Returns amount of all available triangles in this selector
	virtual s32 getTriangleCount() const;

protected:

	ISceneNode* SceneNode;
	mutable core::array<core::triangle3df> Triangles;
};

} // end namespace scene
} // end namespace irr


#endif