#ifndef CITEM_H
#define CITEM_H

#include "irrlicht.h"
//#include <irrKlang.h>
#include "CPlayer.h"
//using namespace irrklang;
using namespace irr;
class CItem
{
public:
    CItem(int mtype,scene::ISceneManager* mgr,s32 mat,ITimer*mtimer,scene::ISceneNode*mLevelNode,scene::ITriangleSelector* mSelector);
    virtual ~CItem();

    //positions coins
    void spwan()
    {
        core::vector3df pos=makeSpwanPoint();
        node->setPosition(pos);
        //update special coin 3d sound position
    //    if (type==49)
//            twinkle->setPosition(pos);
    }

    void checkHit(int& score,int& comboHolder,int& airCoins,CPlayer* player,bool dir)
    {
        //update irrklang ears
//        engine->setListenerPosition(smgr->getActiveCamera()->getPosition(),
//                                    smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getAbsolutePosition());

        if (node->getPosition().getDistanceFromSQ(smgr->getActiveCamera()->getPosition())<50000)
        {
            //play sound and respwan coin
//            engine->play2D("coin.wav");
            this->spwan();

            //increase air coin counter
            airCoins+=1;
            if (airCoins>1)
            {
           //     engine->play2D("cheer.wav");
                player->showCoinAirCombo();
            }

            //if speical coin...
            if (type==49)
            {
                comboHolder+=5000;
          //      engine->play2D("cheer.wav");
                player->showSpecialCoinCombo();
            }
            else
                //evil coin
                if (type==0)
                    score*=0.5;
            //normal coin
                else
                {
                    if (dir)
                        score+=400;
                    else
                        score+=200;
                }



        }
    }
     scene::ISceneNode* outNode;
protected:
private:
//rnd number genrator within a range
    int makeRand(int min,int max)
    {

        int range=max-min+1;
        return rand()%range+min;
    }
    //spwan coins on map,makes sure coins are above the surface by casting rays down like rain
    core::vector3df makeSpwanPoint()
    {
        float x=makeRand(worldBox.MinEdge.X*500,worldBox.MaxEdge.X*500);
        float z=makeRand(worldBox.MinEdge.Z*500,worldBox.MaxEdge.Z*500);

        core::line3d<f32> line;
        line.start = core::vector3df(x,10000,z);
        line.end = core::vector3df(x,-100000,z);

        core::vector3df intersection;
        core::triangle3df tri;
        float y;

        if (smgr->getSceneCollisionManager()->getCollisionPoint(
                    line, selector, intersection, tri,outNode))
        {
            y=makeRand(intersection.Y+300,intersection.Y+1000);


        }

        return core::vector3df (x,y,z);
    }


    scene::IAnimatedMeshSceneNode* node;
    scene::ISceneManager* smgr;
    video::IVideoDriver* driver;
    scene::ISceneNode*levelNode;
    scene::ITriangleSelector* selector;
    core::aabbox3df worldBox;
    ITimer*timer;
    int type;
//    ISound* twinkle;

};

#endif // CITEM_H
