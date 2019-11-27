/*

Radar - cpp file

*/

#include "Radar.h"


// constructor
cRadar::cRadar() {
}

// destructor
cRadar::~cRadar() {
}

// Initialization
void cRadar::Initialize() {
}

// draw radar
core::position2d<s32> cRadar::calcRadarPosition(bool ancientRadar, core::vector3df lookAt, core::vector3df pos3d, core::position2d<s32> pos2d, core::position2d<s32> center) {
	
	const f32 pi = 3.1415;
	
	core::vector3df a = lookAt;
	
	a.Y=0;

	core::vector3df b = pos3d;
	
	irr::f32 distance = pos3d.getLength();

	b*=-1;
	b.Y=0;

	a.normalize();
	b.normalize();

	f32 alpha = acos((a.dotProduct(b)) / (a.getLength() * b.getLength()));

	core::vector3df v1 = core::vector3df(0,1,0);
	core::vector3df v2 = core::vector3df(pos2d.X,pos2d.Y,0);

	core::vector3df c = a.crossProduct(b);

	if (c.Y > 0) {
		(alpha > pi/2) ? (alpha = -alpha+pi/2) : (alpha = -alpha+pi/2);
	} else {
		alpha += pi/2;
	}

	irr::u32 x,y;

	if (ancientRadar==false) {
		x = (irr::u32) (300*cos(alpha) + center.X);
		y = (irr::u32) (round(-150*sin(alpha) + center.Y));
	} else {
		
		irr::f32 radiusX, radiusY;
		radiusX = distance / 500.f;
		radiusY = distance / 220.f;
		
		if (radiusX > 1.f)
			radiusX=1.f;
		if (radiusY > 1.f)
			radiusY=1.f;

		radiusX *= 80.f;
		radiusY *= 35.f;

		pos2d.X = (irr::u32) (radiusX*cos(alpha) + center.X);
		pos2d.Y = (irr::u32) (round(-radiusY*sin(alpha) + 2*center.Y-52));
		
		return pos2d;
		
	}



	a.X=x;
	a.Y=y;

	if (ancientRadar==false) {
		b.X = pos2d.X;
		b.Y = pos2d.Y;
	}



	c.X = center.X;
	c.Y = center.Y;


	a -= c;
	b -= c;
	
	if (ancientRadar==false) {

		if (b.getLength() > a.getLength()) {
			pos2d.X=x;
			pos2d.Y=y;
		}

	}


	
	return pos2d;

}
