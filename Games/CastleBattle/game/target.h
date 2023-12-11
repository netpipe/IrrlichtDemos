#ifndef TARGET_H
#define TARGET_H
#include<irrlicht.h>
#include "physics.h"
using namespace irr;
/** Target represent the treasure of the castle */
class Target{
    private:
        /** Irrlicht scenemanger */
        scene::ISceneManager* smgr;
        /** Physics instance */
        Physics* physics;
        /** Irrlicht scene node */
        scene::IMeshSceneNode* irrTarget;
        /** Bullet btRigiBody reference */
        btRigidBody* btTarget;
        /** Position of the target */
        core::vector3df position;
    public:
        /** Default constructor. Initilize graphich and physics instance of the object at specified position.
        * @see ::Castle::setTreasure()
        * @see ::Cannon::getRange()
        * @see ::GameManager::initGamePlay()
        * @see ::Player::getCannonRange()
        * @see ::Player::checkTarget()
        * @see ::Physics::createTreasure()
        */
        Target(core::vector3df position, scene::ISceneManager* smgr, video::IVideoDriver* driver, Physics* physics);
        /** Get Irrlicht node */
        scene::IMeshSceneNode* getIrrNode();
};
#endif
