#include "IHoverCraft.h"
#include "../core/Game.h"
#include "../scene/Terrain.h"


/**
	It would be even better to find the point on the surface closest to
	vecChassisCorner rather than what's directly below it, but that is, as
	they say, left to the reader. :-)
		
	It might also be a good idea to add damping - to reduce the spring force
	by some value proportional to the vertical component of the velocity of
	the hovercraft's chassis at vecChassisCorner.  That might not be necessary
	though, I'd try without and see how it goes.
			
	One last thing - you probably don't want to apply any springs if the
	hovercraft flips inverted.  Perhaps the normal of the bottom of the
	hovercraft should figure into those calculations somewhere... or the angle
	between the hovercraft normal and the terrain normal.  I dunno... mess
	with it and see what happens. :-)
*/ 
class Spring
{

private:

	const Game* game;
	
	const IHoverCraft* craft;

	const Terrain* terrain;

	dGeomID ray;

	irr::core::vector3df corner;

	dReal desiredAltitude;

	dReal springRate;
	
	irr::core::vector3df collisionPoint;
	
	irr::core::vector3df collisionPointNormal;

	const void drawRay() const
	{
		irr::video::SMaterial mat;
		mat.Lighting = false;

		game->getVideoDriver()->beginScene(false, false, irr::video::SColor());
		game->getVideoDriver()->setMaterial(mat);
		game->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
		game->getVideoDriver()->draw3DLine(corner, collisionPoint, irr::video::SColor(255, 255, 255 , 0));
		game->getVideoDriver()->endScene();
	}

	const void applyForce() const
	{
		// get the current altitude as a scalar
		const dReal altitude = (corner - collisionPoint).getLength();

		// if the craft gets below the desired altitude, use a spring to push it back, in the direction of the terrain normal.
		if (altitude < desiredAltitude)
		{
			// compute the spring force
			irr::core::vector3df vecSpringForce = collisionPointNormal*(altitude - desiredAltitude);
			vecSpringForce *= springRate;

			// apply the spring force
			dBodyAddForceAtPos(craft->getBody(), -vecSpringForce.X, -vecSpringForce.Y, -vecSpringForce.Z, corner.X, corner.Y, corner.Z);
		}	
	}
	
	const bool checkCollision()
	{
		const irr::u32 MAX_CONTACTS = 1;
		dContact contact[MAX_CONTACTS];

		const irr::u32 numc = dCollide(ray, terrain->getGeom(), MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
		if(numc > 0)
		{
			//ray collided with terrain at this point
			collisionPoint.set(contact[0].geom.pos[0], contact[0].geom.pos[1], contact[0].geom.pos[2]);

			//normal of collison point
			collisionPointNormal.set(contact[0].geom.normal[0], contact[0].geom.normal[1], contact[0].geom.normal[2]);

			return true;
		}
		else return false;
	}
	
public:
	
	Spring(
		const Game* game, const IHoverCraft* craft, const Terrain* terrain,
		const irr::core::vector3df& corner, const dReal springRate, const dReal desiredAltitude, const bool drawHooverRays = false) :
			game(game), craft(craft), terrain(terrain), ray(0), corner(corner), desiredAltitude(desiredAltitude), springRate(springRate),
			collisionPoint(0.0f, 0.0f, 0.0f), collisionPointNormal(0.0f, 0.0f, 0.0f)
	{
		//cast ode ray
		ray = dCreateRay(game->getPhysics()->getSpace(), desiredAltitude);
		dGeomRaySet(ray, corner.X, corner.Y, corner.Z, 0.0, -1.0, 0.0);
		
		//apply some spring force if ray collides with terrain
		if (checkCollision())
		{
			//draw ray (debug output)
			if (drawHooverRays)
				this->drawRay();

			this->applyForce();
		}
	}

	~Spring()
	{
		dGeomDestroy(ray);
	}
};


IHoverCraft::IHoverCraft(Game* game, const irr::core::stringc& name,
	const irr::core::stringc& meshPath, const irr::core::stringc& texture,
	const irr::core::vector3df& position, const irr::core::vector3df& rotation,
	const irr::core::vector3df& scale,

	const dReal density, const dReal weight,
	const bool createBody, const bool gravity
) :
	GameObject(game, name, meshPath, texture, position, rotation, scale, density, weight, createBody, gravity),

	turnSpeed(500.0),
	rollSpeed(250.0),
	pitchSpeed(500.0),
	moveSpeed(8000.0),

