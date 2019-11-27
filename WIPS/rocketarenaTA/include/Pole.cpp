/*

Pole - cpp file

*/

#include "Pole.h"


// constructor
cPole::cPole() {
}

// destructor
cPole::~cPole() {
	einstein.Initialize();
}

// Initialization
void cPole::Initialize() {
}

// add a new pole
void cPole::addPole(TA::StaticObject* addPole, core::vector3df pos) {
	poleArray.push_back(addPole);
	polePosition.push_back(pos);
}

// get amount of poles
u32 cPole::getPoleCount() {
	return poleArray.size();
}

// checking cables for collision
u32 cPole::checkForCollision(TA::Physics &physics, core::line3d<f32> cable) {
	
	TA::Collision collision;
	
	TA::Vec3 start;
	TA::Vec3 end;
	TA::Vec3 normal;
		
	start = einstein.convertIrrlichtPos(cable.start);
	end = einstein.convertIrrlichtPos(cable.end);
	
	core::vector3df n = cable.end - cable.start;
	normal = einstein.convertIrrlichtPos(n.normalize());
	

	collision = physics.TestLineForCollision(
						start,
						end);
		
	if (collision.CollisionOccurred()) {

		for (u32 i=0; i<poleArray.size(); i++) {
			
			if (poleArray[i] == collision.GetStaticObject())
				return i+1;
		}
	}

	return 0;
}

// calculate the angular velocity to let the rocket rotate around the pole
core::vector3df cPole::calculateAngularVelocity(core::vector3df rocketLookAt, core::vector3df poleLookAt, f32 radius) {
	
	f32 alpha;
	f32 delta;
	core::vector3df angularVelocity = core::vector3df(0,0,0);
	delta = poleLookAt.getLength()-radius;
	
	if (delta<0.f)
		delta*=-1.f;
	
	rocketLookAt.Y = 0.f;
	poleLookAt.Y = 0.f;
	
	rocketLookAt.normalize();
	poleLookAt.normalize();
	
	alpha = acos( rocketLookAt.dotProduct(poleLookAt) / rocketLookAt.getLength() * poleLookAt.getLength() );
	alpha -= 3.1415/1.8f;

	if (alpha<0.f) {
		angularVelocity = core::vector3df(0,-alpha ,0);
		angularVelocity *= 1.5f*delta;
	}
	
	return angularVelocity;	
}

// return pole position
const core::vector3df cPole::getPolePosition(u32 i) {
	if (i==0)
		return core::vector3df(0,0,0);
	else
		return polePosition[i-1];
}

// return polePosition array
const core::array<core::vector3df> cPole::getPolePosition() {
	return polePosition;
}




