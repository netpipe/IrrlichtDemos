#include "TKBuilding.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKBuilding::TKBuilding(ISceneManager* mgr, stringc newtype, vector3df pos)
{
    smgr=mgr;
    type=newtype;
    info=new TKHudInfo();

    if(type=="MILITARY_CAMP")
    {
        mesh=smgr->getMesh("../media/Buildings/militaryCamp.b3d");
        shadowMesh=smgr->getMesh("../media/Buildings/militaryCamp_shadow.b3d");
        rubbleMesh=smgr->getMesh("../media/Buildings/militaryRubble.b3d");

        ///TODO: load from db
        info->setLabel("Edificacao Militar: Cria Soldados");
        info->setLife(1000);
        info->addBtAction("CREATE_WARRIOR");
        info->setImage("MILITAR");
    }

    if(type=="HOUSE")
    {
        mesh=smgr->getMesh("../media/Buildings/house.b3d");
        shadowMesh=smgr->getMesh("../media/Buildings/house_shadow.b3d");
        rubbleMesh=smgr->getMesh("../media/Buildings/houseRubble.b3d");

        ///TODO: load from db
        info->setLabel("Casa: Aumenta o limite populacional");
        info->setLife(300);
        info->setImage("HOUSE");
    }

    if(type=="TEMPLE")
    {
        mesh=smgr->getMesh("../media/Buildings/temple.b3d");
        shadowMesh=smgr->getMesh("../media/Buildings/temple_shadow.b3d");
        rubbleMesh=smgr->getMesh("../media/Buildings/templeRubble.b3d");

        ///TODO: load from db
        info->setLabel("Templo: Edificacao principal");
        info->setLife(500);
        info->addBtAction("CREATE_VILLAGER");
        info->setImage("TEMPLE");
    }


    node=smgr->addMeshSceneNode(mesh);
    shadowNode=smgr->addMeshSceneNode(shadowMesh,node);
    shadowNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
    nodeRubble=smgr->addMeshSceneNode(rubbleMesh);
    nodeRubble->setVisible(false);
    nodeRubble->setPosition(pos);

    selectedNode=smgr->addMeshSceneNode(shadowMesh,node);
    selectedNode->setMaterialTexture(0,smgr->getVideoDriver()->getTexture("../media/Buildings/buildingSelected.png"));
    selectedNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
    selectedNode->setVisible(false);

    node->setPosition(pos);

    selector=smgr->createOctTreeTriangleSelector(mesh,node,4);

    node->setTriangleSelector(selector);

    id="BUILDING_";
    id+=rand();

    node->setMesh(rubbleMesh);
    node->setMesh(mesh);
}

void TKBuilding::decreaseLife(TKNetwork* tkNetwork,TKCore* tkCore)
{
    info->setLife(info->getIntLife()-1);

    if(info->getImage() == "TEMPLE")
    {
        if(info->getIntLife()<=100)
        {
            stringc msg="END ";
            msg+=player;
            tkNetwork->bufferMessage(msg.c_str());
            tkCore->eventNetworkReceive(msg.c_str());
        }
    }

    if(info->getIntLife()<=0)
    {
        node->setVisible(false);
        shadowNode->setVisible(false);
        nodeRubble->setVisible(true);
    }
}

ITriangleSelector* TKBuilding::getTriangleSelector()
{
    return selector;
}

vector3df TKBuilding::getPosition()
{
    return node->getPosition();
}

TKHudInfo* TKBuilding::getInfo()
{
    return info;
}

stringc TKBuilding::getType()
{
    return type;
}

void TKBuilding::setPosition(vector3df pos)
{
    node->setPosition(pos);
    nodeRubble->setPosition(pos);
}

void TKBuilding::setSelected(bool sel)
{
    selectedNode->setVisible(sel);
}

void TKBuilding::setSolid(bool solid)
{
    if(solid)
        node->setMaterialType(EMT_SOLID);
    else
        node->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
}

stringc TKBuilding::getID()
{
    return id;
}

void TKBuilding::setID(stringc nid)
{
    id=nid;
}

stringc TKBuilding::getPlayer()
{
    return player;
}

void TKBuilding::setPlayer(stringc nplayer)
{
    player=nplayer;
}
