#ifndef PHYSICS_H
#define PHYSICS_H
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <irrlicht.h>
#include <list>
using namespace irr;
/** Physics class will handle all things related to collisions, creating Bullet physics objects and the physics simulation.
* Remember that ONLY Bullet shape dimensions are x2 greater than Irrlicht unit.
* That means whenever you want to add an object from irrlicht extent you have to consider only half of it : irrSceneNode->getExtent() * 0.5f.
*/
class Physics{
    private:
        /** Type of discrete rigid body simulation  */
        btDiscreteDynamicsWorld *World;
        /** List of btRigitBody instances. All this btRigidBody have a pointer to corresponding graphic object. This system is not used anymore.
        * @see ::MotionStateManager
        * @deprecated
        */
        std::list<btRigidBody *> Objects;
        /** Default collision system  */
        btDefaultCollisionConfiguration *collisionConfiguration;
        /** Bounding box overlapping interface */
        btBroadphaseInterface *broadPhase;
        /** Collision system dispatcher */
        btCollisionDispatcher *dispatcher;
        /** Internal bullet solver */
        btSequentialImpulseConstraintSolver *solver;
        /** Ground transform properties */
        btTransform ground;
    public:
        /** Default constructor, given a boundbix will create the opposite physics ground.
        * @param irrBox aabbox3df irrlicht world bounding box
        */
        Physics(const core::aabbox3df irrBox);
        ~Physics();
        /**
        * Init physics instances of the World. Like ground, collision configuration, gravity and friction.
        * A ground is created.
        */
        void initWorld();
        /** Return ground transform */
        btTransform getGround();
        /** World step simulation. This must be sync with Irrlicht */
        void UpdatePhysics(u32 delta, u32 fps);
        /** Update graphic object transformation.
        * @see ::MotionStateManager
        * @deprecated
        */
        void UpdateRender(btRigidBody* btbody);

        /** Create the projectile physics instance
        * @param node ISceneNode graphic node
        * @param position vector3df position to set
        * @param radius f32 radius of the projectile
        * @see Physics
        * @see ::Ball
        * @see ::MotionStateManager
        */
        btRigidBody* createCannonBall(scene::ISceneNode* node, core::vector3df position, f32 radius);

        /** Create the treasure physics instance.
        * @param node IMeshSceneNode mesh node of the irrlicht graphic object.
        * @param scale vector3df scaling of the node passed.
        * @param position vector3df position of the object.
        * @see ::Target
        * @see ::checkTarget
        * @see ::MotionStateManager
        */
        btRigidBody* createTreasure(scene::IMeshSceneNode* node, core::vector3df scale, core::vector3df position);
        /** Create a castle block. So it will be sensible to collision.
        * @param node ISceneNoe node of the irrlicht graphic object.
        * @param rotation vector3df rotation of the object
        * @param scale vector3df scaling of the node passed.
        * @param position vector3df position of the object.
        * @see ::Castle::createBlock();
        * @see ::Castle::buildCastle();
        * @see ::MotionStateManager;
        */
        btRigidBody* createCastleBlock(scene::ISceneNode* node, core::vector3df rotation, core::vector3df scale, core::vector3df position);
};
#endif
