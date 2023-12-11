#ifndef __VEHICLE_H_INCLUDED__
#define __VEHICLE_H_INCLUDED__

#include <irrlicht.h>

#include <cstdio>

class Vehicle : public irr::IReferenceCounted
{
public:

	Vehicle(EffectHandler * effhand, irr::IrrlichtDevice * dev, irr::s32 shad) : ehandler(effhand), device(dev), smgr(dev->getSceneManager()), shader(shad)
	{
		this->device->grab();
		//#define STOCKTEST   //only enable if you own or a friend would lend you a copy of counterstrike sometimes there are free promotions on too.
		#ifdef STOCKTEST
		this->mesh = this->smgr->getMesh("media/models/cs_office.obj");
		#else
		this->mesh = this->smgr->getMesh("maps/egyptians.bsp");
		#endif
		if(this->mesh) {
			this->rNode = this->smgr->addAnimatedMeshSceneNode(this->mesh);
		}
		else {
			std::printf("Could not load mesh in Vehicle class constructor!\n");
			return;
		}

		this->rNode->setMaterialType(irr::video::EMT_SOLID);
		this->rNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		this->rNode->setScale(irr::core::vector3df(0.0208f, 0.0208f, 0.0208f));
		this->rNode->setPosition(irr::core::vector3df(10.008f, 0.008f, 10.008f));

		//irr::scene::ISceneNodeAnimator * anim = this->smgr->createFlyCircleAnimator(irr::core::vector3df(0, 1, 0), 2, 0.002);
		//if(anim) {
			//this->rNode->addAnimator(anim);
//			anim->drop();
		//}

		this->ehandler->addShadowToNode(this->rNode, EFT_4PCF, ESM_BOTH);

		//this->ehandler->excludeNodeFromLightingCalculations(this->rNode);
	}

	~Vehicle(void)
	{
		this->device->drop();
	}

private:

	EffectHandler * ehandler;
	irr::IrrlichtDevice * device;
	irr::scene::ISceneManager * smgr;
	irr::scene::IAnimatedMesh * mesh;
	irr::scene::IAnimatedMeshSceneNode * rNode;

	irr::s32 shader;

};

#endif
