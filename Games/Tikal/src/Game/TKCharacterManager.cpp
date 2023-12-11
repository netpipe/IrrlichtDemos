#include "TKCharacterManager.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

TKCharacterManager::TKCharacterManager(ISceneManager* mgr, TKCore* ntkCore, stringc nplayer)
{
    smgr=mgr;
    tkCore=ntkCore;
    player=nplayer;

    miniMapTexture=smgr->getVideoDriver()->addTexture(dimension2du(128,128),"miniMapChars",ECF_A8R8G8B8);
    updateMiniMapTexture();

    killingCharacter=false;
}

TKCharacterManager::~TKCharacterManager()
{
    for(int i=0;i<charactersAlive.size();i++)///TODO:verificar se ta tudo OK na hora de desalocar characters
    {
        delete charactersAlive[i];
        charactersAlive.erase(charactersAlive.begin() + i);
        i=0;
    }
}

void TKCharacterManager::setResources(vector<vector3df> pos, vector<stringc> resType)
{
    resoucesPos=pos;
    resources=resType;
}

TKCharacter* TKCharacterManager::createCharacter(stringc type,vector3df pos,stringc player)
{
    TKCharacter* tempCharacter=new TKCharacter(smgr,tkCore,type);

    tempCharacter->setPosition(pos);
    tempCharacter->setPlayer(player);
    tempCharacter->setSelected(false);

    for(int i=0;i<charactersAlive.size();i++)
    {
        //tempCharacter->addCollisionResponseAnimator( ((TKCharacter*)charactersAlive[i])->getTriangleSelector() );
        //((TKCharacter*)charactersAlive[i])->addCollisionResponseAnimator(tempCharacter->getTriangleSelector());
    }

    for(int i=0;i<selectorsOnMap.size();i++)//add selectors que ja estavam criados no cenario
    {
        tempCharacter->addCollisionResponseAnimator( selectorsOnMap[i] );
    }

    charactersAlive.push_back(tempCharacter);

    return tempCharacter;
}

int TKCharacterManager::getTotalCharactersSelected()
{
    return charactersSelected.size();
}

void TKCharacterManager::moveSelectedTo(vector3df pos)
{
    f32 resourceDistance=320000;
    vector3df nearestResourcePos;
    stringc nearestResource;

    vector3df nearestEnemyPos=vector3df(0,0,0);
    stringc nearestEnemy="";

    for(int i=0;i<charactersAlive.size();i++)
    {
        if( charactersAlive[i]->getPosition().getDistanceFrom(pos) < 3 && charactersAlive[i]->isSelected()==false && charactersAlive[i]->getPlayer()!=player)
        {
            nearestEnemy=((TKCharacter*)charactersAlive[i])->getInfo()->getImage();
            nearestEnemyPos=((TKCharacter*)charactersAlive[i])->getPosition();
        }
    }

    for(int i=0;i<resoucesPos.size();i++)
    {
        if( ((vector3df)resoucesPos[i]).getDistanceFrom(pos) < resourceDistance )
        {
            nearestResource=resources[i];
            nearestResourcePos=resoucesPos[i];
            resourceDistance=((vector3df)resoucesPos[i]).getDistanceFrom(pos);
        }
    }

    for(int i=0;i<charactersSelected.size();i++)
    {
        ((TKCharacter*)charactersSelected[i])->moveTo(pos);

        if( ((TKCharacter*)charactersSelected[i])->getInfo()->getImage() == "VILLAGER")
        {
            if(resourceDistance < 3)
                ((TKCharacter*)charactersSelected[i])->setNearestResource(nearestResourcePos,nearestResource);
            else
                ((TKCharacter*)charactersSelected[i])->setNearestResource(nearestResourcePos,"");
        }
        else
        {
            ((TKCharacter*)charactersSelected[i])->setNearestResource(nearestEnemyPos,nearestEnemy);
        }
    }
}

void TKCharacterManager::moveByIDTo(stringc id,vector3df pos,double rotation,stringc action)
{
    for(int i=0;i<charactersAlive.size();i++)
    {
        if(charactersAlive[i]->getID() == id)
        {
            charactersAlive[i]->setPosition(pos);
            charactersAlive[i]->setRotation(rotation);
            charactersAlive[i]->setAnimation(action);
            break;
        }
    }
}

void TKCharacterManager::setActionByID(stringc id, double rotation, stringc action)
{
    for(int i=0;i<charactersAlive.size();i++)
    {
        if(charactersAlive[i]->getID() == id)
        {
            charactersAlive[i]->setRotation(rotation);
            charactersAlive[i]->setAnimation(action);
            break;
        }
    }
}

