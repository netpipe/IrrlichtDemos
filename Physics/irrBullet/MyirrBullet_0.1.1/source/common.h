#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

#define BIT(x) (1<<(x)) // For collision masking

#include <irrlicht.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>


void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler);

enum ECollisionFlag
{
    ECF_STATIC_OBJECT = 1,
    ECF_KINEMATIC_OBJECT = 2,
    ECF_NO_CONTACT_RESPONSE = 4,
    ECF_CUSTOM_MATERIAL_CALLBACK = 8,
    ECF_CHARACTER_OBJECT = 16
};

enum ECollisionObjectType
{
  ECOT_COLLISION_OBJECT = 1,
  ECOT_RIGID_BODY,
  ECOT_GHOST_OBJECT,
  ECOT_SOFT_BODY,
  ECOT_HF_FLUID
};




#endif // __COMMON_H_INCLUDED__
