#include "BoidSceneNode.h"
#include <ISceneManager.h>
#include <IVideoDriver.h>
#include <SMesh.h>
#include <SMeshBuffer.h>
#include <ISceneCollisionManager.h>
#include <IParticleSystemSceneNode.h>
#include <IMeshManipulator.h>

namespace irr
{
namespace scene
{


BoidSceneNode::BoidSceneNode(
	f32 radius, u32 polyCountX, u32 polyCountY,
	const core::vector3df& target, ITriangleSelector* selector, irr::f32 borders[4],
	ISceneNode* const parent, ISceneManager* const mgr, const s32 id,
	const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale) :
		IMeshSceneNode(parent, mgr, id, position, rotation, scale),	Mesh(0),
		radius(radius), selector(selector), perchTimer(0.0f), perching(false), dontPerchTimer(0.0f), dontperch(false)
{
#ifdef _DEBUG
		setDebugName("BoidSceneNode");
#endif

	Mesh = createSphereMesh(radius, polyCountX, polyCountY);

	video::SMaterial& mat = Mesh->getMeshBuffer(0)->getMaterial();
	mat.Shininess = 20.0f;
	mat.EmissiveColor.set(255, 0, 0, 255);
	mat.SpecularColor.set(255, 255, 255, 255);
	mat.TextureLayer[0].Texture = SceneManager->getVideoDriver()->getTexture("media/images/star.bmp");
	mat.NormalizeNormals = true;

	ISceneNodeAnimator* const anim = SceneManager->createRotationAnimator(core::vector3df(0.0f, 1.5f, 1.5f));
	this->addAnimator(anim);
	anim->drop();

	//copy borders
	memcpy(this->borders, borders, sizeof(irr::f32)*4);

	//set velocity to 0
	memset(velocity, 0, sizeof(irr::f32)*3);

	//pos should be near the target
	const irr::core::vector3df pos(
		rand()%((irr::s32)target.X + 1000) + ((irr::s32)target.X - 1000),
		rand()%((irr::s32)target.Y + 2000) + ((irr::s32)target.Y + 1000),
		rand()%((irr::s32)target.Z + 1000) + ((irr::s32)target.Z - 1000));
	this->setPosition(pos);
}

BoidSceneNode::~BoidSceneNode()
{
	if (Mesh)
		Mesh->drop();
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

	ISceneNode::OnRegisterSceneNode();
}

void BoidSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	if (Mesh && driver)
	{
		driver->setMaterial(Mesh->getMeshBuffer(0)->getMaterial());
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawMeshBuffer(Mesh->getMeshBuffer(0));
		if ( DebugDataVisible & scene::EDS_BBOX )
		{
			video::SMaterial m;
			m.Lighting = false;
			driver->setMaterial(m);
			driver->draw3DBox(Mesh->getMeshBuffer(0)->getBoundingBox(), video::SColor(255,255,255,255));
		}
	}
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
	if (i>0 || !Mesh)
		return ISceneNode::getMaterial(i);
	else
		return Mesh->getMeshBuffer(i)->getMaterial();
}

void BoidSceneNode::applyRules(
	const irr::core::array<BoidSceneNode*>& boids,
	const irr::f32 distanceToOtherBoids,
	const irr::f32 seekCenterOfMass,
	const irr::f32 matchVelocity,
	const irr::core::vector3df& target,
	const irr::f32 tendencyTowardsPlace,
	const irr::f32 mimimumAboveGround,
	const irr::f32 tendencyAvoidPlace,
	const bool scatterFlock,
	const irr::f32 scatterFlockModifier,
	const irr::f32 deltaTime,
	const irr::f32 speedLimit
#ifdef _SOUND
	, irrklang::ISoundEngine* const soundEngine, bool soundEnabled
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
		if (this->dontperch)
		{
			this->dontPerchTimer -= deltaTime;

			if (this->dontPerchTimer <= 0)
			{
				this->dontperch = false;
			}
		}
	}

	//reset rules
	const size_t arraySize = sizeof(irr::f32)*3;
	memset(this->rule_1, 0, arraySize);
	memset(this->rule_2, 0, arraySize);
	memset(this->rule_3, 0, arraySize);
	memset(this->seek, 0, arraySize);
	memset(this->avoid, 0, arraySize);

