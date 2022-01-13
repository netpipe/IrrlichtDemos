#ifndef CIrrBP_FPSCAM_H_
#define CIrrBP_FPSCAM_H_
#include <irrlicht.h>
#include "body/CIrrBPRigidBody.h"
#include "animator/CIrrBPFollowAnimator.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "convert.h"

class CSceneNodeAnimatorCameraFPS;

class CIrrBPCamera : public CIrrBPRigidBody
{
public:

	/*!
	  Main constructor for a Physics Camera.
	  The body is approximated to a shpere.
	  @param cam Camera scene node to attach to.
	  @param dev Irrlicht Device
	  @param mass Body's Mass
	  @param size "Block" size of the camera. The camera "body" is approximate to a sphere with this size
	  @param bodyId Body's id
	*/
	CIrrBPCamera(irr::scene::ICameraSceneNode * cam,irr::IrrlichtDevice *dev,irr::f32 mass,int size = 15, irr::s32 bodyId=-1);

	void drop() ;
private:
	irr::scene::ICameraSceneNode * camera;
	CSceneNodeAnimatorCameraFPS * anim;
};
#endif