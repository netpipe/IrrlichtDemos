#ifndef CTILE_H
#define CTILE_H
#include <irrlicht.h>
#include <GameVariables.h>

class CSprite;
class CTile
{
    public:
        /** Default constructor */
        CTile();
        /** Default destructor */
        virtual ~CTile();

        void init(int x, int y, CTile* a, CTile* b, CTile* c, CTile* d, irr::scene::ISceneManager* manager);

        void setType(E_TILE_TYPE type);
        E_TILE_TYPE getType(void);

        void select(bool s);

        void setUp(CTile* tile);
        void setRight(CTile* tile);
        void setDown(CTile* tile);
        void setLeft(CTile* tile);
        CTile* getUp(void);
        CTile* getRight(void);
        CTile* getDown(void);
        CTile* getLeft(void);
        irr::core::vector2di getGridPos(void)
        {
            return irr::core::vector2di(Pos[0], Pos[1]);
        }
        irr::core::vector3df getPos(void)
        {
            return Position;
        }
    protected:
        CSprite* Sprite;
        E_TILE_TYPE Type;
        irr::core::vector3df Position;
        int Pos[2];
        CTile* Around[4];/**    0 - up
                                1 - right
                                2 - down
                                3 - left
                        */
    private:
};

#endif // CTILE_H
