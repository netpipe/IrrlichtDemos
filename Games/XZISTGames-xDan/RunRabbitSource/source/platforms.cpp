
#include "platforms.h"
#include <vector>
#include "carrots.h"
#include "stringaling.h"
#include "event.h"
#include "misc.h"
#include "text.h"
//#include <irrKlang.h>

//using namespace audio;

extern scene::ISceneManager *smgr;
extern bool rabbitFallen;
//extern ISoundEngine *sengine;

std::vector <scene::IAnimatedMeshSceneNode *> platforms;

// platform movement parameters
core::vector3df startPos;
core::vector3df endPos;
f32 vel;

// platform creation parameters
int lastTime = 0;
int timeToPass = 0;

// game difficulty parameters
double playTime;
double levelTime;
double areaTime;
int timeGap;
int minTimeGap;
int maxTimeGap;
f32 minOffY;
f32 maxOffY;

void resetPlatforms()
{
    removePlatforms();
    startPos = core::vector3df(0,0,300.0);
    endPos = core::vector3df(0,0,-100.0);
    // reset platform creation
    lastTime = 0;
    timeToPass = 0;
    // reset difficulty
    vel = 0.05;
    playTime = 0;
    levelTime = 0;
    areaTime = 0;
    timeGap = 0;
    minTimeGap = 200;
    maxTimeGap = 300;
    minOffY = 15.0;
    maxOffY = 25.0;
}

void updatePlatformDifficulty(double dt)
{
    // increase speeds etc... at intervals...
    playTime += dt;
    levelTime += dt;
    areaTime += dt;

    if (levelTime > 20 && !rabbitFallen)
    {
//        sengine->play2D("media/speed.wav", false,false,false,ESM_NO_STREAMING);
        blinkMessage(L"< administering speed >", screenHH(), 3.5);

        levelTime = 0;

        vel += 0.0025;
        minTimeGap += 50;
        maxTimeGap += 50;
        maxOffY += 1.0;
    }
}

void createPlatformUpdate(int time)
{
    if (time > lastTime+timeGap+timeToPass || lastTime == 0)
    {
        // create a platform
        scene::IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(
            smgr->getMesh(strparty("media/plat", rand()%3, ".obj").c_str())
            );

        f32 s = 1;//0.75;
        node->setScale(core::vector3df(s,s,s));

        node->setPosition(startPos+core::vector3df(0,0,node->getBoundingBox().getExtent().Z/2.0));
        node->updateAbsolutePosition();
        // make it move in a straight line...
        //scene::ISceneNodeAnimator *anim = smgr->createFlyStraightAnimator(
        //    startPos+core::vector3df(0,0,node->getBoundingBox().getExtent().Z/2.0),
        //    endPos,
        //    (int)(startPos.getDistanceFrom(endPos)/vel));
        //node->addAnimator(anim);
        //anim->drop();
        // add to list
        platforms.push_back(node);
        // add carrots to it?
        addCarrots(node);

        // change generation point
        if (state == IN_GAME)
        {
            f32 yOff = (minOffY + (maxOffY-minOffY)*frandU()) * randSign();
            startPos.Y += yOff;
            endPos.Y += yOff;
        }

        timeGap = minTimeGap + rand()%(maxTimeGap-minTimeGap);

        timeToPass = (int) (node->getBoundingBox().getExtent().Z / vel);

        lastTime = time;

        if (rand()%100 < 30)// && state == IN_STARTSCREEN)
        {
            scene::IAnimatedMeshSceneNode *r = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/rabbit2.b3d"), node);
            r->setRotation(core::vector3df(90,frandU()*360.0,0));
            r->setPosition(core::vector3df(frandS()*6.0,6+frandU()*1.0,frandS()*2.0));
            r->setAnimationSpeed(2000 + rand()%2000);
            r->setCurrentFrame(rand()%r->getEndFrame());
            f32 s = 1.2 + frandU()*0.5;
            r->setScale(core::vector3df(s,s,s));
        }
    }
}

void freezePlatforms()
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        platforms[i]->removeAnimators();
    }
}

void updatePlatforms(double dt)
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        platforms[i]->setPosition(platforms[i]->getAbsolutePosition()+core::vector3df(0,0,-vel*dt*1000.0));
        platforms[i]->updateAbsolutePosition();
        // cull platform if out area
        if (platforms[i]->getAbsolutePosition().Z <= endPos.Z)
        {
            platforms[i]->remove();
            platforms.erase(platforms.begin()+i);
            i --;
            continue;
        }
    }
}

void removePlatforms()
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        platforms[i]->remove();
    }
    platforms.clear();
    removeCarrots();
}

int getPlatformCollision(core::vector3df pos, core::aabbox3df *box)
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        if (platforms[i]->getTransformedBoundingBox().isPointInside(pos))
        {
            *box = platforms[i]->getTransformedBoundingBox();
            return true;
        }
    }
    return false;
}

f32 getClosestPlatformDistance2D(core::vector3df pos)
{
    f32 closestDist = 100000.0;
    for (int i = 0; i < platforms.size(); i ++)
    {
        f32 dist = core::vector2df(pos.X,pos.Z).getDistanceFrom(
            core::vector2df(platforms[i]->getAbsolutePosition().X,
                platforms[i]->getAbsolutePosition().Z)
            );
        if (dist < closestDist)
        {
            closestDist = dist;
        }
    }
    return closestDist;
}

core::vector3df getLowestPlatformPoint()
{
    core::vector3df lowest = core::vector3df(0,100000,0);

    for (int i = 0; i < platforms.size(); i ++)
    {
        if (platforms[i]->getAbsolutePosition().Y < lowest.Y)
        {
            lowest.Y = platforms[i]->getAbsolutePosition().Y;
        }
    }
    return lowest;
}

