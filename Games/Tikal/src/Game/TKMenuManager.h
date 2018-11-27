#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

#ifndef __TKMENUMANAGER__
#define __TKMENUMANAGER__

class TKMenuManager
{
public:
    void loadMenu();

    void hideAllMenus();

    void showMainMenu();
    void hideMainMenu();

    void showOptionsMenu();
    void hideOptionsMenu();

    void showCreateGameMenu();
    void hideCreateGameMenu();
    void setReadyToStartGame(bool ready);
    void setIP(stringc ip);
    void setPlayerName(stringc name);

    void hideJoinGameMenu();
    void showJoinGameMenu();

    void updateMenus();

    TKMenuManager(IGUIEnvironment* newguienv, IrrlichtDevice* dvc);

    stringc getServerIP();

    void startButtonSetEnable(bool enable);
    void joinButtonSetEnable(bool enable);

private:
    IrrlichtDevice* device;
    IGUIEnvironment* guienv;
    ISceneManager* smgr;
    IVideoDriver* driver;
    rect<s32> myRect(s32 x,s32 y, s32 w, s32 h);

    IGUIFont* hud_Font8;
    IGUIFont* hud_Font12;
    IGUIFont* hud_Font16_50;

    ///Profile
    IGUIButton* btOKProfile;
    IGUIButton* btExitProfile;
    IGUIComboBox* profiles;


    ///Main Menu
    IGUIButton* btCriarPartida;
    IGUIButton* btEntrarPartida;
    IGUIButton* btOpcoes;
    IGUIButton* btSair;
    IGUIStaticText* creditos;

    ///Options
    IGUIButton* btVoltarMainMenu;
    IGUIScrollBar* soundBar;
    IGUIStaticText* labelSound;
    IGUIComboBox* resolution;
    IGUIStaticText* labelResolution;
    IGUIStaticText* feedbackOptions;
    IGUIButton* btSalvarOpcoes;

    ///Criar partida
    IGUIStaticText* feedbackCreateGame;
    IGUIButton* btStartGame;
    IGUIStaticText* labelIP;
    IGUIStaticText* labelName;
    IGUIComboBox* maps;
    IGUIStaticText* labelMaps;

    ///Conectar a uma partida
    IGUIStaticText* feedbackJoinGame;
    IGUIEditBox* ipServer;
    IGUIStaticText* labelIPServer;
    IGUIButton* btJoinGame;
};
#endif
