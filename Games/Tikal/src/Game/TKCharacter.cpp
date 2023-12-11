#include "TKCharacter.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKCharacter::TKCharacter(ISceneManager* mgr, TKCore* ntkCore, stringc type)
{
    smgr=mgr;
    tkCore=ntkCore;

    id="CHARACTER_";
    id+=rand();

    node=smgr->addEmptySceneNode(0,-1);

    if(type == "WARRIOR")
    {///TODO:rename vars from villager.. to character...
        villagerWaitMesh=smgr->getMesh("../media/Character/warriorWait.b3d");
        villagerWaitNode=smgr->addAnimatedMeshSceneNode(villagerWaitMesh,node);
            villagerWaitNode->setVisible(false);
        villagerWalkMesh=smgr->getMesh("../media/Character/warriorWalk.b3d");
        villagerWalkNode=smgr->addAnimatedMeshSceneNode(villagerWalkMesh,node);
            villagerWalkNode->setVisible(false);
            villagerWalkNode->setFrameLoop(1,30);
            villagerWalkNode->setAnimationSpeed(30);

        villagerWoodMesh=smgr->getMesh("../media/Character/warriorAttacking.b3d");
        villagerWoodNode=smgr->addAnimatedMeshSceneNode(villagerWoodMesh,node);
            villagerWoodNode->setVisible(false);
            villagerWoodNode->setAnimationSpeed(30);

        villagerStoneMesh=smgr->getMesh("../media/Character/villagerCuttingWood.b3d");
        villagerStoneNode=smgr->addAnimatedMeshSceneNode(villagerWoodMesh,node);
            villagerStoneNode->setVisible(false);

        villagerFoodMesh=smgr->getMesh("../media/Character/villagerCuttingWood.b3d");
        villagerFoodNode=smgr->addAnimatedMeshSceneNode(villagerWoodMesh,node);
            villagerFoodNode->setVisible(false);

        info=new TKHudInfo();
        info->setLabel("Soldado: Blind pig killer!");
        info->setLife(250);
        info->setImage("WARRIOR");
    }

    if(type == "VILLAGER")
    {
        villagerWaitMesh=smgr->getMesh("../media/Character/villagerWait.b3d");
        villagerWaitNode=smgr->addAnimatedMeshSceneNode(villagerWaitMesh,node);
            villagerWaitNode->setVisible(false);
        villagerWalkMesh=smgr->getMesh("../media/Character/villagerWalk.b3d");
        villagerWalkNode=smgr->addAnimatedMeshSceneNode(villagerWalkMesh,node);
            villagerWalkNode->setVisible(false);
            villagerWalkNode->setFrameLoop(1,30);
            villagerWalkNode->setAnimationSpeed(30);

        villagerWoodMesh=smgr->getMesh("../media/Character/villagerCuttingWood.b3d");
        villagerWoodNode=smgr->addAnimatedMeshSceneNode(villagerWoodMesh,node);
            villagerWoodNode->setVisible(false);
            villagerWoodNode->setFrameLoop(1,32);
            villagerWoodNode->setAnimationSpeed(30);

        villagerStoneMesh=smgr->getMesh("../media/Character/villagerMining.b3d");
        villagerStoneNode=smgr->addAnimatedMeshSceneNode(villagerStoneMesh,node);
            villagerStoneNode->setVisible(false);
            villagerStoneNode->setFrameLoop(1,32);
            villagerStoneNode->setAnimationSpeed(30);

        villagerFoodMesh=smgr->getMesh("../media/Character/villagerCollecting.b3d");
        villagerFoodNode=smgr->addAnimatedMeshSceneNode(villagerFoodMesh,node);
            villagerFoodNode->setVisible(false);
            villagerFoodNode->setFrameLoop(1,20);
            villagerFoodNode->setAnimationSpeed(30);

        info=new TKHudInfo();
        info->setLabel("Aldeao: Construtor e Coletor");
        info->setLife(100);
        info->setImage("VILLAGER");
        info->addBtAction("BUILD_HOUSE");
        info->addBtAction("BUILD_MILITAR");
    }

    selectedCharacterMesh=smgr->getMesh("../media/Character/selectedCharacter.b3d");
    selectedCharacterNode=smgr->addMeshSceneNode(selectedCharacterMesh,node);
    selectedCharacterNode->setMaterialTexture(0,smgr->getVideoDriver()->getTexture("../media/Character/selectedCharacter.png"));
    selectedCharacterNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
    selectedCharacterNode->setVisible(false);
    selectedCharacterNode->setPosition(vector3df(0,0.2,0));

    shadowCharacterMesh=smgr->getMesh("../media/Character/selectedCharacter.b3d");
    shadowCharacterNode=smgr->addMeshSceneNode(shadowCharacterMesh,node);
    shadowCharacterNode->setMaterialTexture(0,smgr->getVideoDriver()->getTexture("../media/Character/characterShadow.png"));
    shadowCharacterNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    shadowCharacterNode->setPosition(vector3df(0,0.1,0));

    node->setTriangleSelector(smgr->createTriangleSelectorFromBoundingBox(villagerWaitNode));

    currentAction = "WAIT";
    villagerWaitNode->setVisible(true);

    moveSpeed = 0.1;///TODO:Load from DB... mover pra especialização!

    selector=smgr->createTriangleSelectorFromBoundingBox(villagerWaitNode);

    nearestResource = vector3df(-1000,-1000,-1000);
    nearestResourceType = "";

    alive=true;

    nearestEnemyBuilding=vector3df(0,0,0);
}

