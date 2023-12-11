#include "trigger.h"

CTrigger::CTrigger(int type)
{
    CTrigger::type = type;
    CTrigger::Active = true;
}

void CTrigger::setAmmoValues(int type, int amount, int action)
{
    CTrigger::Ammo_type = type;
    CTrigger::Ammo_amount = amount;
    CTrigger::Ammo_action = action;
}

void CTrigger::setSoundValues(std::string filename, bool loop)
{
    CTrigger::Sound_filename = filename;
    CTrigger::Sound_loop = loop;
}
