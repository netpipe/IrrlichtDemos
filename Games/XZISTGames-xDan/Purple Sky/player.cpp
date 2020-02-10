
#include "player.h"
#include "platforms.h"
#include "state.h"
#include <irrKlang.h>
#include "stringaling.h"

using namespace audio;

extern IrrlichtDevice *device;
extern ISoundEngine *sengine;
extern scene::ISceneManager *smgr;
extern s32 mouseDX, mouseDY;

scene::IAnimatedMeshSceneNode *node;
scene::IAnimatedMeshSceneNode *markerNode; // mouse pointer

const f32 minCamDist = 14.0;
const f32 maxCamDist = 20.0;
const f32 maxPlayerJump = 10.0;
core::vector2df jumpStart;
core::vector2df jumpTarget;
bool jumping;
double jumpdt;
double jumpTime;
const f32 jumpVel = 20.0;
f32 cameraTurnVel;
bool falling;
const f32 maxFallDist = -50.0; // after which Game Over is shown

//int cameraType = CAMERA_PIVOT;
int cameraType = CAMERA_FOLLOW;

core::vector3df toVector3d(core::vector2df pos)
{
    return core::vector3df(pos.X,0,pos.Y);
}

core::vector2df toVector2d(core::vector3df pos)
{
    return core::vector2df(pos.X,pos.Z);
}

void turnCamera(int dir)
{
    f32 d = dir;
    cameraTurnVel = dir*1.0;
}

core::vector3df getPlayerPos()
{
    return node->getAbsolutePosition();
}

void createPlayer()
{
    core::vector3df camPos;
    camPos.Z = -20.0;
    smgr->getActiveCamera()->setPosition(camPos);
    smgr->getActiveCamera()->updateAbsolutePosition();
    cameraTurnVel = 0;
    
    jumping = false;
    falling = false;
    
    node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/skwerlyskwerl.b3d"));
    node->updateAbsolutePosition();
    
    markerNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/marker.b3d"));
    markerNode->setRotation(core::vector3df(-90,0,0));
    markerNode->setPosition(core::vector3df(0,0,minPlatformSeparation));
    markerNode->updateAbsolutePosition();
}

void removePlayer()
{
    node->remove();
    markerNode->remove();
}    

bool playerOnPlatform()
{
    return !jumping && !falling;
}

