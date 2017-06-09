#include <irrlicht.h>
using namespace irr;

#include "CGameCamera.h"

#include "FunCollision.h"
using namespace COLL;


//! constructor
CGameCamera::CGameCamera(IrrlichtDevice* Device, ICollManager* Cmgr, CGameInput *Input,
                         const core::vector3df& Pos, const core::vector3df& Scale, const core::vector3df& Origin)
{
    // instances
    device = Device;
    smgr = device->getSceneManager();
    driver = device->getVideoDriver();
	cmgr = Cmgr;
    input = Input;

    // create camera scene node:
    cam = smgr->addCameraSceneNode();
	smgr->setActiveCamera(cam);
	cam->bindTargetAndRotation(true);
	camPos = cam->getAbsolutePosition();
	camRot = cam->getRotation();

	/* We create the camera entity now: First we need a sampler which is pracically for
	   defineing our character's shape. It's going to be an ellipsoid.

	   Then we'll add the entity itself, and bind the sampler and the camera to it. */
	ICollEllipsoidSampler* sampler = cmgr->addEllipsoidSampler(Scale, Origin);
	camEntity = cmgr->addEntity(Pos, sampler, cam);
}

//! update
void CGameCamera::update()
{
    // ----- Camera variables:
    float runSpeed = 0.3f;
    float jumpStrength = 10.0f;
    float mouseSpeed = 0.1f;
    camPos = cam->getAbsolutePosition();

    // ----- Camera rotation:
    s32 rmx = 0, rmy = 0;
    input->getRelMousePos(rmx, rmy);
    camRot.Y -= rmx * mouseSpeed;
    camRot.X += rmy * mouseSpeed;
    // Trim camera rotation values, not to flip over
    camRot.Y = (camRot.Y < 0) ? camRot.Y + 360 : camRot.Y;
    camRot.Y = (camRot.Y >= 360) ? camRot.Y - 360 : camRot.Y;
    camRot.X = (camRot.X < -89.9f) ? -89.9f : camRot.X;
    camRot.X = (camRot.X > 89.9f) ? 89.9f : camRot.X;
    // Set camera target
    core::vector3df camVect = core::vector3df(0, 0, 1);
    camVect.rotateYZBy(camRot.X, core::vector3df(0, 0, 0));
    camVect.rotateXZBy(camRot.Y, core::vector3df(0, 0, 0));
    cam->setTarget(camPos + camVect);
    // Update camera scene node
    cam->updateAbsolutePosition();

    // Define camera up, left and forward directions
    core::vector3df camUp(0, 1, 0);
    core::vector3df camLeft = core::vector3df(camVect.X, 0, camVect.Z).normalize();
    camLeft.rotateXZBy(90, core::vector3df(0, 0, 0));
    core::vector3df camForward = core::vector3df(camVect.X, 0, camVect.Z).normalize();

    // ----- Camera movement:
    /* We're going to use forces and impulses to move our character in the world. Why? To move them
       framerate independently, and smoothly.

       There's a difference between force and impulse. Forces are continuous, while impulses happen
       in very brief moment.

       We're gonne use forces for running around, because if the game stops for a second, and the player
       were holding UP key (running forward), we suppose that the character should continue running forward. */
    if (input->getKeyDown(irr::KEY_KEY_W) || input->getKeyDown(irr::KEY_UP))      camEntity->addForce(camForward * runSpeed);
    if (input->getKeyDown(irr::KEY_KEY_S) || input->getKeyDown(irr::KEY_DOWN))    camEntity->addForce(-camForward * runSpeed);
    if (input->getKeyDown(irr::KEY_KEY_D) || input->getKeyDown(irr::KEY_RIGHT))   camEntity->addForce(-camLeft * runSpeed);
    if (input->getKeyDown(irr::KEY_KEY_A) || input->getKeyDown(irr::KEY_LEFT))    camEntity->addForce(camLeft * runSpeed);

    // For jumping, we need to check if the entity is not falling
    if (input->getKeyDown(irr::KEY_SPACE) && camEntity->getState() != ECES_FALLING)
    {
        // The power of a jump is a sudden impulse: once we've jumped up, we cannot push ourselves upper and upper.
        camEntity->addImpulse(camUp * jumpStrength);
        // Jumping is against the gravity, so we should ignore gravity here.
        camEntity->resetGravity();
    }
}
