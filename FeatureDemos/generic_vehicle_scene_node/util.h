#ifndef _IRRLICHT_UTIL_H_
#define _IRRLICHT_UTIL_H_


#include <irrlicht.h>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <set>

using namespace irr;


class Util
{
public:

Util();
virtual ~Util();


static
void print(const core::vector3df& vec);

static
void drawNodeLabel
(
	IrrlichtDevice* device,
	scene::ISceneNode* node,
	const core::stringw& text,
	s32 xOffset = 0,
	s32 yOffset = 0
);


static
void rotateCameraTowardsNodeAroundYAxis
(
	scene::ICameraSceneNode* camera,
	scene::ISceneNode* node,
	f32 minDist = 0.0f,
	bool rotateCameraBy90Deg = false
);


static
void rotateCameraTowardsPointAroundYAxis
(
	scene::ICameraSceneNode* camera,
	const core::vector3df& pos,
	f32 minDist,
	bool rotateCameraBy90Deg
);


static
void drawXYZAxes
(
	IrrlichtDevice* device,
	const core::vector3df& pos,
	u32 length = 10
);


private:


};

#endif // _IRRLICHT_UTIL_H_