	const irr::core::vector3df& currentBoidPos = this->getAbsolutePosition();
	const irr::u32 numBoids = boids.size();

	irr::u32 other;
	for (other = 0; other < numBoids; ++other)
	{
		if (*boids[other] != *this)
		{
			const irr::core::vector3df& otherBoidPos = boids[other]->getAbsolutePosition();

			//rule 1
			this->rule_1[0] += otherBoidPos.X;
			this->rule_1[1] += otherBoidPos.Y;
			this->rule_1[2] += otherBoidPos.Z;

			//rule 2
			if (fabs(otherBoidPos.X - currentBoidPos.X) < distanceToOtherBoids)
				this->rule_2[0] -= otherBoidPos.X - currentBoidPos.X;
			if (fabs(otherBoidPos.Y - currentBoidPos.Y) < distanceToOtherBoids)
				this->rule_2[1] -= otherBoidPos.Y - currentBoidPos.Y;
			if (fabs(otherBoidPos.Z - currentBoidPos.Z) < distanceToOtherBoids)
				this->rule_2[2] -= otherBoidPos.Z - currentBoidPos.Z;

			//rule 3
			this->rule_3[0] += boids[other]->velocity[0];
			this->rule_3[1] += boids[other]->velocity[1];
			this->rule_3[2] += boids[other]->velocity[2];
		}
	}

	const irr::u32 numBoidsExceptThis = numBoids - 1;

	this->rule_1[0] /= (numBoidsExceptThis);
	this->rule_1[1] /= (numBoidsExceptThis);
	this->rule_1[2] /= (numBoidsExceptThis);

	this->rule_1[0] = (this->rule_1[0] - currentBoidPos.X)/seekCenterOfMass;
	this->rule_1[1] = (this->rule_1[1] - currentBoidPos.Y)/seekCenterOfMass;
	this->rule_1[2] = (this->rule_1[2] - currentBoidPos.Z)/seekCenterOfMass;


	this->rule_3[0] /= (numBoidsExceptThis);
	this->rule_3[1] /= (numBoidsExceptThis);
	this->rule_3[2] /= (numBoidsExceptThis);

	this->rule_3[0] = (this->rule_3[0] - this->velocity[0])/matchVelocity;
	this->rule_3[1] = (this->rule_3[1] - this->velocity[1])/matchVelocity;
	this->rule_3[2] = (this->rule_3[2] - this->velocity[2])/matchVelocity;


	//own rules

	//seek : Tendency towards a particular place
	if (!scatterFlock)
	{
		//Makes the boids in the flock fly towards the target.
		//Especially for distant goals, one may want to limit the magnitude of the returned vector.
		this->seek[0] = (target.X - currentBoidPos.X)/tendencyTowardsPlace;
		this->seek[1] = (target.Y - currentBoidPos.Y)/tendencyTowardsPlace;
		this->seek[2] = (target.Z - currentBoidPos.Z)/tendencyTowardsPlace;
	}


	//avoid: Avoid ground and check if a boid wants to perch (if boid is touching the terrain already)
	const irr::core::line3d<irr::f32> ray(currentBoidPos, currentBoidPos - irr::core::vector3df(0.0f, mimimumAboveGround, 0.0f));

