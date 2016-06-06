#include "powerup.h"

CPowerup::CPowerup(int Type, vector3df Position, int Value, ISceneManager* smgr, IVideoDriver* driver, CPlayer* Player)
{
    CPowerup::Player = Player;
    CPowerup::Type = Type;
    CPowerup::Position = Position;
    CPowerup::Value = Value;
    CPowerup::smgr = smgr;
    CPowerup::driver = driver;

    stringw path = "./Media/Models/Powerups/";

    if(Type == PU_HEALTH)
        path += "health.obj";
    if(Type == PU_A_ATTRACTIVE)
        path += "attractive.obj";
    if(Type == PU_A_EXPLOSIVE)
        path += "explosive.obj";
    if(Type == PU_A_NORMAL)
        path += "normal.obj";

    CPowerup::Node = smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)smgr->getMesh(path.c_str()));
    CPowerup::Node->setPosition(Position);

    ITexture* texture;

    switch(Player->getCurrentWeapon())
    {
        case WT_NORMAL:
            texture = driver->getTexture("./Media/Models/Primitives/Normal.jpg");
            break;

       case WT_ATTRACTIVE:
            texture = driver->getTexture("./Media/Models/Primitives/Attractive.jpg");
            break;

        case WT_EXPLOSIVE:
            texture = driver->getTexture("./Media/Models/Primitives/Explosive.jpg");
            break;
    }
    CPowerup::Node->setMaterialTexture(0, texture);
}

void CPowerup::PickedUp()
{
    switch(CPowerup::Type)
    {
        case PU_HEALTH:
            CPowerup::Player->setHealth(CPowerup::Player->getHealth()+CPowerup::Value);
            break;

        case PU_A_ATTRACTIVE:
            CPowerup::Player->addAmmo(WT_ATTRACTIVE, CPowerup::Value);
            break;

        case PU_A_EXPLOSIVE:
            CPowerup::Player->addAmmo(WT_EXPLOSIVE, CPowerup::Value);
            break;

        case PU_A_NORMAL:
            CPowerup::Player->addAmmo(WT_NORMAL, CPowerup::Value);
            break;
    }
    CPowerup::Node->remove();
}

IAnimatedMeshSceneNode* CPowerup::getNode()
{
    return CPowerup::Node;
}

void CPowerup::Update()
{
    vector3df Rotation = CPowerup::Node->getRotation();
    Rotation.Y += 1.0f;
    CPowerup::Node->setRotation(Rotation);
}
