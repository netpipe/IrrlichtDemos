#include "WhirlSceneNode.h"
#include <IVideoDriver.h>
#include <ISceneManager.h>
#include <ICameraSceneNode.h>

namespace irr
{
namespace scene
{

WhirlSceneNode::WhirlSceneNode(const u32 num, ISceneNode* const parent, ISceneManager* const mgr, s32 id, const f32 distance) :
	ISceneNode(parent, mgr, id),
	num(num)
{

#ifdef _DEBUG
	setDebugName("WhirlSceneNode");
#endif

	this->stars.set_used(this->num);

	u32 i;
	for(i = 0; i < num; ++i)
	{
		this->stars[i].color = video::SColor(255, rand()%256, rand()%256, rand()%256);
		this->stars[i].angle = 0.0f;
		this->stars[i].dist = (f32(i)/num)*distance;
	}

	video::ITexture* const texture = SceneManager->getVideoDriver()->getTexture("media/images/star.bmp");

	this->Material.Lighting = false;
	this->Material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	this->Material.TextureLayer[0].Texture = texture;

	this->vertices[0] = irr::video::S3DVertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor(), 0.0f, 0.0f); //top left
	this->vertices[1] = irr::video::S3DVertex( 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor(), 1.0f, 0.0f); //top right
	this->vertices[2] = irr::video::S3DVertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor(), 0.0f, 1.0f); //bottom left
	this->vertices[3] = irr::video::S3DVertex( 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, irr::video::SColor(), 1.0f, 1.0f); //bottom right

	this->indices[0] = 0;
	this->indices[1] = 1;
	this->indices[2] = 2;

	this->indices[3] = 2;
	this->indices[4] = 1;
	this->indices[5] = 3;


	this->Box.reset(this->vertices[0].Pos);
	for (u32  i = 1; i < 4; ++i)
		this->Box.addInternalPoint(this->vertices[i].Pos);

//	this->updateAbsolutePosition();
}

void WhirlSceneNode::OnRegisterSceneNode()
{
	if (this->IsVisible)
		this->SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

const irr::core::aabbox3d<irr::f32>& WhirlSceneNode::getBoundingBox() const
{
	return this->Box;
}

irr::u32 WhirlSceneNode::getMaterialCount() const
{
	return 1;
}

irr::video::SMaterial& WhirlSceneNode::getMaterial(irr::u32 i)
{
	return this->Material;
}

void WhirlSceneNode::OnAnimate(irr::u32 timeMs)
{
	//always face camera
	const irr::core::vector3df toTarget(
			this->AbsoluteTransformation.getTranslation() - SceneManager->getActiveCamera()->getAbsolutePosition());
	this->setRotation(toTarget.getHorizontalAngle());

	ISceneNode::OnAnimate(timeMs);
}


void WhirlSceneNode::render()
{
	irr::video::IVideoDriver* const driver = this->SceneManager->getVideoDriver();
	driver->setMaterial(this->Material);

	irr::core::matrix4 rot, pos, world;

	irr::u32 i;
	for(i = 0; i < this->num; ++i)
	{
		this->vertices[0].Color = this->stars[i].color;
		this->vertices[1].Color = this->stars[i].color;
		this->vertices[2].Color = this->stars[i].color;
		this->vertices[3].Color = this->stars[i].color;

		this->stars[i].angle += irr::f32(i)/(this->num*25);

		pos.setTranslation(irr::core::vector3df(this->stars[i].dist, 0, 0));
		rot.setRotationRadians(irr::core::vector3df(0, 0, this->stars[i].angle));
		world = this->AbsoluteTransformation*rot*pos*rot;

		driver->setTransform(irr::video::ETS_WORLD, world);
		driver->drawIndexedTriangleList(&this->vertices[0], 4, &this->indices[0], 2);
	}

	if (DebugDataVisible & scene::EDS_BBOX)
	{
		driver->setMaterial(this->Material);
		driver->draw3DBox(this->Box, video::SColor(255, 255, 255, 255));
	}
}

} // end namespace scene
} // end namespace irr
