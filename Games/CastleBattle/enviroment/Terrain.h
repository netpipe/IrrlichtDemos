#include <irrlicht.h>

using namespace irr;
/** Terrain of the world. */
class Terrain{
    private:
        /** Irrlicht terrain node. */
        scene::ITerrainSceneNode* terrain;

    public:
        /** Default constructor. */
        Terrain(scene::ISceneManager* smgr, video::IVideoDriver* driver, const core::stringw* pathTexture, const core::vector3df &scale);
        ~Terrain();
        /** Get center of the terrain */
        core::vector3df getCenter();
        /** @deprecated collision sytem with terrain. Not used. Bullet is better. */
        void collisionResponseAnimator(scene::ISceneManager* smgr,scene::ISceneNode* obj);
        /** @deprecated collision sytem with terrain. Not used. Bullet is better. */
        void addCollisionNode(scene::ISceneManager* smgr,scene::IAnimatedMeshSceneNode* obj);
        /** Get bounding box of the terrain*/
        core::aabbox3df getTerrainBox();
        /** Get irrlicht node terrain instance */
        scene::ITerrainSceneNode* getTerrain();

};
