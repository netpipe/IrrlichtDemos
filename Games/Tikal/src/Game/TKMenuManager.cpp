#include "TKMenuManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKMenuManager::TKMenuManager(IGUIEnvironment* newguienv, IrrlichtDevice* dvc)
{
    device=dvc;

    smgr=device->getSceneManager();
    driver=smgr->getVideoDriver();
    guienv=newguienv;

    ///Fonts
    hud_Font8=guienv->getFont("../media/Art/Fonts/font8.png");
    hud_Font12=guienv->getFont("../media/Art/Fonts/font12.png");
    hud_Font16_50=guienv->getFont("../media/Art/Fonts/font16_50.png");
}

void TKMenuManager::loadMenu()
{
    s32 scrw=driver->getScreenSize().Height;

    btCriarPartida=guienv->addButton(myRect(50,scrw-310,200,50),0,1,L"Criar Partida",L"Criar um servidor e iniciar uma nova partida");
    btCriarPartida->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btCriarPartida->setOverrideFont(hud_Font16_50);

    btEntrarPartida=guienv->addButton(myRect(50,scrw-240,200,50),0,2,L"Conectar",L"Conectar-se a um servidor já existente");
    btEntrarPartida->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btEntrarPartida->setOverrideFont(hud_Font16_50);

    btOpcoes=guienv->addButton(myRect(50,scrw-170,200,50),0,3,L"Opções",L"Alterar configurações do jogo");
    btOpcoes->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btOpcoes->setOverrideFont(hud_Font16_50);

    btSair=guienv->addButton(myRect(50,scrw-100,200,50),0,4,L"Sair",L"Sai do jogo");
    btSair->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btSair->setOverrideFont(hud_Font16_50);

    creditos=guienv->addStaticText(L"TIKAL v0.1 Pré-Alpha\n\ndesenvolvido por: \n\nAndres Jessé Porfirio\nJosé Garcia Netto\n\nUnicentro 2009",
        myRect(50,50,350,170));
    creditos->setOverrideFont(hud_Font12);
    ///TODO:Alinhar texto a direita


    ///Options Menu
    btVoltarMainMenu=guienv->addButton(myRect(50,scrw-100,200,50),0,5,L"Voltar",L"Volta para o menu principal descartando as alterações");
    btVoltarMainMenu->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btVoltarMainMenu->setOverrideFont(hud_Font16_50);

    soundBar=guienv->addScrollBar(true,myRect(50,scrw-145,200,25),0,-1);
    labelSound=guienv->addStaticText(L"Volume do Audio",myRect(58,scrw-170,200,25));
    labelSound->setOverrideFont(hud_Font12);

    resolution=guienv->addComboBox(myRect(50,scrw-215,200,25));
    resolution->addItem(L"1366x768");
    resolution->addItem(L"1024x768");
    labelResolution=guienv->addStaticText(L"Resolução de Tela",myRect(50,scrw-240,200,25));
    labelResolution->setOverrideFont(hud_Font12);

    feedbackOptions=guienv->addStaticText(L"OPÇÕES\nVocê está na tela de configuraçoes, aqui você pode alterar o volume do audio e a resolução de tela, faça as alteraçoes necessárias e clique em salvar para guardar suas opçoes, esta operação implica no reinicio do jogo. Para voltar ao menu principal sem salvar suas opçoes clique em ''Voltar''.",myRect(50,50,600,200));

    btSalvarOpcoes=guienv->addButton(myRect(280,scrw-100,200,50),0,6,L"Salvar",L"Grava as opçoes. ATENÇÃO: Esta operação implicará no reinicio do jogo");
    btSalvarOpcoes->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btSalvarOpcoes->setOverrideFont(hud_Font16_50);


    ///CreateGame Menu
    feedbackCreateGame=guienv->addStaticText(L"CRIAR PARTIDA\nEsta tela permite que você crie uma nova partida e aguarde a conexão de outros jogadores. Você pode alterar o cenário do jogo na lista de cenários. Seu IP está sendo exibido no canto da tela, você deve repassá-lo ao seu oponente para que ele possa se conectar ao servidor que você acabou de criar. Quando seu oponente estiver conectado você perceberá que o botão ''Iniciar'' ficará disponivel. Para cancelar esta partida clique em ''Voltar''.",myRect(50,50,600,200));
    btStartGame=guienv->addButton(myRect(driver->getScreenSize().Width-250,scrw-100,200,50),0,7,L"Iniciar",L"Inicia a partida");
    btStartGame->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btStartGame->setOverrideFont(hud_Font16_50);

    labelIP=guienv->addStaticText(L"",myRect(driver->getScreenSize().Width-280,50,230,50));
    labelName=guienv->addStaticText(L"",myRect(driver->getScreenSize().Width-280,80,230,30));///TODO: alinhar a direita

    maps=guienv->addComboBox(myRect(50,scrw-215,300,25));
    maps->addItem(L"1 Ilha dos Javalis Cégos");
    maps->addItem(L"2 Floresta Negra");
    labelMaps=guienv->addStaticText(L"Seleção do Cenário",myRect(50,scrw-240,250,25));
    labelMaps->setOverrideFont(hud_Font12);

    ///Join Game
    feedbackJoinGame=guienv->addStaticText(L"CONECTAR A UMA PARTIDA\nEsta tela permite que você se conecte a uma partida ja existente. Para isto você deve inserir o IP do servidor da partida no campo ''IP do servidor'' e, apos isto, clicar em ''Conectar''. Para cancelar esta partida clique em ''Voltar''.",myRect(50,50,600,200));
    ipServer=guienv->addEditBox(L"10.42.43.10",myRect(50,scrw-215,200,25));
    labelIPServer=guienv->addStaticText(L"IP do Servidor",myRect(50,scrw-240,250,25));
    labelIPServer->setOverrideFont(hud_Font12);

    btJoinGame=guienv->addButton(myRect(driver->getScreenSize().Width-250,scrw-100,200,50),0,8,L"Conectar",L"Conecta ao servidor");
    btJoinGame->setImage(driver->getTexture("../media/Art/hud_Button_200.png"));
    btJoinGame->setOverrideFont(hud_Font16_50);
}

