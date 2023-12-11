#ifndef _CATMULL_ROM_H_
#define _CATMULL_ROM_H_

#include <irrlicht.h>
#include <string>
#include <vector>

using namespace irr;

class CatmullRom
{
	public:
	CatmullRom();
	
	CatmullRom
	(
		const core::vector3df& p0,
		const core::vector3df& p1,
		const core::vector3df& p2,
		const core::vector3df& p3,
		f32 speed
	);
	
	virtual ~CatmullRom();
	
	// t is the normalized time
	static
	core::vector3df getCatmullRomPos
	(
		const core::vector3df& p0,
		const core::vector3df& p1,
		const core::vector3df& p2,
		const core::vector3df& p3,
		f32 t
	);
	
	void setup
	(
		const core::vector3df& p0,
		const core::vector3df& p1,
		const core::vector3df& p2,
		const core::vector3df& p3,
		f32 speed
	);
	
	bool getPos(f32 elapsedTime, core::vector3df& pos);
	
	// estimated time to go from P1 to P2
	f32 getEstimatedTime();
	
	// dt is the (dimensionless) normalized time
	f32 getCatmullRomDist(f32 dt);
	
	static
	f32 getTotalDistance(const std::vector<core::vector3df>& points);
	
	
	f32 getSpeed() const { return Speed; }
	f32 getTotalTime() const { return TotalTime; }

	
	
private:
	core::vector3df P0;
	core::vector3df P1;
	core::vector3df P2;
	core::vector3df P3;
	
	f32 Speed;
	
	// in seconds
	f32 TotalTime;
};

#endif // _CATMULL_ROM_H_
