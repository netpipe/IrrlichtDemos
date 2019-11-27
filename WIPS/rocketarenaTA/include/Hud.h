/*

Hud - header file

*/

#ifndef __hud_h
#define __hud_h

#include <irrlicht.h>
#include <Radar.h>

using namespace irr;

enum HUD_PLAYER_MODE {
	HUD_PLAYER_STAND,
	HUD_PLAYER_WALK,
	HUD_PLAYER_ROCKET,
	HUD_PLAYER_COUNT
};

#include <Pole.h>
#include <Rocket.h>
#include <Player.h>
#include <Init.h>


class cHud {
public:

	// constructor
	cHud();
	
	// destructor
	~cHud();
	
	// initialization
	void Initialize(cInit *init);
	
	// draw hud
	void drawHud(IrrlichtDevice *device, scene::ICameraSceneNode *camera, cRocket *rocket, cPlayer *player, cPole *poles);
	
	// setting player mode to show in hud
	void setHudPlayerMode(HUD_PLAYER_MODE mode);
	
	// get radar mode none=0/old=1/new=2
	const irr::u32 getRadarMode();
	
	// set radar mode none=0/old=1/new=2
	void setRadarMode(irr::u32 set);
	
	
private:	
	
	SIrrlichtCreationParameters *parameters;
	
	video::ITexture *images;
	video::ITexture *radarImage;
	
	// radar class
	cRadar radar;
	// variable for radar mode old=false/new=true
	irr::u32 radarMode;
	
	core::dimension2d<s32> resolution;
	irr::u32 timeTemp;
	irr::u32 color;
	irr::u32 colorSpeed;
	bool timeFlip;
	bool playerImageFlip;
	
	gui::IGUIFont *fontBig;
	
	core::rect<s32> playerImage;
	
	HUD_PLAYER_MODE pMode;
	HUD_PLAYER_MODE pModeLast;
		
};

#endif