void TKCharacterManager::setNearestEnemyBuilding(vector<vector3df> pos,vector<stringc> ids)
{
    buildsPos=pos;
    buildIDs=ids;

    f32 nearestPos=32000;
    f32 distanceFromBuilding;
    vector3df buildPos=vector3df(0,0,0);

    for(int i=0;i<charactersAlive.size();i++)
    {
        if(pos.size()>0)
        {
            nearestPos=pos[0].getDistanceFrom(charactersAlive[i]->getPosition());
            buildPos=pos[0];
        }
        else
            nearestPos=32000;


        for(int j=0;j<pos.size();j++)
        {
            distanceFromBuilding = pos[j].getDistanceFrom(charactersAlive[i]->getPosition());

            if(distanceFromBuilding < nearestPos)
            {
                nearestPos = distanceFromBuilding;
                buildPos = pos[j];
            }
        }

        ((TKCharacter*)charactersAlive[i])->setNearestEnemyBuilding(buildPos);
    }
}

TKCharacter* TKCharacterManager::selectCharacters(vector3df mouseClickPos)///TODO:verificar player
{
    for(int i=0;i<charactersSelected.size();i++)
        charactersSelected[i]->setSelected(false);

    charactersSelected.clear();

    for(int i=0;i<charactersAlive.size();i++)
    {
        TKCharacter* tempCharacter=((TKCharacter*)charactersAlive[i]);

        vector3df charPos = tempCharacter->getPosition();

        if( charPos.getDistanceFrom(mouseClickPos) < 1 && tempCharacter->getPlayer() == player)
        {
            charactersSelected.push_back(tempCharacter);
            tempCharacter->setSelected(true);
        }
    }

    if(charactersSelected.size()>0)
        return charactersSelected[0];
    else
        return NULL;
}


TKCharacter* TKCharacterManager::selectCharacters(vector3df p1, vector3df p2)
{
    for(int i=0;i<charactersSelected.size();i++)
        charactersSelected[i]->setSelected(false);

    charactersSelected.clear();

    if(p2.X < p1.X)
    {
        f32 temp=p1.X;
        p1.X=p2.X;
        p2.X=temp;
    }
    if(p2.Z < p1.Z)
    {
        f32 temp=p1.Z;
        p1.Z=p2.Z;
        p2.Z=temp;
    }

    for(int x=p1.X; x<p2.X;x++)
    {
        for(int z=p1.Z;z<p2.Z;z++)
        {

            for(int i=0;i<charactersAlive.size();i++)
            {
                TKCharacter* tempCharacter=((TKCharacter*)charactersAlive[i]);

                vector3df charPos = tempCharacter->getPosition();

                if( charPos.getDistanceFrom(vector3df(x,tempCharacter->getPosition().Y,z)) < 1 && tempCharacter->getPlayer() == player)
                {
                    charactersSelected.push_back(tempCharacter);
                    tempCharacter->setSelected(true);
                    return charactersSelected[0]; //adicionar isso para selecionar mais de um cara
                }
            }
        }
    }

    if(charactersSelected.size()>0)
        return charactersSelected[0];
    else
        return NULL;
}

void TKCharacterManager::selectNone()
{
    for(int i=0;i<charactersSelected.size();i++)
    {
        charactersSelected[i]->setSelected(false);
    }
    charactersSelected.clear();
}


void TKCharacterManager::updateMiniMapTexture()
{
    u32 *pixelsMiniMap = (u32*)miniMapTexture->lock();

    SColor colorGreen(200,255,255,0);//minhas unidades
    SColor colorBlue(200,255,0,0);//unidades inimigas ///TODO: visualizar unidades inimigas quando implementar fog of war
    SColor colorTransparency(0,0,0,0);

    for(int z=0;z < miniMapTexture->getSize().Height;z++)
    {
        for(int x=0;x < miniMapTexture->getSize().Width;x++)
        {
            pixelsMiniMap[z*miniMapTexture->getSize().Width + (x)] = colorTransparency.color;
        }
    }

    for(int i=0;i<charactersAlive.size();i++)
    {
        int x = ((TKCharacter*)charactersAlive[i])->getPosition().X/2;
        int z = ((TKCharacter*)charactersAlive[i])->getPosition().Z/2;
        pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)-1] = colorGreen.color;
        pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)-2] = colorGreen.color;
        pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)] = colorGreen.color;
        pixelsMiniMap[(miniMapTexture->getSize().Width-z)*miniMapTexture->getSize().Width + (x)+1] = colorGreen.color;
    }
    //apply changes
    miniMapTexture->unlock();
}

ITexture* TKCharacterManager::getMiniMapTexture()
{
    return miniMapTexture;
}

