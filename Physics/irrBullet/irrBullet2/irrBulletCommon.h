// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#define BIT(x) (1<<(x)) // For collision masking

#include <matrix4.h>
#include <quaternion.h>
#include <ISceneNode.h>
#include "irrBulletCompileConfig.h"

struct SWheelInfo;
class btVector3;
class btQuaternion;
class btTransform;
struct btWheelInfo;

//void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler);
void btTransformToIrrlichtMatrix(const btTransform& worldTrans, irr::core::matrix4& matr);

void btTransformFromIrrlichtMatrix(const irr::core::matrix4& irrmat, btTransform& transform);

void btWheelInfoToSWheelInfo(const btWheelInfo& btInfo, SWheelInfo& info, const btTransform& transform);

void btWheelInfoFromSWheelInfo(const SWheelInfo& info, btWheelInfo& btInfo);

irr::core::vector3df compensateForNodeType(const irr::core::vector3df& scale, irr::scene::ESCENE_NODE_TYPE type);

btVector3 irrlichtToBulletVector(const irr::core::vector3df& vec);

btVector3 toBulletVector(float x, float y, float z);

irr::core::vector3df bulletToIrrlichtVector(const btVector3& vec);

irr::core::vector3df toIrrlichtVector(float x, float y, float z);

btQuaternion irrlichtToBulletQuaternion(const irr::core::quaternion& quat);

irr::core::quaternion bulletToIrrlichtQuaternion(const btQuaternion& quat);

enum class irrPhysicsDebugMode
{
  EPDM_NoDebug = 0,
  EPDM_DrawWireframe = 1,
  EPDM_DrawAabb = 2,
  EPDM_DrawFeaturesText = 4,
  EPDM_DrawContactPoints = 8,
  EPDM_NoDeactivation = 16,
  EPDM_NoHelpText = 32,
  EPDM_DrawText = 64,
  EPDM_ProfileTimings = 128,
  EPDM_EnableSatComparison = 256,
  EPDM_DisableBulletLCP = 512,
  EPDM_EnableCCD = 1024,
  EPDM_DrawConstraints = (1 << 11),
  EPDM_DrawConstraintLimits = (1 << 12),
  EPDM_FastWireframe = (1<<13),
  EPDM_MAX_DEBUG_DRAW_MODE
};

inline irrPhysicsDebugMode operator|(irrPhysicsDebugMode a, irrPhysicsDebugMode b)
{
	return static_cast<irrPhysicsDebugMode>(static_cast<int>(a) | static_cast<int>(b));
}
