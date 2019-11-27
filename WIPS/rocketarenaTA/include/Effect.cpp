/*

Effect - cpp file

*/

#include "Effect.h"


// constructor
cEffect::cEffect() {
}

// destructor
cEffect::~cEffect() {
}

// Initialization
void cEffect::Initialize() {
}

// create fire effect
void cEffect::createFireEffect(IrrlichtDevice *device) {
	
	video::IVideoDriver *driver = device->getVideoDriver();
	scene::ISceneManager *smgr = device->getSceneManager();
	
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(core::vector3df(40,5,40));
	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));

	scene::IParticleEmitter* em = ps->createBoxEmitter(
		core::aabbox3d<f32>(-5,0,-5,5,1,5),
		core::vector3df(0.0f,0.06f,0.0f),
		150,400,
		video::SColor(0,255,120,0), video::SColor(0,0,0,50),
		800,1100);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createFadeOutParticleAffector(video::SColor(10,0,0,0), 1000);

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("media/effects/puff.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

}

// create rocket waste gas
void cEffect::createRocketWasteGas(IrrlichtDevice *device) {
	
	video::IVideoDriver *driver = device->getVideoDriver();
	scene::ISceneManager *smgr = device->getSceneManager();
	
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(core::vector3df(10,50,0));
	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(5,5));

	scene::IParticleEmitter* em = ps->createCylinderEmitter(
		core::vector3df(0,0,0),
		.5f,
		core::vector3df(.05,0,0),
		.5f,
		false,
		core::vector3df(1.f,0.f,0.f),
		200,400,
		video::SColor(0,255,150,0), video::SColor(0,0,0,50),
		100,100,5);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createFadeOutParticleAffector(video::SColor(0,0,0,50), 136);

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("media/effects/puff.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	ps->setRotation(core::vector3df(20,33,100));
	
	particleRocketWasteGas=ps;
	emitterRocketWasteGas=em;
}

// create a explosion effect
void cEffect::createExplosionEffect(IrrlichtDevice *device) {
	
	video::IVideoDriver *driver = device->getVideoDriver();
	scene::ISceneManager *smgr = device->getSceneManager();
	
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	ps->setPosition(core::vector3df(-40,5,40));
	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(10.0f, 10.0f));

	scene::IParticleEmitter* em = ps->createSphereEmitter(
		core::vector3df(0,0,0),
		5.5f,
		core::vector3df(0,0,0),
		200,300,
		video::SColor(0,255,120,0), video::SColor(0,0,0,5),
		500,600,7);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createFadeOutParticleAffector(video::SColor(0,0,0,5), 1000);

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("media/effects/puff.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

}

const scene::IParticleSystemSceneNode *cEffect::getPS() {
	return particleRocketWasteGas;
}
	
const scene::IParticleEmitter *cEffect::getEM() {
	return emitterRocketWasteGas;
}

// create an menu effect
scene::IParticleSystemSceneNode *cEffect::createMenuEffect(IrrlichtDevice *device, bool right) {
	video::IVideoDriver *driver = device->getVideoDriver();
	scene::ISceneManager *smgr = device->getSceneManager();
	
	scene::IParticleSystemSceneNode* ps = 0;
	ps = smgr->addParticleSystemSceneNode(false);
	
	ps->setPosition(core::vector3df(22,-3.8,40));

	ps->setScale(core::vector3df(2,2,2));

	ps->setParticleSize(core::dimension2d<f32>(3.0f, 3.0f));
	
	core::vector3df direction = core::vector3df(-.04f,0.f,.04f);
	if (!right) {
		direction*=-1.f;
		ps->setPosition(ps->getPosition() + core::vector3df(17.5,0,-17.5));
	}

	scene::IParticleEmitter* em = ps->createCylinderEmitter(
		core::vector3df(0,0,0),
		.5f,
		core::vector3df(.1,0,0),
		2.5f,
		false,
		direction,
		100,200,
		video::SColor(0,255,150,0), video::SColor(0,0,0,50),
		50,80,0);

	ps->setEmitter(em);
	em->drop();

	scene::IParticleAffector* paf =
		ps->createFadeOutParticleAffector(video::SColor(0,0,0,50), 180);

	ps->addAffector(paf);
	paf->drop();

	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps->setMaterialTexture(0, driver->getTexture("media/effects/puff.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	
	return ps;
}

