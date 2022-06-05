#ifndef CSPATIALHASH_H
#define CSPATIALHASH_H

#include <irrlicht.h>
#include <GameVariables.h>

class CEntity;
template<typename T = CEntity*, int Width = game_width, int Height = game_height>
class CSpatialHash
{
    public:
        CSpatialHash(float size) : BoxSize(size)
        {
        }
        virtual ~CSpatialHash(void)
        {
        }
        irr::core::list<T>* operator[](const irr::core::vector3df& pos)
        {
            return getData(pos);
        }
        irr::core::list<T>* getData(const irr::core::vector3df& pos)
        {
            irr::s32 X = pos.X/BoxSize;
            irr::s32 Y = pos.Y/BoxSize;
            if (X >=0 && X < Width && Y >= 0 && Y < Height)
                return &Table[X+Y*Width];
            exit(1);
            return (( irr::core::list<T>*)NULL);
        }
        irr::core::list<T>* getData(int X, int Y)
        {
            if (X >=0 && X < Width && Y >= 0 && Y < Height)
                return &Table[X+Y*Width];
            exit(1);
            return (( irr::core::list<T>*)NULL);
        }
    protected:
        const irr::f32 BoxSize;
        irr::core::list<T> Table[Width*Height];
    private:
};

#endif // CSPATIALHASH_H
