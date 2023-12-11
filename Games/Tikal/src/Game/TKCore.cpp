#include "TKCore.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKCore::TKCore()
{
    device = createDevice(EDT_OPENGL, dimension2du(1024, 700), 32, false, false, true, 0);
    device->setWindowCaption(L"Tikal");

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    obmgr = new ObjectsManager(device);
    obmgr->loadAllResources();

    playerName="SERVER";///TODO: load player name from DB

    characterManager = new TKCharacterManager(smgr,this,playerName);

    tkEventReceiver=new TKEventReceiver(this);
    device->setEventReceiver(tkEventReceiver);

    device->getCursorControl()->setVisible(false);
    mouseTexture = driver->getTexture("../media/Art/mouseNormal.png");

    ///Fonts
    hud_Font8=guienv->getFont("../media/Art/Fonts/font8.png");
    hud_Font10=guienv->getFont("../media/Art/Fonts/font10.png");
    hud_Font12=guienv->getFont("../media/Art/Fonts/font12.png");
    hud_Font16_50=guienv->getFont("../media/Art/Fonts/font16_50.png");
    guienv->getSkin()->setFont(hud_Font12);

    ///HUD
    hud_Left = driver->getTexture("../media/Art/hud_Left.png");
    hud_Middle = driver->getTexture("../media/Art/hud_Middle.png");
    hud_Right = driver->getTexture("../media/Art/hud_Right.png");

    hud_Food54 = driver->getTexture("../media/Art/Milho54.png");
    hud_Wood54 = driver->getTexture("../media/Art/Madeira54.png");
    hud_Stone54 = driver->getTexture("../media/Art/Pedra54.png");
    hud_Warrior54 = driver->getTexture("../media/Art/Warrior54.png");
    hud_Villager54 = driver->getTexture("../media/Art/Villager54.png");
    hud_Militar54 = driver->getTexture("../media/Art/Militar54.png");
    hud_House54 = driver->getTexture("../media/Art/House54.png");
    hud_Temple54 = driver->getTexture("../media/Art/Temple54.png");

    hud_Food34 = driver->getTexture("../media/Art/Food34.png");
    hud_Wood34 = driver->getTexture("../media/Art/Madeira34.png");
    hud_Stone34 = driver->getTexture("../media/Art/Stone34.png");


    hud_LabelSelectedItem = guienv->addStaticText(L"",rect<s32>(//126,18, 277,63
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 126,
        device->getVideoDriver()->getScreenSize().Height - 192 + 10,
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 277,
        device->getVideoDriver()->getScreenSize().Height - 192 + 61));
    hud_LabelSelectedItem->setOverrideFont(hud_Font10);

    hud_SelectedLife = guienv->addStaticText(L"10000",myRect(
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 18,
        device->getVideoDriver()->getScreenSize().Height - 192 + 128,
        100,
        30));
    hud_SelectedLife->setOverrideFont(hud_Font10);


    hud_LabelFood=guienv->addStaticText(L"",rect<s32>(//65, 35
        5+65,
        device->getVideoDriver()->getScreenSize().Height - 192 + 28,
        5+178,
        device->getVideoDriver()->getScreenSize().Height - 192 + 60));

    hud_LabelFood->setOverrideFont(hud_Font16_50);

    hud_LabelWood=guienv->addStaticText(L"",rect<s32>(//65, 35
        5+65,
        device->getVideoDriver()->getScreenSize().Height - 192 + 76,
        5+178,
        device->getVideoDriver()->getScreenSize().Height - 192 + 108));

    hud_LabelWood->setOverrideFont(hud_Font16_50);

    hud_LabelStone=guienv->addStaticText(L"",rect<s32>(
        5+65,
        device->getVideoDriver()->getScreenSize().Height - 192 + 126,
        5+178,
        device->getVideoDriver()->getScreenSize().Height - 192 + 158));

    hud_LabelStone->setOverrideFont(hud_Font16_50);

    hud_LabelPopulationLimit=guienv->addStaticText(L"",rect<s32>(
        5+65+140,
        device->getVideoDriver()->getScreenSize().Height - 192 + 126,
        5+178+140,
        device->getVideoDriver()->getScreenSize().Height - 192 + 158));

    hud_LabelPopulationLimit->setOverrideFont(hud_Font16_50);

    IGUIButton* temp;

    temp=guienv->addButton(myRect(
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 125,
        device->getVideoDriver()->getScreenSize().Height - 192 + 105,30,30),0,101);
    temp->setUseAlphaChannel(true);
    hud_bt.push_back(temp);

    temp=guienv->addButton(myRect(
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 157,
        device->getVideoDriver()->getScreenSize().Height - 192 + 105,30,30),0,102);
    temp->setUseAlphaChannel(true);
    hud_bt.push_back(temp);

    temp=guienv->addButton(myRect(
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 189,
        device->getVideoDriver()->getScreenSize().Height - 192 + 105,30,30),0,103);
    temp->setUseAlphaChannel(true);
    hud_bt.push_back(temp);

    hud_btImg_build_house=driver->getTexture("../media/Art/btBuildHouse.png");
    hud_btImg_build_militar=driver->getTexture("../media/Art/btBuildMilitar.png");
    hud_btImg_create_villager=driver->getTexture("../media/Art/btCreateVillager.png");
    hud_btImg_create_warrior=driver->getTexture("../media/Art/btCreateWarrior.png");


    isLMBDown=false;
    p1Selection3D=vector3df(0,0,0);
    p2Selection3D=vector3df(0,0,0);

    buildingManager=new TKBuildingManager(smgr,playerName,this);

    appState="MAIN_MENU";

    menuMgr=new TKMenuManager(guienv,device);

    hud_Middle_setVisible(false);

    plantingBuilding=NULL;

    ///TKNETWORK-_________________________
    tkNetwork = new TKNetwork();
    tkNetwork->setTKCore(this);

    buildingManager->setTKNetwork(tkNetwork);

    this->loadLevel("MAP0");///TODO:Mudar para quando iniciar a partida

    srand(time(0));
}

