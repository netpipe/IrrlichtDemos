#include "Compass.h"
#include <IGUIEnvironment.h>
#include <IVideoDriver.h>

Compass::Compass(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* const env, irr::gui::IGUIElement* const parent) :
	irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT, env, parent, -1, rect)
{
	SetupQuadMesh(BodyMesh, 1.0f);
	SetupQuadMesh(NeedleMesh, 1.0f);
}

//! render the compass
void Compass::draw()
{
	if (!IsVisible) return;

	irr::video::IVideoDriver* driver = Environment->getVideoDriver();

	const irr::core::rect<irr::s32> oldViewPort = driver->getViewPort();
	driver->setViewPort(AbsoluteRect);

	// clear the projection matrix
	const irr::core::matrix4 oldProjMat = driver->getTransform(irr::video::ETS_PROJECTION);
	driver->setTransform(irr::video::ETS_PROJECTION, irr::core::matrix4());

	// clear the view matrix
	const irr::core::matrix4 oldViewMat = driver->getTransform(irr::video::ETS_VIEW);
	driver->setTransform(irr::video::ETS_VIEW, irr::core::matrix4());
	driver->setTransform(irr::video::ETS_WORLD, Matrix);

	// draw compass body
	driver->setMaterial(BodyMesh.Material);
	driver->drawMeshBuffer(&BodyMesh);

	driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

	// draw the needle
	driver->setMaterial(NeedleMesh.Material);
	driver->drawMeshBuffer(&NeedleMesh);


	// restore view matrix
	driver->setTransform(irr::video::ETS_VIEW, oldViewMat);

	// restore projection matrix
	driver->setTransform(irr::video::ETS_PROJECTION, oldProjMat);

	// restore the viewport
	driver->setViewPort(oldViewPort);
}

const void Compass::SetupQuadMesh(irr::scene::SMeshBuffer& mesh, const irr::f32 f32Width) const
{
	const irr::f32 f32HalfWidth = f32Width/2.0f;
	mesh.Vertices.set_used(4);
	mesh.Indices .set_used(6);

	const irr::video::SColor white(255, 255, 255, 255);

	mesh.Vertices[0] = irr::video::S3DVertex(-f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 1.f);
	mesh.Vertices[1] = irr::video::S3DVertex(-f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 0.f, 0.f);
	mesh.Vertices[2] = irr::video::S3DVertex( f32HalfWidth,  f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 0.f);
	mesh.Vertices[3] = irr::video::S3DVertex( f32HalfWidth, -f32HalfWidth, 0.f, 0.f, 0.f, 1.f, white, 1.f, 1.f);

	mesh.Indices[0] = 0;
	mesh.Indices[1] = 1;
	mesh.Indices[2] = 2;
	mesh.Indices[3] = 2;
	mesh.Indices[4] = 3;
	mesh.Indices[5] = 0;

	mesh.getMaterial().Lighting = false;
	mesh.getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
}
