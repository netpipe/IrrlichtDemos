#include "BoidSceneNode.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <SMesh.h>
#include <SMeshBuffer.h>
#include <ISceneCollisionManager.h>
#include <IParticleSystemSceneNode.h>
//#include <ITriangleSelector.h>
#include <IMeshManipulator.h>

#ifdef _SOUND
#include <ik_ISoundEngine.h>
#endif

namespace irr
{
namespace scene
{


BoidSceneNode::BoidSceneNode(
	irr::scene::IMesh* const boidMesh,
	const core::vector3df& target, const irr::f32 borders[4], ISceneManager* const mgr, const s32 id) :
	IMeshSceneNode(mgr->getRootSceneNode(), mgr, id), Mesh(boidMesh),
	perching(false), perchTimer(0.0f), dontPerchTimer(0.0f)
{
#ifdef _DEBUG
		setDebugName("BoidSceneNode");
#endif

	boidMesh->grab();

	this->radius = this->Mesh->getBoundingBox().getExtent().Y/2;

	this->material.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture("media/boids/temporal-wake.jpg");
	this->material.Shininess = 128.0f;
	this->material.EmissiveColor.set(255, 0, 0, 255);
	this->material.SpecularColor.set(255, 255, 255, 255);
	this->material.NormalizeNormals = true;

	this->debugMat.Lighting = false;



	//copy borders
	memcpy(this->borders, borders, sizeof(irr::f32)*4);

	//set velocity to 0
	memset(this->velocity, 0, sizeof(irr::f32)*3);

	//start position should be near the target
	this->RelativeTranslation = core::vector3df(
		rand()%((irr::s32)target.X + 1000) + ((irr::s32)target.X - 1000),
		rand()%((irr::s32)target.Y + 2000) + ((irr::s32)target.Y + 1000),
		rand()%((irr::s32)target.Z + 1000) + ((irr::s32)target.Z - 1000));
}

BoidSceneNode::~BoidSceneNode()
{
	this->Mesh->drop();
}

const core::aabbox3d<float>& BoidSceneNode::getBoundingBox() const
{
	return Mesh ? Mesh->getBoundingBox() : Box;
}

f32 al = 0.0f;
f32 lastScaleTime = 0.0f;
bool forward = true;

void BoidSceneNode::OnAnimate(u32 timeMs)
{
	if (this->perching)
	{
		const f32 elapsed = (timeMs - lastScaleTime);
		lastScaleTime += elapsed;


		if (forward)
		{
			al += .01;

			if (al > 2)
			{
				al = 2;
				forward = !forward;
			}
		}
		else
		{
			al -= .01;

			if (al < 1.0f)
			{
				al = 1;
				forward = !forward;
			}
		}

		if (elapsed > 1500.0)
		{
			this->setScale(irr::core::vector3df(al, al, al));
			lastScaleTime = 0;
		}
	}

	ISceneNode::OnAnimate(timeMs);
}

void BoidSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	IMeshSceneNode::OnRegisterSceneNode();
}

void BoidSceneNode::render()
{
	video::IVideoDriver* const driver = SceneManager->getVideoDriver();

	//draw mesh
	const IMeshBuffer* const mb = Mesh->getMeshBuffer(0);
	driver->setMaterial(this->material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawMeshBuffer(mb);


	driver->setMaterial(this->debugMat);

	//draw velocity
	const irr::core::vector3df currVel(this->velocity[0], this->velocity[1], this->velocity[2]);
	driver->draw3DLine(irr::core::vector3df(0.0f, 0.0f, 0.0f), currVel*10, irr::video::SColor(255, 0, 255,0));

	//draw normals
	if (DebugDataVisible & scene::EDS_NORMALS)
	{
		core::vector3df normalizedNormal;
		static const f32 DebugNormalLength = 3.0f;

		// draw normals
		for (u32 g = 0; g < Mesh->getMeshBufferCount(); ++g)
		{
			const scene::IMeshBuffer* mb = Mesh->getMeshBuffer(g);
			const u32 vSize = video::getVertexPitchFromType(mb->getVertexType());
			const video::S3DVertex* v = (const video::S3DVertex*)mb->getVertices();
			const bool normalize = mb->getMaterial().NormalizeNormals;

			const u32 vCount = mb->getVertexCount();
			for (u32 i = 0; i != vCount; ++i)
			{
				normalizedNormal = v->Normal;
				if (normalize)
					normalizedNormal.normalize();

				driver->draw3DLine(v->Pos, v->Pos + (normalizedNormal*DebugNormalLength), irr::video::SColor(255, 128, 128, 0));

				v = (const video::S3DVertex*) ( (u8*) v + vSize );
			}
		}
	//	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	}


	//draw oabb
	if (DebugDataVisible & scene::EDS_BBOX)
		driver->draw3DBox(mb->getBoundingBox(), video::SColor(255,255,255,255));

	//draw ground ray
	driver->setTransform(video::ETS_WORLD, core::matrix4());
	driver->draw3DLine(this->groundRay.start, this->groundRay.end, irr::video::SColor(255, 0, 0, 255));
}

u32 BoidSceneNode::getMaterialCount() const
{
	return 1;
}

void BoidSceneNode::setMesh(IMesh*)
{
	//The mesh cannot be changed
}

bool BoidSceneNode::isReadOnlyMaterials() const
{
	return false;
}

void BoidSceneNode::setReadOnlyMaterials(bool)
{

}

video::SMaterial& BoidSceneNode::getMaterial(u32 i)
{
	return this->material;
}

void BoidSceneNode::applyRules(
	scene::ITriangleSelector* selector,
	const core::array<BoidSceneNode*>& boids,
	const f32 distanceToOtherBoids,
	const f32 seekCenterOfMass,
	const f32 matchVelocity,
	const core::vector3df& target,
	const f32 tendencyTowardsPlace,
	const f32 mimimumAboveGround,
	const f32 tendencyAvoidPlace,
	const bool scatterFlock,
	const f32 scatterFlockModifier,
	const f32 deltaTime,
	const f32 speedLimit
#ifdef _SOUND
	, irrklang::ISoundEngine* const soundEngine, const bool soundEnabled
#endif
	)
{
	if (this->perching)
	{
		this->perchTimer -= deltaTime;

		if (this->perchTimer > 0)
		{
			//dont apply rules if boid is perching
			return;
		}
		else
		{
			this->stopPerching();
		}
	}
	else
	{
		if (this->dontPerchTimer > 0.0f)
		{
			this->dontPerchTimer -= deltaTime;

			if (this->dontPerchTimer <= 0.0f)
			{
				//reset color
				this->material.EmissiveColor.set(255, 0, 0, 255);

				//show normals
				this->DebugDataVisible &= (~irr::scene::EDS_NORMALS);
			}
		}
	}

	//reset rules
	memset(this->rule_1,		0, sizeof(irr::f32)*3);
	memset(this->rule_2,		0, sizeof(irr::f32)*3);
	memset(this->rule_3,		0, sizeof(irr::f32)*3);
	memset(this->seek,			0, sizeof(irr::f32)*3);
	memset(this->avoid,			0, sizeof(irr::f32)*3);
	memset(this->avoidBorders,	0, sizeof(irr::f32)*2); //only 2d


	const u32 numBoids = boids.size();
	u32 boidsPerching = 0;

	u32 other;
	for (other = 0; other < numBoids; ++other)
	{
		if (*boids[other] != *this)
		{
			if (boids[other]->perching) // don't take perching boids into account
			{
				++boidsPerching;
				continue;
			}

			//rule 1
			this->rule_1[0] += boids[other]->RelativeTranslation.X;
			this->rule_1[1] += boids[other]->RelativeTranslation.Y;
			this->rule_1[2] += boids[other]->RelativeTranslation.Z;

			//rule 2
			if (fabs(boids[other]->RelativeTranslation.X - this->RelativeTranslation.X) < distanceToOtherBoids)
			{
				this->rule_2[0] -= boids[other]->RelativeTranslation.X - this->RelativeTranslation.X;
			}
			if (fabs(boids[other]->RelativeTranslation.Y - this->RelativeTranslation.Y) < distanceToOtherBoids)
			{
				this->rule_2[1] -= boids[other]->RelativeTranslation.Y - this->RelativeTranslation.Y;
			}
			if (fabs(boids[other]->RelativeTranslation.Z - this->RelativeTranslation.Z) < distanceToOtherBoids)
			{
				this->rule_2[2] -= boids[other]->RelativeTranslation.Z - this->RelativeTranslation.Z;
			}

			//rule 3
			this->rule_3[0] += boids[other]->velocity[0];
			this->rule_3[1] += boids[other]->velocity[1];
			this->rule_3[2] += boids[other]->velocity[2];
		}
	}

	const irr::u32 numBoidsNotPerchingExceptThis = numBoids - boidsPerching - 1;

	this->rule_1[0] /= (numBoidsNotPerchingExceptThis);
	this->rule_1[1] /= (numBoidsNotPerchingExceptThis);
	this->rule_1[2] /= (numBoidsNotPerchingExceptThis);

	this->rule_1[0] = (this->rule_1[0] - this->RelativeTranslation.X)/seekCenterOfMass;
	this->rule_1[1] = (this->rule_1[1] - this->RelativeTranslation.Y)/seekCenterOfMass;
	this->rule_1[2] = (this->rule_1[2] - this->RelativeTranslation.Z)/seekCenterOfMass;


	this->rule_3[0] /= (numBoidsNotPerchingExceptThis);
	this->rule_3[1] /= (numBoidsNotPerchingExceptThis);
	this->rule_3[2] /= (numBoidsNotPerchingExceptThis);

	this->rule_3[0] = (this->rule_3[0] - this->velocity[0])/matchVelocity;
	this->rule_3[1] = (this->rule_3[1] - this->velocity[1])/matchVelocity;
	this->rule_3[2] = (this->rule_3[2] - this->velocity[2])/matchVelocity;


	//own rules

	//seek : Tendency towards a particular place
//	if (!scatterFlock)
	{
		//Makes the boids in the flock fly towards the target.
		//Especially for distant goals, one may want to limit the magnitude of the returned vector.
		this->seek[0] = (target.X - this->RelativeTranslation.X)/tendencyTowardsPlace;
		this->seek[1] = (target.Y - this->RelativeTranslation.Y)/tendencyTowardsPlace;
		this->seek[2] = (target.Z - this->RelativeTranslation.Z)/tendencyTowardsPlace;
	}


	///avoid: Avoid ground and check if a boid wants to perch (if boid is touching the terrain already)
	this->groundRay = core::line3d<irr::f32>(
		this->RelativeTranslation, this->RelativeTranslation - irr::core::vector3df(0.0f, mimimumAboveGround, 0.0f));

	//cast ray downwards to see if boid touches the terrain
	core::vector3df outCollisionPoint;
	core::triangle3df outTriangle;
	ISceneNode* node = 0;
	if (this->SceneManager->getSceneCollisionManager()->getCollisionPoint(
			this->groundRay, &*selector, outCollisionPoint, outTriangle, node))
	{
		//terrain is this->mimimumAboveGround or less below, go away or land if boid touches the terrain
		if (outCollisionPoint.Y >= this->RelativeTranslation.Y - this->radius)
		{
			//too late, we are already on the ground. start perching routine
			this->startPerching(outCollisionPoint);

#ifdef _SOUND
			//play landing sound
			if (soundEngine && soundEnabled)
			{
				irrklang::ISound* const snd = soundEngine->play3D("media/sounds/mechanical_1.wav", this->RelativeTranslation, false, true);
				if (snd)
				{
					snd->setMinDistance(800.0f);
					snd->setIsPaused(false);
					snd->drop();
				}
			}
#endif
		}
		else //outCollisionPoint.Y < currentBoidPos.Y - radius
		{
			//The calculation required is identical to that of moving towards a particular
			//place, implemented above as 'seek' all that is required is a negative multiplier:

			//avoid = -tendencyAvoidPlace*(outCollisionPoint - currentBoidPos)/tendencyTowardsPlace;
			this->avoid[0] = -tendencyAvoidPlace*(outCollisionPoint.X - this->RelativeTranslation.X)/tendencyTowardsPlace;
			this->avoid[1] = -tendencyAvoidPlace*(outCollisionPoint.Y - this->RelativeTranslation.Y)/tendencyTowardsPlace;
			this->avoid[2] = -tendencyAvoidPlace*(outCollisionPoint.Z - this->RelativeTranslation.Z)/tendencyTowardsPlace;
		}
	}

	if (this->perching)
		return;


	//avoid x and z borders of the terrain
	if (this->RelativeTranslation.X < this->borders[0])
		this->avoidBorders[0] = tendencyTowardsPlace;
	else if (this->RelativeTranslation.X > this->borders[1])
		this->avoidBorders[0] = -tendencyTowardsPlace;

	if (this->RelativeTranslation.Z < this->borders[2])
		this->avoidBorders[1] = tendencyTowardsPlace;
	else if (this->RelativeTranslation.Z > this->borders[3])
		this->avoidBorders[1] = -tendencyTowardsPlace;


	//set new velocity based upon rules and elapsed time
	this->velocity[0] += ((scatterFlock ? -scatterFlockModifier*rule_1[0] : rule_1[0]) + rule_2[0] + rule_3[0] + seek[0] + avoid[0] + avoidBorders[0])*deltaTime;
	this->velocity[1] += ((scatterFlock ? -scatterFlockModifier*rule_1[1] : rule_1[1]) + rule_2[1] + rule_3[1] + seek[1] + avoid[1])*deltaTime;
	this->velocity[2] += ((scatterFlock ? -scatterFlockModifier*rule_1[2] : rule_1[2]) + rule_2[2] + rule_3[2] + seek[2] + avoid[2] + avoidBorders[1])*deltaTime;


	//Limiting the speed. It is a good idea to limit the magnitude of the boids' velocities,
	//this way they don't go too fast. Without such limitations, their speed will actually
	//fluctuate with a flocking-like tendency, and it is possible for them to momentarily go very fast.
	//irr::core::vector3df vel = this->velocity;
	const f32 absX = fabs(this->velocity[0]);
	const f32 absY = fabs(this->velocity[1]);
	const f32 absZ = fabs(this->velocity[2]);

	if(absX > speedLimit)
		this->velocity[0] = (this->velocity[0]/absX)*speedLimit;
	if(absY > speedLimit)
		this->velocity[1] = (this->velocity[1]/absY)*speedLimit;
	if(absZ > speedLimit)
		this->velocity[2] = (this->velocity[2]/absZ)*speedLimit;


	//set new position based upon rules
	this->RelativeTranslation += core::vector3df(this->velocity[0], this->velocity[1], this->velocity[2]);
}

void BoidSceneNode::startPerching(const core::vector3df& outCollisionPoint)
{
	//don't perch if we perched some time ago
	//if (this->dontperch)
	if (this->dontPerchTimer > 0)
		return;

	this->perching = true;

	//land on ground
	this->RelativeTranslation.Y = outCollisionPoint.Y + radius;

	//set velocity to 0
	memset(this->velocity, 0, sizeof(irr::f32)*3);

	//decide how long to stay
	const irr::u32 perchTime = (rand()%6) + 3;
	this->perchTimer = perchTime;

	//change color
	this->material.EmissiveColor.set(255, 255, 0, 0);



	//create a particle system
	IParticleSystemSceneNode* const ps = this->SceneManager->addParticleSystemSceneNode(false, 0, -1, outCollisionPoint);
	ps->setMaterialFlag(video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, this->SceneManager->getVideoDriver()->getTexture("media/boids/particle.bmp"));
	ps->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	ps->setScale(core::vector3df(50.0f, 50.0f, 50.0f));

	IParticleEmitter* const em = ps->createPointEmitter(
			core::vector3df(0.0f, 0.003f, 0.0f),
			500,600,
			irr::video::SColor(0, 20, 20, 20), video::SColor(0, 70, 70, 70),
			900,1800,
			45);
	em->setMinStartSize(core::dimension2d<f32>(10.0f, 10.0f));
	em->setMaxStartSize(core::dimension2d<f32>(50.0f, 50.0f));
	ps->setEmitter(em);
	em->drop();


	//create particle affectors
	IParticleAffector* paf = 0;

	paf = ps->createFadeOutParticleAffector(video::SColor(0,0,0,0), 500);
	ps->addAffector(paf);
	paf->drop();

	paf = ps->createGravityAffector(core::vector3df(0.0f, 0.006f, 0.0f), 2000);
	ps->addAffector(paf);
	paf->drop();


	//create deletion animator for particle system
	ISceneNodeAnimator* const anim = this->SceneManager->createDeleteAnimator(perchTime*1000);
	ps->addAnimator(anim);
	anim->drop();
}

void BoidSceneNode::stopPerching()
{
	this->perching = false;

	//reset scale to normal scale
	this->setScale(core::vector3df(1.0f, 1.0f, 1.0f));

	//a little help with takeoff ;)
	this->velocity[1] = radius*5.0f;//?

	//change color
	this->material.EmissiveColor.set(255, 255, 255, 0);

	//reset perchTimer
	this->perchTimer = 0;

	//don't perch again for 3 seconds
//	this->dontperch = true;
	this->dontPerchTimer = 3.0f;

	//add a particle system
	irr::scene::IParticleSystemSceneNode* const ps = this->SceneManager->addParticleSystemSceneNode(false, this);
	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, this->SceneManager->getVideoDriver()->getTexture("media/boids/particle.bmp"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
	ps->setScale(irr::core::vector3df(50.0f, 50.0f, 50.0f));

	irr::scene::IParticleEmitter* const em = ps->createPointEmitter(
			irr::core::vector3df(0.0f, 0.003f, 0.0f),
			50,100,
			irr::video::SColor(0, 255, 255, 0), irr::video::SColor(0, 128, 128, 0),
			200, 500,
			0);
	em->setMinStartSize(irr::core::dimension2d<irr::f32>(5.0f, 5.0f));
	em->setMaxStartSize(irr::core::dimension2d<irr::f32>(30.0f, 30.0f));
	ps->setEmitter(em);
	em->drop();

	//create particle affector
	irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(irr::video::SColor(0,0,0,0), 200);
	ps->addAffector(paf);
	paf->drop();

	//create deletion animator for particle system
	irr::scene::ISceneNodeAnimator* const anim = this->SceneManager->createDeleteAnimator(this->dontPerchTimer*1000);
	ps->addAnimator(anim);
	anim->drop();

	this->DebugDataVisible |= irr::scene::EDS_NORMALS;
}

}
}