TKCore::~TKCore()
{
    obmgr->clearWorld();
    delete obmgr;
    device->drop();
    delete camera;
}

void TKCore::drawHud()
{
    if(isLMBDown)//Draw selection rectangle
    {
        p2Selection3D=getMousePosition3D();
        p2Selection2D=device->getCursorControl()->getPosition();
        //characterManager->selectCharacters(p1Selection3D,p2Selection3D);

        position2di upLeft=p1Selection2D;
        position2di downRight=p2Selection2D;

        if(downRight.X < upLeft.X)
        {
            s32 temp=upLeft.X;
            upLeft.X = downRight.X;
            downRight.X = temp;
        }
        if(downRight.Y < upLeft.Y)
        {
            s32 temp=upLeft.Y;
            upLeft.Y = downRight.Y;
            downRight.Y = temp;
        }

        driver->draw2DRectangle(SColor(70,100,100,100),rect<s32>(upLeft,downRight),0);
    }//End draw selection rectangle

    driver->draw2DImage(hud_Left, position2d<s32>(5,device->getVideoDriver()->getScreenSize().Height - 192 -5),
        rect<s32>(0,0,300,192),
        0,
        SColor(255,255,255,255),
        true);



    driver->draw2DImage(hud_Right, position2d<s32>(device->getVideoDriver()->getScreenSize().Width - 305,device->getVideoDriver()->getScreenSize().Height - 192 -5),
        rect<s32>(0,0,300,192),
        0,
        SColor(255,255,255,255),
        true);

    ///Draw MiniMap
    driver->draw2DImage(obmgr->getMiniMapTexture(), position2d<s32>(device->getVideoDriver()->getScreenSize().Width - 305 + 138 ,device->getVideoDriver()->getScreenSize().Height - 192 -5 + 32 ),//138x32
        rect<s32>(0,0,128,128),
        0,
        SColor(255,255,255,255),
        true);

    driver->draw2DImage(characterManager->getMiniMapTexture(), position2d<s32>(device->getVideoDriver()->getScreenSize().Width - 305 + 138 ,device->getVideoDriver()->getScreenSize().Height - 192 -5 + 32 ),//138x32
        rect<s32>(0,0,128,128),
        0,
        SColor(255,255,255,255),
        true);


    ///Resources34
        driver->draw2DImage(hud_Wood34, position2d<s32>(5+23,device->getVideoDriver()->getScreenSize().Height - 192 + 74),
            rect<s32>(0,0,34,32),
            0,
            SColor(255,255,255,255),
            true);
        driver->draw2DImage(hud_Food34, position2d<s32>(5+23,device->getVideoDriver()->getScreenSize().Height - 192 + 24),
            rect<s32>(0,0,34,32),
            0,
            SColor(255,255,255,255),
            true);
        driver->draw2DImage(hud_Stone34, position2d<s32>(5+23,device->getVideoDriver()->getScreenSize().Height - 192 + 124),
            rect<s32>(0,0,34,32),
            0,
            SColor(255,255,255,255),
            true);


    ///draw hud_Middle images
    if(hud_Middle_isVisible)
    {
        ///hud_Middle background
        driver->draw2DImage(hud_Middle, position2d<s32>(
            device->getVideoDriver()->getScreenSize().Width/2 - 150,
            device->getVideoDriver()->getScreenSize().Height - 192 -5),
            rect<s32>(0,0,300,192),
            0,
            SColor(255,255,255,255),
            true);

        drawHUDSelectedItem();
    }

    hud_LabelFood->setText(toStringw(playerFood).c_str());
    hud_LabelStone->setText(toStringw(playerStone).c_str());
    hud_LabelWood->setText(toStringw(playerWood).c_str());
}

