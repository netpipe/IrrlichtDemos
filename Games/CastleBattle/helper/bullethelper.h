/** Tiny helper functions. Also to create a tiny wrapper between Bullet and Irrlicht */
#ifndef BULLET_HELPER_H
#define BULLET_HELPER_H
#include <irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
using namespace irr;
    /** Convert given Irrlicht vector3d vector to Bullect btVector3.
    * @param vec vector3df Irrlicht vector
    * @return bullet btVector3
    */
    btVector3 toBulletVector(core::vector3df vec);

    /** Convert given Irrlicht euler angle to Bullet quaternion angle.
    * @param euler vector3df irrlicht euler angle(x,y,z)(pitch, yaw. roll)
    * @return Quaternion bullet angle
    */
    btQuaternion EulerToQuaternion(const core::vector3df &euler);

    /** Inverse of ::EulerToQuaternion() */
    core::vector3df QuaternionToEuler(const btQuaternion &TQuat);

    /** Inverse of ::toBulletVector() */
    core::vector3df toIrrlichtVector(btVector3 vec);

    /** Get a random value.
    * @param: maxValue f32 maximum value acceptable
    * @param offset f32 offset to add to a generated value. Optional.
    * @return f32 generated number
    */
    f32 getRand(const f32 maxValue, const f32 offset = 0.f);
    /** Get a size of a 3D Irrlicth mesh.
    * @param mesh ISceneNode node to get the size
    * @return array of 3 float (X, Y, Z)
    */
    f32* getMeshSize(scene::ISceneNode* mesh);
    /** Get random color.
    * There is predefined color palette. This because get rgba with some randomness is not a good idea because
    * often you will get weird colors that will not suit in the graphic scene.
    * @return SColor in Irrlicht object
    */
    video::SColor randomColor();
    /** Get sign of a integer.
    * @return -1 or 1
    */
    int sign(int x);
    /** Used for logger. Convert a vector to a string
    * @param vector vector3df vector to Convert
    * @return stringc string 
    */
    core::stringc vectorToString(core::vector3df vector);

#endif
