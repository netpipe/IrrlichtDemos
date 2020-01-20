#include <irrlicht.h>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <math.h>
#include "bullethelper.h"
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace irr;

btVector3 toBulletVector(core::vector3df vec) {
    btVector3 vecBullet = btVector3((btScalar) vec.X,(btScalar) vec.Y,(btScalar) vec.Z);
    return vecBullet;
}
core::vector3df toIrrlichtVector(btVector3 vec){
    return core::vector3df(vec.getX(), vec.getY(), vec.getZ());
}
btQuaternion EulerToQuaternion(const core::vector3df &euler){
    btMatrix3x3 mat;
    mat.setIdentity();
    mat.setEulerYPR(euler.X*core::DEGTORAD64, euler.Y*core::DEGTORAD64, euler.Z*core::DEGTORAD64);
    btQuaternion quat;
    mat.getRotation(quat);
   return quat;
}
core::vector3df QuaternionToEuler(const btQuaternion &TQuat) {
    btVector3 TEuler;
    btScalar W = TQuat.getW();
    btScalar X = TQuat.getX();
    btScalar Y = TQuat.getY();
    btScalar Z = TQuat.getZ();
    float WSquared = W * W;
    float XSquared = X * X;
    float YSquared = Y * Y;
    float ZSquared = Z * Z;

    TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
    TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
    TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
    TEuler *= core::RADTODEG;
    return toIrrlichtVector(TEuler);
}

f32 getRand(f32 maxValue, f32 offset){
 return (f32)(rand() % (int)maxValue + offset);
}
f32* getMeshSize(scene::ISceneNode* mesh){
   core::vector3df * edges = new core::vector3df[8];
   core::aabbox3df boundingbox = mesh->getTransformedBoundingBox();
   boundingbox.getEdges(edges);

   f32 height = edges[1].Y - edges[0].Y;

   f32 width = edges[5].X - edges[1].X;

   f32 depth = edges[2].Z - edges[0].Z;
   f32* arr = new f32[3];
   arr[0] = width;
   arr[1] = height;
   arr[2] = depth;
   return arr;
}
video::SColor randomColor(){
    video::SColor colors[10] = {
        video::SColor(255,0,0,255),
        video::SColor(255,0,255,0),
        video::SColor(255,255,0,0),
        video::SColor(255,247,0,115),
        video::SColor(255,29,171,226),
        video::SColor(255,255,255,0),
        video::SColor(255,0,255,255),
        video::SColor(255,255,255,255),
        video::SColor(255,255,123,0),
        video::SColor(255,171,111,232)
    };
        return colors[(int)getRand(10)];

}

 int sign(int x){
    return (int(0) < x) - (x < int(0));
}
core::stringc vectorToString(core::vector3df vector){
  core::stringc string = core::stringc(" ");
  string+=vector.X;
  string+=" ";
  string+=vector.Y;
  string+=" ";
  string+=vector.Z;
  string+=" ";
  return string.c_str();
}
