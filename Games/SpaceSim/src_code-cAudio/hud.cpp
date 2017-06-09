#include "stdafx.h"
#include "hud.h"


using namespace irr;
using namespace scene;
using namespace gui;

//the hud
//here be dragons.

Hud::Hud(irr::IrrlichtDevice *graphics, Player *CPlayer)
{
	this->CPlayer = CPlayer;
	this->graphics = graphics;


	//setup fonts
	core::dimension2d<u32> t = graphics->getVideoDriver()->getScreenSize();
	gui::IGUIFont *large_font= graphics->getGUIEnvironment()->getFont("res/font/large.xml");
	gui::IGUIFont *small_font = graphics->getGUIEnvironment()->getFont("res/font/small.xml");
	gui::IGUIFont *micro = graphics->getGUIEnvironment()->getFont("res/font/micro.xml");

	graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/hud.png"),core::position2d<s32>(-90,t.Height-190));
	target_icon = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/target.png"),core::vector2d<s32>(0,0));
	target_icon->setVisible(false);


	//Create all the text shown on the screen
	velocity = graphics->getGUIEnvironment()->addStaticText(L"velocity",rect<s32>(10,t.Height-110,t.Width/2+35,t.Height),false);
	if(velocity)
	{
		velocity->setOverrideColor(video::SColor(255,255,255,255));
		velocity->setOverrideFont(large_font);

	}
	hull = graphics->getGUIEnvironment()->addStaticText(L"hull",rect<s32>(10,t.Height-70,120,t.Height));
	if(hull)
	{
		hull->setOverrideColor(video::SColor(255,255,255,255));
		hull->setOverrideFont(small_font);
	}
	armor = graphics->getGUIEnvironment()->addStaticText(L"armor",rect<s32>(10,t.Height-55,120,t.Height));
	if(armor)
	{
		armor->setOverrideColor(video::SColor(255,255,255,255));
		armor->setOverrideFont(small_font);
	}
	shield = graphics->getGUIEnvironment()->addStaticText(L"shield",rect<s32>(10,t.Height-40,120,t.Height));
	if(shield)
	{
		shield->setOverrideColor(video::SColor(255,255,255,255));
		shield->setOverrideFont(small_font);
	}

	cannons_primary = graphics->getGUIEnvironment()->addStaticText(L"Primary Turrets",rect<s32>(200,t.Height-40,400,t.Height));
	if(cannons_primary)
	{
		cannons_primary->setOverrideColor(video::SColor(255,255,255,255));
		cannons_primary->setOverrideFont(small_font);
	}
	cannons_primary_range = graphics->getGUIEnvironment()->addStaticText(L"Railguns 6500m",rect<s32>(200,t.Height-20,400,t.Height));
	if(cannons_primary_range)
	{
		cannons_primary_range->setOverrideColor(video::SColor(255,255,255,255));
		cannons_primary_range->setOverrideFont(small_font);
	}

	target = graphics->getGUIEnvironment()->addStaticText(L"No Target Selected",rect<s32>(t.Width-120,20,t.Width-20,120), true);
	if(target)
	{
		target->setOverrideColor(video::SColor(255,255,255,255));
		target->setOverrideFont(micro);
	}
	target_hull = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-120,50,t.Width-20,120));
	if(target_hull)
	{
		target_hull->setOverrideColor(video::SColor(255,255,255,255));
		target_hull->setOverrideFont(micro);
	}
	target_armor = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-120,60,t.Width-20,120));
	if(target_armor)
	{
		target_armor->setOverrideColor(video::SColor(255,255,255,255));
		target_armor->setOverrideFont(micro);
	}
	target_shield = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-120,70,t.Width-20,120));
	if(target_shield)
	{
		target_shield->setOverrideColor(video::SColor(255,255,255,255));
		target_shield->setOverrideFont(micro);
	}
	target_distance = graphics->getGUIEnvironment()->addStaticText(L"",rect<s32>(t.Width-120,80,t.Width-20,120));
	if(target_distance)
	{
		target_distance->setOverrideColor(video::SColor(255,255,255,255));
		target_distance->setOverrideFont(micro);
	}

}

