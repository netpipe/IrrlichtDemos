#include <irrlicht.h>
#include "ball.h"
#include "keyboard.h"
#include "camera.h"

using namespace irr;
using namespace KEYBOARD;
const f32 INCLINATE_FACTOR = 0.1f; /*!< Speed of barrel of the cannon */
const f32 MAX_ANGLE_BOTTOM = 5.f;
const f32 MAX_ANGLE_TOP = 45.f;
const f32 MAX_ANGLE_LEFT = -20.f;
const f32 MAX_ANGLE_RIGHT = 20.f;
const f32 MAX_CANNON_FORCE = 50.f; /*!< Max cannon power */
const f32 CANNON_POWER = 50.f; /*!< Cannon power */
const f32 BARREL_RADIUS = 0.6f; /*!< The radius of the barrel  */
/** Cannon handles the cannon object. All things like shoot, moving, camera effects are handled by this class. See also ::Player, ::Camera, ::Ball */
class Cannon{
    private:
        scene::ISceneManager* smgr;
        IrrlichtDevice* device;
        video::IVideoDriver* driver;
        /** Cannon scene node  */
        scene::IMeshSceneNode* cannon;
        /** Barrel (cane) of the cannon.  IMeshBuffer  */
        scene::IMeshBuffer* barrel;
        /** Wagon is the bottom part of the 3d cannon object. IMeshBuffer  */
        scene::IMeshBuffer* wagon;
        /** Angle of the cannon */
        f32 angle;
        /** Fixed Camera of the cannon */
        Camera* camera;
        /** Projectile of the cannon */
        Ball* btBall;
        /** ::Physics instance */
        Physics* physics;
        /** Initial vector ray.
        * The vector represent the initial raytrace of the barrel.
        * Since the barrel in the 3d model is angled the ::Cannon::initAngles() will setup this vector.
        * That means if the user change the angle, the barrel vector will change too, so the start position of the projectile will be correct
        */
        core::vector3df initialBarrelVector;
        /** See ::initialBarrelVector. This is the same. */
        core::vector3df initBarrelVector;
        /** The plane of the cannon */
        core::vector3df plane;
    public:
        core::vector3df rotation;
        /** Default constructor initialize the angles, setup meshes.
        * @param position vector3df position of the cannon
        * @param physics Physics physics instance
        * @see ::Ball, ::Camera, ::Keyboard
        */
        core::vector3df rotationconstraint;
        Cannon(IrrlichtDevice* device, scene::ISceneManager* smgr, video::IVideoDriver* driver, core::vector3df position, Physics* physics);
        ~Cannon();
        /** Set angle variable and calculate the angle between the plane and the initBarrelVector. */
        f32 refreshAngle();
        /** Get bounding box of the cannon. */
        core::aabbox3df getBoundingBox();
        /** Get meshnode of the cannon. */
        scene::IMeshSceneNode* getCannon();

        /** Handle user input to change cannon rotations.
        * @param ACTION related to the key pressed
        * @return matrix with the transformation
        * @see ::Keyboard
        */
        core::matrix4 getInclinateValues(ACTION_KEYBOARD key);
        /** With some randomness get a position Z based on the cannon power- So there always be a trajectory to hit something in that position(e.g Target).
        @see ::Player::getCannonRange()
        */
        core::vector3df getRange();
        /** Get camera of the cannon */
        Camera* getCamera();
        /** Set rotation of the cannon */
        void setRotation(core::vector3df rotation);
        /** Based on the angles vector: initBarrelVector and the cannon power, this will shoot a ::Ball.
        * The method also will instance the camera related to the ball while this will be in the air
        * @param power f32 power
        * @see ::Camera
        * @see ::CANNON_POWER
        */
        void shoot(f32 power);
        /** Set camera of the cannon.
        * @param offset vector3df offset from the position
        * @param position vector3df position of the camera
        * @param smgr ISceneManager Irrlciht scenemanaer
        * @param node ISceneNode ICameraSceneNode object
        */
        void setCamera(core::vector3df offset, core::vector3df rotation, scene::ISceneManager* smgr,scene::ISceneNode* node);
        /** Set position of the cannon */
        void setPosition(core::vector3df position);
        /** Handle moves in cannon left or right top and down (only angles ) */
        bool moveCannon(ACTION_KEYBOARD action);
        /** Move projectile camera.
        * I didnt use a parent-child camera related to the ::Ball becaouse there are some trouble to set a desired position.
        * So i make another camera based on the fixed cannon camera whenever the user shoot.
        * While in this new camer is moved. When the camera reaches the terrain it is deleted
        * @return true don't move camera, false move camera
        */
        bool moveCamera();
        /** Calculate init angles, @see initialBarrelVector */
        void initAngles();
        /** Initialize with desired position and rotation */
        void initCannon(core::vector3df position, core::vector3df rotation);

};
