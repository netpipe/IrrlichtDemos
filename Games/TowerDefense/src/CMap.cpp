#include "CMap.h"
#include "CTile.h"
#include "CStateManager.h"
#include <irrlicht.h>

CMap::CMap()
{
    //ctor
    Map = NULL;
    Goal = NULL;
    Start = NULL;
}

CMap::~CMap()
{
    //dtor
    if (Map)
        delete [] Map;
    Map = NULL;
}

void CMap::init(unsigned int x, unsigned int y, irr::scene::ISceneManager* sceneManager)
{
    Width = x;
    Height = y;
    SceneManager = sceneManager;
    if (Map)
    {
        delete [] Map;
    }
    Map = new CTile[Width*Height];
    for (int w=0;w < Width; ++w)
    {
        for (int h=0; h < Height; ++h)
        {
            Map[w+h*Width].init(w, h, NULL, NULL, NULL, NULL, SceneManager);

            if ((h+1) < Height)
                Map[w+h*Width].setUp(&Map[w+(h+1)*Width]);
            if ((h-1) >= 0)
                Map[w+h*Width].setDown(&Map[w+(h-1)*Width]);
            if ((w-1) >= 0)
                Map[w+h*Width].setLeft(&Map[w-1+h*Width]);
            if ((w+1) < Width)
                Map[w+h*Width].setRight(&Map[w+1+h*Width]);

        }
    }

    Goal = NULL;
    Start = NULL;
}

CTile* CMap::getTile(irr::core::vector3df pos)
{
    int X = pos.X/tile_size;
    int Y = pos.Y/tile_size;
    if (X >= 0 && X < Width && Y >= 0 && Y < Height)
    {
        return &Map[X+Y*Width];
    }
    return NULL;
}

CTile* CMap::getTile(int X, int Y)
{
    if (X >= 0 && X < Width && Y >= 0 && Y < Height)
    {
        return &Map[X+Y*Width];
    }
    return NULL;
}

void CMap::setSelected(int x, int y, E_TILE_TYPE type)
{
    if (x >= 0 && x < Width && y >= 0 && y < Height)
    {
        if (type == ETT_GOAL)
        {
            if (Goal)
                Goal->setType(ETT_FORBIDDEN);
            Goal = &Map[x+y*Width];
        }
        else if (type == ETT_START)
        {
            if (Start)
                Start->setType(ETT_FORBIDDEN);
            Start = &Map[x+y*Width];
        }
        Map[x+y*Width].setType(type);
    }
}

void CMap::saveMap(const char* name, CStateManager* manager)
{
    irr::io::IWriteFile* file = manager->getDevice()->getFileSystem()->createAndWriteFile(name, false);
    file->write(&GAME_VERSION, 4);
    for (int i=0;i<game_width*game_height;++i)
    {
        int t = Map[i].getType();
        file->write(&t, 4);
    }
    file->drop();
}

void CMap::loadMap(const char* name, CStateManager* manager)
{
    Goal = NULL;
    Start = NULL;
    irr::io::IReadFile* file = manager->getDevice()->getFileSystem()->createAndOpenFile(name);
    int g = 0;
    file->read(&g, 4);
    if (g != GAME_VERSION)
    {
        printf("Wrong map version\n");
        return;
    }
    for (int i=0;i<game_width*game_height;++i)
    {
        E_TILE_TYPE t;
        file->read(&t, sizeof(E_TILE_TYPE));
        if ((E_TILE_TYPE)t == ETT_GOAL)
        {
            if (Goal)
                Goal->setType(ETT_FORBIDDEN);
            Goal = &Map[i];
        }
        else if ((E_TILE_TYPE)t == ETT_START)
        {
            if (Start)
                Start->setType(ETT_FORBIDDEN);
            Start = &Map[i];
        }
        Map[i].setType((E_TILE_TYPE)t);
    }
    file->drop();
}