Hud::~Hud()
{
}

void Hud::updateHud(CShip* player_target)
{
	//update all the strings on the hud
	if(velocity)
	{
		int v = CPlayer->getVelocity();
		core::stringw velo_str(L"");
		velo_str+=v;
		velo_str+=L"m/s";
		velocity->setText(velo_str.c_str());
	}
	if(hull)
	{
		int h = CPlayer->getHull();
		core::stringw hull_str(L"Hull [");
		hull_str += h;
		hull_str+= L"]";
		hull->setText(hull_str.c_str());
	}
	if(armor)
	{
		int a = CPlayer->getArmor();
		core::stringw armor_str(L"Armor [");
		armor_str += a;
		armor_str+= L"]";
		armor->setText(armor_str.c_str());
	}
	if(shield)
	{
		int s = CPlayer->getShield();
		if( s < 0)
			s=0;
		core::stringw shield_str(L"Shield [");
		shield_str +=s;
		shield_str+=L"]";
		shield->setText(shield_str.c_str());
	}

	if(cannons_primary)
	{
		//gets the recharge time of player cannons
		//converts it to a percent value
		float primary = ((graphics->getTimer()->getTime()+2000) - CPlayer->cannonFired.primary_time);
		if(primary>2000)
			primary=2000;
		primary=primary/2000;
		primary=primary*100;
		int c = primary;
		core::stringw cannon_p_str(L"Primary Turrets [");
		cannon_p_str+=c;
		cannon_p_str+=L"%]";
		cannons_primary->setText(cannon_p_str.c_str());

	}
	if(target)
	{
		if(player_target!=0)
		{
			core::vector2d<s32> pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(player_target->getPos(),graphics->getSceneManager()->getActiveCamera());
			target_icon->setVisible(true);
			target_icon->setRelativePosition(core::vector2d<s32>(pos.X-32,pos.Y-32));

			//hull
			int th = player_target->getHullPoints();
			core::stringw target_hull_str(L"Hull [");
			target_hull_str+=th;
			target_hull_str+=L"]";
			target_hull->setText(target_hull_str.c_str());

			//armor
			int ta = player_target->getArmorPoints();
			core::stringw target_armor_str(L"Armor [");
			target_armor_str+=ta;
			target_armor_str+=L"]";
			target_armor->setText(target_armor_str.c_str());

			//shields
			int ts = player_target->getShieldPoints();
			if (ts<0)
				ts = 0;
			core::stringw target_shield_str(L"Shield [");
			target_shield_str+=ts;
			target_shield_str+=L"]";
			target_shield->setText(target_shield_str.c_str());

			//distance
			int dist = getDistance(player_target);
			core::stringw target_dist_str(L"");
			target_dist_str+=dist;
			target_dist_str+=L"m";
			target_distance->setText(target_dist_str.c_str());

			if(player_target->getShipFaction()==FACTION_PROVIAN_CONSORTIUM)
			{
				core::stringw tar(L"Provian Consortium");
				target->setText(tar.c_str());
			}
		}
		else
		{
			//if theres no target
			//hide everything
			target_icon->setVisible(false);
			target->setText(L"No Target Selected");
			target_hull->setText(L"");
			target_armor->setText(L"");
			target_shield->setText(L"");
			target_distance->setText(L"");
		}
	}
}

//gets the distance from player to target
int Hud::getDistance(CShip* player_target)
{
	float z = CPlayer->getPos().Z-player_target->getPos().Z;
	float y = CPlayer->getPos().Y-player_target->getPos().Y;
	float x = CPlayer->getPos().X-player_target->getPos().X;

	float dist = (sqrt(pow(x,2)+pow(y,2)+pow(z,2)));

	return dist;
}