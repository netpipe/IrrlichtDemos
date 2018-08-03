#include "tach.h"

using namespace irr;
using namespace core;

Tach::Tach
(
	const core::rect<s32>& rect,
	gui::IGUIEnvironment* env,
	gui::IGUIElement* parent
)
:	gui::IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, rect)
	//_env(env)
{
	video::IVideoDriver* driver = env->getVideoDriver();
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	//TachTexture = driver->getTexture("tach_small.png");
	TachTexture = driver->getTexture("tach.png");
	//NeedleTexture = driver->getTexture("needle_small.png");
	NeedleTexture = driver->getTexture("needle.png");
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	
	TachMaterial.Lighting = false;
	TachMaterial.Thickness = 2.0f;
	TachMaterial.FrontfaceCulling = false;
	TachMaterial.BackfaceCulling = false;
	//TachMaterial.MaterialType = video::EMT_SOLID;
	TachMaterial.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	//TachMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
	TachMaterial.setTexture(0, TachTexture);
	
	NeedleMaterial.Lighting = false;
	NeedleMaterial.Thickness = 2.0f;
	NeedleMaterial.FrontfaceCulling = false;
	NeedleMaterial.BackfaceCulling = false;
	//NeedleMaterial.MaterialType = video::EMT_SOLID;
	NeedleMaterial.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	//NeedleMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
	NeedleMaterial.setTexture(0, NeedleTexture);
	
	setupMeshBuffer(TachMeshBuffer);
	
	this->setReading(0.0f);

}


Tach::~Tach()
{

}


core::rect<s32> Tach::getRect
(
	IrrlichtDevice* device,
	f32 fx, f32 fy, f32 fs
)
{
	video::IVideoDriver* driver = device->getVideoDriver();
	f32 sw = f32(driver->getScreenSize().Width);
	f32 sh = f32(driver->getScreenSize().Height);
	
	s32 x1 = s32(sw*fx);
	s32 y1 = s32(sh*fy);
	
	s32 x2 = x1 + s32(sw*fs);
	s32 y2 = y1 + s32(sw*fs);
	printf("x1=%i,y1=%i,x2=%i,y2=%i\n", x1,y1,x2,y2);
	return core::rect<s32>(x1,y1,x2,y2);
}


void Tach::setReading(f32 reading)
{
	f32 deg = this->getNeedleRot(reading);
	this->setNeedleRot(deg);
}


f32 Tach::getNeedleRot(f32 reading)
{
	return (1.0f-reading)*237.0f/11.0f - 18.5f;
}



void Tach::setNeedleRot(f32 deg)
{
	Matrix.makeIdentity();
	Matrix.setRotationDegrees(core::vector3df(0, 0, deg));
}

void Tach::setTexture(video::ITexture* texture)
{
	if (!texture)
		return;
	//TachMeshBuffer.getMaterial().setTexture(0,texture);
	
	/***
	video::IVideoDriver* driver = Environment->getVideoDriver();
	if (!driver)
		return;
	
	driver->makeColorKeyTexture(texture, video::SColor(255, 255, 255, 255));
	***/
}


void Tach::setupMeshBuffer(scene::SMeshBuffer& buf)
{
	buf.Vertices.set_used(4);
	buf.Indices.set_used(6); // 2 triangles!
	
	
	video::SColor color(255, 255, 255, 255);
	//video::SColor color(128, 255, 255, 255);
	
	//S3DVertex => pos, normal, color, texcoords
	
	buf.Vertices[0] = video::S3DVertex(-1.f, -1.f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 1.f);
	buf.Vertices[1] = video::S3DVertex(-1.f,  1.f, 0.f, 0.f, 0.f, 1.f, color, 0.f, 0.f);
	buf.Vertices[2] = video::S3DVertex( 1.f,  1.f, 0.f, 0.f, 0.f, 1.f, color, 1.f, 0.f);
	buf.Vertices[3] = video::S3DVertex( 1.f, -1.f, 0.f, 0.f, 0.f, 1.f, color, 1.f, 1.f);
	
	buf.Indices[0] = 0;
	buf.Indices[1] = 1;
	buf.Indices[2] = 2;
	buf.Indices[3] = 2;
	buf.Indices[4] = 3;
	buf.Indices[5] = 0;
}


void Tach::draw()
{
	video::IVideoDriver* driver = Environment->getVideoDriver();
	
	if (!(driver && IsVisible))
		return;
	
	core::rect<s32> oldViewPort = driver->getViewPort();
	driver->setViewPort(getAbsolutePosition());

	// clear the projection matrix
	core::matrix4 oldProjMat = driver->getTransform(video::ETS_PROJECTION);
	driver->setTransform(video::ETS_PROJECTION, core::matrix4());
	
	// clear the view matrix
	core::matrix4 oldViewMat = driver->getTransform(video::ETS_VIEW);
	driver->setTransform(video::ETS_VIEW, core::matrix4());

	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->setMaterial(TachMaterial);
	driver->drawMeshBuffer(&TachMeshBuffer);
	
	driver->setTransform(video::ETS_WORLD, Matrix);
	driver->setMaterial(NeedleMaterial);
	driver->drawMeshBuffer(&TachMeshBuffer);
	
	// restore view matrix
	driver->setTransform(video::ETS_VIEW, oldViewMat);
	
	// restore projection matrix
	driver->setTransform(video::ETS_PROJECTION, oldProjMat);
	
	// restore the view area
	driver->setViewPort(oldViewPort);
}