void TKCore::drawHUDSelectedItem()
{
    driver->draw2DImage(hud_Selected_Item_Image54, position2d<s32>(
        device->getVideoDriver()->getScreenSize().Width/2 - 150 + 19,
        device->getVideoDriver()->getScreenSize().Height - 192 +9 ),
        rect<s32>(0,0,54,52),
        0,
        SColor(255,255,255,255),
        true );
        //20,15
}

void TKCore::drawMousePointer()
{
    //Draw Mouse
    driver->draw2DImage(mouseTexture, device->getCursorControl()->getPosition(),
        rect<s32>(0,0,64,64),
        0,
        SColor(255,255,255,255),
        true );
}

void TKCore::eventKeyPressed(int key)
{
    if(appState=="GAMEPLAY")
    {
        switch(key)
        {
            case KEY_LEFT:
                camera->setCameraDirection(cameraAxisX,-1);
                break;
            case KEY_RIGHT:
                camera->setCameraDirection(cameraAxisX,1);
                break;
            case KEY_UP:
                camera->setCameraDirection(cameraAxisZ,1);
                break;
            case KEY_DOWN:
                camera->setCameraDirection(cameraAxisZ,-1);
                break;
            case KEY_ESCAPE:
                device->closeDevice();
            default:
                break;
        }
    }
    if(appState=="MAIN_MENU")
    {
        switch(key)
        {
            case KEY_ESCAPE:
                device->closeDevice();
                break;
            default:
                break;
        }
    }
}

void TKCore::eventKeyReleased(int key)
{
    if(appState=="GAMEPLAY")
    {
        switch(key)
        {
            case KEY_LEFT:
                camera->setCameraDirection(cameraAxisX,0);
                break;
            case KEY_RIGHT:
                camera->setCameraDirection(cameraAxisX,0);
                break;
            case KEY_UP:
                camera->setCameraDirection(cameraAxisZ,0);
                break;
            case KEY_DOWN:
                camera->setCameraDirection(cameraAxisZ,0);
                break;
            default:
                break;
        }
    }
}

