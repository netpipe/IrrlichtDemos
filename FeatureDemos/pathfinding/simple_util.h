#ifndef _SIMPLE_UTIL_H_
#define _SIMPLE_UTIL_H_


#include <irrlicht.h>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>


using namespace irr;

#ifndef SIMPLE_UTIL_PROPERTIES
#define SIMPLE_UTIL_PROPERTIES
#define MAX_DIST_SQ 200000.0f
#endif


class SimpleUtil
{
public:
SimpleUtil();
virtual ~SimpleUtil();
//IrrlichtDevice* device


static
std::vector<core::vector3df> getRandomGridPoints
(
	scene::ISceneNode* scene,
	f32 yoffset, // from -1.0f to 1.0f
	u32 count
);


static
std::vector<core::vector3df> getHexGridPoints
(
	scene::ISceneNode* scene,
	f32 radius,
	f32 yoffset, // from -1.0f to 1.0f
	bool withCenters = false
);

static
bool containsPoint(const std::vector<core::vector3df>& ps, const core::vector3df& p);

static
void drawHexGrid
(
	IrrlichtDevice* device,
	scene::ISceneNode* scene,
	f32 radius,
	f32 yoffset, // from -1.0f to 1.0f
	video::SColor color
);


static
bool getHexRowAndCols
(
	f32 w, f32 h, f32 r,
	u32& rows, u32& cols
);

static
core::vector3df getHexPos(f32 x0, f32 z0, f32 ypos, f32 r, u32 row, u32 col);


static
void drawHex
(
	IrrlichtDevice* device,
	const core::vector3df& pos,
	f32 radius,
	video::SColor color
);


static
std::vector<core::vector3df> getPointsOnHex
(
	const core::vector3df& pos,
	f32 radius
);


static
void drawBBoxTop
(
	IrrlichtDevice* device,
	scene::ISceneNode* scene,
	video::SColor color
);

static
f32 getRand();

static
void drawTrajectory(IrrlichtDevice* device, const std::deque<core::vector3df>& points, video::SColor color);

static
void changeCameraFPSMoveSpeed
(
	scene::ICameraSceneNode* camera,
	f32 factor,
	f32 MIN_CAM_MOVE_SPEED = 0.001f,
	f32 MAX_CAM_MOVE_SPEED = 20.0f
);

//makeScreenshot(device, L"screenshot_", 15);
static
void makeScreenshot
(
	IrrlichtDevice* device,
	const core::stringw& basename,
	u32 num
);


static
void makeScreenshot
(
	IrrlichtDevice* device,	
	const core::stringw& filename
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
std::vector<core::vector3df> getPointsOnHorizontalCircle
(
	const core::vector3df& center,
	f32 radius,
	f32 offsetY,
	u32 count = 64
);

static
void print(const core::vector3df& vec);

private:
	
};


#endif // _SIMPLE_UTIL_H_
