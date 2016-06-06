#include "Ship.h"
#include "../core/Game.h"
#include "../scene/Terrain.h"
#include <IParticleSystemSceneNode.h>
#include <ILightSceneNode.h>

Ship::Ship(Game* game, const irr::core::stringc& name,
	const irr::core::stringc& meshPath, const irr::core::stringc& texture,
	const irr::core::vector3df& position, const irr::core::vector3df& rotation,
	const irr::core::vector3df& scale,

	const dReal density, const dReal weight) :

	IHoverCraft(game, name, meshPath, texture, position, rotation, scale, density, weight),

	light(0)
{
	addIrrlichtStuff();
}

Ship::~Ship()
{
	light->remove();
}

const void Ship::addIrrlichtStuff()
{
	irr::video::SMaterial& mat = this->node->getMaterial(0);
	mat.Shininess = 200.0f;

	const irr::video::SColor& c = game->getConfiguration().getShipColor();
	mat.EmissiveColor.set(c.getAlpha(), c.getRed(), c.getGreen(), c.getBlue());

	irr::scene::ISceneManager* smgr = game->getSceneManager();
	light = smgr->addLightSceneNode(node);
	light->setPosition(irr::core::vector3df(0.0f, 20.0f, 0.0f));
	light->setRotation(irr::core::vector3df(90.0f, 0.0f, 90.0f));
/*
	irr::video::SLight& lightData = this->light->getLightData();
	lightData.AmbientColor.set(0.0f, 0.0f, 0.0f, 0.0f);
	lightData.DiffuseColor.set(c.getAlpha(), c.getRed(), c.getGreen(), c.getBlue());
	lightData.SpecularColor.set(0.0,0.0,0.0,0.0);
	lightData.Radius = 10.0f;
	lightData.Type = irr::video::ELT_SPOT;
*/
}

const void Ship::move(const irr::f32 elapsed) const
{
	//we are a hoover craft, so move like one first
	IHoverCraft::move(elapsed);

	//here comes the fancy stuff
    irr::scene::IParticleSystemSceneNode* em = 0;
    em = game->getSceneManager()->addParticleSystemSceneNode(
    	false,
    	node,
    	-1,
    	irr::core::vector3df(0.0f, 0.0f, 0.0f),
    	irr::core::vector3df(-90.0f, 0.0f, 0.0f),
    	irr::core::vector3df(1.0f, 1.0f, 1.0f)
    );
    em->setParticleSize(irr::core::dimension2d<irr::f32>(.05f, .05f));
	em->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	em->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
	em->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);

	//invert the ship color
	const irr::video::SColor& hm = game->getConfiguration().getShipColor();
	irr::s32 newRed = hm.getRed() - 255;		newRed *=-1;
	irr::s32 newGreen = hm.getGreen() - 255;	newGreen *=-1;
	irr::s32 newBlue = hm.getBlue() - 255;		newBlue *=-1;
	const irr::video::SColor invertedShipColor(255, newRed, newGreen, newBlue);

    irr::scene::IParticleRingEmitter* pas = em->createRingEmitter(
    	irr::core::vector3df(0.0f, 0.0f, -1.5f), //center
    	.5f, //radius
    	.5, //ring thickness
    	irr::core::vector3df(0.0f, 0.0f, 0.0f), //direction
    	150, //minParticlesPerSecond
    	300, //maxParticlesPerSecond
    	hm, //minStartColor
    	invertedShipColor, //maxStartColor
    	250, //lifeTimeMin
    	500, //lifeTimeMax
    	0 //maxAngleDegrees
    );

	em->setEmitter(pas);
	pas->drop();

	//create FadeOut particle affector
	irr::scene::IParticleAffector* paf = em->createFadeOutParticleAffector();
	em->addAffector(paf);
	paf->drop();

	//create deletion animator
	irr::scene::ISceneNodeAnimator* anim = game->getSceneManager()->createDeleteAnimator(500);
	em->addAnimator(anim);
	anim->drop();
}