void TKMenuManager::hideJoinGameMenu()
{
    btVoltarMainMenu->setVisible(false);
    feedbackJoinGame->setVisible(false);
    ipServer->setVisible(false);
    labelIPServer->setVisible(false);
    btJoinGame->setVisible(false);
}

void TKMenuManager::showJoinGameMenu()
{
    btVoltarMainMenu->setVisible(true);
    feedbackJoinGame->setVisible(true);
    ipServer->setVisible(true);
    labelIPServer->setVisible(true);
    btJoinGame->setVisible(true);
}

void TKMenuManager::hideCreateGameMenu()
{
    btVoltarMainMenu->setVisible(false);
    feedbackCreateGame->setVisible(false);
    btStartGame->setVisible(false);
    labelIP->setVisible(false);
    labelName->setVisible(false);
    maps->setVisible(false);
    labelMaps->setVisible(false);
}

void TKMenuManager::showCreateGameMenu()
{
    btVoltarMainMenu->setVisible(true);
    feedbackCreateGame->setVisible(true);
    btStartGame->setVisible(true);
    labelIP->setVisible(true);
    labelName->setVisible(true);
    maps->setVisible(true);
    labelMaps->setVisible(true);
}

void TKMenuManager::hideOptionsMenu()
{
    btVoltarMainMenu->setVisible(false);
    soundBar->setVisible(false);
    labelSound->setVisible(false);
    resolution->setVisible(false);
    labelResolution->setVisible(false);

    btSalvarOpcoes->setVisible(false);
    feedbackOptions->setVisible(false);
}

void TKMenuManager::showOptionsMenu()
{
    btVoltarMainMenu->setVisible(true);
    soundBar->setVisible(true);
    labelSound->setVisible(true);
    resolution->setVisible(true);
    labelResolution->setVisible(true);

    btSalvarOpcoes->setVisible(true);
    feedbackOptions->setVisible(true);
}

void TKMenuManager::hideMainMenu()
{
    btCriarPartida->setVisible(false);
    btEntrarPartida->setVisible(false);
    btOpcoes->setVisible(false);
    btSair->setVisible(false);
    creditos->setVisible(false);
}

void TKMenuManager::showMainMenu()
{
    btCriarPartida->setVisible(true);
    btEntrarPartida->setVisible(true);
    btOpcoes->setVisible(true);
    btSair->setVisible(true);
    creditos->setVisible(true);
}

void TKMenuManager::hideAllMenus()
{
    hideMainMenu();
    hideOptionsMenu();
    hideCreateGameMenu();
    hideJoinGameMenu();
}

void TKMenuManager::updateMenus()
{
    driver->draw2DImage(driver->getTexture("../media/Art/logo.png"),position2d<s32>(
        driver->getScreenSize().Width/2-300,
        driver->getScreenSize().Height/2-238/2),
        rect<s32>(0,0,600,238),
        0,
        SColor(255,255,255,255),
        true);
}

void TKMenuManager::setIP(stringc ip)
{
    stringw wideIP="IP: ";
    wideIP+=ip;
    labelIP->setText(wideIP.c_str());
}

void TKMenuManager::setPlayerName(stringc name)
{
    stringw wideName="";
    wideName+=name;
    labelName->setText(wideName.c_str());
}

void TKMenuManager::setReadyToStartGame(bool ready)
{
    btStartGame->setEnabled(ready);
}

rect<s32> TKMenuManager::myRect(s32 x,s32 y, s32 w, s32 h)
{
    return rect<s32>(x,y,x+w,y+h);
}

stringc TKMenuManager::getServerIP()
{
    stringc temp="";
    temp+=ipServer->getText();
    return temp;
}

void TKMenuManager::startButtonSetEnable(bool enable)
{
    btStartGame->setEnabled(enable);
}

void TKMenuManager::joinButtonSetEnable(bool enable)
{
    btJoinGame->setEnabled(enable);
}
