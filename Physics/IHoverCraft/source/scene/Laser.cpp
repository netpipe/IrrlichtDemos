#include "Laser.h"
#include <ISceneCollisionManager.h>
#include <IParticleSystemSceneNode.h>
#include "Ship.h"
#include "Terrain.h"
#include "../core/Game.h"

Laser::Laser(const Game* game) :
	Weapon(game),
	
	color(255, 255, 0, 0)
{
	mat.Lighting = false;
	mat.Thickness = 2.0f;		
}

const void Laser::draw(const irr::core::vector3df& start, const irr::core::vector3df& end) const
{
	//draw laser
	game->getVideoDriver()->beginScene(false, false, irr::video::SColor());
	game->getVideoDriver()->setMaterial(mat);		
	game->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
	game->getVideoDriver()->draw3DLine(start, end, color);
	game->getVideoDriver()->endScene();
}

const void Laser::fire(const Ship* ship, const Terrain* terrain) const
{

	// get transformation matrix of node
	irr::core::matrix4 m;
	m.setRotationDegrees(ship->getNode()->getRotation());

	// transform forward vector of camera
	irr::core::vector3df frv(0.0f, 0.0f, 1.0);
	m.transformVect(frv);

	const irr::core::vector3df& laserStart = ship->getNode()->getAbsolutePosition();

	dGeomID ray = dCreateRay(game->getPhysics()->getSpace(), 100000.0); //dInfinity lags a bit
	dGeomRaySet(ray, laserStart.X, laserStart.Y, laserStart.Z, frv.X, frv.Y, frv.Z);

	//generate contact point
	const irr::u32 MAX_CONTACTS = 1;
	dContact contact[MAX_CONTACTS];

	const irr::u32 numc = dCollide(ray, terrain->getGeom(), MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
	if(numc > 0)
	{
		//ray collided with terrain

		const irr::core::vector3df collisionPoint(
			contact[0].geom.pos[0],contact[0].geom.pos[1], contact[0].geom.pos[2]);

		draw(ship->getNode()->getAbsolutePosition(), collisionPoint);

		// create smoke particle system
		irr::scene::IParticleSystemSceneNode* pas = 0;
		pas = this->game->getSceneManager()->addParticleSystemSceneNode(false, 0, 0, collisionPoint);
		pas->setParticleSize(irr::core::dimension2d<irr::f32>(1.f, 1.0f));

		//create box emitter
		irr::scene::IParticleEmitter* em = pas->createBoxEmitter(
			irr::core::aabbox3d<irr::f32>(-1.5, -1.5,-1.5, 1.5, 1.5, 1.5),
			irr::core::vector3df(0,0,0),
			10, 20,
			irr::video::SColor(0,255,255,255),irr::video::SColor(0,255,255,255),
			300, 800, 0
		);
		pas->setEmitter(em);
		em->drop();

		//create FadeOut particle affector
		irr::scene::IParticleAffector* paf = pas->createFadeOutParticleAffector();
		pas->addAffector(paf);
		paf->drop();

		pas->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		pas->setMaterialTexture(0, this->game->getVideoDriver()->getTexture("media/images/smoke.bmp"));
		pas->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);

		//create deletion animator
		irr::scene::ISceneNodeAnimator* anim = this->game->getSceneManager()->createDeleteAnimator(800);
		pas->addAnimator(anim);
		anim->drop();

	}

	dGeomDestroy(ray);
}
