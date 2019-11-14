#ifndef IRRBULLET_DEBUG_MODE
    #define IRRBULLET_DEBUG_MODE
#endif

////////////////
// To-Do List //
////////////////
/*
- Add irrBulletCallback interface for custom user callbacks like onCollide, onLiquidPenetration, etc.
- Remove boolean parameter from irrBulletWorld::debugDrawWorld() and ::debugDrawProperties(), adding
    four new functions: ::isDebugDrawEnabled(), ::setDebugDrawEnabled(), ::isDebugPropertiesEnabled(), setDebugPropertiesEnabled()
- Add raycast function to irrBulletWorld
- Add raycast example (after implementing raycasting)
- Add cylinder shape
- Add capsule shape
- Add support for ITerrainSceneNode
*/

/////////////////////
// Known Bugs List //
/////////////////////
/*
- In the raycast tank example, if the tank is tilted at certain angles while turning, "random" results will happen
- When using the attraction affector, Bullet will sometimes report an AABB overflow error and remove the object
*/



#define IRRBULLET_VER_MAJOR 0
#define IRRBULLET_VER_MINOR 1
#define IRRBULLET_VER_MICRO 8