	//cast ray downwards to see if boid touches the terrain
	irr::core::vector3df outCollisionPoint;
	irr::core::triangle3df outTriangle;
	if (this->SceneManager->getSceneCollisionManager()->getCollisionPoint(
			ray, this->selector, outCollisionPoint, outTriangle))
	{
		//terrain is this->mimimumAboveGround or less below, go away or land if boid touches the terrain
		if (outCollisionPoint.Y >= currentBoidPos.Y - radius)
		{
			//too late, we are already on the ground. start perching routine
			this->startPerching(outCollisionPoint);

#ifdef _SOUND
			//play landing sound
			if (soundEngine && soundEnabled)
			{
				irrklang::ISound* const snd = soundEngine->play3D("media/sounds/mechanical_1.wav", currentBoidPos, false, true);
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
			this->avoid[0] = -tendencyAvoidPlace*(outCollisionPoint.X - currentBoidPos.X)/tendencyTowardsPlace;
			this->avoid[1] = -tendencyAvoidPlace*(outCollisionPoint.Y - currentBoidPos.Y)/tendencyTowardsPlace;
			this->avoid[2] = -tendencyAvoidPlace*(outCollisionPoint.Z - currentBoidPos.Z)/tendencyTowardsPlace;
		}
	}

	if (this->perching)
		return;


	//avoid x and z borders of the terrain
	if (currentBoidPos.X < this->borders[0])
		velocity[0] *= -0.5;
	else if (currentBoidPos.X > this->borders[1])
		velocity[0] *= -0.5;

	if (currentBoidPos.Z < this->borders[2])
		velocity[2] *= -0.5;
	else if (currentBoidPos.Z > this->borders[3])
		velocity[2] *= -0.5;



	//set new velocity based upon rules and elapsed time
	this->velocity[0] += ((scatterFlock ? -scatterFlockModifier*rule_1[0] : rule_1[0]) + rule_2[0] + rule_3[0] + seek[0] + avoid[0])*deltaTime;
	this->velocity[1] += ((scatterFlock ? -scatterFlockModifier*rule_1[1] : rule_1[1]) + rule_2[1] + rule_3[1] + seek[1] + avoid[1])*deltaTime;
	this->velocity[2] += ((scatterFlock ? -scatterFlockModifier*rule_1[2] : rule_1[2]) + rule_2[2] + rule_3[2] + seek[2] + avoid[2])*deltaTime;


	//Limiting the speed. It is a good idea to limit the magnitude of the boids' velocities,
	//this way they don't go too fast. Without such limitations, their speed will actually
	//fluctuate with a flocking-like tendency, and it is possible for them to momentarily go very fast.
	//irr::core::vector3df vel = this->velocity;
	const irr::f32 absX = fabs(this->velocity[0]);
	const irr::f32 absY = fabs(this->velocity[1]);
	const irr::f32 absZ = fabs(this->velocity[2]);

	if(absX > speedLimit)
		this->velocity[0] = (this->velocity[0]/absX)*speedLimit;

	if(absY > speedLimit)
		this->velocity[1] = (this->velocity[1]/absY)*speedLimit;

	if(absZ > speedLimit)
		this->velocity[2] = (this->velocity[2]/absZ)*speedLimit;


	//set new position based upon rules
	this->setPosition(irr::core::vector3df(
			currentBoidPos.X + this->velocity[0],
			currentBoidPos.Y + this->velocity[1],
			currentBoidPos.Z + this->velocity[2]));
}

void BoidSceneNode::startPerching(const irr::core::vector3df& outCollisionPoint)
{
	//don't perch if we perched some time ago
	if (this->dontperch)
		return;

	this->perching = true;

	irr::core::vector3df pos(this->getAbsolutePosition());

	//land on ground
	pos.Y = outCollisionPoint.Y + radius;
	this->setPosition(pos);

	//set velocity to 0
	memset(this->velocity, 0, sizeof(irr::f32)*3);

	//decide how long to stay
	const irr::u32 perchTime = (rand()%6) + 3;
	this->perchTimer = perchTime;

	//change color
	video::SMaterial& mat = Mesh->getMeshBuffer(0)->getMaterial();
	mat.EmissiveColor.set(255, 255, 0, 0);
/*
	// create smoke particle system
	IParticleSystemSceneNode* const pas = this->SceneManager->addParticleSystemSceneNode(false, 0, -1, outCollisionPoint);

	//create box emitter
	irr::scene::IParticleEmitter* const em = pas->createBoxEmitter(
			irr::core::aabbox3d<irr::f32>(-radius, 0.0f, -radius, radius, radius, radius), //box
			irr::core::vector3df(0.0f, 0.25f, 0.0f), //direction
			200, 600, //minParticlesPerSecond, maxParticlesPerSecond
			irr::video::SColor(0, 255, 0, 0), irr::video::SColor(0, 255, 255, 255),
			2000, 4000, 20); //lifeTimeMin, lifeTimeMax, maxAngleDegrees
	em->setMinStartSize(irr::core::dimension2d<irr::f32>(0.01f, 0.01f));
	//em->setMaxStartSize(irr::core::dimension2d<irr::f32>(10.0f, 10.0f));
	pas->setEmitter(em);
	em->drop();

	//create FadeOut particle affector
	irr::scene::IParticleAffector* const paf = pas->createFadeOutParticleAffector();
	pas->addAffector(paf);
	paf->drop();

	pas->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	pas->setMaterialTexture(0, this->SceneManager->getVideoDriver()->getTexture("media/images/star.bmp"));
	pas->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
*/

	irr::scene::IParticleSystemSceneNode* ps = this->SceneManager->addParticleSystemSceneNode(false, 0, -1, outCollisionPoint);
	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, this->SceneManager->getVideoDriver()->getTexture("media/images/particle.bmp"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
	ps->setScale(irr::core::vector3df(50.0f, 50.0f, 50.0f));

	irr::scene::IParticleEmitter* em = ps->createPointEmitter(
			irr::core::vector3df(0.0f, 0.003f, 0.0f),
			500,600,
			irr::video::SColor(0, 20, 20, 20), irr::video::SColor(0, 70, 70, 70),
			900,1800,
			45);
	em->setMinStartSize(irr::core::dimension2d<irr::f32>(10.0f, 10.0f));
	em->setMaxStartSize(irr::core::dimension2d<irr::f32>(50.0f, 50.0f));
	ps->setEmitter(em);
	em->drop();

	irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(irr::video::SColor(0,0,0,0), 500);
	ps->addAffector(paf);
	paf->drop();

	paf = ps->createGravityAffector(irr::core::vector3df(0.0f, 0.006f, 0.0f), 2000);
	ps->addAffector(paf);
	paf->drop();


	//create deletion animator
	//irr::scene::ISceneNodeAnimator* const anim = this->getSceneManager()->createDeleteAnimator(perchTime*1000);
	//ps->addAnimator(anim);
	//anim->drop();

}

void BoidSceneNode::stopPerching()
{
	this->setScale(core::vector3df(1.0f, 1.0f, 1.0f));

	this->perching = false;

	//a little help with takeoff ;)
	this->velocity[1] = radius*5.0f;

	//reset color
	video::SMaterial& mat = Mesh->getMeshBuffer(0)->getMaterial();
	mat.EmissiveColor.set(255, 0, 0, 255);

	//reset perchTimer
	this->perchTimer = 0;

	//don't perch again for 3 seconds
	this->dontperch = true;
	this->dontPerchTimer = 3.0f;
}

IMesh* BoidSceneNode::createSphereMesh(f32 radius, u32 polyCountX, u32 polyCountY)
{
	SMeshBuffer* buffer = new SMeshBuffer();

	// thanks to Alfaz93 who made his code available for Irrlicht on which
	// this one is based!

	// we are creating the sphere mesh here.

	if (polyCountX < 2)
		polyCountX = 2;
	if (polyCountY < 2)
		polyCountY = 2;
	if (polyCountX * polyCountY > 32767) // prevent u16 overflow
	{
		if (polyCountX > polyCountY) // prevent u16 overflow
			polyCountX = 32767/polyCountY-1;
		else
			polyCountY = 32767/(polyCountX+1);
	}

	u32 polyCountXPitch = polyCountX+1; // get to same vertex on next level
	buffer->Vertices.set_used((polyCountXPitch * polyCountY) + 2);
	buffer->Indices.set_used((polyCountX * polyCountY) * 6);

	const video::SColor clr(100, 255,255,255);

	u32 i=0;
	u32 level = 0;

	for (u32 p1 = 0; p1 < polyCountY-1; ++p1)
	{
		//main quads, top to bottom
		for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
		{
			const u32 curr = level + p2;
			buffer->Indices[i] = curr + polyCountXPitch;
			buffer->Indices[++i] = curr;
			buffer->Indices[++i] = curr + 1;
			buffer->Indices[++i] = curr + polyCountXPitch;
			buffer->Indices[++i] = curr+1;
			buffer->Indices[++i] = curr + 1 + polyCountXPitch;
			++i;
		}

		// the connectors from front to end
		buffer->Indices[i] = level + polyCountX - 1 + polyCountXPitch;
		buffer->Indices[++i] = level + polyCountX - 1;
		buffer->Indices[++i] = level + polyCountX;
		++i;

		buffer->Indices[i] = level + polyCountX - 1 + polyCountXPitch;
		buffer->Indices[++i] = level + polyCountX;
		buffer->Indices[++i] = level + polyCountX + polyCountXPitch;
		++i;
		level += polyCountXPitch;
	}

	const u32 polyCountSq = polyCountXPitch * polyCountY; // top point
	const u32 polyCountSq1 = polyCountSq + 1; // bottom point
	const u32 polyCountSqM1 = (polyCountY - 1) * polyCountXPitch; // last row's first vertex

	for (u32 p2 = 0; p2 < polyCountX - 1; ++p2)
	{
		// create triangles which are at the top of the sphere

		buffer->Indices[i] = polyCountSq;
		buffer->Indices[++i] = p2 + 1;
		buffer->Indices[++i] = p2;
		++i;

		// create triangles which are at the bottom of the sphere

		buffer->Indices[i] = polyCountSqM1 + p2;
		buffer->Indices[++i] = polyCountSqM1 + p2 + 1;
		buffer->Indices[++i] = polyCountSq1;
		++i;
	}

	// create final triangle which is at the top of the sphere

	buffer->Indices[i] = polyCountSq;
	buffer->Indices[++i] = polyCountX;
	buffer->Indices[++i] = polyCountX-1;
	++i;

	// create final triangle which is at the bottom of the sphere

	buffer->Indices[i] = polyCountSqM1 + polyCountX - 1;
	buffer->Indices[++i] = polyCountSqM1;
	buffer->Indices[++i] = polyCountSq1;

	// calculate the angle which separates all points in a circle
	const f64 AngleX = 2 * core::PI / polyCountX;
	const f64 AngleY = core::PI / polyCountY;

	i = 0;
	f64 axz;

	// we don't start at 0.

	f64 ay = 0;//AngleY / 2;

	for (u32 y = 0; y < polyCountY; ++y)
	{
		ay += AngleY;
		const f64 sinay = sin(ay);
		axz = 0;

		// calculate the necessary vertices without the doubled one
		for (u32 xz = 0;xz < polyCountX; ++xz)
		{
			// calculate points position

			const core::vector3df pos(static_cast<f32>(radius * cos(axz) * sinay),
					static_cast<f32>(radius * cos(ay)),
					static_cast<f32>(radius * sin(axz) * sinay));
			// for spheres the normal is the position
			core::vector3df normal(pos);
			normal.normalize();

			// calculate texture coordinates via sphere mapping
			// tu is the same on each level, so only calculate once
			f32 tu = 0.5f;
			if (y==0)
			{
				if (normal.Y != -1.0f && normal.Y != 1.0f)
					tu = static_cast<f32>(acos(core::clamp(normal.X/sinay, -1.0, 1.0)) * 0.5 *core::RECIPROCAL_PI64);
					if (normal.Z < 0.0f)
						tu=1-tu;
			}
			else
				tu = buffer->Vertices[i-polyCountXPitch].TCoords.X;
			buffer->Vertices[i] = video::S3DVertex(pos.X, pos.Y, pos.Z,
					normal.X, normal.Y, normal.Z,
					clr, tu,
					static_cast<f32>(ay*core::RECIPROCAL_PI64));
			++i;
			axz += AngleX;
		}
		// This is the doubled vertex on the initial position
		buffer->Vertices[i] = video::S3DVertex(buffer->Vertices[i-polyCountX]);
		buffer->Vertices[i].TCoords.X=1.0f;
		++i;
	}

	// the vertex at the top of the sphere
	buffer->Vertices[i] = video::S3DVertex(0.0f,radius,0.0f, 0.0f,1.0f,0.0f, clr, 0.5f, 0.0f);

	// the vertex at the bottom of the sphere
	++i;
	buffer->Vertices[i] = video::S3DVertex(0.0f,-radius,0.0f, 0.0f,-1.0f,0.0f, clr, 0.5f, 1.0f);

	// recalculate bounding box

	buffer->BoundingBox.reset(buffer->Vertices[i].Pos);
	buffer->BoundingBox.addInternalPoint(buffer->Vertices[i-1].Pos);
	buffer->BoundingBox.addInternalPoint(radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(-radius,0.0f,0.0f);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,radius);
	buffer->BoundingBox.addInternalPoint(0.0f,0.0f,-radius);

	SMesh* mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	buffer->drop();

	mesh->recalculateBoundingBox();
	return mesh;
}

}
}
