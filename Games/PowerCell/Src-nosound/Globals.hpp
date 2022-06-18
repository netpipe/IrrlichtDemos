#ifndef _GLOBALS_HPP_
#define _GLOBALS_HPP_

#define _DEBUG_MODE_
#define _PLAY_SOUND_

#include <iostream>
#include <fstream>
#include <irrlicht.h>

enum EFFECT
{
  EFFECT_POWERCELL_PICKUP = 345654,
  EFFECT_HEALTHCELL_PICKUP
};

enum MUSIC_LIST
{
  MUSIC_MENU = 927834,
  MUSIC_DEAD,
  MUSIC_CLEAR,
  MUSIC_STAGE1,
  MUSIC_STAGE2,
  MUSIC_STAGE3,
  MUSIC_STAGE4
};

enum STATE
{
  STATE_MENU_MAIN = 43743042,
  STATE_INGAME,
  DYSYR_LEVEL_COMPLETE,
  STATE_GAME_OVER
};

enum GUI_ELEMENTS
{
  BUTTON_MAIN_MENU_STAGE1 = 236267724,
  BUTTON_MAIN_MENU_STAGE2,
  BUTTON_MAIN_MENU_STAGE3,
  BUTTON_MAIN_MENU_STAGE4
};

using namespace std;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "Stage.hpp"
#include "CGUITexturedSkin.h"
#include "Event.hpp"
#include "Timer.hpp"
#include "Scene.hpp"
#include "Highscore.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "Collision.hpp"
#include "GameGUI.hpp"
#include "HealthCell.hpp"
#include "PlayerHelper.hpp"
#include "PowerCell.hpp"
#include "Sound.hpp"
#include "Obstacle.hpp"
#include "Output.hpp"
#include "Elevator.hpp"
#include "MenuGUI.hpp"
#include "worldNodes/water.h"

#endif
