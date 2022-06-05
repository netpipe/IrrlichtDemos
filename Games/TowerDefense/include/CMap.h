#ifndef CMAP_H
#define CMAP_H

#include <GameVariables.h>
#include <irrlicht.h>

namespace irr
{
    namespace scene
    {
        class ISceneManager;
    }
}
class CTile;
class CStateManager;
class CMap
{
    public:
        /** Default constructor */
        CMap();
        /** Default destructor */
        virtual ~CMap();

        void init(unsigned int x, unsigned int y, irr::scene::ISceneManager* sceneManager);

        void setSelected(int x, int y, E_TILE_TYPE type);

        CTile* getTileMap(void)
        {
            return Map;
        }

        CTile* getGoal(void)
        {
            return Goal;
        }

        CTile* getStart(void)
        {
            return Start;
        }

        CTile* getTile(irr::core::vector3df pos);

        CTile* getTile(int x, int y);

        void saveMap(const char* name, CStateManager* manager);
        void loadMap(const char* name, CStateManager* manager);
    protected:
        CTile* Map;
        CTile* Goal;
        CTile* Start;
        irr::scene::ISceneManager* SceneManager;
        unsigned int Width;
        unsigned int Height;
    private:
};

#endif // CMAP_H
