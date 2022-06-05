#include "CGame.h"
#include "CEntityManager.h"
#include "CStateManager.h"
#include "gui_catch.h"
#include "CSprite.h"
#include "CTile.h"
#include "CPlayer.h"

//components
#include "CEnemyBehaviour.h"
#include "CTurretBehaviour.h"
#include "CEnemyBehaviourFly.h"
#include "CSimpleGraphic.h"
#include "CEnemyMovement.h"
#include "CHealthComponent.h"

#include "CWaveSpawner.h"

CGame::CGame(CStateManager* manager) : CState(manager), UnitGrid(tile_size)
{
    //setup Camera
    irr::scene::ICameraSceneNode* camera = Manager->getDevice()->getSceneManager()->getActiveCamera();
    if (camera)
        camera->setPosition(irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f, -10));
    else
        camera = Manager->getDevice()->getSceneManager()->addCameraSceneNode(NULL, irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f,-20), irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f, 0.f), -1);
    irr::core::matrix4 mat;
    mat.buildProjectionMatrixOrthoLH(game_width*tile_size, game_height*tile_size, 0, 1000);
    camera->setProjectionMatrix(mat);

    Map.init(game_width, game_height, manager->getDevice()->getSceneManager());
    Map.loadMap("maps/autosave.map", Manager);

    EntityManager = new CEntityManager(Manager);

    //register components
    EntityManager->registerComponent(new CEnemyBehaviourFactory(&Map));
    EntityManager->registerComponent(new CEnemyBehaviourFlyFactory(&Map));
    EntityManager->registerComponent(new CSimpleComponentFactory<CSimpleGraphic>("SimpleGraphic"));
    EntityManager->registerComponent(new CSimpleComponentFactory<CHealthComponent>("DataSheet"));
    EntityManager->registerComponent(new CEnemyMovementFactory(&UnitGrid));
    EntityManager->registerComponent(new CTurretBehaviourFactory(&UnitGrid));

    //Enemies
    Spawner = new CWaveSpawner(EntityManager, Map.getStart()->getPos());

    Player = new CPlayer(Manager, EntityManager, &Map, &UnitGrid);
}

CGame::~CGame()
{
    delete EntityManager;
    delete Spawner;
    delete Player;
}

void CGame::update(irr::f32 timediff)
{
    Spawner->update(timediff);

    EntityManager->update(timediff);

    Player->update(timediff);
}

void CGame::key_event(const irr::SEvent::SKeyInput& key)
{
    if (key.Key == irr::KEY_ESCAPE)
    {
        Manager->createState("MainMenu");
        remove();
    }
}

void CGame::becomeMainState(void)
{
    Manager->getInput()->registerInput((IKeyboardReceiver*)this);
    Player->init();
}

void CGame::becomeSlaveState(void)
{
    Manager->getInput()->unregisterInput((IKeyboardReceiver*)this);
    Player->deinit();
}