void TKCore::eventMouseClick(int mouse)
{
    TKCharacter* tempCharacter;

    if(appState=="GAMEPLAY")
    {
        switch(mouse)
        {
        case 0://click LMB
            //Ignorar a area do HUD quando selecionar os jogadores
            if(device->getCursorControl()->getPosition().Y < driver->getScreenSize().Height - 192)
            {
                if(plantingBuilding!=NULL)
                {
                    plantingBuilding->setSolid(true);

                    stringc msg="NEW ";
                    msg+=plantingBuilding->getInfo()->getImage();
                    msg+=" ";
                    msg+=plantingBuilding->getID();
                    msg+=" ";
                    msg+=playerName;
                    msg+=" ";
                    msg+=plantingBuilding->getPosition().X;
                    msg+=" ";
                    msg+=plantingBuilding->getPosition().Y;
                    msg+=" ";
                    msg+=plantingBuilding->getPosition().Z;

                    tkNetwork->bufferMessage(msg.c_str());

                    if(DEBUG_NETWORK) cout << msg.c_str() << endl;

                    plantingBuilding=NULL;
                }
                else
                {
                    hud_Middle_setVisible(false);
                    buildingManager->setSelected(NULL);//clear building selection
                    characterManager->selectNone();
                    hud_Middle_setVisible(false);

                    TKBuilding* tempBuild=buildingManager->selectBuilding(getMousePosition3D());
                    if(tempBuild != NULL)
                    {
                        if(tempBuild->getInfo()->getIntLife() > 0)
                        {
                            buildingManager->setSelected(tempBuild);
                            hud_Middle_setVisible(true);
                            hud_Middle_SelectedItemPosition=tempBuild->getPosition();
                            hud_Middle_SelectedItemInfo=tempBuild->getInfo();
                            hud_MIddle_setSelectedItem(tempBuild->getInfo());
                        }
                    }
                    else
                    {
                        isLMBDown=true;
                        p1Selection2D=device->getCursorControl()->getPosition();
                        p1Selection3D=getMousePosition3D();
                    }
                }
            }

            break;
        case 1://click RMB
            characterManager->moveSelectedTo(getMousePosition3D());
            ///Verificar se é pra pegar recursos/ lutar etc
            ///Ou entao verificar isso dentro do char,
            /// >> se esta perto de recurso coleta,
            /// >> se esta perto de inimigo ataca
            break;
        case 3://release LMB
            tempCharacter=characterManager->selectCharacters(p1Selection3D,p2Selection3D);
            if(tempCharacter!=NULL)
            {
                hud_Middle_setVisible(true);
                hud_MIddle_setSelectedItem(tempCharacter->getInfo());
            }

            isLMBDown=false;
            break;
        case 6://move mouse
            break;
        }
    }
}

void TKCore::eventCollectResource(stringc player, stringc resource)
{
    if(resource=="FOOD") playerFood+=0.1;
    if(resource=="STONE") playerStone+=0.1;
    if(resource=="WOOD") playerWood+=0.1;
}

