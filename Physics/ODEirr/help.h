// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

/// \file help.h
/// \brief Some helpful functions
#ifndef HELP_H
#define HELP_H

#include "settings.h"
#include <cstdlib>

/// Print vector
void PrintVector (const dReal *vec);
/// Print vector
void PrintVector (const core::vector3df vec);
/// Convert quaternion to Euler
void QuaternionToEuler (const dQuaternion q, core::vector3df &euler);
/// Convert Euler to quaternion
void EulerToQuaternion (core::vector3df euler, dQuaternion quaternion);
/// Convert radian to degree
float RadianToDegree (float radian);
/// Convert degree to radian
float DegreeToRadian (float degree);
/// Helps in creating geom from a mesh
dTriMeshDataID GetGeomFromMesh (IMesh *mesh);
/// Check if body is ready to be disabled

/// Check if body satisfies angular and linear velocities' thresholds and
/// can be disabled
bool CheckForDisabling (dBodyID b, float lin_vel, float ang_vel);
//float FRandom ();
/// Update node position and rotation based on geom position and rotation
void UpdateObject (dGeomID g, ISceneNode *n);

#endif // HELP_H

