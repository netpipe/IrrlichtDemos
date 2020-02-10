
#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(WorldBase *world)
    : WorldBase(world)
{
    playerNode = NULL;
}

void ThirdPersonCamera::setup(scene::ISceneNode *node)
{
    this->playerNode = node;
    
    if (node)
    {
        positionOffset.Y = node->getBoundingBox().getExtent().Y/2.0;
    }
}

void ThirdPersonCamera::update(f32 dt)
{
    if (!playerNode) return;
    
    // Most code here from Irrlicht forums, http://irrlicht.sourceforge.net
    // Why write code when you can copy and paste!?
    
    static float direction = 0.0f;
    static float zdirection = 0.0f;
    
    core::position2d<f32> cursorPos = device->getCursorControl()->getRelativePosition();
    scene::ICameraSceneNode *camera = device->getSceneManager()->getActiveCamera();

    float change_x = ( cursorPos.X - 0.5 ) * 256.0f;
    float change_y = ( cursorPos.Y - 0.5 ) * 256.0f;
    direction += change_x;
    zdirection -= change_y;
    if( zdirection <- 90 )
        zdirection = -90;
    else
    if( zdirection > 90 )
        zdirection = 90;
    device->getCursorControl()->setPosition( 0.5f, 0.5f );
    core::vector3df playerPos = playerNode->getPosition();
    f32 thirdPersonRadius = 5.0;
    core::vector3df nextCamPos;
    nextCamPos.X = playerPos.X - cos( direction * M_PI / 180.0f ) * thirdPersonRadius;
    nextCamPos.Y = playerPos.Y - sin( zdirection * M_PI / 180.0f ) * thirdPersonRadius;
    nextCamPos.Z = playerPos.Z + sin( direction * M_PI / 180.0f ) * thirdPersonRadius;
    
    //core::vector3df camPos = camera->getAbsolutePosition();
    //nextCamPos = camPos + (nextCamPos-camPos).normalize()*10.0*dt;
    
    // line from player to next pos
    // NEED TO UPDATE TO USE ODE
    /*
    if (triangleSelector)
    {
    core::line3df camRay = core::line3df(playerPos, nextCamPos);
    
    core::line3df segmentRay = camRay;
    core::vector3df segmentVec = segmentRay.getVector().normalize();
    segmentRay.start = segmentRay.end;
    segmentRay.start -= segmentVec;
    segmentRay.end += segmentVec;
    
    camRay.end += camRay.getVector().normalize()*1.0;
    core::triangle3df triangle;
    
    core::vector3df tempVec;
    if (smgr->getSceneCollisionManager()->getCollisionPoint(segmentRay, triangleSelector, tempVec, triangle))
    {
        smgr->getSceneCollisionManager()->getCollisionPoint(camRay, triangleSelector, nextCamPos, triangle);
    }
    }
    */
    nextCamPos = camera->getPosition().getInterpolated(nextCamPos, 1.0 - (10.0*dt));
    
    camera->setPosition(nextCamPos);
    camera->setTarget(playerPos + positionOffset);
}