void TKCore::eventGui(int id)
{
    TKCharacter* tempCharacter;

    switch(id)
    {
        case 1://Criar Partida
            menuMgr->hideAllMenus();
            menuMgr->setPlayerName("Andres");
            menuMgr->setIP("192.168.244.200");
            menuMgr->showCreateGameMenu();
            tkNetwork->createServer(1234);
            playerName="SERVER";
            break;
        case 2://join
            menuMgr->hideAllMenus();
            menuMgr->showJoinGameMenu();
            break;
        case 3://OPcoes
            menuMgr->hideAllMenus();
            menuMgr->showOptionsMenu();
            break;
        case 4://Exit
            device->closeDevice();
            break;
        case 5://Voltar menu principal
            menuMgr->hideAllMenus();
            menuMgr->showMainMenu();
            break;
        case 6://Salvar OPcoes
            break;
        case 7://Iniciar Partida
            tkNetwork->bufferMessage("STARTGAME");
            menuMgr->hideAllMenus();///TODO:Loading Screen
            //this->loadLevel("MAP0");///TODO:getCenario no menuMgr, sincronizar inicio da partida
            appState="GAMEPLAY";
            showHUD();
            break;
        case 8://Join Game
            tkNetwork->createClient();
            tkNetwork->connectToServer(menuMgr->getServerIP().c_str(),1234);
            tkNetwork->bufferMessage("CONNECT CLIENT");
            playerName="CLIENT";
            menuMgr->joinButtonSetEnable(false);
            break;
        case 101:///hud_bt_1
            if(hud_Middle_SelectedItemInfo->getImage() == "TEMPLE")///TODO: substituir por type, colocar type no hud_info
            {
                if(playerFood>=200 && playerPopulation<playerPopulationLimit)
                {
                    tempCharacter=characterManager->createCharacter("VILLAGER",hud_Middle_SelectedItemPosition-vector3df(0,0,10),playerName);
                    tempCharacter->moveTo(hud_Middle_SelectedItemPosition-vector3df(rand()%30 - 15,0,20));
                    playerFood-=200;
                    playerPopulation++;
                    hud_updateLabelPopulation();

                    stringc msg="NEW CHARACTER ";
                    msg+=tempCharacter->getID();
                    msg+=" ";
                    msg+=playerName;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().X;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().Y;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().Z;

                    tkNetwork->bufferMessage(msg.c_str());

                    if(DEBUG_NETWORK) cout << msg.c_str() << endl;
                }
                else
                {
                    guienv->addMessageBox(L"Recursos insuficientes",L"Voce nao possui recursos suficientes: 200 de comida");
                }
            }
            if(hud_Middle_SelectedItemInfo->getImage() == "MILITAR")
            {
                if(playerFood>= 200 && playerWood >= 50 && playerStone >= 50 && playerPopulation<playerPopulationLimit)
                {
                    tempCharacter=characterManager->createCharacter("WARRIOR",hud_Middle_SelectedItemPosition-vector3df(0,0,6),playerName);
                    tempCharacter->moveTo(hud_Middle_SelectedItemPosition-vector3df(rand()%30 - 15,0,20));
                    playerFood-=200;
                    playerWood-=50;
                    playerStone-=50;
                    playerPopulation++;
                    hud_updateLabelPopulation();

                    stringc msg="NEW WARRIOR ";
                    msg+=tempCharacter->getID();
                    msg+=" ";
                    msg+=playerName;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().X;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().Y;
                    msg+=" ";
                    msg+=tempCharacter->getPosition().Z;

                    tkNetwork->bufferMessage(msg.c_str());

                    if(DEBUG_NETWORK) cout << msg.c_str() << endl;
                }
                else
                {
                    guienv->addMessageBox(L"Recursos insuficientes",L"Voce nao possui recursos suficientes: 200 de comida\n50 de pedra\n50 de madeira");
                }
            }
            if(hud_Middle_SelectedItemInfo->getImage() == "VILLAGER")///Build House
            {
                if(playerWood>=200)
                {
                    plantingBuilding=buildingManager->createHouse(vector3df(0,0,0));
                    characterManager->updateCollisions(plantingBuilding->getTriangleSelector());
                    plantingBuilding->setSolid(false);
                    playerWood-=200;
                    playerPopulationLimit+=3;
                    hud_updateLabelPopulation();
                }
                else
                {
                    guienv->addMessageBox(L"Recursos insuficientes",L"Voce nao possui recursos suficientes: 200 de madeira");
                }
            }
            break;
        case 102:
            if(hud_Middle_SelectedItemInfo->getImage() == "VILLAGER")///Build Military Camp
            {
                if(playerStone>=500 && playerWood>=200)
                {
                    plantingBuilding=buildingManager->createMilitaryCamp(vector3df(0,0,0));
                    characterManager->updateCollisions(plantingBuilding->getTriangleSelector());
                    plantingBuilding->setSolid(false);
                    playerStone-=500;
                    playerWood-=200;
                }
                else
                {
                    guienv->addMessageBox(L"Recursos insuficientes",L"Voce nao possui recursos suficientes: 500 de pedra\n200 de madeira");
                }
            }
            break;
        case 103:
            break;
    }
}

