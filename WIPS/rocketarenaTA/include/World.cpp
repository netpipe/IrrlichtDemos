/*

World class - cpp

*/

#include "World.h"


// constructor
cWorld::cWorld() {
}

// destructor

cWorld::~cWorld() {
}

// Initialisation
void cWorld::Initialize(cInit *init) {

	device = init->getDevice();
	smgr = device->getSceneManager();
	driver = device->getVideoDriver();
	
	levelMesh = (IAnimatedMesh*) smgr->getMesh("media/map/map.b3d");
	scaleFactor = core::vector3df(12,12,12);
	smgr->getMeshManipulator()->scaleMesh(levelMesh, scaleFactor);

	level = (IAnimatedMeshSceneNode*) smgr->addAnimatedMeshSceneNode(levelMesh);

	// adding a light
	video::SLight l;
	scene::ILightSceneNode *light = smgr->addLightSceneNode(0,core::vector3df(50,50,50),video::SColorf(1,1,1,.5f),1000);
	l = light->getLightData();
	l.Type = video::ELT_POINT ;
	l.AmbientColor = video::SColorf(.7,.7,.55);
	l.SpecularColor = video::SColorf(1,1,.5);
	l.DiffuseColor = video::SColorf(1,1,1);
	l.CastShadows = true;

	light->setLightData(l);
	
	light->setPosition(core::vector3df(900,350,315));

	// create light billboards	
	lightBillboard.Initialize(device);
	core::vector3df position = core::vector3df(-764,-3,342);
	core::vector3df normal = core::vector3df(1.6,0,1.5);
	f32 angle = 95.f;

	lightBillboard.addLight(position,normal,angle);

	position = core::vector3df(-752,-3,400);
	normal = core::vector3df(1.9,0,-1.5);

	lightBillboard.addLight(position,normal,angle);

	scene::IAnimatedMesh *mesh = smgr->getMesh("media/garage/prop.b3d");
	prop = smgr->addAnimatedMeshSceneNode(mesh);
	prop->setPosition(core::vector3df(-704.6f,17.f,342));

}

// returns the level
IAnimatedMeshSceneNode *cWorld::getLevel() {
	return level;
}

// returns level scene node
IAnimatedMeshSceneNode *cWorld::getLevelSceneNode() {
	return level;
}

// return level scaleFactor
const core::vector3df cWorld::getScaleFactor() {
	return scaleFactor;
}

// add flags
void cWorld::addFlags(core::array<core::vector3df> positionArray) {
	core::vector3df position;
	core::vector3df rotation = core::vector3df(0,-140,0);
	core::vector3df scale = core::vector3df(4.3,4.3,4.3);
	
	for (u32 i=0; i<positionArray.size(); i++) {
		position = positionArray[i];
		position += core::vector3df(1.1f,80.7f,1.6f);

		cFlag flag;
		flag.Initialize(device,position,rotation,scale);
	
		flags.push_back(flag);
	}
	
	position = core::vector3df(241.f,129.f,319.5f);
	scale = core::vector3df(5,5,5);

	cFlag flagTower;
	flagTower.Initialize(device,position,rotation,scale);
	flags.push_back(flagTower);

}
	
// animates the world
void cWorld::drawWorld(scene::ICameraSceneNode *camera) {
	
	for (u32 i=0; i<flags.size(); i++)
		flags[i].drawFlag();

	lightBillboard.drawLight(camera);

	prop->setRotation(prop->getRotation()+core::vector3df(0,6,0));
	
}