	altitude(0)
{

}

IHoverCraft::~IHoverCraft()
{

}

/*
	dJointID Amotor;
	Amotor = dJointCreateAMotor(game->getPhysics()->getWorld(), 0);
	dJointAttach(Amotor, this->body, 0);
	dJointSetAMotorMode(Amotor, dAMotorEuler);
	dJointSetAMotorNumAxes(Amotor, 3);
	dJointSetAMotorAxis(Amotor, 0, 0, 1.0f, 0.0f, 0.0f);
	dJointSetAMotorAxis(Amotor, 1, 0, 0.0f, 1.0f, 0.0f);
	dJointSetAMotorAxis(Amotor, 2, 0, 0.0f, 0.0f, 1.0f);
	dJointSetAMotorAngle(Amotor, 0, 0.0f);
	dJointSetAMotorAngle(Amotor, 1, 0.0f);
	dJointSetAMotorAngle(Amotor, 2, 0.0f);
	dJointSetAMotorParam(Amotor, dParamLoStop, -0.0f);
	dJointSetAMotorParam(Amotor, dParamLoStop3, -0.0f);
	dJointSetAMotorParam(Amotor, dParamLoStop2, -0.0f);
	dJointSetAMotorParam(Amotor, dParamHiStop, 0.0f);
	dJointSetAMotorParam(Amotor, dParamHiStop3, 0.0f);
	dJointSetAMotorParam(Amotor, dParamHiStop2, 0.0f);
*/

void IHoverCraft::hover(const Terrain* terrain, const bool drawHooverRays)
{
	const irr::core::aabbox3df& box = this->node->getBoundingBox();
	const irr::core::matrix4& m = this->getTransformationMatrix(this->body);

	//get center of hovercraft
	irr::core::vector3df center = box.getCenter();
	m.transformVect(center);

	//get altitude (we don't need it for the hoovering itself. it's just to check if we *should* hoover)
	{
		//cast ray
		dGeomID ray = dCreateRay(game->getPhysics()->getSpace(), 1000000.0); //dInfinity lags a bit
		dGeomRaySet(ray, center.X, center.Y, center.Z, 0.0, -1.0, 0.0);
		
		irr::core::vector3df collisionPoint;
		
		const irr::u32 MAX_CONTACTS = 1;
		dContact contact[MAX_CONTACTS];
	
		const irr::u32 numc = dCollide(ray, terrain->getGeom(), MAX_CONTACTS, &contact[0].geom, sizeof(dContact));
		if(numc > 0)
		{
			//ray collided with terrain at this point
			collisionPoint.set(contact[0].geom.pos[0], contact[0].geom.pos[1], contact[0].geom.pos[2]);
			
			// get the current altitude as a scalar
			altitude = (center - collisionPoint).getLength();
			
			if (drawHooverRays)
			{
				irr::video::SMaterial mat;
				mat.Lighting = false;

				game->getVideoDriver()->beginScene(false, false, irr::video::SColor());
				game->getVideoDriver()->setMaterial(mat);
				game->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());
				game->getVideoDriver()->draw3DLine(center, collisionPoint, irr::video::SColor(255, 0, 0 , 255));
				game->getVideoDriver()->endScene();
			}
		}

		dGeomDestroy(ray);
	}

	//check if we really need to apply springs
	if (altitude > 15.0) return; //desiredAltitude*3


	/**
	 * Add springs
	 */

	const dReal desiredAltitude = 5.0; // we want to hover 5 units above ground
	const dReal normalSpringRate = 5.5; // the force of the spring
	const dReal weakSpring = normalSpringRate/2;

	//add a spring to the center
	{
		const Spring spring5(this->game, this, terrain, center, weakSpring, desiredAltitude, drawHooverRays);
	}
	
	//add springs to the 4 lower corners of the object aligned bounding box
	{
		irr::core::vector3df edges[8];
		box.getEdges(edges);

		irr::core::vector3df frontLeft = edges[2];
		irr::core::vector3df frontRight = edges[6];
		irr::core::vector3df rearLeft = edges[0];
		irr::core::vector3df rearRight = edges[4];

		//transform them to current transformation matrix
		m.transformVect(frontLeft);
		m.transformVect(frontRight);
		m.transformVect(rearLeft);
		m.transformVect(rearRight);

		const Spring spring1(this->game, this, terrain, frontLeft, normalSpringRate, desiredAltitude, drawHooverRays);
		const Spring spring2(this->game, this, terrain, frontRight, normalSpringRate, desiredAltitude, drawHooverRays);
		const Spring spring3(this->game, this, terrain, rearLeft, normalSpringRate, desiredAltitude, drawHooverRays);
		const Spring spring4(this->game, this, terrain, rearRight, normalSpringRate, desiredAltitude, drawHooverRays);
	}

	irr::f32 boxdepth = box.getExtent().Z/2;
	
	const irr::core::vector3df center1(center + irr::core::vector3df(0,0,boxdepth));
	const Spring spring6(this->game, this, terrain, center1, weakSpring, desiredAltitude, drawHooverRays);
	
	const irr::core::vector3df center2(center + irr::core::vector3df(0,0,-boxdepth));
	const Spring spring7(this->game, this, terrain, center2, weakSpring, desiredAltitude, drawHooverRays);

	boxdepth = box.getExtent().X/2;

	const irr::core::vector3df center3(center + irr::core::vector3df(boxdepth, 0, 0));
	const Spring spring8(this->game, this, terrain, center3, weakSpring, desiredAltitude, drawHooverRays);

	const irr::core::vector3df center4(center + irr::core::vector3df(-boxdepth, 0, 0));
	const Spring spring9(this->game, this, terrain, center4, weakSpring, desiredAltitude, drawHooverRays);
}

///Turn ship left or right.
const void IHoverCraft::turn(const irr::f32 rot) const
{ dBodyAddRelTorque(this->body, 0.0f, rot*turnSpeed, 0.0f); }

///Pitch ship up or down.
const void IHoverCraft::pitch(const irr::f32 rot) const
{ dBodyAddRelTorque(this->body, rot*pitchSpeed, 0.0f, 0.0f); }

///Roll ship left or right.
const void IHoverCraft::roll(const irr::f32 rot) const
{ dBodyAddRelTorque(this->body, 0.0f, 0.0f, rot*rollSpeed); }

const void IHoverCraft::move(const irr::f32 elapsed) const
{
	irr::core::matrix4 m;
	m.setRotationDegrees(this->node->getRotation());

	irr::core::vector3df vel(0.0f, 0.0f, elapsed*moveSpeed);
	m.transformVect(vel);

	dBodyAddForce(this->body, vel.X, vel.Y, vel.Z);
}
