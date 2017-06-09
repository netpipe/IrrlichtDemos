#include <iostream>
#include <sstream>
#include <cstdlib>
#include <fstream>
#include <ctime>


#include "simple_util.h"

SimpleUtil::SimpleUtil()
{
	
}


SimpleUtil::~SimpleUtil()
{
}


std::vector<core::vector3df> SimpleUtil::getRandomGridPoints
(
	scene::ISceneNode* scene,
	f32 yoffset, // from -1.0f to 1.0f
	u32 count
)
{
	std::vector<core::vector3df> ps;
	
	scene->updateAbsolutePosition();
	//core::aabbox3d<f32> box = scene->getBoundingBox();
	core::aabbox3d<f32> box = scene->getTransformedBoundingBox();
	core::vector3df extent = box.getExtent();
	core::vector3df center = box.getCenter();
	
	f32 dx = extent.X * 0.5f;
	f32 dy = extent.Y * 0.5f;
	f32 dz = extent.Z * 0.5f;
	
	f32 cx = center.X;
	f32 cy = center.Y;
	f32 cz = center.Z;
	

	for (u32 n=0; n<count; ++n)
	{
		f32 x = cx + dx*getRand();
		f32 z = cz + dz*getRand();
		f32 y = cy + dy*yoffset;

		ps.push_back(core::vector3df(x,y,z));
	}
	
	return ps;
}

std::vector<core::vector3df> SimpleUtil::getHexGridPoints
(
	scene::ISceneNode* scene,
	f32 radius,
	f32 yoffset, // from -1.0f to 1.0f
	bool withCenters
)
{
	std::vector<core::vector3df> ps;
	
	scene->updateAbsolutePosition();
	//core::aabbox3d<f32> box = scene->getBoundingBox();
	core::aabbox3d<f32> box = scene->getTransformedBoundingBox();
	core::vector3df extent = box.getExtent();
	
	u32 rows = 0;
	u32 cols = 0;
	if (!getHexRowAndCols(extent.X, extent.Z, radius, rows, cols))
		return ps;
	
	core::vector3df center = box.getCenter();
	f32 dx = extent.X * 0.5f;
	f32 dy = extent.Y * 0.5f;
	f32 dz = extent.Z * 0.5f;
	
	f32 cx = center.X;
	f32 cy = center.Y;
	f32 cz = center.Z;
	
	f32 x0 = cx - dx;
	f32 z0 = cz + dz;
	f32 ypos = cy + dy*yoffset;
	
	
	for (u32 r=0; r<rows; ++r)
	{
		for (u32 c=0; c<cols; ++c)
		{
			core::vector3df pos = getHexPos(x0, z0, ypos, radius, r, c);
			if (withCenters)
				ps.push_back(pos);
			std::vector<core::vector3df> pointsOnHex = getPointsOnHex(pos, radius);
			for (int n=0; n<int(pointsOnHex.size()); ++n)
			{
				core::vector3df p = pointsOnHex[n];
				if (containsPoint(ps, p))
					continue;
				ps.push_back(p);
			}
			
		}
	}
	
	return ps;
}


bool SimpleUtil::containsPoint(const std::vector<core::vector3df>& ps, const core::vector3df& p)
{
	for (int n=0; n<int(ps.size()); ++n)
	{
		if (p.equals(ps[n], 0.001f))
			return true;
	}
	return false;
}

void SimpleUtil::drawHexGrid
(
	IrrlichtDevice* device,
	scene::ISceneNode* scene,
	f32 radius,
	f32 yoffset,
	video::SColor color
)
{
	scene->updateAbsolutePosition();
	//core::aabbox3d<f32> box = scene->getBoundingBox();
	core::aabbox3d<f32> box = scene->getTransformedBoundingBox();
	core::vector3df extent = box.getExtent();
	
	u32 rows = 0;
	u32 cols = 0;
	if (!getHexRowAndCols(extent.X, extent.Z, radius, rows, cols))
		return;
	
	//printf("rows=%i, cols=%i\n", rows, cols);
	
	core::vector3df center = box.getCenter();
	f32 dx = extent.X * 0.5f;
	f32 dy = extent.Y * 0.5f;
	f32 dz = extent.Z * 0.5f;
	
	f32 cx = center.X;
	f32 cy = center.Y;
	f32 cz = center.Z;
	
	f32 x0 = cx - dx;
	f32 z0 = cz + dz;
	f32 ypos = cy + dy*yoffset;
	
	
	for (u32 r=0; r<rows; ++r)
	{
		for (u32 c=0; c<cols; ++c)
		{
			core::vector3df pos = getHexPos(x0, z0, ypos, radius, r, c);
			drawHex(device, pos, radius, color);
		}
	}
}




