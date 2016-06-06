#ifndef OBJECT_H
#define OBJECT_H

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;

class cObject
{
	public:
		cObject()
		{
			type = 0;
			pos = vector2df(0, 0);
			vel = vector2df(0, 0);
			size = 10;
			mass = 10; 
		};
					
		int type;			// Type
		vector2df pos;		// position
		vector2df vel;		// velocity
		int size;			// size
		int mass;			// mass
};

#endif