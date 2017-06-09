
    #ifndef _SIMPLE_FLIGHT_CONTROL_H_
    #define _SIMPLE_FLIGHT_CONTROL_H_

    #include <vector>
    #include <irrlicht.h>


    using namespace irr;


    #ifndef SIMPLE_FLIGHT_CONTROL_PROPERTIES
    #define SIMPLE_FLIGHT_CONTROL_PROPERTIES
    #define YAW_RATE 40.0f
    #define MAX_SPEED 0.5f
    #define MIN_SPEED 0.01f
    #define ACCELERATION 0.01f
    #define DECELERATION 0.02f
    #endif


    class SimpleFlightControl: public IEventReceiver
    {

        public:

        SimpleFlightControl
        (
            scene::ISceneManager* smgr,
            scene::ISceneNode* node,
            f32 speed = 0.2f
        );
        virtual ~SimpleFlightControl();
        virtual bool OnEvent(const SEvent& event);

        void resetRotation(); //for debugging
        void showDebugInfo();
        void update(u32 time);


        // movement:
        void move(f32 distance);
        void rotate(scene::ISceneNode* node, const core::vector3df& rot);

        void yaw(f32 degs);
        void pitch(f32 degs);
        void roll(f32 degs);

        void accelerate();
        void brake();
        void stop() { Speed = 0.0f; }

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

        bool isInverted();


        protected:

        private:
        scene::ISceneManager* Smgr;
        scene::ISceneNode* Node;
        scene::ISceneNode* RootEmpty;
        scene::ISceneNode* YawEmpty;
        scene::ISceneNode* PitchEmpty;
        scene::ISceneNode* RollEmpty;


        f32 PitchChange;

        f32 Speed;
        u32 LastTime;


    };

    #endif // _SIMPLE_FLIGHT_CONTROL_H_