bool SimpleUtil::getHexRowAndCols
(
	f32 w,
	f32 h,
	f32 r,
	u32& rows,
	u32& cols
)
{
	//TODO:
	if ((w <= 0.0f) || (h <= 0.0f) || (r <= 0.0f))
		return false;
	
	f32 x = 0.0f;
	cols = 0;
		
	while (x < w)
	{
		if (cols % 2 == 0) // even no.
			x += r;
		else // odd no.
			x += 2.0 * r;
		
		++cols;
	}
	
	
	f32 dh = fabs(2.0f * r * sin(60.0f * core::DEGTORAD));
	rows = u32(floorf(h / dh));
	
	//printf("rows=%i, cols=%i, h=%f, dh=%f\n", rows, cols, h, dh);
	return true;
}




core::vector3df SimpleUtil::getHexPos
(
	f32 x0,
	f32 z0,
	f32 ypos,
	f32 r,
	u32 row,
	u32 col
)
{
	f32 x = x0 + f32(col) * 1.5f * r + r;
	f32 z;
	if ((col%2) == 0)
		z = z0 - f32(2*row-1) * r * sin(60.0f * core::DEGTORAD);
	else
		z = z0 - f32(2*row) * r * sin(60.0f * core::DEGTORAD);
	return core::vector3df(x, ypos, z);
}




