#include "stdafx.h"
#include "projectile.h"

//Projectile Code
//Here we have the 'template' projectile
//All other projectiles are based off its code
//same stats as a railgun, except it has a different, cooler FX
projectile::projectile(irr::IrrlichtDevice *graphics, core::vector3df pos,  core::vector3df rot,irr::scene::IAnimatedMeshSceneNode *ship)
{
	//setup variables
	//this is just the base projectile, so none of these stats really matter
	this->pos = pos;
	this->rot = rot;
	this->graphics = graphics;
	this->velocity = 1750;
	this->damage = 50;
	this->range = 6000;
	this->ship = ship;
	//Creates tiny box to use as projectile

	bullet = graphics->getSceneManager()->addCubeSceneNode(2);
	bullet->setPosition(pos);
	bullet->setRotation(rot);
	//setup particle system
	ps=graphics->getSceneManager()->addParticleSystemSceneNode(false,bullet);
	scene::IParticleEmitter *em = ps->createBoxEmitter(core::aabbox3d<f32>(-2,0,-2,2,1,2),core::vector3df(0.0f,0.0f,0.0f),200,400,video::SColor(0,255,255,255),video::SColor(0,0,0,0),100,200,0,core::dimension2df(3.f,3.f),core::dimension2df(6.f,6.f));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/fire.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
}
projectile::~projectile()
{
	bullet->setVisible(false);
	bullet->remove();
}

//Checks the range from the original point of origin to set max range of bullets
int projectile::checkRange()
{
	float z = pos.Z-bullet->getPosition().Z;
	float y = pos.Y-bullet->getPosition().Y;
	float x = pos.X-bullet->getPosition().X;
	int dist = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	return dist;
}

void projectile::projRun(f32 frameDeltaTime)
{
	//if still has range left
	//move toward initial rotation
	pPos = bullet->getPosition();
	float i = bullet->getRotation().Y;
	float z = -(bullet->getRotation().X);
	pPos.X=frameDeltaTime*velocity*(sin(i*3.14/180));
	pPos.X+=bullet->getPosition().X;
	pPos.Z=frameDeltaTime*velocity*(cos(i*3.14/180));
	pPos.Z+=bullet->getPosition().Z;
	pPos.Y=frameDeltaTime*velocity*(sin(z*3.14/180));
	pPos.Y+=bullet->getPosition().Y;
	bullet->setPosition(pPos);
}

void projectile::drop()
{
	delete this;
}



//actual projectile
//set its stats to different stats
railgunShot::railgunShot(irr::IrrlichtDevice *graphics, core::vector3df pos,  core::vector3df rot,irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage=50;
	this->range = 6500;
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
}

photonCannonShot::photonCannonShot(irr::IrrlichtDevice *graphics, core::vector3df pos,  core::vector3df rot,irr::scene::IAnimatedMeshSceneNode *ship) : projectile(graphics,pos,rot,ship)
{
	this->damage = 70;
	this->range =  5000;
}
