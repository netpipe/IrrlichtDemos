#include "trajectory.h"
using namespace irr;
Trajectory::Trajectory
(
IrrlichtDevice* device,
scene::ISceneNode* node
)
:
Device(device),
Node(node),
Color(0x800000FF),
LastUpdateTime(0)
{
}
Trajectory::~Trajectory()
{
}
void Trajectory::showDebugInfo()
{
}
////////////////////////////////////////////////////////////////////////////////
void Trajectory::update()
{
u32 time = Device->getTimer()->getTime();
if (time > LastUpdateTime + UPDATE_INTERVAL)
{
addTrajectoryPoint(Node, Points, POINT_COUNT_MAX);
LastUpdateTime = time;
}
}
void Trajectory::draw()
{
drawTrajectory(Device, Points, Color);
}
void Trajectory::addTrajectoryPoint
(
scene::ISceneNode* node,
std::deque<core::vector3df>& points,
u32 pointCountMax
)
{
u32 pointCount = points.size();
if (pointCount >= pointCountMax)
{
points.pop_front();
}
node->updateAbsolutePosition();
core::vector3df pos = node->getAbsolutePosition();
if (!points.empty())
{
core::vector3df lastPoint = points[points.size()-1];
if (core::iszero(pos.getDistanceFromSQ(lastPoint)))
return;
}
points.push_back(pos);
}
void Trajectory::drawTrajectory(IrrlichtDevice* device, const std::deque<core::vector3df>& points, video::SColor
color)
{
if (points.size() <= 1)
return;
//video::SColor blue(128, 0, 0, 255);
// set mat:
video::SMaterial lineMaterial;
lineMaterial.Lighting = false;
lineMaterial.Thickness = 3.0f;
lineMaterial.FrontfaceCulling = false;
lineMaterial.BackfaceCulling = false;
lineMaterial.MaterialType = video::EMT_SOLID;
//lineMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
video::IVideoDriver* driver = device->getVideoDriver();
driver->setMaterial(lineMaterial);
driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
for (u32 n=0; n<(points.size()-1); ++n)
{
core::vector3df p1 = points[n];
core::vector3df p2 = points[n+1];
if (p1.equals(p2))
continue;
//driver->draw3DLine(p1, p2, blue);
driver->draw3DLine(p1, p2, color);
}
}