void TKCore::eventNetworkReceive(stringc msg)
{
    if(DEBUG_NETWORK) cout << "NETWORK MESSAGE:" << msg.c_str() << endl;

    if(getStr(msg.c_str(),0) == "CONNECT")
    {
        enemyName=getStr(msg.c_str(),1).c_str();
        menuMgr->setReadyToStartGame(true);
    }

    if(getStr(msg.c_str(),0) == "STARTGAME" && appState!="GAMEPLAY")
    {
        menuMgr->hideAllMenus();///TODO:Loading Screen
        appState="GAMEPLAY";
        showHUD();
    }

    if(getStr(msg.c_str(),0) == "NEW" && appState=="GAMEPLAY")
    {
        if(getStr(msg.c_str(),1) == "CHARACTER")
        {
            stringc id=getStr(msg.c_str(),2).c_str();
            stringc tplayer=getStr(msg.c_str(),3).c_str();

            if(tplayer==playerName) return;

            double x=atof(getStr(msg.c_str(),4).c_str());
            double y=atof(getStr(msg.c_str(),5).c_str());
            double z=atof(getStr(msg.c_str(),6).c_str());

            TKCharacter* tempCharacter=characterManager->createCharacter("VILLAGER",vector3df(x,y,z),tplayer);
            tempCharacter->setID(id);
        }

        if(getStr(msg.c_str(),1) == "WARRIOR")
        {
            stringc id=getStr(msg.c_str(),2).c_str();
            stringc tplayer=getStr(msg.c_str(),3).c_str();

            if(tplayer==playerName) return;

            double x=atof(getStr(msg.c_str(),4).c_str());
            double y=atof(getStr(msg.c_str(),5).c_str());
            double z=atof(getStr(msg.c_str(),6).c_str());

            TKCharacter* tempCharacter=characterManager->createCharacter("WARRIOR",vector3df(x,y,z),tplayer);
            tempCharacter->setID(id);
        }

        if(getStr(msg.c_str(),1) == "HOUSE")
        {
            stringc id=getStr(msg.c_str(),2).c_str();
            stringc tplayer=getStr(msg.c_str(),3).c_str();

            if(tplayer==playerName) return;

            double x=atof(getStr(msg.c_str(),4).c_str());
            double y=atof(getStr(msg.c_str(),5).c_str());
            double z=atof(getStr(msg.c_str(),6).c_str());

            TKBuilding* tempBuilding=buildingManager->createHouse(vector3df(x,y,z));
            characterManager->updateCollisions(tempBuilding->getTriangleSelector());
            tempBuilding->setSolid(true);
            tempBuilding->setID(id);
            tempBuilding->setPlayer(tplayer);
        }

        if(getStr(msg.c_str(),1) == "MILITAR")
        {
            stringc id=getStr(msg.c_str(),2).c_str();
            stringc tplayer=getStr(msg.c_str(),3).c_str();

            if(tplayer==playerName) return;

            double x=atof(getStr(msg.c_str(),4).c_str());
            double y=atof(getStr(msg.c_str(),5).c_str());
            double z=atof(getStr(msg.c_str(),6).c_str());

            TKBuilding* tempBuilding=buildingManager->createMilitaryCamp(vector3df(x,y,z));
            characterManager->updateCollisions(tempBuilding->getTriangleSelector());
            tempBuilding->setSolid(true);
            tempBuilding->setID(id);
            tempBuilding->setPlayer(tplayer);
        }
    }

    if(getStr(msg.c_str(),0) == "DECREASE_LIFE" && appState=="GAMEPLAY")
    {
        characterManager->decreaseLife(getStr(msg.c_str(),1).c_str());//charMgr invoca decreaseLife do char segundo a ID
    }

    if(getStr(msg.c_str(),0) == "DECREASE_LIFE_BUILDING" && appState=="GAMEPLAY")
    {
        buildingManager->decreaseLife(getStr(msg.c_str(),1).c_str());
    }

    if(getStr(msg.c_str(),0) == "CHARACTER_MOVE" && appState=="GAMEPLAY")
    {
        double x = atof(getStr(msg.c_str(),2).c_str());
        double y = atof(getStr(msg.c_str(),3).c_str());
        double z = atof(getStr(msg.c_str(),4).c_str());
        double rotation = atof(getStr(msg.c_str(),5).c_str());
        stringc action=getStr(msg.c_str(),6).c_str();

        characterManager->moveByIDTo(getStr(msg.c_str(),1).c_str(),vector3df(x,y,z),rotation,action);
    }

    if(getStr(msg.c_str(),0) == "CHARACTER_ACTION" && appState=="GAMEPLAY")
    {
        double rotation = atof(getStr(msg.c_str(),2).c_str());
        stringc action=getStr(msg.c_str(),3).c_str();

        characterManager->setActionByID(getStr(msg.c_str(),1).c_str(),rotation,action);
    }

    if(getStr(msg.c_str(),0) == "KILL" && appState=="GAMEPLAY")
    {
        if(getStr(msg.c_str(),1) == "CHARACTER")
        {
            characterManager->bufferCharacterToKill(getStr(msg.c_str(),2).c_str());
        }
    }

    if(getStr(msg.c_str(),0) == "END" && appState=="GAMEPLAY")
    {
        if(getStr(msg.c_str(),1) == playerName.c_str())
        {
            guienv->addMessageBox(L"FIM DE JOGO",L"Voce foi derrotado!");
        }
        else
        {
            guienv->addMessageBox(L"FIM DE JOGO",L"Parabens, voce venceu!");
        }

        hideHUD();
        appState="MAIN_MENU";
        menuMgr->showMainMenu();
    }
}

