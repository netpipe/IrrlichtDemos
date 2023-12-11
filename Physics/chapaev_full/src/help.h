// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#ifndef OSRPG_HELP_H
#define OSRPG_HELP_H

/// \ingroup CoreGame
///@{
/// \file help.h
/// \brief Some helpful functions

#include <ode/ode.h>
#include <irrlicht.h>

/// Check if body can be disabled

/// \param b Body
/// \param lin_vel Linear velocity threshold
/// \param ang_vel Angular velocity threshold
bool CheckForDisabling (const dBodyID b, const float lin_vel,
		const float ang_vel);
/// Conver Euler angle to quaternion

/// \param euler Euler angle
/// \param quaternion Quaternion
void EulerToQuaternion (const irr::core::vector3df euler,
		dQuaternion quaternion);
/// Get geom from mesh

/// \param mesh IMesh
/// \return dTriMeshDataID
dTriMeshDataID GetGeomFromMesh (irr::scene::IMesh *mesh);
/// Print vector to console

/// \param vec Vector
///
void PrintVector (const dReal *vec);
/// Print vector to console

/// \param vec Vector
///
void PrintVector (const irr::core::vector3df vec);
/// Report message to console

/// \param msg Message
/// \param sender Sender
void Report (const char *msg, const char *sender);
/// Convert quaternion to Euler angle

/// \param q Quaternion
/// \param euler Euler angle
void QuaternionToEuler (const dQuaternion q, irr::core::vector3df &euler);
/// Update object's graphics representation according to the physics one

/// \param g Geom
/// \param n Scene node
void UpdateObject (const dGeomID g, irr::scene::ISceneNode *n);

///@}

#endif // OSRPG_HELP_H

