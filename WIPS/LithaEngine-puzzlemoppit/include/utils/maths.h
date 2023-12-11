
#ifndef UTILS_MATHS_H
#define UTILS_MATHS_H

#include "litha_internal.h"

namespace utils
{
namespace maths
{

// Copies the rotation from one matrix to another, using direct access of elements.
// (rather than using Irrlicht get/setRotationDegrees methods which may introduce rounding errors)
void copy_rotation(const core::matrix4 &source, core::matrix4 &dest);

// convert quaternion to euler in degrees
core::vector3df quaternion_to_euler(const core::quaternion &quat);

// convert euler rotation in degrees to a quaternion
core::quaternion euler_to_quaternion(const core::vector3df &rot);

// Only position and rotation are interpolated. Rotation uses a quaternion and slerp.
// Scale is ignored.
void interpolate_matrix(const core::matrix4 &a, const core::matrix4 &b, f32 alpha, core::matrix4 &dest);

// Combine two euler rotations (assumed in degrees).
core::vector3df combine_rotations(const core::vector3df &a, const core::vector3df &b);

// Interpolate a position...
core::vector3df interpolate_position(const core::vector3df &a, const core::vector3df &b, f32 alpha);

// Interpolate an euler rotation (in degrees, from a to b).
core::vector3df interpolate_rotation(const core::vector3df &a, const core::vector3df &b, f32 alpha);

// Returns a single f32 value representing in degrees the difference between two euler rotations (also in degrees).
// So if they completely match you would get 0.0, if they were polar opposite you would get 180.0
// and at right angles you would get 90.0
f32 scalar_rotation_difference(const core::vector3df &a, const core::vector3df &b);

// Difference between two rotations as a rotation. (euler, degrees)
core::vector3df rotation_difference(const core::vector3df &a, const core::vector3df &b);

// Returns the rotation required (euler, degrees) to get from {0,0,1} to the specified vector.
// "opposite" specifies the rotation to be returned when the specified vector is exactly opposite,
// {0,0,-1} and there are many possible solutions.
// (e.g. you might put vector3df(0,180,0) here if you want your rotation to occur about the Y axis)
core::vector3df rotation_required(const core::vector3df &vec, const core::vector3df &opposite);

// Returns the single angle rotation required (euler, degrees) to get from {0,1} to the specified vector.
// vec should be normalised.
f32 rotation_required_2d(const core::vector2df &vec);

// Returns the rotation (euler, degrees) as a vector.
// (rotation applied to unit vector {0,0,1})
core::vector3df rotation_to_direction(const core::vector3df &rot);

// interpolate a single angle in degrees
// handles the wrap around.
f32 interpolate_angle(f32 a, f32 b, f32 alpha);

// Extract the Y component from a rotation. (euler,degrees)
// Rotation starting at {0,0,1}
f32 extract_y_rotation(const core::vector3df &rot);

// Get the vector of motion this rotation gives in the X,Z plane.
// Rotation starting at {0,0,1}
core::vector2df get_horizontal_vec(const core::vector3df &rot);

// Returns a vector pointing in the direction of the 2D angle.
// Assumes 0 degrees is {0,1} (up).
core::vector2df vec_from_angle(f32 angle);

}
}

#endif

