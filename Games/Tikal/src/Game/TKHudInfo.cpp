#include "TKHudInfo.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKHudInfo::TKHudInfo()
{
}

void TKHudInfo::setLabel(stringc newLabel)
{
    label=newLabel;
}

stringw TKHudInfo::getLabel()
{
    return toStringw(label);
}

void TKHudInfo::setLife(int newLife)
{
    life=newLife;
}

stringw TKHudInfo::getLife()
{
    return toStringw(life);
}

int TKHudInfo::getIntLife()
{
    return life;
}

void TKHudInfo::clearButtons()
{
    bt_actions.clear();
}

void TKHudInfo::addBtAction(stringc action)
{
    bt_actions.push_back(action);
}

stringc TKHudInfo::getBtAction(int bt)
{
    if(bt<bt_actions.size())
    {
        return bt_actions[bt];
    }
    else
    {
        return "NULL_ACTION";
    }
}

int TKHudInfo::getBtSize()
{
    return bt_actions.size();
}

stringw TKHudInfo::toStringw(stringc str)
{
    stringw temp="";
    temp+=str;
    return temp;
}

stringw TKHudInfo::toStringw(int i)
{
    stringw temp="";
    temp+=i;
    return temp;
}

void TKHudInfo::setImage(stringc newImage)
{
    image=newImage;
}

stringc TKHudInfo::getImage()
{
    return image;
}
