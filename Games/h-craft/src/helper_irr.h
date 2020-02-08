// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HELPER_IRR_H
#define HELPER_IRR_H

#include <irrlicht.h>

// tool function which work with Irrlicht classes
namespace ExtIrr
{

// smooth alignment to vectors
extern void AlignToUpVector(irr::core::matrix4 &newMat_, const irr::core::matrix4 &oldMat_, const irr::core::vector3df &newUp_, float interpolate_=1.f);
extern void SmoothAlignToVector(irr::core::matrix4 &newMat_, const irr::core::matrix4 &oldMat_, const irr::core::vector3df &oldVec_, const irr::core::vector3df &newVec_, float interpolate_=1.f);

// returns the delta for the position between line_.start and line_.end where the point is (0 for Start, 1 for End or any value between)
// return in target_ the point on the line which is closest to point_
extern float GetClosestPointOnLineWithDelta(const irr::core::line3df &line_, const irr::core::vector3df &point_, irr::core::vector3df & target_);

// given a line and a point on this line with missing Y-coordinate, fill in the Y
extern void CalcYForPointOnLine(const irr::core::line3df &line_, irr::core::vector3df &point_);

// return length when Y would be 0
extern float GetLengthXZ(const irr::core::vector3df &vec_);

// copy the rotation part of a matrix (from source_ to target_)
extern void SetMatrixRotation(irr::core::matrix4 &target_, const irr::core::matrix4 &source_);

// get rotation degrees without crashing on floating point precision problems
// (note; not sure if that would still happen with modern Irrlicht, I remember there was some fix once about something causing similar troubles)
extern irr::core::vector3df GetRotationDegreesSave(const irr::core::matrix4 &mat_);

// return true if both vectors show in the same direction
extern bool IsSameDir(const irr::core::vector3df& v1, const irr::core::vector3df& v2);

// return true if both vectors show in the same direction
extern bool IsSameDir(const irr::core::vector2df& v1, const irr::core::vector2df& v2);

// Read a  file from the Irrlicht filesystem into the target buffer.
// \param nullTerminated When set to true the resulting buffer will be filesize+1 and null-terminated
extern bool ReadFile(irr::io::IFileSystem * fs, const irr::c8 * fileName, irr::core::array<irr::c8>& targetBuffer, bool nullTerminated=false);

// Faster mesh welding than then one in IMeshManipulator
extern irr::scene::IMesh* CreateMeshWelded(irr::scene::IMesh *mesh);

// Create a submesh from the given mesh which only contains triangles with normals which go in the same direction as targetNormal.
// For checking the dot-product is used - anything below minDot is removed.
// So minDot > 1 keeps all faces and < -1 removes all faces. 0 Would will keep faces with a normal up to a 90° angle compared to targetNormal.
extern irr::scene::IMesh* CreateSubMeshForNormal(irr::scene::IMesh *mesh, const irr::core::vector3df& targetNormal, float minDot=0.f);
};

#endif
