#ifndef _HUD_H
#define _HUD_H

#include "globals.h"
#include "main.h"

class CHUD
{
    public:
    CHUD(IGUIEnvironment* gui, IVideoDriver* driver);
    void setPlayerHealth(int Health);
    void setWeaponType(int type);
    void setFiringMode(int mode);
    void setAmmo(int ammo);
    void setFireForce(int force);
    void setDebugVisible(bool visible);
    void updateDebug(vector3df PlayerPosition);

    void remove();

    private:
    IGUIEnvironment* gui;
    IVideoDriver* driver;
    IGUIStaticText* playerHealth;
    IGUIStaticText* weaponType;
    IGUIStaticText* firingMode;
    IGUIStaticText* ammo;
    IGUIStaticText* fireForce;
    IGUIStaticText* DEBUG_FPS;
    IGUIStaticText* DEBUG_Position;

    int lastFPS;
};

#endif