TKCharacter::~TKCharacter()
{
    villagerWaitNode->remove();
    villagerWalkNode->remove();
    villagerWoodNode->remove();
    villagerStoneNode->remove();
    villagerFoodNode->remove();
    selectedCharacterNode->remove();
    shadowCharacterNode->remove();
}

void TKCharacter::setDead()
{
    alive=false;
    hideAllNodes();
}

TKHudInfo* TKCharacter::getInfo()
{
    return info;
}

stringc TKCharacter::getPlayer()
{
    return player;
}

double TKCharacter::grad2rad(double grad)
{
   return (grad*PI/180);
}

bool TKCharacter::isSelected()
{
    return selected;
}

void TKCharacter::moveTo(vector3df pos)
{
    currentAction = "WALK";

    hideAllNodes();
    villagerWalkNode->setVisible(true);///TODO:verificar se é villager ou warrior

    moveToPos=pos;
}

void TKCharacter::setNearestResource(vector3df resPos, stringc resType)
{
    nearestResource = resPos;
    nearestResourceType = resType;
}

void TKCharacter::setNearestEnemyBuilding(vector3df buildPos)
{
    nearestEnemyBuilding = buildPos;
}

void TKCharacter::setPlayer(stringc newPlayer)
{
    player = newPlayer;
}

vector3df TKCharacter::getPosition()
{
    return node->getPosition();
}

void TKCharacter::setPosition(vector3df pos)
{
    node->setPosition(pos);
}

void TKCharacter::setRotation(double rotation)
{
    node->setRotation(vector3df(0,rotation,0));
}

void TKCharacter::setAnimation(stringc anim)
{
    if(anim == "WALK")
    {
        hideAllNodes();
        villagerWalkNode->setVisible(true);
    }
    if(anim == "WAIT")
    {
        hideAllNodes();
        villagerWaitNode->setVisible(true);
    }
    if(anim == "COLLECT_FOOD")
    {
        hideAllNodes();
        villagerFoodNode->setVisible(true);
    }
    if(anim == "COLLECT_STONE")
    {
        hideAllNodes();
        villagerStoneNode->setVisible(true);
    }
    if(anim == "COLLECT_WOOD")
    {
        hideAllNodes();
        villagerWoodNode->setVisible(true);
    }

    if(anim == "WARRIOR_ATTACK")
    {
        hideAllNodes();
        villagerWoodNode->setVisible(true);
    }
}

void TKCharacter::setSelected(bool newSelected)
{
    selected=newSelected;
}

