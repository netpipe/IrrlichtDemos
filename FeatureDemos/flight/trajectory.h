#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_
#include <vector>
#include <irrlicht.h>
#include <string>
#include <vector>
#include <deque>





using namespace irr;
#ifndef TRAJECTORY_PROPERTIES
#define TRAJECTORY_PROPERTIES
#define UPDATE_INTERVAL 200
#define POINT_COUNT_MAX 250
#endif




class Trajectory
{
public:
Trajectory
(
IrrlichtDevice* device,
scene::ISceneNode* node
);
virtual ~Trajectory();
void showDebugInfo();
void update();
void draw();
void setColor(video::SColor color) { Color = color; }
void addTrajectoryPoint
(
scene::ISceneNode* node,
std::deque<core::vector3df>& points,
u32 pointCountMax
);
void drawTrajectory(IrrlichtDevice* device, const std::deque<core::vector3df>& points, video::SColor color);
protected:
private:
IrrlichtDevice* Device;
scene::ISceneNode* Node;
video::SColor Color;
u32 LastUpdateTime;
std::deque<core::vector3df> Points;
};
#endif // _TRAJECTORY_H_

