#ifndef GAMEVARIABLES_H_INCLUDED
#define GAMEVARIABLES_H_INCLUDED

const int GAME_VERSION = 1;

enum E_TILE_TYPE
{
    ETT_BUILD,
    ETT_STREET,
    ETT_FORBIDDEN,
    ETT_START,
    ETT_GOAL,
    ETT_COUNT
};

enum E_ENTITY_TYPE
{
    EET_ENEMY,
    EET_TOWER,
    EET_POWER_PLANT,
    EET_COUNT
};

enum E_TOWER
{
    ET_GROUND_NORMAL,
    ET_NONE,
    ET_COUNT
};

enum E_COMPONENT_TYPE
{
    ECT_GRAPHIC,
    ECT_LOGIC,
    ECT_MOVE,
    ECT_DATASHEET,
    ECT_DONT_KNOW,
    ECT_COUNT
};

const float tile_size = 32.f;
const int game_width = 30;
const int game_height = 20;

#endif // GAMEVARIABLES_H_INCLUDED
