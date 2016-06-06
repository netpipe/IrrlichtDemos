// Copyright (C) 2009 Gaz Davidson
// Released under The Irrlicht Engine License, see irrlicht.h

#include "CBeamSceneNode2.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "SViewFrustum.h"
#include "ISceneCollisionManager.h"

namespace irr
{
namespace scene
{

	u16 indices[] = {0,2,1, 0,3,2, 8,10,9, 8,11,10, 4,6,5, 5,6,7 };

//! constructor
CBeamSceneNode2::CBeamSceneNode2(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& startPosition, const core::vector3df& endPosition, f32 size)
	: ISceneNode(parent, mgr, id), Size(size)
{
	#ifdef _DEBUG
	setDebugName("CBeamSceneNode2");
	#endif

	Material.ZWriteEnable = false;
	Material.Lighting = false;

	setPosition(startPosition, endPosition);

	// back billboard
	vertices[0].TCoords.set(1.0f, 1.0f);
	vertices[1].TCoords.set(1.0f, 0.0f);
	vertices[2].TCoords.set(0.0f, 0.0f);
	vertices[3].TCoords.set(0.0f, 1.0f);

	// line
	vertices[4].TCoords.set(0.5f, 0.0f);
	vertices[5].TCoords.set(0.5f, 1.0f);
	vertices[6].TCoords.set(0.5f, 0.0f);
	vertices[7].TCoords.set(0.5f, 1.0f);

	// front billboard
	vertices[ 8].TCoords.set(1.0f, 1.0f);
	vertices[ 9].TCoords.set(1.0f, 0.0f);
	vertices[10].TCoords.set(0.0f, 0.0f);
	vertices[11].TCoords.set(0.0f, 1.0f);

	video::SColor c(255,255,255,255);

	for (u32 i=0; i<12; ++i)
		vertices[i].Color = c;

}


//! pre render event
void CBeamSceneNode2::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


//! render
void CBeamSceneNode2::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	//
	// make billboards face the camera
	//

	core::vector3df campos = camera->getAbsolutePosition();
	core::vector3df target = camera->getTarget();
	core::vector3df up = camera->getUpVector();
	core::vector3df view = target - campos;
	view.normalize();

	core::vector3df horizontal = up.crossProduct(view);
	if ( horizontal.getLength() == 0 )
		horizontal.set(up.Y,up.X,up.Z);

	horizontal.normalize();
	horizontal *= 0.5f * Size;

	core::vector3df vertical = horizontal.crossProduct(view);
	vertical.normalize();
	vertical *= 0.5f * Size;

	view *= -1.0f;

	core::vector3df startPos = getAbsolutePosition();
	core::vector3df endPos   = EndPos;
	AbsoluteTransformation.transformVect(endPos);

	// start billboard
	vertices[ 0].Pos = startPos + horizontal + vertical;
	vertices[ 1].Pos = startPos + horizontal - vertical;
	vertices[ 2].Pos = startPos - horizontal - vertical;
	vertices[ 3].Pos = startPos - horizontal + vertical;

	// end billboard
	vertices[ 8].Pos = endPos + horizontal + vertical;
	vertices[ 9].Pos = endPos + horizontal - vertical;
	vertices[10].Pos = endPos - horizontal - vertical;
	vertices[11].Pos = endPos - horizontal + vertical;

	// set normals
	for (s32 i=0; i<12; ++i)
		vertices[i].Normal = view;

	//
	// get screen space angle of beam
	//

	core::line3df l(startPos, endPos);
	//camera->getViewFrustum()->clipLine(l);

	// project to screen
	core::matrix4 trans = camera->getProjectionMatrix();
	trans *= camera->getViewMatrix();
	f32 s[4] = { l.start.X, l.start.Y, l.start.Z, 1.0f };
	f32 e[4] = { l.end.X,   l.end.Y,   l.end.Z,   1.0f };
	trans.multiplyWith1x4Matrix(s);
	trans.multiplyWith1x4Matrix(e);
	const f32 zS = s[3] == 0.0f ? 1.0f : core::reciprocal(s[3]);
	const f32 zE = e[3] == 0.0f ? 1.0f : core::reciprocal(e[3]);

	core::vector2df v2d( (e[0]*zE) - (s[0]*zS),
						 (e[1]*zE) - (s[1]*zS));

	// get angle
	f32 angle = v2d.getAngle();

	// rotate around view
	core::quaternion q;
	q.fromAngleAxis(angle* -core::DEGTORAD, view);
	core::matrix4 m = q.getMatrix();
	m.rotateVect(vertical);

	// set beam vertex positions
	vertices[ 4].Pos = startPos - vertical;
	vertices[ 5].Pos = startPos + vertical;
	vertices[ 6].Pos = endPos   - vertical;
	vertices[ 7].Pos = endPos   + vertical;

	// draw debug box
	if ( DebugDataVisible & scene::EDS_BBOX )
	{
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		video::SMaterial m;
		m.Lighting = false;
		driver->setMaterial(m);
		driver->draw3DBox(BBox, video::SColor(0,208,195,152));
	}

	// draw beam
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	driver->setMaterial(Material);
	driver->drawIndexedTriangleList(vertices, 12, indices, 6);

	if ( DebugDataVisible & scene::EDS_MESH_WIRE_OVERLAY )
	{
		video::SMaterial m;
		m.Lighting = false;
		m.Wireframe = true;
		driver->setMaterial(m);
		driver->drawIndexedTriangleList(vertices, 12, indices, 6);
	}
}


//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CBeamSceneNode2::getBoundingBox() const
{
	return BBox;
}


//! sets the size of the beam
void CBeamSceneNode2::setSize(f32 size)
{
	Size = size;

	if (Size <= 0.0f)
		Size = 1.0f;

	recalculateBoundingBox();
}

//! sets the position of the beam
void CBeamSceneNode2::setPosition(const core::vector3df& startPos, const core::vector3df& endPos)
{
	ISceneNode::setPosition(startPos);
	EndPos = endPos - startPos;

	recalculateBoundingBox();
}


void CBeamSceneNode2::recalculateBoundingBox()
{
	// todo: use size to add width
	BBox.reset( core::vector3df(0,0,0) );
	BBox.addInternalPoint(EndPos);
}


video::SMaterial& CBeamSceneNode2::getMaterial(u32 i)
{
	return Material;
}


//! returns amount of materials used by this scene node.
u32 CBeamSceneNode2::getMaterialCount() const
{
	return 1;
}


//! gets the size of the billboard
f32 CBeamSceneNode2::getSize() const
{
	return Size;
}


//! Writes attributes of the scene node.
void CBeamSceneNode2::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	ISceneNode::serializeAttributes(out, options);

	out->addVector3d("EndPosition", EndPos);
	out->addFloat("Size", Size);
}


//! Reads attributes of the scene node.
void CBeamSceneNode2::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	ISceneNode::deserializeAttributes(in, options);

	Size = in->getAttributeAsFloat("Size");
	EndPos = in->getAttributeAsVector3d("EndPosition");

	recalculateBoundingBox();
}


/*
//! Creates a clone of this scene node and its children.
ISceneNode* CBeamSceneNode2::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent)
		newParent = Parent;
	if (!newManager)
		newManager = SceneManager;

	CBeamSceneNode2* nb = new CBeamSceneNode2(newParent,
		newManager, ID, RelativeTranslation, Size);

	nb->cloneMembers(this, newManager);
	nb->Material = Material;

	nb->drop();
	return nb;
}
*/

} // end namespace scene
} // end namespace irr

