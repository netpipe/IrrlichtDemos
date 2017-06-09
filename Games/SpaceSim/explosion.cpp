//Explosion code
//Creates initial explosion, is then added to gamemanager to be timed
//then deleted

#include "stdafx.h"
#include "explosion.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

//Theres alot of code here, but no real complex code. Just setup of nodes and stuff like that
//So I'm not going to explain any of it

explosion::explosion(irr::IrrlichtDevice *graphics,  core::vector3df pos)
{
	//2 second timer
	last_time = graphics->getTimer()->getTime()+2000;
	this->graphics = graphics;
	this->end=false;
	node = graphics->getSceneManager()->addCubeSceneNode();
	node->setPosition(pos);


	ps = graphics->getSceneManager()->addParticleSystemSceneNode(false, node);
	scene::IParticleEmitter *em = ps->createSphereEmitter(core::vector3df(0,0,0),100,vector3df(0,0,0),50,100,SColor(0,255,255,255),SColor(0,255,255,255),1000,2000,0,dimension2df(200,200),dimension2df(400,400));
	ps->setEmitter(em);
	em->drop();
	ps->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/fire.bmp"));
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	scene::IParticleAffector *explo = ps->createAttractionAffector(node->getPosition(),-500.F);
	ps->addAffector(explo);
	scene::IParticleAffector *af = ps->createFadeOutParticleAffector(SColor(0,0,0,0),1000);
	ps->addAffector(af);
	debris = graphics->getSceneManager()->addParticleSystemSceneNode(false,node);
	scene::IParticleEmitter *deb = debris->createSphereEmitter(vector3df(0,0,0),300,vector3df(0,0,0),50,100,SColor(0,255,255,255),SColor(0,255,255,255),500,1000,0,dimension2df(20,20),dimension2df(40,40));
	debris->setEmitter(deb);
	deb->drop();
	scene::IParticleAffector *explo2 = debris->createAttractionAffector(node->getPosition(),-2000.f);

	debris->addAffector(explo2);
	debris->addAffector(af);
	debris->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/explo.pcx"));
	debris->setMaterialFlag(video::EMF_LIGHTING,false);
	debris->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	shockwave = graphics->getSceneManager()->addCubeSceneNode(10);
	shockwave->setScale(core::vector3df(10,0,10));
	shockwave->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/shockwave1.tga"));
	shockwave->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	shockwave->setParent(node);


}

//This is run by the GameManager object
void explosion::loop()
{
	core::vector3df tmp = shockwave->getScale();
	tmp.X+=2;
	tmp.Z+=2;
	shockwave->setScale(tmp);

	if(last_time<graphics->getTimer()->getTime())
	{
		ps->setEmitter(0);
		debris->setEmitter(0);
		end=true;
	}

}
void explosion::drop()
{
	delete this;
}

explosion::~explosion()
{
	//ps->setVisible(false);
	//node->setVisible(false);
	shockwave->remove();
	node->remove();
}
