#include <irrlicht.h>
#include "inceptumUnit.h"
#include "inceptumEnum.h"

IrrlichtDevice* UnitClass::device = 0;
scene::ISceneManager* UnitClass::smgr = 0;
video::IVideoDriver* UnitClass::driver = 0;
f32* UnitClass::deltaTime = 0;
scene::ITerrainSceneNode* UnitClass::terrain = 0;
scene::IAnimatedMesh* UnitClass::mesh1 = 0;
video::ITexture* UnitClass::texture1_1 = 0;
video::ITexture* UnitClass::texture1_2 = 0;

void UnitClass::setPointers(IrrlichtDevice* const deviceIn,
	scene::ITerrainSceneNode* const terrainIn, f32* const deltaTimeIn)
{
	device = deviceIn;
	driver = deviceIn->getVideoDriver();
	smgr = deviceIn->getSceneManager();
	terrain = terrainIn;
	deltaTime = deltaTimeIn;
	mesh1 = smgr->getMesh("./media/Cannonram.x");
	texture1_1 = driver->getTexture("./media/CannonramTexture.jpg");
	texture1_2 = driver->getTexture("./media/sphere2.jpg");
}

void UnitClass::Initialize(core::vector2d<f32> initPos, f32 initRot, bool Sph)
{
	node = smgr->addAnimatedMeshSceneNode(mesh1);
	node->setID(Pickable);
	node->setPosition(core::vector3df(initPos.X, terrain->getHeight(initPos.X, initPos.Y), initPos.Y));
	node->setScale(core::vector3df(3.0f, 3.0f, 3.0f));
	node->setRotation(core::vector3df(0.0f, initRot, 0.0f));
	node->setFrameLoop(0, 64);
	node->setAnimationSpeed(24);
	node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	//node->setMaterialFlag(video::EMF_LIGHTING, false);

	if (Sph)
		node->setMaterialTexture(0, texture1_1);
	else
		node->setMaterialTexture(0, texture1_2);

	selector = smgr->createTriangleSelector(node);
	node->setTriangleSelector(selector);
	selector->drop();
}

void UnitClass::setPosition(core::vector3df position)
{
	node->setPosition(position);
}

bool UnitClass::compareNodes(scene::ISceneNode* inNode)
{
	return this->node == inNode;
}