void SimpleUtil::drawHex
(
	IrrlichtDevice* device,
	const core::vector3df& pos,
	f32 radius,
	video::SColor color
)
{
	std::vector<core::vector3df> pts = getPointsOnHex(pos, radius);
	if (int(pts.size()) <= 1)
		return;
	
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 2;
	lineMaterial.BackfaceCulling = false;
	//lineMaterial.MaterialType = video::EMT_SOLID;
	lineMaterial.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	
	video::IVideoDriver* driver = device->getVideoDriver();
	driver->setMaterial(lineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
		
	int n1, n2;
	for (int n = 0; n < int(pts.size()); n++)
	{
		n1 = n;
		n2 = n + 1;
		if (n2 >= int(pts.size()))
			n2 = 0;
		driver->draw3DLine(pts[n1], pts[n2], color);
	}

}


std::vector<core::vector3df> SimpleUtil::getPointsOnHex
(
	const core::vector3df& pos,
	f32 radius
)
{
	std::vector<core::vector3df> points;

	core::vector3df point;
	core::vector3df heading(1.0f, 0.0f, 0.0f);
	core::vector3df vector;
	
	f32 deltaDegs = 360.0f / 6.0f;
	for (u32 n = 0; n < 6; ++n)
	{
		vector = heading;
		vector.rotateXZBy(n * deltaDegs);
		
		point = vector * radius + pos;
		points.push_back(point);
	}
	
	return points;

}

void SimpleUtil::drawBBoxTop
(
	IrrlichtDevice* device,
	scene::ISceneNode* scene,
	video::SColor color
)
{
	scene->updateAbsolutePosition();
	//core::aabbox3d<f32> box = scene->getBoundingBox();
	core::aabbox3d<f32> box = scene->getTransformedBoundingBox();
	core::vector3df extent = box.getExtent();
	core::vector3df center = box.getCenter();
	
	f32 dx = extent.X * 0.5f;
	f32 dy = extent.Y * 0.5f;
	f32 dz = extent.Z * 0.5f;
	
	f32 cx = center.X;
	f32 cy = center.Y;
	f32 cz = center.Z;

	// draw:
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 2;
	lineMaterial.BackfaceCulling = false;
	//lineMaterial.MaterialType = video::EMT_SOLID;
	lineMaterial.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	
	video::IVideoDriver* driver = device->getVideoDriver();
	driver->setMaterial(lineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	
	core::vector3df p0(cx-dx,cy+dy,cz+dz);
	core::vector3df p1(cx+dx,cy+dy,cz+dz);
	core::vector3df p2(cx+dx,cy+dy,cz-dz);
	core::vector3df p3(cx-dx,cy+dy,cz-dz);
	
	driver->draw3DLine(p0, p1, color);
	driver->draw3DLine(p1, p2, color);
	driver->draw3DLine(p2, p3, color);
	driver->draw3DLine(p3, p0, color);
}

f32 SimpleUtil::getRand()
{
	return
	2.0f * rand() / RAND_MAX - 1.0f;
}

void SimpleUtil::drawTrajectory(IrrlichtDevice* device, const std::deque<core::vector3df>& points, video::SColor color)
{
	if (points.size() <= 1)
		return;
	
	//video::SColor blue(128, 0, 0, 255);
	
	// set mat:
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 3.0f;
	//lineMaterial.FrontfaceCulling = false;
	lineMaterial.BackfaceCulling = false;
	//lineMaterial.MaterialType = video::EMT_SOLID;
	lineMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	
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

void SimpleUtil::changeCameraFPSMoveSpeed
(
	scene::ICameraSceneNode* camera,
	f32 factor,
	f32 MIN_CAM_MOVE_SPEED,
	f32 MAX_CAM_MOVE_SPEED
)
{
	const core::list<scene::ISceneNodeAnimator*> anims = camera->getAnimators();
	//bool found = false;
	scene::ISceneNodeAnimatorCameraFPS* anim = 0;
	core::list<scene::ISceneNodeAnimator*>::ConstIterator iter;
	for (iter=anims.begin(); iter!=anims.end(); ++iter)
	{
		if ((*iter)->getType() == scene::ESNAT_CAMERA_FPS)
		{
			//found = true;
			anim = static_cast<scene::ISceneNodeAnimatorCameraFPS*>(*iter);
			break;
		}
	}
	
	if (!anim)
		return;
	
	f32 speed = anim->getMoveSpeed();
	f32 newSpeed = speed*factor;
	
	if (newSpeed > MAX_CAM_MOVE_SPEED)
		newSpeed = MAX_CAM_MOVE_SPEED;
	
	if (newSpeed < MIN_CAM_MOVE_SPEED)
		newSpeed = MIN_CAM_MOVE_SPEED;
	
	printf("move speed of camera=%f\n", newSpeed);
	anim->setMoveSpeed(newSpeed);
}


void SimpleUtil::makeScreenshot
(
	IrrlichtDevice* device,
	const core::stringw& basename,
	u32 num
)
{
	core::stringw filename = basename;
	filename += num;
	filename += L".png";
	
	makeScreenshot(device, filename);
}


void SimpleUtil::makeScreenshot
(
	IrrlichtDevice* device,
	const core::stringw& filename
)
{
	video::IImage* image = device->getVideoDriver()->createScreenShot(video::ECF_A8R8G8B8);
	if (!image)
		return;
	
	video::ECOLOR_FORMAT cf = image->getColorFormat();
	//printf("color format of image=%i\n", cf);
	
	device->getVideoDriver()->writeImageToFile(image, filename);
	image->drop();
}


void SimpleUtil::rotateCameraTowardsNodeAroundYAxis
(
	scene::ICameraSceneNode* camera,
	scene::ISceneNode* node,
	f32 minDist,
	bool rotateCameraBy90Deg
)
{
	node->updateAbsolutePosition();
	core::vector3df pos = node->getAbsolutePosition();
	
	rotateCameraTowardsPointAroundYAxis(camera, pos, minDist, rotateCameraBy90Deg);
}

void SimpleUtil::rotateCameraTowardsPointAroundYAxis
(
	scene::ICameraSceneNode* camera,
	const core::vector3df& pos,
	f32 minDist,
	bool rotateCameraBy90Deg
)
{
	camera->updateAbsolutePosition();
	
	// rotate to face target Node
	core::vector3df cameraPos = camera->getAbsolutePosition();
	core::vector3df delta = pos - cameraPos;
	
	
	core::vector3df rot = delta.getHorizontalAngle();
	//rot.X = 0.0f;
	//rot.Z = 0.0f;
	if (rotateCameraBy90Deg)
		rot.Y -= 90.0f;
	
	camera->setRotation(rot);

	if (minDist <= 0.0f)
		return;
	
	if (delta.getLengthSQ() < (minDist * minDist))
		return;
	
	delta *= 0.5f;
	
	camera->setPosition(core::vector3df
	(
		cameraPos.X + delta.X,
		cameraPos.Y + delta.Y,
		cameraPos.Z + delta.Z
	));
	
}

std::vector<core::vector3df> SimpleUtil::getPointsOnHorizontalCircle
(
	const core::vector3df& center,
	f32 radius,
	f32 offsetY,
	u32 count
)
{
	std::vector<core::vector3df> points;
	if (count <= 1)
		return points;
	
	core::vector3df point;
	core::vector3df heading(1.0f, 0.0f, 0.0f);
	core::vector3df vector;
	
	f32 deltaDegs = 360.0 / count;
	for (u32 n = 0; n < count; ++n)
	{
		vector = heading;
		vector.rotateXZBy(n * deltaDegs);
		
		point = vector * radius + center;
		point.Y += offsetY;
		
		points.push_back(point);
	}
	
	return points;
}


void SimpleUtil::print(const core::vector3df& vec)
{
	printf("(%.2f,%.2f,%.2f)\n", vec.X, vec.Y, vec.Z);
}

//void SimpleUtil::
//IrrlichtDevice* device

