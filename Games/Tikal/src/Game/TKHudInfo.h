#include <irrlicht.h>
#include <vector>

#ifndef __TK_HUD_INFO__
#define __TK_HUD_INFO__

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class TKHudInfo
{
public:
    TKHudInfo();

    void setLabel(stringc newLabel);
    stringw getLabel();

    void setLife(int newLife);
    stringw getLife();
    int getIntLife();

    void addBtAction(stringc action);
    stringc getBtAction(int bt);
    int getBtSize();
    void clearButtons();

    void setImage(stringc newImage);
    stringc getImage();

private:
    stringc label;
    int life;
    vector<stringc> bt_actions;
    stringc image;

    stringw toStringw(stringc str);
    stringw toStringw(int i);
};

#endif
