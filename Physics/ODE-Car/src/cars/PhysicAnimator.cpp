#include "PhysicAnimator.h"

#include <irrlicht.h>

using namespace irr;

void CPhysicAnimator::QuaternionToEuler(const dQuaternion quat, core::vector3df &euler) {
	dReal w,x,y,z;
	w = quat[0];
	x = quat[1];
	y = quat[2];
	z = quat[3];
	double sqw = w*w;
	double sqx = x*x;
	double sqy = y*y;
	double sqz = z*z;

	euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*irr::core::RADTODEG);
	euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*irr::core::RADTODEG);  
	euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*irr::core::RADTODEG);
}

scene::ISceneNodeAnimator* CPhysicAnimator::createClone(irr::scene::ISceneNode* node,	
														irr::scene::ISceneManager* newManager)
{
	scene::ISceneNodeAnimator* new_animator = new CPhysicAnimator(m_geom);
	return new_animator;
}

void CPhysicAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
{
	core::vector3df pos;
	core::vector3df rot;

	dReal * ode_pos = (dReal*)dGeomGetPosition(m_geom);
	pos.set((f32)ode_pos[0], (f32)ode_pos[1], (f32)ode_pos[2]);
	node->setPosition(pos);

	dQuaternion result;
	dGeomGetQuaternion(m_geom, result);
	QuaternionToEuler(result, rot);
	node->setRotation(rot);
}

