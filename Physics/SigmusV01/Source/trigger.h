#ifndef _TRIGGER_H
#define _TRIGGER_H

#include "globals.h"
#include "main.h"

class CTrigger
{
    public:
    CTrigger(int type);
    void setSoundValues(std::string filename, bool loop);
    void setAmmoValues(int type, int amount, int action);

    bool Active;
    int type;
    std::string Sound_filename;
    bool Sound_loop;
    int Ammo_type;
    int Shader_type;
    int EOSA_type;
    bool EOSAction;
    bool Shader_Action;
    int Ammo_amount;
    int Ammo_action;
    bool Player_Control;
};

#endif
