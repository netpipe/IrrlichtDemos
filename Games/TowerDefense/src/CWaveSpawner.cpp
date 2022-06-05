#include "CWaveSpawner.h"
#include <CEntityManager.h>
#include <CEntity.h>
#include "CStateManager.h"

CWaveSpawner::CWaveSpawner(CEntityManager* manager, irr::core::vector3df startpos)
{
    Manager = manager;
    Timer = 0.f;
    TimeToSpawn = 20.f;
    Wave = 0;
    UnitsToSpawn = 1;
    StartPos = startpos;
    Info = manager->getManager()->getDevice()->getGUIEnvironment()->addStaticText(L"", irr::core::rect<irr::s32>(300,50,500,100), false, true);
}

CWaveSpawner::~CWaveSpawner()
{
    Info->remove();
}

void CWaveSpawner::update(float timediff)
{
    Timer += timediff;

    irr::core::stringw t = L"Wave: ";
    t.append(irr::core::stringw(Wave));
    t.append(L"\n");
    t.append(L"Time till next Wave: ");
    t.append(irr::core::stringw((int)(TimeToSpawn-Timer)));
    t.append(L"\n");
    Info->setText(t.c_str());

    if (Timer >= TimeToSpawn)//Spawnwave
    {
        t = L"Wave: ";
        t.append(irr::core::stringw(Wave));
        t.append(L"\n");
        Info->setText(t.c_str());

        CEntity* enemyTest = Manager->createEntity("Enemy");
        enemyTest->getPropertyValue<irr::core::stringc>("Texture") = "textures/tank.png";
        enemyTest->getPropertyValue<irr::core::vector3df>("Position") = StartPos;
        enemyTest->getPropertyValue<irr::f32>("Speed") = tile_size;
        enemyTest->getPropertyValue<bool>("Alive") = true;
        enemyTest->getPropertyValue<int>("Health") = 2+Wave;
        enemyTest->getPropertyValue<E_ENTITY_TYPE>("EntityType") = EET_ENEMY;
        enemyTest->addComponent("DataSheet");
        enemyTest->addComponent("EnemyBehaviour");
        enemyTest->addComponent("SimpleGraphic");
        enemyTest->addComponent("EnemyMovement");
        UnitsToSpawn--;
        if (UnitsToSpawn > 0)
            TimeToSpawn += 1.f;
        else
        {
            Wave++;
            Timer = 0.f;
            TimeToSpawn = 10.f;
            UnitsToSpawn = Wave+1;
        }
    }
}
