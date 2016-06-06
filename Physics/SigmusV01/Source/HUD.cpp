#include "HUD.h"

CHUD::CHUD(IGUIEnvironment* gui, IVideoDriver* driver)
{
    CHUD::gui = gui;
    CHUD::driver = driver;
    CHUD::lastFPS = 0;

    CHUD::playerHealth = gui->addStaticText(L"Health: 0", rect<s32>(20,20,300,35));
    CHUD::weaponType = gui->addStaticText(L"Weapon Type: Normal", rect<s32>(20,40,300,55));
    CHUD::firingMode = gui->addStaticText(L"Mode: Single", rect<s32>(20,60,300,75));
    CHUD::ammo = gui->addStaticText(L"Ammo: 0", rect<s32>(20,80,300,95));
    CHUD::fireForce = gui->addStaticText(L"Weapon Force: 0%", rect<s32>(20,100,300,115));
    CHUD::DEBUG_FPS = gui->addStaticText(L" ", rect<s32>(20, 950, 300, 965));
    CHUD::DEBUG_Position = gui->addStaticText(L" ", rect<s32>(20, 970, 600, 985));

    CHUD::playerHealth->setOverrideColor(SColor(255, 255, 255, 255));
    CHUD::weaponType->setOverrideColor(SColor(255, 255, 255, 255));
    CHUD::firingMode->setOverrideColor(SColor(255, 255, 255, 255));
    CHUD::ammo->setOverrideColor(SColor(255, 255, 255, 255));
    CHUD::fireForce->setOverrideColor(SColor(255, 255, 255, 255));
    CHUD::DEBUG_FPS->setOverrideColor(SColor(255, 0, 255, 0));
    CHUD::DEBUG_Position->setOverrideColor(SColor(255, 0, 255, 0));
}

void CHUD::remove()
{
    CHUD::playerHealth->remove();
    CHUD::weaponType->remove();
    CHUD::firingMode->remove();
    CHUD::ammo->remove();
    CHUD::fireForce->remove();
    CHUD::DEBUG_FPS->remove();
    CHUD::DEBUG_Position->remove();

    if(DEBUG_CONSOLE)
        cout<<"Removed HUD"<<endl;
}

void CHUD::setAmmo(int ammo)
{
    stringw text = L"Ammo: ";
    text += ammo;
    CHUD::ammo->setText(text.c_str());
}

void CHUD::setPlayerHealth(int Health)
{
    stringw text = L"Health: ";
    text += Health;
    CHUD::playerHealth->setText(text.c_str());
}

void CHUD::setWeaponType(int type)
{
    stringw text = L"Weapon Type: ";

    if(type == WT_NORMAL)
        text += L"Normal";
    if(type == WT_ATTRACTIVE)
        text += L"Attractive";
    if(type == WT_EXPLOSIVE)
        text += L"Explosive";

    CHUD::weaponType->setText(text.c_str());
}

void CHUD::setFiringMode(int mode)
{
    stringw text = L"Mode: ";

    if(mode == FM_SINGLE)
        text += L"Single";
    if(mode == FM_TRIPLE)
        text += L"Triple";
    if(mode == FM_LIMITED)
        text += L"Limited";

    CHUD::firingMode->setText(text.c_str());
}

void CHUD::setFireForce(int force)
{
    stringw text = L"Weapon Force: ";
    text += force;
    text += L"%";

    CHUD::fireForce->setText(text.c_str());
}

void CHUD::updateDebug(vector3df PlayerPosition)
{
	stringw pos;
	vector3df pos_ = PlayerPosition;
	int x = pos_.X;
	int y = pos_.Y;
	int z = pos_.Z;
	pos = L"Position (X,Y,Z) -> ";
	pos += x;
	pos += L", ";
	pos += y;
	pos += L", ";
	pos += z;

	CHUD::DEBUG_Position->setText(pos.c_str());

	int fps_int = CHUD::driver->getFPS();
	if (CHUD::lastFPS!=fps_int)
	{
		stringw fps = L"Engine [";
		fps += driver->getName();
		fps += "] FPS:";
		fps += fps_int;
		CHUD::DEBUG_FPS->setText(fps.c_str());
		CHUD::lastFPS = fps_int;
	};
}

void CHUD::setDebugVisible(bool visible)
{
    if(visible)
    {
        CHUD::DEBUG_FPS->setVisible(true);
        CHUD::DEBUG_Position->setVisible(true);
    }

    if(!visible)
    {
        CHUD::DEBUG_FPS->setVisible(false);
        CHUD::DEBUG_Position->setVisible(false);
    }
}
