/*-----------------------------------------------------------------------------*
| headerfile MCamera.h                                                         |
|                                                                              |
| version 1.20                                                                 |
| date: (29.04.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| for Irrlicht engine                                                          |
| firsth person and orbiting cameras                                           |
*-----------------------------------------------------------------------------*/

#ifndef MCAMERA_H
#define MCAMERA_H

#include <irrlicht.h>
using namespace irr;

class MCameraFPS
{
    scene::ICameraSceneNode* camera;
    f32 rotationX;
    f32 rotationY;
    core::vector3df direction;
    
public:
    MCameraFPS(scene::ISceneManager* smgr)
    {
        camera = smgr->addCameraSceneNode();
        rotationX = 0.0f;
        rotationY = 0.0f;
        direction = core::vector3df(0,0,1);
    }
    
    ~MCameraFPS(){}
    
    void turnRight(f32 i)
    {
        rotationY += i;
        if(rotationY>=360)rotationY-=360;
        if(rotationY<0)rotationY+=360;
        
        direction = core::vector3df(0,0,1);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (rotationX,rotationY,0));
        matrix.rotateVect(direction);
        
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void turnLeft(f32 i)
    {
        rotationY -= i;
        if(rotationY>=360)rotationY-=360;
        if(rotationY<0)rotationY+=360;
        
        direction = core::vector3df(0,0,1);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (rotationX,rotationY,0));
        matrix.rotateVect(direction);
        
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void turnUp(f32 i)
    {
        rotationX += i;
        if(rotationX>=360)rotationX-=360;
        if(rotationX<0)rotationX+=360;
        
        direction = core::vector3df(0,0,1);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (rotationX,rotationY,0));
        matrix.rotateVect(direction);
        
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void turnDown(f32 i)
    {
        rotationX -= i;
        if(rotationX>=360)rotationX-=360;
        if(rotationX<0)rotationX+=360;
        
        direction = core::vector3df(0,0,1);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (rotationX,rotationY,0));
        matrix.rotateVect(direction);
        
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void moveForward(f32 i)
    {
        core::vector3df step = core::vector3df(0,0,i);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (0,rotationY,0));
        matrix.rotateVect(step);
        
        camera->setPosition(camera->getPosition() + step);
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void moveBack(f32 i)
    {
        core::vector3df step = core::vector3df(0,0,-i);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (0,rotationY,0));
        matrix.rotateVect(step);
        
        camera->setPosition(camera->getPosition() + step);
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void moveRight(f32 i)
    {
        core::vector3df step = core::vector3df(i,0,0);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (0,rotationY,0));
        matrix.rotateVect(step);
        
        camera->setPosition(camera->getPosition() + step);
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void moveLeft(f32 i)
    {
        core::vector3df step = core::vector3df(-i,0,0);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (0,rotationY,0));
        matrix.rotateVect(step);
        
        camera->setPosition(camera->getPosition() + step);
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void setHeight(f32 i)
    {
        camera->setPosition(core::vector3df(camera->getPosition().X, i, camera->getPosition().Z));
        camera->setTarget(camera->getPosition() + direction);
        camera->updateAbsolutePosition();
    }
    
    void setPosition(core::vector3df pos)
    {
        camera->setPosition(pos);
        camera->updateAbsolutePosition();
    }
    
    core::vector3df getPosition()
    {
        return camera->getPosition();
    }
    
    core::vector3df getDirection()
    {
        return direction;
    }
    
    core::vector3df getTarget()
    {
        return camera->getTarget();
    }
    
    f32 getHeading()
    {
        return rotationY;
    }
    
    f32 getPitch()
    {
        return rotationX;
    }
    
    f32 getFarValue()
    {
        return camera->getFarValue();
    }
    
    void setFarValue(f32 f)
    {
        camera->setFarValue(f);
    }
    
    f32 getNearValue()
    {
        return camera->getNearValue();
    }
    
    void setNearValue(f32 n)
    {
        camera->setNearValue(n);
    }
    
    f32 getFOV()
    {
        return camera->getFOV();
    }
    
    void setFOV(f32 v)
    {
        camera->setFOV(v);
    }
    
    f32 getAspectRatio()
    {
        return camera->getAspectRatio();
    }
    
    void setAspectRatio(f32 a)
    {
        camera->setAspectRatio(a);
    }
    
    scene::ISceneNode* getNode()
    {
        return camera;
    }
};




class MCameraOrbit
{
    scene::ICameraSceneNode* camera;
    f32 rotationX;
    f32 rotationY;
    f32 distance;
    
    void update()
    {
        core::vector3df direction = core::vector3df(0,0,distance);
        
        core::matrix4 matrix;
        matrix.setRotationDegrees(core::vector3df (rotationX,rotationY,0));
        matrix.rotateVect(direction);
        
        camera->setPosition(camera->getTarget() + direction);
        camera->updateAbsolutePosition();
    }
    
public:
    MCameraOrbit(scene::ISceneManager* smgr)
    {
        camera = smgr->addCameraSceneNode();
        rotationX = 0.0f;
        rotationY = 0.0f;
        distance = 1.0f;
        camera->setTarget(core::vector3df(0,0,0));
        update();
    }
    
    ~MCameraOrbit(){}
    
    void turnRight(f32 i)
    {
        rotationY += i;
        if(rotationY>=360)rotationY-=360;
        if(rotationY<0)rotationY+=360;
        update();
    }
    
    void turnLeft(f32 i)
    {
        rotationY -= i;
        if(rotationY>=360)rotationY-=360;
        if(rotationY<0)rotationY+=360;
        update();
    }
    
    void turnUp(f32 i)
    {
        rotationX += i;
        if(rotationX>=360)rotationX-=360;
        if(rotationX<0)rotationX+=360;
        update();
    }
    
    void turnDown(f32 i)
    {
        rotationX -= i;
        if(rotationX>=360)rotationX-=360;
        if(rotationX<0)rotationX+=360;
        update();
    }
    
    core::vector3df getTarget()
    {
        return camera->getTarget();
    }
    
    void setTarget(core::vector3df target)
    {
        camera->setTarget(target);
        update();
    }
    
    core::vector3df getPosition()
    {
        return camera->getPosition();
    }
    
    f32 getDistance()
    {
        return distance;
    }
    
    void setDistance(f32 i)
    {
        distance = i;
        update();
    }
    
    void adjustDistance(f32 i)
    {
        distance += i;
        update();
    }
    
    f32 getHeading()
    {
        return rotationY;
    }
    
    f32 getPitch()
    {
        return rotationX;
    }
    
    f32 getFarValue()
    {
        return camera->getFarValue();
    }
    
    void setFarValue(f32 f)
    {
        camera->setFarValue(f);
    }
    
    f32 getNearValue()
    {
        return camera->getNearValue();
    }
    
    void setNearValue(f32 n)
    {
        camera->setNearValue(n);
    }
    
    f32 getFOV()
    {
        return camera->getFOV();
    }
    
    void setFOV(f32 v)
    {
        camera->setFOV(v);
    }
    
    f32 getAspectRatio()
    {
        return camera->getAspectRatio();
    }
    
    void setAspectRatio(f32 a)
    {
        camera->setAspectRatio(a);
    }
    
    scene::ISceneNode* getNode()
    {
        return camera;
    }
};
#endif
