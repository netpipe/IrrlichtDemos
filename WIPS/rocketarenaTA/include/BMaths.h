/*

BMaths.h

*/

#include <irrlicht.h>

using namespace irr;
using namespace core;

// rotates a bone
core::matrix4 rotateBone(core::matrix4 m, core::vector3df rot);

// set the rotation between to euler angel vectors
core::vector3df matrixRotation(core::vector3df rotation, core::vector3df vec);
