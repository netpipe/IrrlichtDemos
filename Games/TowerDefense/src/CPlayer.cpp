#include "CPlayer.h"
#include "CEntityManager.h"
#include "CEntity.h"
#include "CStateManager.h"
#include "gui_catch.h"
#include "CSprite.h"
#include "CTile.h"

CPlayer::CPlayer(CStateManager* manager, CEntityManager* entityManager, CMap* map, CSpatialHash<>* unitGrid)
{
    Manager = manager;
    EntityManager = entityManager;
    SelectedSprite = NULL;
    BluePause = new CSprite(Manager->getDevice()->getSceneManager()->getRootSceneNode(), Manager->getDevice()->getSceneManager(), -1,
                                     irr::core::vector3df(0,0,-1),
                                     irr::core::vector3df(0,0,0),
                                     irr::core::vector3df(tile_size, tile_size, -1.0f));
    BluePause->setTexture("textures/blaupause.png");
    BluePause->setVisible(false);
    RangeIdentifier = NULL;
    BuildMenu = NULL;
    Info = NULL;
    Map = map;
    UnitGrid = unitGrid;
    Tower2Build = ET_NONE;

    Money = 500;
    LifePoints = 20;
    TimePassed = 0;
}

CPlayer::~CPlayer()
{
    BluePause->remove();
    BluePause->drop();
}

void CPlayer::init(void)
{
    Manager->getInput()->registerInput((IMouseReceiver*)this);
    Manager->getInput()->registerInput((IKeyboardReceiver*)this);
    Info = Manager->getDevice()->getGUIEnvironment()->addStaticText(L"", irr::core::rect<irr::s32>(50,50,200,300), false, true);
    if (!SelectedSprite)
    {
        SelectedSprite = new CSprite(Manager->getDevice()->getSceneManager()->getRootSceneNode(), Manager->getDevice()->getSceneManager(), -1,
                                     irr::core::vector3df(0,0,-1),
                                     irr::core::vector3df(0,0,0),
                                     irr::core::vector3df(tile_size, tile_size, 1.0f));
        SelectedSprite->setTexture("textures/selected.png");
    }
    else
        SelectedSprite->setParent(Manager->getDevice()->getSceneManager()->getRootSceneNode());

    //register callbacks
    Manager->registerCallback("EnemyKilled", this, &CPlayer::EnemyKilled);
    Manager->registerCallback("EnemyWin", this, &CPlayer::EnemyWin);
}

void CPlayer::deinit(void)
{
    Manager->getInput()->unregisterInput((IMouseReceiver*)this);
    Manager->getInput()->unregisterInput((IKeyboardReceiver*)this);
    Info->remove();
    if (BuildMenu)
    {
        BuildMenu->remove();
        BuildMenu->drop();
    }
    BuildMenu = NULL;
    if (SelectedSprite)
        SelectedSprite->remove();
}

void CPlayer::key_event(const irr::SEvent::SKeyInput& key)
{
    if (key.Key == irr::KEY_KEY_I)
    {
        if (BuildMenu)
        {
            BuildMenu->remove();
            BuildMenu->drop();
        }
        BuildMenu = new gui_catch(Manager->getDevice()->getGUIEnvironment(), Manager->getDevice()->getGUIEnvironment()->getRootGUIElement());
        BuildMenu->grab();
        Manager->getDevice()->getGUIEnvironment()->addWindow(irr::core::rect<irr::s32>(100,100,500,150), false, L"Build Menu", BuildMenu);
    }
    else if (key.Key == irr::KEY_KEY_1)
    {
        Tower2Build = ET_GROUND_NORMAL;
    }
}

