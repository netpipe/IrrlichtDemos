#include "util.h"
#include <iostream>
#include <sstream>


using namespace irr;

Util::Util()
{
}

Util::~Util()
{
}


void Util::print(const core::vector3df& vec)
{
	printf("(%.2f,%.2f,%.2f)\n", vec.X, vec.Y, vec.Z);
}


void Util::drawNodeLabel
(
	IrrlichtDevice* device,
	scene::ISceneNode* node,
	const core::stringw& text,
	s32 xOffset,
	s32 yOffset
)
{
	gui::IGUIFont* font = device->getGUIEnvironment()->getFont("media/fonthaettenschweiler.bmp");
	if (!font)
		return;
	
	// find pos on screen to render text:
	video::IVideoDriver* driver = device->getVideoDriver();
	
	scene::ISceneCollisionManager* cmgr = device->getSceneManager()->getSceneCollisionManager();
	node->updateAbsolutePosition();
	core::vector2d<s32> pos = cmgr->getScreenCoordinatesFrom3DPosition(node->getAbsolutePosition());
	
	s32 cx = pos.X + xOffset;
	s32 cy = pos.Y + yOffset;
	
	// find text size
	core::dimension2d<u32> textSize = font->getDimension(text.c_str());
	s32 tw = textSize.Width;
	s32 th = textSize.Height;
	
	// draw bg:
	s32 border = 2;
	core::rect<s32> bgRect
	(
		cx - tw / 2 - border * 2,
		cy - th / 2 - border,
		cx + tw / 2 + border,
		cy + th / 2 + border
	);
	
	video::SColor bg(128, 255, 255, 255);
	driver->draw2DRectangle(bg, bgRect);
	
	// draw text:
	core::rect<s32> rect
	(
		cx - tw / 2,
		cy - th / 2,
		cx + tw / 2,
		cy + th / 2
	);
	
	video::SColor blue(0xFF0000FF);
	font->draw(text, rect, blue, true, true); // hcenter, vcenter
}


void Util::rotateCameraTowardsNodeAroundYAxis
(
	scene::ICameraSceneNode* camera,
	scene::ISceneNode* node,
	f32 minDist,
	bool rotateCameraBy90Deg
)
{
	node->updateAbsolutePosition();
	core::vector3df pos = node->getAbsolutePosition();
	
	rotateCameraTowardsPointAroundYAxis(camera, pos, minDist, rotateCameraBy90Deg);
}

void Util::rotateCameraTowardsPointAroundYAxis
(
	scene::ICameraSceneNode* camera,
	const core::vector3df& pos,
	f32 minDist,
	bool rotateCameraBy90Deg
)
{
	camera->updateAbsolutePosition();
	
	// rotate to face target Node
	core::vector3df cameraPos = camera->getAbsolutePosition();
	core::vector3df delta = pos - cameraPos;
	
	
	core::vector3df rot = delta.getHorizontalAngle();
	//rot.X = 0.0f;
	//rot.Z = 0.0f;
	if (rotateCameraBy90Deg)
		rot.Y -= 90.0f;
	
	camera->setRotation(rot);

	if (minDist <= 0.0f)
		return;
	
	if (delta.getLengthSQ() < (minDist * minDist))
		return;
	
	delta *= 0.5f;
	
	camera->setPosition(core::vector3df
	(
		cameraPos.X + delta.X,
		cameraPos.Y + delta.Y,
		cameraPos.Z + delta.Z
	));
	
}


void Util::drawXYZAxes
(
	IrrlichtDevice* device,
	const core::vector3df& pos,
	u32 length
)
{
	core::vector3df p0 = pos;
	core::vector3df p1 = pos; p1.X -= length;
	core::vector3df p2 = pos; p2.Y += length;
	core::vector3df p3 = pos; p3.Z += length;
	
	////////////////////////////////////////////////////////////////////////////
	// drawing stuff:
	
	// set mat:
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 2.0f;
	lineMaterial.BackfaceCulling = false;
	lineMaterial.MaterialType = video::EMT_SOLID;
	
	
	video::IVideoDriver* driver = device->getVideoDriver();
	driver->setMaterial(lineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);	
	
	driver->draw3DLine(p0, p1, 0xFFFF0000);
	driver->draw3DLine(p0, p2, 0xFF00FF00);
	driver->draw3DLine(p0, p3, 0xFF0000FF);

	////////////////////////////////////////////////////////////////////////////
	// draw labels:
	gui::IGUIFont* font = device->getGUIEnvironment()->getFont("media/lucida.xml");
	if (!font)
		return;
	
	scene::ISceneCollisionManager* cmgr = device->getSceneManager()->getSceneCollisionManager();

	s32 tw = 8;
	s32 th = 8;

	core::vector2d<s32> spos;
	core::stringw text;
	core::rect<s32> rect;
	
	
	text = L"X";
	
	p1.X -= 0.1f;
	spos = cmgr->getScreenCoordinatesFrom3DPosition(p1);
	rect = core::rect<s32>(spos.X-tw/2, spos.Y-th/2, spos.X+tw/2, spos.Y+th/2);
		
	font->draw(text, rect, 0xFFFF0000, true, true);
	
	
	text = L"Y";
	p2.Y += 0.1f;
	spos = cmgr->getScreenCoordinatesFrom3DPosition(p2);
	rect = core::rect<s32>(spos.X-tw/2, spos.Y-th/2, spos.X+tw/2, spos.Y+th/2);
	
	font->draw(text, rect, 0xFF00FF00, true, true);


	text = L"Z";
	p3.Z += 0.1f;
	spos = cmgr->getScreenCoordinatesFrom3DPosition(p3);
	rect = core::rect<s32>(spos.X-tw/2, spos.Y-th/2, spos.X+tw/2, spos.Y+th/2);
	
	font->draw(text, rect, 0xFF0000FF, true, true);
	
	
}


