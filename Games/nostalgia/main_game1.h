/// This is the main header file for game Nostalgia

#ifndef __MAIN_GAME1_H_INCLUDED__
#define __MAIN_GAME1_H_INCLUDED__
#define USE_IRRKLANG
#include <iostream>
#include <stdlib.h>
#include <irrlicht.h>
		#ifdef _IRR_WINDOWS_
		#include <windows.h>
		#endif
#include "constants.h"
#include "cshell.h"
#include "settings.h"


using namespace irr;

// audio support

#ifdef USE_IRRKLANG
//	#include <irrKlang.h>	// problem here? go to http://www.ambiera.com/irrklang and download
							// the irrKlang library or undefine USE_IRRKLANG at the beginning
							// of this file.
	#ifdef _IRR_WINDOWS_
	#pragma comment (lib, "libirrKlang.a")
	#endif
#endif
#ifndef IRRLICHT_FAST_MATH
#define IRRLICHT_FAST_MATH
#endif

irr::s32 temp;

extern level data_level[];
extern wave data_wave[];

struct SParticleImpact
	{
		irr::u32 when;
		core::vector3df pos;
		core::vector3df outVector;
		irr::scene::ISceneNode* node;

	};

void debug();
void tir();
void createShellImpacts();
void createEnemyImpacts();
void mircanon();
bool init_wave();
bool init_level();
bool init();
void all_game_over();
void HelpText();
void AboutText();
void NewGame();
bool game_done = false;
gui::IGUIWindow* end_box;
void	check_collision();
void targeting();
u32 now=0,level_on_time=3000;
core::array<SParticleImpact> Impacts;
core::array<SParticleImpact> Enemy_Impacts;
//alignment for data display
core::rect<s32> angle_pos;
core::rect<s32> gravity_pos;
core::rect<s32> level_pos;
core::rect<s32> wave_pos;
video::SColor level_color =video::SColor(130,255,5,55);
bool level_on = false;
irr::f32 anglex_degree;
core::stringw angle_text;
core::stringw gravity_text;
core::stringw level_text;
core::stringw wave_text;
core::stringw pts;
core::position2d<s32> crosshair_pos,mire_pos;
core::rect<s32> pts_pos;
video::SColor data =video::SColor(130,155,5,155);
gui::ICursorControl* cursorcontrol;
core::position2d<f32> cursorpos;
const f32 screen_center = 0.5;

//targeting a jet
u32 time_to_beep =0;
u32 time_to_lock = 1000;
u32 time_on_target =0;
core::position2d<irr::s32> target_2d_pos;
bool mire_jets = 0;

video::ITexture* mire;//curseur de mire des jets
//alignment for data display
const u32 tableau_x= 300;
const u32 tableau_y= 60;



#endif//main_game1 included