void CPlayer::update(irr::f32 timediff)
{
    GameState = Manager->getActiveState();

    TimePassed += timediff;

    irr::core::stringc data =   "Keys:\n"
                                "Press \'I\' to open the build menu\n"
                                "Escape - Leave GAME\n\n"
                                "Misc:\n";
    data.append("Money: ");
    data.append(irr::core::stringc(Money));
    data.append("\n");
    data.append("Life: ");
    data.append(irr::core::stringc(LifePoints));
    data.append("\n");
    Info->setText(irr::core::stringw(data.c_str()).c_str());

    if (SelectedSprite)
    {
        SelectedSprite->setPosition(irr::core::vector3df(TileUnderMoue.X*tile_size+tile_size/2.f, TileUnderMoue.Y*tile_size+tile_size/2.f, -2.f));
    }
    if (Tower2Build != ET_NONE)
    {
        BluePause->setVisible(true);
        switch (Tower2Build)
        {
            case ET_GROUND_NORMAL:
                SelectedSprite->setTexture("textures/turret.png");
                break;
        }
        BluePause->setPosition(irr::core::vector3df(TileUnderMoue.X*tile_size+tile_size/2.f, TileUnderMoue.Y*tile_size+tile_size/2.f, -3.f));
        if (Map->getTile(TileUnderMoue.X, TileUnderMoue.Y) && (Map->getTile(TileUnderMoue.X, TileUnderMoue.Y)->getType() != ETT_BUILD || (UnitGrid->getData(TileUnderMoue.X, TileUnderMoue.Y) && UnitGrid->getData(TileUnderMoue.X, TileUnderMoue.Y)->getSize() > 0)))
        {
            BluePause->setTexture("textures/rotpause.png");
            CanBuild = false;
        }
        else
        {
            BluePause->setTexture("textures/blaupause.png");
            CanBuild = true;
        }
    }
    else
    {
        BluePause->setVisible(false);
    }
}

void CPlayer::mouse_event(const irr::SEvent::SMouseInput& mouse)
{
    if (mouse.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
    {
        if (SelectedSprite && Tower2Build == ET_NONE)
            SelectedSprite->setTexture("textures/selected_down.png");
        else if (Tower2Build != ET_NONE && CanBuild)
        {
            irr::core::stringc tex;
            switch (Tower2Build)
            {
                case ET_GROUND_NORMAL:
                    if (Money-100 < 0)
                        return;
                    Money -=100;
                    tex = "textures/turret.png";
                    break;
            }
            //build tower
            CEntity* newTower = EntityManager->createEntity("Tower");
            newTower->getPropertyValue<irr::core::stringc>("Texture") = tex;
            newTower->getPropertyValue<irr::core::vector3df>("Position") = irr::core::vector3df(TileUnderMoue.X*tile_size+tile_size/2.f, TileUnderMoue.Y*tile_size+tile_size/2.f, -3.f);
            newTower->getPropertyValue<E_ENTITY_TYPE>("EntityType") = EET_TOWER;
            newTower->getPropertyValue<int>("Damage") = 1;
            newTower->addComponent("TurretBehaviour");
            newTower->addComponent("SimpleGraphic");
        }
    }
    else if (mouse.Event == irr::EMIE_LMOUSE_LEFT_UP)
    {
        if (SelectedSprite && Tower2Build == ET_NONE)
            SelectedSprite->setTexture("textures/selected.png");
    }
    else if (mouse.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
    {
        Tower2Build = ET_NONE;
        if (SelectedSprite)
            SelectedSprite->setTexture("textures/selected.png");
    }

    if (mouse.Event == irr::EMIE_MOUSE_MOVED)
    {
        TileUnderMoue.X = mouse.X/tile_size;
        TileUnderMoue.Y = (game_height*tile_size-mouse.Y)/tile_size;
    }
}

void CPlayer::EnemyKilled(Base_Property* property, CEntity* sender)
{
    Property<int>* p = (Property<int>*)property;
    Money += p->getValue();
}

void CPlayer::EnemyWin(Base_Property* property, CEntity* sender)
{
    Property<int>* p = (Property<int>*)property;
    LifePoints -= p->getValue();
    if (LifePoints == 0 && GameState)
    {
        Manager->createState("MainMenu");
        GameState->remove();
        GameState = NULL;
    }
}
