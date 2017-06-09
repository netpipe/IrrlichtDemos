#ifndef _HUD_H_
#define _HUD_H_

#include "irrlicht.h"
#include "irrKlang.h"
#include "player.h"
#include "math.h"


using namespace irr;

class Hud
{
public:



	Player *CPlayer;
	irr::IrrlichtDevice *graphics;
	irr::gui::IGUIStaticText *velocity;
	irr::gui::IGUIStaticText *hull;
	irr::gui::IGUIStaticText *armor;
	irr::gui::IGUIStaticText *shield;
	irr::gui::IGUIStaticText *cannons_primary;
	irr::gui::IGUIStaticText *cannons_primary_range;

	irr::gui::IGUIStaticText *target;
	irr::gui::IGUIStaticText *target_distance;
	irr::gui::IGUIStaticText *target_hull;
	irr::gui::IGUIStaticText *target_armor;
	irr::gui::IGUIStaticText *target_shield;

	gui::IGUIImage *target_icon;


	Hud(irr::IrrlichtDevice *graphics, Player *CPlayer);
	~Hud();
	void updateHud(CShip* player_target);

	int getDistance(CShip* player_target);
};

#endif