void TKCharacterManager::updateAll(TKNetwork* tkNetwork,TKBuildingManager* tkBuildingManager)
{
    //gambia!!! faltam dois dias pra entregar essa porra!!!!!!!!!!!!!!
    vector3df quemEstouAtacando=vector3df(-1000,-1000,-1000);

    for(int i=0;i<charactersAlive.size();i++)
    {
        if(charactersAlive[i]->getInfo()->getIntLife() <= 0 )
        {
            stringc msg="KILL CHARACTER ";
            msg+=charactersAlive[i]->getID();

            if(DEBUG_NETWORK) cout << msg.c_str() << endl;

            tkNetwork->bufferMessage(msg.c_str());
            killCharacter(i);
            i=0;
        }
        else
        {
            quemEstouAtacando=charactersAlive[i]->update(tkNetwork);

            if(quemEstouAtacando != vector3df(-1000,-1000,-1000))
            {
                for(int i=0;i<buildsPos.size();i++)
                {
                    if(buildsPos[i].getDistanceFrom(quemEstouAtacando) < 1)
                    {
                        stringc msg="DECREASE_LIFE_BUILDING ";
                        msg+=buildIDs[i];

                        if(DEBUG_NETWORK) cout << msg.c_str() << endl;

                        tkNetwork->bufferMessage(msg.c_str());

                        tkBuildingManager->decreaseLife(buildIDs[i].c_str());

                        break;
                    }
                }

                for(int i=0;i<charactersAlive.size();i++)
                {
                    if(charactersAlive[i]->getPosition().getDistanceFrom(quemEstouAtacando) < 1)
                    {
                        stringc msg="DECREASE_LIFE ";
                        msg+=((TKCharacter*)charactersAlive[i])->getID();
                        msg+=" ";
                        msg+=((TKCharacter*)charactersAlive[i])->getInfo()->getIntLife();

                        if(DEBUG_NETWORK) cout << msg.c_str() << endl;

                        tkNetwork->bufferMessage(msg.c_str());

                        break;
                    }
                }
            }
        }
    }

    updateMiniMapTexture();
}

void TKCharacterManager::bufferCharacterToKill(stringc id)
{
    charactersToKill.push_back(id);
}

void TKCharacterManager::killCharactersBuffer()
{
    for(int i=0;i<charactersToKill.size();i++)
    {
        killCharacter(charactersToKill[i]);
    }
}

void TKCharacterManager::killCharacter(stringc id)
{
    for(int i=0;i<charactersAlive.size();i++)
    {
        if(charactersAlive[i]->getID() == id)
        {
            killCharacter(i);
            break;
        }
    }
}

void TKCharacterManager::killCharacter(int i)
{
    if(killingCharacter)
        do{}while(killingCharacter);
    else
        killingCharacter=true;

    for(int s=0;s<selectorsOnMap.size();s++)
    {
        if( selectorsOnMap[s] == ((TKCharacter*)charactersAlive[i])->getTriangleSelector() )
        {
            selectorsOnMap.erase(selectorsOnMap.begin() + s);
        }
    }

    for(int j=0;j<charactersAlive.size();j++)
    {
        if( ((TKCharacter*)charactersAlive[j])!=((TKCharacter*)charactersAlive[i]) )
        {
            ((TKCharacter*)charactersAlive[j])->removeCollisions();

            for(int c=0;c<selectorsOnMap.size();c++)
            {
                ((TKCharacter*)charactersAlive[j])->addCollisionResponseAnimator(selectorsOnMap[c]);
            }
        }
    }

    delete charactersAlive[i];
    charactersAlive.erase(charactersAlive.begin() + i);

    killingCharacter=false;
}

void TKCharacterManager::decreaseLife(stringc id)
{
    for(int i=0;i<charactersAlive.size();i++)
    {
        if(charactersAlive[i]->getID() == id)
        {
            charactersAlive[i]->decreaseLife();
            break;
        }
    }
}

void TKCharacterManager::updateCollisions(ITriangleSelector* selector)
{
    for(int i=0;i<charactersAlive.size();i++)
    {
        charactersAlive[i]->addCollisionResponseAnimator(selector);
    }

    selectorsOnMap.push_back(selector);
}

void TKCharacterManager::updateCollisions(vector<ITriangleSelector*> selectors)
{
    /*for(int i=0;i<charactersAlive.size();i++)
    {
        for(int j=0;j<selectors.size();j++)
            charactersAlive[i]->addCollisionResponseAnimator(selectors[j]);
    }*/

    for(int i=0;i<selectors.size();i++)
    {
        selectorsOnMap.push_back(selectors[i]);
    }
}

