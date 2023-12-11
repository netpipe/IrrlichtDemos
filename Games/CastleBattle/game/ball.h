#ifndef BALL_H
#define BALL_H
#include <irrlicht.h>
#include <vector>
#include "physics.h"
#include "bullethelper.h"
#include "camera.h"
using namespace irr;

const f32 CAMERA_OFFSET_BALL = 2.5f;
/** Ball represent the projectile of the cannon */
class Ball{

    public:
        scene::ISceneManager* smgr;
        /** Bullet btRigitBody instance of the projectile */
        btRigidBody* btBall;
        /** Irrlicht instance of the projectile */
        scene::ISceneNode* irrBall;
        /** Radius of the projectile */
        f32 radius = 0.3f;
        /** Start position of the camera */
        core::vector3df cameraStartPosition;
        /** Camera of related to the ball @see ::Camera */
        Camera* camera;
        /** Irrlicht device */
        IrrlichtDevice* device;
        /** Parent camera of the ball, in particular is the cannon camera
        @see ::Cannon
        @see ::Cannon::moveCamera()
         */
        scene::ICameraSceneNode* parentCamera;
        /** Default constructor will instance both irrlicht and bullet objects at specified position. */
        Ball(IrrlichtDevice* device, scene::ISceneManager* smgr, video::IVideoDriver* driver, Physics* physics, core::vector3df position);
        ~Ball();
        /** Points of the ball while moving in air. It is used for a special effect. For now it is not used */
        core::array<core::vector3df> points;
        /** Set camera to the ball, at specified rotation */
        void setCamera(scene::ICameraSceneNode* camera, core::vector3df rotation = core::vector3df(0,0,0));
        /** Move camera, @see ::Cannon::moveCamera() */
        bool moveCamera();
        /** Remove active camera and substitue with a parent camera */
        void deleteCamera();


};
#endif