//return the real pointer position at the scenario.. go to hell english!!
vector3df TKCore::getMousePosition3D()
{
    position2d<s32> pos=device->getCursorControl()->getPosition();
    line3df line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(pos, smgr->getActiveCamera());

    core::vector3df intersection;
    core::triangle3df tri;
    ITriangleSelector* selector = obmgr->getTerrainTriangleSelector();
 ISceneNode *outnode;

    if (smgr->getSceneCollisionManager()->getCollisionPoint(line, selector, intersection, tri,outnode))
    {
        return intersection;
    }
    else
    {
        return vector3df(0,10,0);//nunca vai acontecer porque nunca sera possivel clicar fora da terrain!
    }
}


void TKCore::loadLevel(stringc levelName)
{
    ///StartUP Objects! TODO: load level from xml

    obmgr->createSky();

    stringc realLevelName="../media/Terrain/";
    realLevelName+=levelName;
    realLevelName+=".png";

    obmgr->createTerrain(realLevelName);
    obmgr->createOcean  (realLevelName);

    camera=new TKCamera(smgr);
    camera->setLimits(50,200,200,10);
    if(playerName=="SERVER")
        camera->setPosition(vector3df(60,0,22));
    else
        camera->setPosition(vector3df(60,0,190));



    TKBuilding* mil1=buildingManager->createMilitaryCamp(vector3df(-1000,0.5,-1000));
    mil1->setPlayer("GAIA");

    TKBuilding* house1=buildingManager->createHouse(vector3df(-1000,0.5,-1000));
    house1->setPlayer("GAIA");


    TKBuilding* templo=buildingManager->createTemple(vector3df(60,0.5,30));
    ITriangleSelector* selectorTemplo=templo->getTriangleSelector();
    characterManager->updateCollisions(selectorTemplo);
    templo->setPlayer("SERVER");

    TKBuilding* temploClient=buildingManager->createTemple(vector3df(60,0.5,200));
    ITriangleSelector* selectorTemploClient=temploClient->getTriangleSelector();
    characterManager->updateCollisions(selectorTemploClient);
    temploClient->setPlayer("CLIENT");


    vector<ITriangleSelector*> obmgrSelectors=obmgr->getTriangleSelectors();
    characterManager->updateCollisions(obmgrSelectors);

    ///Update resources
    vector<vector3df> resPos=obmgr->getResourcesPos();
    vector<stringc> resType=obmgr->getResourcesType();

    characterManager->setResources(resPos,resType);

    ///Define start resources
    playerFood=6000;
    playerStone=2000;
    playerWood=2000;
    playerPopulationLimit=50;
    playerPopulation=0;

    characterManager->createCharacter("VILLAGER",vector3df(-10000,0,-10000),"GAIA");
    characterManager->createCharacter("WARRIOR",vector3df(-10000,0,-10000),"GAIA");

    characterManager->createCharacter("WARRIOR",vector3df(80,0.5,60),"SERVER");
    TKCharacter* enemyChar=characterManager->createCharacter("WARRIOR",vector3df(70,0.5,60),"CLIENT");
    enemyChar->setID("1234");
}

void TKCore::runGame()
{
    this->hideHUD();
    menuMgr->loadMenu();
    menuMgr->hideAllMenus();
    menuMgr->showMainMenu();
    //menuMgr->setReadyToStartGame(false);


    /*menuMgr->loadMenu();
    menuMgr->hideAllMenus();///TODO:Loading Screen
    this->loadLevel("MAP0");///TODO:getCenario no menuMgr, sincronizar inicio da partida
    appState="GAMEPLAY";
    showHUD();
    tkNetwork->createServer(1234);*/

    while(device->run())
    {
        driver->beginScene(true, true, SColor(0,255,255,255));

        if(appState=="GAMEPLAY")
        {
            characterManager->killCharactersBuffer();
            characterManager->setNearestEnemyBuilding(buildingManager->getEnemyBuildingsPos(), buildingManager->getEnemyBuildingsIDs());

            smgr->drawAll();

            this->drawHud();

            camera->update();
            obmgr->update();
            characterManager->updateAll(tkNetwork,buildingManager);

            if(plantingBuilding!=NULL) plantBuilding(plantingBuilding);
        }
        else
        if(appState=="MAIN_MENU")
        {
            menuMgr->updateMenus();
        }

        guienv->drawAll();
        this->drawMousePointer();

        driver->endScene();
    }
}

