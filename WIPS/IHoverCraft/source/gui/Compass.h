#ifndef COMPASS_H_
#define COMPASS_H_

class Compass : public irr::gui::IGUIElement 
{

private:

	irr::scene::SMeshBuffer BodyMesh;
	irr::scene::SMeshBuffer NeedleMesh;
	irr::core::matrix4 Matrix;

	//! sets up a quad mesh of given dimension 
	const void SetupQuadMesh(irr::scene::SMeshBuffer& mesh, const irr::f32 f32Width)
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
		//mesh.getMaterial().BackfaceCulling = false;
		//mesh.getMaterial().Wireframe = true;
		mesh.getMaterial().MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	}

public:

	Compass(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent);

	//! set body of compass texture
	const void SetCompassBodyTexture(irr::video::ITexture* texture);

	//! set needle of compass texture 
	const void SetCompassNeedleTexture(irr::video::ITexture* texture);

	//! set compass heading
	const void SetCompassHeading(const irr::f32 deg);

	//! render the compass
	virtual void draw();
};

Compass::Compass(const irr::core::rect<irr::s32>& rect, irr::gui::IGUIEnvironment* env, irr::gui::IGUIElement* parent) :
	irr::gui::IGUIElement(irr::gui::EGUIET_ELEMENT, env, parent, -1, rect)
{
	SetupQuadMesh(BodyMesh, 1.0f);
	SetupQuadMesh(NeedleMesh, 1.0f);
}

//! set body of compass texture 
const void Compass::SetCompassBodyTexture(irr::video::ITexture* texture)
{
    BodyMesh.getMaterial().setTexture(0, texture);
}

//! set needle of compass texture
const void Compass::SetCompassNeedleTexture(irr::video::ITexture* texture)
{
    NeedleMesh.getMaterial().setTexture(0, texture);
}
 
//! set compass heading
const void Compass::SetCompassHeading(const irr::f32 deg)
{
    Matrix.makeIdentity();
    Matrix.setRotationDegrees(irr::core::vector3df(0, 0, deg));
}
 
//! render the compass 
void Compass::draw()
{
	if (!IsVisible) return;

	irr::video::IVideoDriver* driver = Environment->getVideoDriver();  

	const irr::core::rect<irr::s32> oldViewPort = driver->getViewPort(); 
	driver->setViewPort(getAbsolutePosition()); 

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

	// restore the view area
	driver->setViewPort(oldViewPort);
}

#endif /*COMPASS_H_*/
