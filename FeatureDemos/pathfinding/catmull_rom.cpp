#include <irrlicht.h>
#include <iostream>
#include <fstream>

#include <vector>

#include "catmull_rom.h"

using namespace irr;
CatmullRom::CatmullRom()
:	P0(0.0f),
	P1(0.0f),
	P2(0.0f),
	P3(0.0f),
	Speed(0.0f),
	TotalTime(FLT_MAX)
{
	
}


CatmullRom::CatmullRom
(
	const core::vector3df& p0,
	const core::vector3df& p1,
	const core::vector3df& p2,
	const core::vector3df& p3,
	f32 speed
)
:	P0(p0),
	P1(p1),
	P2(p2),
	P3(p3),
	Speed(speed)
{
	if (speed <= 0.0f)
	{
		TotalTime = FLT_MAX;
		
		return;
	}
	
	/***
	f32 d = P1.getDistanceFrom(P2);
	Time = d/speed;
	***/
	
	f32 dt = 0.1f;
	TotalTime = getCatmullRomDist(dt)/speed;
}


CatmullRom::~CatmullRom()
{
}


core::vector3df CatmullRom::getCatmullRomPos
(
	const core::vector3df& p0,
	const core::vector3df& p1,
	const core::vector3df& p2,
	const core::vector3df& p3,
	f32 t
)
{
	return 0.5f *
	(
		(2 * p1)
		+ (-1.0f * p0 + p2) * t
		+ (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * powf(t, 2.0f)
		+ (-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * powf(t, 3.0f)
	);
}


void CatmullRom::setup
(
	const core::vector3df& p0,
	const core::vector3df& p1,
	const core::vector3df& p2,
	const core::vector3df& p3,
	f32 speed
)
{
	P0 = p0;
	P1 = p1;
	P2 = p2;
	P3 = p3;
	
	Speed = speed;
	if (Speed <= 0.0f)
	{
		TotalTime = FLT_MAX;
	}
	else
	{
		f32 dt = 0.1f;
		TotalTime = getCatmullRomDist(dt)/Speed;
	}
}


bool CatmullRom::getPos(f32 elapsedTime, core::vector3df& pos)
{
	if (elapsedTime < 0.0f)
		return false; // bail out
	
	if (elapsedTime <= TotalTime)
	{
		f32 t = elapsedTime / TotalTime;
		pos = getCatmullRomPos(P0, P1, P2, P3, t);
		return true; // re-run on next cycle
	}
	else
	{
		return false; // bail out
	}
}




f32 CatmullRom::getCatmullRomDist(f32 dt)
{
	int nums = int(1.0f/dt);
	if (nums <= 1)
		return -1.0f;
	
	f32 dist = 0.0f;
	core::vector3df v1, v2;
	for (int n = 1; n < nums; ++n)
	{
		v1 = getCatmullRomPos(P0, P1, P2, P3, (n-1) * dt);
		v2 = getCatmullRomPos(P0, P1, P2, P3, n * dt);
		dist += v1.getDistanceFrom(v2);
	}
	
	return dist;

}


f32 CatmullRom::getTotalDistance(const std::vector<core::vector3df>& points)
{
	int nums = int(points.size());
	if (nums < 2)
		return -1.0;
	
	f32 dist = 0.0f;
	core::vector3df v1, v2;
	for (int n = 1; n < nums; ++n)
	{
		v1 = points.at(n-1);
		v2 = points.at(n);
		
		dist += v1.getDistanceFrom(v2);
	}
	return dist;
}