void updatePlayer(double dt)
{
    f32 angle = (smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getPosition())
        .getHorizontalAngle().Y;
    f32 angleRadians = angle * M_PI/180.0;
    
    core::vector2df newMarkerPos = toVector2d(markerNode->getAbsolutePosition());
    
    if (playerOnPlatform()) // allow pointer to be moved
    {
        newMarkerPos += 
            core::vector2df(cos(angleRadians)*mouseDX/10.0,-sin(angleRadians)*mouseDX/10.0)
            + core::vector2df(-sin(angleRadians)*mouseDY/10.0,-cos(angleRadians)*mouseDY/10.0);
    }
    
    core::vector2df playerPos2d = toVector2d(node->getAbsolutePosition());
    if (newMarkerPos.getDistanceFrom(playerPos2d) > maxPlayerJump)
    {
        core::vector2df vec = newMarkerPos - playerPos2d;
        vec.normalize();
        vec *= maxPlayerJump;
        newMarkerPos = playerPos2d + vec;
    }
    if (newMarkerPos.getDistanceFrom(playerPos2d) < minPlatformSeparation
        && cameraType == CAMERA_PIVOT)
    {
        core::vector2df vec = newMarkerPos - playerPos2d;
        if (vec.getLength() > 1.0) vec.normalize();
        vec *= minPlatformSeparation;
        newMarkerPos = playerPos2d + vec;
    }
    if (playerOnPlatform())
    {
        markerNode->setPosition(toVector3d(newMarkerPos));
        getClosestPlatformExcluding(newMarkerPos, toVector2d(node->getAbsolutePosition()), jumpTarget);
        setSelected(jumpTarget);
    }
    
    markerNode->setVisible(playerOnPlatform());
    
    // camera positioning
    core::vector3df camPos;
    camPos.Z = -20.0;
    
    if (cameraType == CAMERA_PIVOT) // MODIFY TO USE A CAMERA DISTANCE VARIABLE!!??
    {
        camPos = node->getAbsolutePosition()-toVector3d(newMarkerPos);
        if (camPos.getLength() > 1.0)
        {
            camPos.normalize();
        }
        else
        {
            camPos.X = 0;
            camPos.Z = 1.0;
        }
        camPos *= 10.0;
        camPos.Y = 10.0;
        camPos += node->getAbsolutePosition();
    }
    else if (cameraType == CAMERA_FOLLOW)
    {
        core::vector2df camPos2d = toVector2d(smgr->getActiveCamera()->getAbsolutePosition());
        core::vector2df vec = toVector2d(node->getAbsolutePosition()) - camPos2d;
        f32 dist = vec.getLength();
        vec.normalize();
        
        if (dist > maxCamDist)
        {
            camPos2d += vec*(dist-maxCamDist);
        }
        
        if (dist < minCamDist)
        {
            camPos2d -= vec*(minCamDist-dist);
        }
        
        f32 angle = cameraTurnVel * M_PI/180.0 * 100.0;
        camPos2d.rotateBy(angle, toVector2d(node->getAbsolutePosition()));
        
        camPos = toVector3d(camPos2d);
    }
    
    camPos.Y = 10.0;
    smgr->getActiveCamera()->setPosition(camPos);
    smgr->getActiveCamera()->updateAbsolutePosition();
    smgr->getActiveCamera()->setTarget(node->getAbsolutePosition());
    
    device->getCursorControl()->setPosition(100,100);
    
    // character rotation
    
    f32 currentAngle = node->getRotation().Y;
    
    core::vector2df currentAim = core::vector2df(
        cos(currentAngle * M_PI/180.0),
        -sin(currentAngle * M_PI/180.0)
        );
    core::vector2df desiredAim = jumpTarget - toVector2d(node->getAbsolutePosition());
    
    currentAim.normalize();
    desiredAim.normalize();
    
    f32 aVel = 400.0;
    double aaa = acos(currentAim.dotProduct(desiredAim));
    
    if (fabs(aaa - 1.57) > 0.1)
    {
        if (aaa < 1.57)
        {
            currentAngle += aVel*dt;
        }
        else
        {
            currentAngle -= aVel*dt;
        }
        node->setRotation(core::vector3df(-90,currentAngle,0));
    }
    
    if (jumping)
    {
        jumpdt += dt;
        // v = dist / t
        core::vector3df pos = node->getAbsolutePosition();
        core::vector3df vec = toVector3d(jumpTarget-toVector2d(pos)).normalize();
        pos += vec*jumpVel*dt;
        
        // curved jump
        core::vector2df pos2d = toVector2d(pos);
        pos.Y = 2.0 * sin(pos2d.getDistanceFrom(jumpTarget)/jumpStart.getDistanceFrom(jumpTarget) * M_PI);
        
        if (jumpdt >= jumpTime)// || pos.getDistanceFrom(toVector3d(jumpTarget)) < 0.5)
        {
            pos.X = jumpTarget.X;
            pos.Y = 0;
            pos.Z = jumpTarget.Y;
            jumping = false;
            
            setTouchedPlatform(jumpTarget);
        }
        
        node->setPosition(pos);
    }
    else
    {
        if (!onPlatform(toVector2d(node->getAbsolutePosition())))
        {
            if (!falling) sengine->play2D("media/catscream.wav");
            falling = true;
        }
    }
    
    if (falling)
    {
        core::vector3df pos = node->getAbsolutePosition();
        pos.Y -= 10.0 * dt;
        node->setPosition(pos);
        
        if (pos.Y < maxFallDist)
        {
            gameOver();
        }
    }
}

void playerPlatformJump()
{
    if (jumping) return;
    
    jumpStart = toVector2d(node->getAbsolutePosition());
    jumping = true;
    jumpdt = 0;
    jumpTime = jumpStart.getDistanceFrom(jumpTarget) / jumpVel;
    
    markerNode->setPosition(markerNode->getAbsolutePosition() + toVector3d(jumpTarget-jumpStart));
    markerNode->updateAbsolutePosition();
    
    sengine->play2D(strparty("media/jump", rand()%1, ".wav").c_str());
}