rect<s32> TKCore::myRect(s32 x,s32 y, s32 w, s32 h)
{
    return rect<s32>(x,y,x+w,y+h);
}

void TKCore::showHUD()
{
    hud_LabelFood->setVisible(true);
    hud_LabelWood->setVisible(true);
    hud_LabelStone->setVisible(true);
    //hud_LabelSelectedItem->setVisible(true);
    //hud_SelectedLife->setVisible(true);
}

void TKCore::hideHUD()
{
    hud_LabelSelectedItem->setVisible(false);
    hud_LabelFood->setVisible(false);
    hud_LabelWood->setVisible(false);
    hud_LabelStone->setVisible(false);
    hud_SelectedLife->setVisible(false);
}

void TKCore::setHudBt(int bt,stringc type)
{
    IGUIButton* temp=hud_bt[bt];

    if(type=="BUILD_HOUSE")
    {
        temp->setVisible(true);
        temp->setImage(hud_btImg_build_house);
        temp->setToolTipText(L"Construir casa");
    }
    else if(type=="BUILD_MILITAR")
    {
        temp->setVisible(true);
        temp->setImage(hud_btImg_build_militar);
        temp->setToolTipText(L"Construir edificação militar");
    }
    else if(type=="CREATE_WARRIOR")
    {
        temp->setVisible(true);
        temp->setImage(hud_btImg_create_warrior);
        temp->setToolTipText(L"Criar um guerreiro");
    }
    else if(type=="CREATE_VILLAGER")
    {
        temp->setVisible(true);
        temp->setImage(hud_btImg_create_villager);
        temp->setToolTipText(L"Criar um aldeão");
    }
    else if(type=="")
    {
        temp->setVisible(false);
    }
}

void TKCore::hud_Middle_setVisible(bool visible)
{
    hud_Middle_isVisible=visible;
    hud_LabelSelectedItem->setVisible(visible);
    hud_SelectedLife->setVisible(visible);

    for(int i=0;i<hud_bt.size();i++)
        hud_bt[i]->setVisible(visible);
}

void TKCore::hud_MIddle_setSelectedItem(TKHudInfo* info)
{
    hud_SelectedLife->setText( info->getLife().c_str() );
    hud_LabelSelectedItem->setText( info->getLabel().c_str() );

    for(int i=0;i<hud_bt.size();i++) setHudBt(i,"");//clear all buttons

    for(int i=0;i<info->getBtSize();i++) setHudBt(i,info->getBtAction(i));//set Correct buttons

    hud_Middle_SelectedItemInfo = info;

    setHudImg54(info->getImage());
}

void TKCore::setHudImg54(stringc img)
{
    if(img=="HOUSE")
    {
        hud_Selected_Item_Image54=hud_House54;
    }
    else if(img=="MILITAR")
    {
        hud_Selected_Item_Image54=hud_Militar54;
    }
    else if(img=="TEMPLE")
    {
        hud_Selected_Item_Image54=hud_Temple54;
    }
    else if(img=="WARRIOR")
    {
        hud_Selected_Item_Image54=hud_Warrior54;
    }
    else if(img=="VILLAGER")
    {
        hud_Selected_Item_Image54=hud_Villager54;
    }
}

stringw TKCore::toStringw(stringc str)
{
    stringw temp="";
    temp+=str;
    return temp;
}

stringw TKCore::toStringw(int i)
{
    stringw temp="";
    temp+=i;
    return temp;
}

void TKCore::plantBuilding(TKBuilding* building)
{
    building->setPosition(getMousePosition3D());
}

void TKCore::hud_updateLabelPopulation()
{
    stringw temp=L"";
    temp+=playerPopulation;
    temp+="/";
    temp+=playerPopulationLimit;
    hud_LabelPopulationLimit->setText(temp.c_str());
}

std::string TKCore::getStr(std::string str, int index)
{
	std::string temp="";
	for (int i=0; i<=index;i++)
	{
		temp=str.substr(0,str.find(" "));
		str=str.substr(str.find(" ")+1,str.length()-str.find(" "));
	}
	return temp;
}
