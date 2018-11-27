
#include <irrlicht.h>
#include <iostream>
#include <vector>

using namespace std;

#include "../Objects/ObjectsManager.h"
#include "TKCamera.h"

#include "TKCharacter.h"

#include "TKEventReceiver.h"

#include "TKBuildingManager.h"

#include "TKCharacterManager.h"

#include "TKCharacter.h"

#include "TKMenuManager.h"

#include "TKHudInfo.h"

#include "TKNetwork.h"

#ifndef __TKCORE__
#define __TKCORE__


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const bool DEBUG_NETWORK=false;

class TKCore
{
public:
    TKCore();
    ~TKCore();
    void drawHud();
    void drawHUDSelectedItem();
    void drawMousePointer();
    void eventKeyPressed(int key);
    void eventKeyReleased(int key);
    void eventMouseClick(int mouse);
    void eventCollectResource(stringc player, stringc resource);
    void eventGui(int id);
    void eventNetworkReceive(stringc msg);
    void loadLevel(stringc levelName);
    void runGame();///TODO:verificar questao servidor/cliente

    void showHUD();
    void hideHUD();

private:
    static const int cameraAxisX=0;
    static const int cameraAxisZ=2;

    IrrlichtDevice *device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;

    TKMenuManager* menuMgr;

    ObjectsManager* obmgr;
    TKCamera* camera;
    IEventReceiver* tkEventReceiver;

    ITexture* mouseTexture;
    ITexture* hud_Left;
    ITexture* hud_Middle;
    ITexture* hud_Right;

    ITexture* hud_Food54;
    ITexture* hud_Stone54;
    ITexture* hud_Wood54;
    ITexture* hud_Warrior54;
    ITexture* hud_Villager54;
    ITexture* hud_Militar54;
    ITexture* hud_House54;
    ITexture* hud_Temple54;

    ITexture* hud_Food34;
    ITexture* hud_Stone34;
    ITexture* hud_Wood34;

    IGUIFont* hud_Font8;
    IGUIFont* hud_Font10;
    IGUIFont* hud_Font12;
    IGUIFont* hud_Font16_50;

    IGUIStaticText* hud_LabelSelectedItem;
    IGUIStaticText* hud_LabelFood;
    IGUIStaticText* hud_LabelWood;
    IGUIStaticText* hud_LabelStone;
    IGUIStaticText* hud_SelectedLife;
    IGUIStaticText* hud_LabelPopulationLimit;

    vector<IGUIButton*> hud_bt;

    ITexture* hud_btImg_build_house;
    ITexture* hud_btImg_build_militar;
    ITexture* hud_btImg_create_warrior;
    ITexture* hud_btImg_create_villager;

    TKCharacterManager* characterManager;

    vector3df getMousePosition3D();

    bool isLMBDown;//Mouse LMB is pressed?
    vector3df p1Selection3D;
    vector3df p2Selection3D;
    position2di p1Selection2D;
    position2di p2Selection2D;

    TKBuildingManager* buildingManager;

    stringc appState;

    rect<s32> myRect(s32 x,s32 y, s32 w, s32 h);


    void hud_MIddle_setSelectedItem(TKHudInfo* info);
    void hud_Middle_setVisible(bool visible);
    bool hud_Middle_isVisible;
    vector3df hud_Middle_SelectedItemPosition;//selected item 3D position, ie: building, char, opetions
    TKHudInfo* hud_Middle_SelectedItemInfo;

    void setHudBt(int bt,stringc type);
    void setHudImg54(stringc img);
    ITexture* hud_Selected_Item_Image54;

    stringw toStringw(stringc str);///TODO:remove
    stringw toStringw(int i);

    TKNetwork* tkNetwork;

    stringc playerName;
    stringc enemyName;

    f32 playerFood;
    f32 playerStone;
    f32 playerWood;
    int playerPopulationLimit;
    int playerPopulation;

    TKBuilding* plantingBuilding;
    void plantBuilding(TKBuilding* building);
    void hud_updateLabelPopulation();

    ///retorna a i-ésima substring (separadas por espaço)
    ///Ex: getStr("A1 A2 A3",0) retorna "A1"
    ///    getStr("A1 A2 A3",1) retorna "A2" ...
    std::string getStr(std::string str, int index);
};

#endif
