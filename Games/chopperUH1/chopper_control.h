    #ifndef _CHOPPER_CONTROL_H_
    #define _CHOPPER_CONTROL_H_
     
    #include <vector>
    #include <irrlicht.h>
     
     
    using namespace irr;
     
     
    #ifndef CHOPPER_CONTROL_PROPERTIES
    #define CHOPPER_CONTROL_PROPERTIES
    #define SIDEWAY_SPEED_CONST 25.0f
    #define ASCEND_SPEED_CONST 1.0f
    #define YAW_RATE 40.0f
    #define MAX_SPEED 0.5f
    #define MIN_SPEED 0.01f
    #define ACCELERATION 0.5f
    #define DECELERATION 0.2f
    #define HOVER_POWER 5.0f
    #define RATE_CONSTANT 1.0f
    #define STOPPING_INTERPOLATE 0.99f
    #endif
     
    //SIDEWAY_SPEED_CONST in ms-1 power-1 degs-1
     
    class ChopperControl: public IEventReceiver, public scene::ICollisionCallback
    {
     
            public:
           
            ChopperControl
            (
                    IrrlichtDevice* device,
                    scene::ISceneNode* node
            );
            virtual ~ChopperControl();
            virtual bool OnEvent(const SEvent& event);
            virtual bool onCollision(const scene::ISceneNodeAnimatorCollisionResponse& anim);
           
            void ascent();
            void descent();
            void hover();
            void pitchDown();
            void pitchUp();
            void slowDown();
            void speedUp();
            void turnLeft();
            void turnRight();
            void rollLeft();
            void rollRight();
           
            void resetRoll();
            void resetPitch();
           
            void resetRotation(); //for debugging
            void showDebugInfo();
            void update();
           
           
            // movement:
            void moveInLocalXDir(f32 distance);
            void moveInLocalZDir(f32 distance);
            void move(f32 distance);
            void ascend(f32 deltaTime);
           
            void rotate(scene::ISceneNode* node, const core::vector3df& rot);
           
            void yaw(f32 degs);
            void pitch(f32 degs);
            void roll(f32 degs);
           
            void throttle();
            void brake();
            void stop();
           
           
            bool isStopped()
            {
                    return core::iszero(Speed);
            }
           
            // getters and setters:
            void setNode(scene::ISceneNode* node) { Node = node; }
            void setRootEmpty(scene::ISceneNode* rootEmpty) { RootEmpty = rootEmpty; }
            void setYawEmpty(scene::ISceneNode* yawEmpty) { YawEmpty = yawEmpty; }
            void setPitchEmpty(scene::ISceneNode* pitchEmpty) { PitchEmpty = pitchEmpty; }
            void setRollEmpty(scene::ISceneNode* rollEmpty) { RollEmpty = rollEmpty; }
           
           
            scene::ISceneNode* getNode() const { return Node; }
            scene::ISceneNode* getRootEmpty() const { return RootEmpty; }
            scene::ISceneNode* getYawEmpty() const { return YawEmpty; }
            scene::ISceneNode* getPitchEmpty() const { return PitchEmpty; }
            scene::ISceneNode* getRollEmpty() const { return RollEmpty; }
           
            core::vector3df getVelocity();
           
            const core::vector3df& getNodeRotation() { return Node->getRotation(); }
            void updateAbsolutePosition();
           
            void setSpeed(f32 speed)
            {
                    Speed = speed;
            }
           
            f32 getSpeed() const { return Speed; }
           
            f32 getRollAngleDegs() const;
            f32 getVerticalAngleRads() const;
           
            bool isInverted();
           
            f32 clampAngleDegs(f32 degs);
            void drawLandingGuide(scene::ITriangleSelector* selector, video::SColor color);
           
            protected:
           
            private:
            IrrlichtDevice* Device;
            scene::ISceneManager* Smgr;
            scene::ISceneNode* Node;
            scene::ISceneNode* RootEmpty;
            scene::ISceneNode* YawEmpty;
            scene::ISceneNode* PitchEmpty;
            scene::ISceneNode* RollEmpty;
           
           
            f32 PitchChange;
           
            f32 Speed;
            f32 Power;
           
            u32 LastTime;
     
            u32 ResetRollStartTime;
            f32 OldRollAngleDegs;
            bool IsResettingRoll;
     
            u32 ResetPitchStartTime;
            f32 OldPitchAngleDegs;
            bool IsResettingPitch;
     
            u32 ResetHoverStartTime;
            f32 OldPower;
            bool IsResettingHover;
           
    };
     
     
     
     
     
     
     
    #endif // _CHOPPER_CONTROL_H_
     