vector3df TKCharacter::update(TKNetwork* tkNetwork)
{
    vector3df quemEstouAtacando=vector3df(-1000,-1000,-1000);

    if(alive)
    {
        if(currentAction == "WALK" && node->getPosition().getDistanceFrom(moveToPos) > 3 )
        {
            lookAt(moveToPos);

            vector3df pos=node->getPosition();
            pos.Z -= cos(grad2rad(node->getRotation().Y))*moveSpeed;
            pos.X -= sin(grad2rad(node->getRotation().Y))*moveSpeed;
            pos.Y = 0.5;//fixar no Y da terrain (gravidade)

            node->setPosition(pos);

            stringc msg="CHARACTER_MOVE ";
            msg+=this->getID();
            msg+=" ";
            msg+=node->getPosition().X;
            msg+=" ";
            msg+=node->getPosition().Y;
            msg+=" ";
            msg+=node->getPosition().Z;
            msg+=" ";
            msg+=node->getRotation().Y;
            msg+=" ";
            msg+=currentAction.c_str();

            if(DEBUG_NETWORK) cout << msg.c_str() << endl;

            tkNetwork->bufferMessage(msg.c_str());
        }
        else if(currentAction == "WALK")
        {
            hideAllNodes();
            villagerWaitNode->setVisible(true);///TODO:verificar se esta perto de recurso começa a coletar

            if(nearestResourceType!="" && info->getImage()=="VILLAGER")
            {
                if(nearestResourceType == "FOOD")///TODO: setar animacoes aqui
                {
                    currentAction="COLLECT_FOOD";
                    hideAllNodes();
                    villagerFoodNode->setVisible(true);
                    lookAt(nearestResource);

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+=currentAction.c_str();

                    tkNetwork->bufferMessage(msg.c_str());

                }
                else if(nearestResourceType == "STONE")
                {
                    currentAction="COLLECT_STONE";
                    hideAllNodes();
                    villagerStoneNode->setVisible(true);
                    lookAt(nearestResource);

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+=currentAction.c_str();

                    tkNetwork->bufferMessage(msg.c_str());
                }
                else if(nearestResourceType == "WOOD")
                {
                    currentAction="COLLECT_WOOD";
                    hideAllNodes();
                    villagerWoodNode->setVisible(true);
                    lookAt(nearestResource);

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+=currentAction.c_str();

                    tkNetwork->bufferMessage(msg.c_str());
                }
            }
            else if(info->getImage()=="WARRIOR")
            {
                if(node->getPosition().getDistanceFrom(nearestEnemyBuilding) < 12)
                {
                    hideAllNodes();
                    villagerWoodNode->setVisible(true);
                    lookAt(nearestResource);
                    quemEstouAtacando=nearestEnemyBuilding;

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+="WARRIOR_ATTACK";

                    tkNetwork->bufferMessage(msg.c_str());
                }
                else if(nearestResourceType!="")
                {
                    hideAllNodes();
                    villagerWoodNode->setVisible(true);
                    lookAt(nearestResource);
                    quemEstouAtacando=nearestResource;

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+="WARRIOR_ATTACK";

                    tkNetwork->bufferMessage(msg.c_str());
                }
                else
                {
                    currentAction = "WAIT";

                    stringc msg="CHARACTER_ACTION ";
                    msg+=this->getID();
                    msg+=" ";
                    msg+=node->getRotation().Y;
                    msg+=" ";
                    msg+=currentAction.c_str();

                    tkNetwork->bufferMessage(msg.c_str());
                }
            }

            else
            {
                currentAction = "WAIT";

                stringc msg="CHARACTER_ACTION ";
                msg+=this->getID();
                msg+=" ";
                msg+=node->getRotation().Y;
                msg+=" ";
                msg+=currentAction.c_str();

                tkNetwork->bufferMessage(msg.c_str());
            }
        }
        else if(currentAction == "COLLECT_FOOD")
        {
            tkCore->eventCollectResource(player,"FOOD");
        }
        else if(currentAction == "COLLECT_STONE")
        {
            tkCore->eventCollectResource(player,"STONE");
        }
        else if(currentAction == "COLLECT_WOOD")
        {
            tkCore->eventCollectResource(player,"WOOD");
        }


        if(isSelected())
            selectedCharacterNode->setVisible(true);
        else
            selectedCharacterNode->setVisible(false);
    }

    return quemEstouAtacando;
}

void TKCharacter::addCollisionResponseAnimator(ITriangleSelector* newselector)
{
    node->addAnimator(smgr->createCollisionResponseAnimator(newselector,node,vector3df(1,2,1),vector3df(0,0,0)) );
}

void TKCharacter::removeCollisions()
{
    node->removeAnimators();
}

ITriangleSelector* TKCharacter::getTriangleSelector()
{
    return selector;
}

void TKCharacter::hideAllNodes()
{
    villagerWaitNode->setVisible(false);
    villagerWalkNode->setVisible(false);
    villagerWoodNode->setVisible(false);
    villagerFoodNode->setVisible(false);
    villagerStoneNode->setVisible(false);
}

void TKCharacter::lookAt(vector3df pos)
{
    vector3df offsetVector = pos - node->getPosition();

    vector3df rot = (-offsetVector).getHorizontalAngle();

    rot.X=0;
    rot.Z=0;

    node->setRotation(rot);
}

void TKCharacter::setID(stringc nid)
{
    id=nid;
}

stringc TKCharacter::getID()
{
    return id;
}

void TKCharacter::decreaseLife()
{
    info->setLife(info->getIntLife()-1);
}
