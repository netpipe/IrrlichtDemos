#include "TKBuildingManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

TKBuildingManager::TKBuildingManager(ISceneManager* mgr, stringc nplayer, TKCore* ntkCore)///TODO: TKBuilding and TKBUildingManager Destructors
{
    smgr=mgr;
    player=nplayer;
    tkCore=ntkCore;
}

void TKBuildingManager::setTKNetwork(TKNetwork* ntkNetwork)
{
    tkNetwork=ntkNetwork;
}

TKBuilding* TKBuildingManager::createMilitaryCamp(vector3df pos)
{
    TKBuilding* temp=new TKBuilding(smgr,"MILITARY_CAMP",pos);

    triangleSelectors.push_back(temp->getTriangleSelector());
    buildingsAlive.push_back(temp);

    temp->setPlayer(player);

    return temp;
}

TKBuilding* TKBuildingManager::createHouse(vector3df pos)
{
    TKBuilding* temp=new TKBuilding(smgr,"HOUSE",pos);

    triangleSelectors.push_back(temp->getTriangleSelector());
    buildingsAlive.push_back(temp);

    temp->setPlayer(player);

    return temp;
}

TKBuilding* TKBuildingManager::createTemple(vector3df pos)
{
    TKBuilding* temp=new TKBuilding(smgr,"TEMPLE",pos);

    triangleSelectors.push_back(temp->getTriangleSelector());
    buildingsAlive.push_back(temp);

    temp->setPlayer(player);

    return temp;
}

///Returns a vector with all buildings triangle selectors.
vector<ITriangleSelector*> TKBuildingManager::getTriangleSelectorVector()
{
    return triangleSelectors;
}

vector<vector3df> TKBuildingManager::getEnemyBuildingsPos()
{
    vector<vector3df> pos;

    for(int i=0;i<buildingsAlive.size();i++)
    {
         if(((TKBuilding*)buildingsAlive[i])->getPlayer() != player  && ((TKBuilding*)buildingsAlive[i])->getInfo()->getIntLife() > 0)
         {
             pos.push_back( ((TKBuilding*)buildingsAlive[i])->getPosition() );;
         }
    }

    return pos;
}

vector<stringc> TKBuildingManager::getEnemyBuildingsIDs()
{
    vector<stringc> ids;

    for(int i=0;i<buildingsAlive.size();i++)
    {
         if(((TKBuilding*)buildingsAlive[i])->getPlayer() != player && ((TKBuilding*)buildingsAlive[i])->getInfo()->getIntLife() > 0)
         {
             ids.push_back( ((TKBuilding*)buildingsAlive[i])->getID() );;
         }
    }

    return ids;
}

TKBuilding* TKBuildingManager::selectBuilding(vector3df mouseClickPos)
{
    for(int i=0;i<buildingsAlive.size();i++)
    {
        if(mouseClickPos.getDistanceFrom(buildingsAlive[i]->getPosition()) < 5 && buildingsAlive[i]->getPlayer() == player)
        {
            return buildingsAlive[i];
        }
    }

    return NULL;
}

void TKBuildingManager::setSelected(TKBuilding* build)
{
    if(build != false)
    {
        build->setSelected(true);
    }
    else
    {
        for(int i=0;i<buildingsAlive.size();i++)
            buildingsAlive[i]->setSelected(false);
    }
}

void TKBuildingManager::decreaseLife(stringc id)
{
    for(int i=0;i<buildingsAlive.size();i++)
    {
        if(((TKBuilding*)buildingsAlive[i])->getID() == id)
        {
            ((TKBuilding*)buildingsAlive[i])->decreaseLife(tkNetwork,tkCore);
        }
    }
}
