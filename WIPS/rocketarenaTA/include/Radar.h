/*

Radar - header file

*/

#ifndef __Radar_h
#define __Radar_h

#include <irrlicht.h>

using namespace irr;


class cRadar {
public:

	// constructor
	cRadar();
	
	// destructor
	~cRadar();
	
	// Initialization
	void Initialize();
	
	// calculates position of items in radar
	core::position2d<s32> calcRadarPosition(bool ancientRadar, core::vector3df lookAt, core::vector3df pos3d, core::position2d<s32> pos2d, core::position2d<s32> center);
	
	
private:

};


#endif
