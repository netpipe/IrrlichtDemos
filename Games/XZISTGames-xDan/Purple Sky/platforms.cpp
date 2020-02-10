
#include "platforms.h"
#include <vector>
#include "player.h"
#include "state.h"
#include <irrKlang.h>

#define RAND_F (((float)(rand()%1000))/1000.0)

using namespace audio;

const f32 platformStep = 40.0; // degrees
extern scene::ISceneManager *smgr;
extern ISoundEngine *sengine;

const f32 minPlatformSeparation = 6.0;
const f32 platformLifeLength = 3.0;
const f32 itemFallDist = 40.0;

#define FALL_AT_DIST maxPlayerJump*3.0

scene::IAnimatedMeshSceneNode *lastSelected;

struct Platform
{
    scene::IAnimatedMeshSceneNode *node;
    scene::IAnimatedMeshSceneNode *nodeSelectable;
    scene::IAnimatedMeshSceneNode *powerUp; // e.g. nut
    int powerUpType;
    bool touched;
    double lifeTime;
    bool readyToFall;
};

struct FallingItem
{
    scene::IAnimatedMeshSceneNode *node;
    core::vector3df move;
    f32 acc;
};

std::vector <Platform> platforms;
std::vector <Platform> fallingPlatforms;
std::vector <scene::IAnimatedMeshSceneNode *> spidersGoingUp;
std::vector <FallingItem> fallingItems;

video::SColor ringEmissive;

enum {POWERUP_NONE = 0, POWERUP_HAZELNUT = 1, POWERUP_SPIDER = 2};

void addPlatformPowerUp(Platform &platform, int type, core::vector3df pos)
{
    platform.powerUpType = type;
    
    if (type == POWERUP_HAZELNUT)
    {
        platform.powerUp = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/hazelnut.b3d"), platform.node);
        platform.powerUp->setPosition(core::vector3df(0,0,platform.node->getBoundingBox().getExtent().Y));
        platform.powerUp->setRotation(core::vector3df(0,30,rand()%360));
    }
    else if (type == POWERUP_SPIDER)
    {
        platform.powerUp = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/spider.b3d"));
        platform.powerUp->setPosition(pos + core::vector3df(0,20 + rand()%20,0));
        platform.powerUp->setRotation(core::vector3df(-90,0,0));
    }
    else
    {
        platform.powerUpType = POWERUP_NONE;
    }
}

void removePlatformPowerUp(Platform *p)
{
    if (p->powerUp)
    {
        p->powerUp->remove();
        p->powerUp = NULL;
    }
    p->powerUpType = POWERUP_NONE;
}

void createPlatform(core::vector3df pos)
{
    Platform platform =
    {
        smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/mushroom.b3d")),
        NULL,
        NULL,
        POWERUP_NONE,
        false,
        0.0,
        false
    };
    
    platform.nodeSelectable = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/ring.b3d"), platform.node);
    
    if (pos != core::vector3df(0,0,0)) // not start platform
    {
        if (rand()%10 < 5)
        {
            addPlatformPowerUp(platform, POWERUP_HAZELNUT, pos);
        }
        else if (rand()%10 < 5)
        {
            addPlatformPowerUp(platform, POWERUP_SPIDER, pos);
        }
    }
    
    platform.node->setRotation(core::vector3df(-90,0,0));
    pos.Y = -platform.node->getBoundingBox().getExtent().Y;
    platform.node->setPosition(pos);
    f32 s = 0.01;
    platform.node->setScale(core::vector3df(s,s,s));
    platform.node->updateAbsolutePosition();
    
    platform.nodeSelectable->setPosition(core::vector3df(0,0,platform.node->getBoundingBox().getExtent().Y-0.4));
    //pos.Y = 0;
    //platform.nodeSelectable->setPosition(pos);
    //platform.nodeSelectable->setRotation(core::vector3df(-90,0,0));
    ringEmissive = platform.nodeSelectable->getMaterial(0).EmissiveColor;
    
    platforms.push_back(platform);
}

void destroyPlatform(Platform *p)
{
    removePlatformPowerUp(p);
    p->node->remove();
}

void genPlatforms(core::vector3df pos, int depth) // called once at level start
{
    if (depth <= 0) return;
    
    std::vector <core::vector3df> positions;
    
    for (f32 angle = 0; angle < 360; angle += platformStep) // step ensures limited possibilities and not too close together
    {
        f32 angleRadians = angle * M_PI/180.0;
        
        core::vector3df newPos = pos;
        newPos.X += cos(angleRadians) * maxPlayerJump;
        newPos.Z += sin(angleRadians) * maxPlayerJump;
        
        int i = 0;
        for (; i < platforms.size(); i ++)
        {
            if (platforms[i].node->getAbsolutePosition().getDistanceFrom(newPos) < minPlatformSeparation)
            {
                break;
            }
        }
        
        if (i == platforms.size()) positions.push_back(newPos);
    }
    
    std::vector <core::vector3df> chosenPositions;
    
    int count = 20;//rand()%3 + 2;
    
    for (int i = 0; i < count; i ++)
    {
        if (positions.size() <= 0)
        {
            //printf("Note: low number of platform positions. Gameplay may be compromised!\n");
            break;
        }
        
        int j = rand()%positions.size();
        chosenPositions.push_back(positions[j]);
        positions.erase(positions.begin()+j);
    }
    
    for (int i = 0; i < chosenPositions.size(); i ++)
    {
        createPlatform(chosenPositions[i]);// + core::vector3df(0,depth*10,0));
    }
    
    // recurse
    depth --;
    for (int i = 0; i < chosenPositions.size(); i ++)
    {
        genPlatforms(chosenPositions[i], depth);
    }
}

void createPlatforms()
{
    core::vector3df startPos = core::vector3df(0,0,0);
    createPlatform(startPos);
    lastSelected = NULL;
}

void removePlatforms()
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        destroyPlatform(&platforms[i]);
    }
    platforms.clear();
    for (int i = 0; i < fallingPlatforms.size(); i ++)
    {
        destroyPlatform(&fallingPlatforms[i]);
    }
    fallingPlatforms.clear();
    for (int i = 0; i < spidersGoingUp.size(); i ++)
    {
        spidersGoingUp[i]->remove();
    }
    spidersGoingUp.clear();
    for (int i = 0; i < fallingItems.size(); i ++)
    {
        fallingItems[i].node->remove();
    }
    fallingItems.clear();
}

void updatePlatforms(double dt) // creates new if necessary
{
    setScoreDisplayCount(score_nutCount);
    
    core::vector3df playerPos = getPlayerPos();
    playerPos.Y = 0;
    genPlatforms(playerPos, 3);
    
    for (int i = 0; i < platforms.size(); i ++)
    {
        core::vector3df pos = platforms[i].node->getAbsolutePosition();
        
        // growing (scale)
        core::vector3df scale = platforms[i].node->getScale();
        if (scale.Y < 1.0)
        {
            f32 inc =  1.0 * dt;
            scale.X += inc;
            scale.Y += inc;
            scale.Z += inc;
            
            if (scale.Y > 1.0)
            {
                scale = core::vector3df(1.0,1.0,1.0);
            }
            platforms[i].node->setScale(scale);
        }
        
        // spider?
        if (platforms[i].powerUp && platforms[i].powerUpType == POWERUP_SPIDER)
        {
            core::vector3df sPos = platforms[i].powerUp->getPosition();
            if (sPos.Y > 0)
            {
                sPos.Y -= 20.0*dt; // make it fall if in air
            }
            else
            {
                sPos.Y = 0;
            }
            platforms[i].powerUp->setPosition(sPos);
            
            // rotate to face player
            f32 angle = (getPlayerPos()-sPos).getHorizontalAngle().Y;
            platforms[i].powerUp->setRotation(core::vector3df(-90,angle,0));
        }
        
        // age
        if (platforms[i].touched) platforms[i].lifeTime += dt;
        
        // selectable?
        core::vector2df pos2d = toVector2d(pos);
        core::vector2df playerPos2d = toVector2d(playerPos);
        platforms[i].nodeSelectable->setVisible(pos2d.getDistanceFrom(playerPos2d) <= maxPlayerJump
            && pos2d != playerPos2d//.getDistanceFrom(playerPos2d) >= minPlatformSeparation
            && playerOnPlatform());
        
        // emissive colour, in case changed by selector?
        platforms[i].nodeSelectable->getMaterial(0).EmissiveColor = ringEmissive;
        platforms[i].nodeSelectable->getMaterial(0).EmissiveColor.setGreen(100);
        platforms[i].nodeSelectable->setScale(core::vector3df(1,1,1));
        
        // nearly falling
        if (platforms[i].lifeTime > platformLifeLength-1.0)
        {
            
            if (!platforms[i].readyToFall)
            {
                if (onPlatform(toVector2d(getPlayerPos()))) sengine->play2D("media/cardooropen.wav");
                platforms[i].readyToFall = true;
            }
            
            //platforms[i].node->setRotation(platforms[i].node->getRotation()
            //    + core::vector3df(0,10.0,0));
            pos.Y -= 1.0 * dt;
            platforms[i].node->setPosition(pos);
        }
        
        // falling
        if ( (pos.getDistanceFrom(playerPos) > FALL_AT_DIST && playerOnPlatform())
            || platforms[i].lifeTime > platformLifeLength)
        {
            if (pos.getDistanceFrom(playerPos) < FALL_AT_DIST)
            {
                sengine->play2D("media/zoomaway.wav");
            }
            
            platforms[i].nodeSelectable->remove();
            fallingPlatforms.push_back(platforms[i]);
            platforms.erase(platforms.begin()+i);
            i --;
        }
    }
    
    for (int i = 0; i < fallingPlatforms.size(); i ++)
    {
        core::vector3df pos = fallingPlatforms[i].node->getAbsolutePosition();
        
        pos.Y -= 8.0 * dt * (-pos.Y/4.0+1.0);
        
        if (fallingPlatforms[i].powerUp && fallingPlatforms[i].powerUpType == POWERUP_SPIDER)
        {
            fallingPlatforms[i].powerUp->setPosition(fallingPlatforms[i].powerUp->getPosition()
                + core::vector3df(0,30.0,0)*dt);
        }
        
        if (pos.Y < -itemFallDist)
        {
            destroyPlatform(&fallingPlatforms[i]);
            fallingPlatforms.erase(fallingPlatforms.begin()+i);
            i --;
        }
        else
        {
            fallingPlatforms[i].node->setPosition(pos);
        }
    }
    
    for (int i = 0; i < spidersGoingUp.size(); i ++)
    {
        core::vector3df pos = spidersGoingUp[i]->getAbsolutePosition();
        pos.Y += 20.0*dt;
        spidersGoingUp[i]->setPosition(pos);
        
        if (pos.Y > 30)
        {
            spidersGoingUp[i]->remove();
            spidersGoingUp.erase(spidersGoingUp.begin()+i);
            i --;
        }
    }
    
    for (int i = 0; i < fallingItems.size(); i ++)
    {
        core::vector3df pos = fallingItems[i].node->getPosition();
        pos += fallingItems[i].move * dt;
        fallingItems[i].move.Y -= fallingItems[i].acc*dt;
        fallingItems[i].node->setPosition(pos);
        
        if (pos.Y < -itemFallDist)
        {
            fallingItems[i].node->remove();
            fallingItems.erase(fallingItems.begin()+i);
            i --;
        }
    }
}

bool getClosestPlatformExcluding(core::vector2df pos, core::vector2df ex, core::vector2df &dest)
{
    f32 lastDist = 100000.0;
    bool found = false;
    for (int i = 0; i < platforms.size(); i ++)
    {
        core::vector2df platPos = toVector2d(platforms[i].node->getAbsolutePosition());
        f32 dist = pos.getDistanceFrom(platPos);
        f32 dist2 = ex.getDistanceFrom(platPos);
        
        if (dist < lastDist && platPos != ex && dist2 <= maxPlayerJump)
        {
            lastDist = dist;
            dest = platPos;
            found = true;
        }
    }
    
    return found;
}

void setTouchedPlatform(core::vector2df platPos)
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        if (platPos == toVector2d(platforms[i].node->getAbsolutePosition()))
        {
            if (platforms[i].powerUp)
            {
                if (platforms[i].powerUpType == POWERUP_HAZELNUT)
                {
                    score_nutCount ++;
                    sengine->play2D("media/blink.wav");
                    removePlatformPowerUp(&platforms[i]);
                }
                else if (platforms[i].powerUpType == POWERUP_SPIDER)
                {
                    sengine->play2D("media/goopy.wav");
                    
                    spidersGoingUp.push_back(platforms[i].powerUp);
                    platforms[i].powerUp = NULL;
                    platforms[i].powerUpType = POWERUP_NONE;
                    
                    // loses some nuts
                    int toLose = 10 + rand()%10;
                    if (score_nutCount-toLose < 0) toLose = score_nutCount;
                    score_nutCount -= toLose;
                    
                    for (int j = 0; j < toLose; j ++)
                    {
                        f32 angle = rand()%360;
                        core::vector3df move = core::vector3df(
                            cos(angle * M_PI/180.0)*8.0,
                            10.0,
                            -sin(angle * M_PI/180.0)*8.0
                            );
                        move *= RAND_F*2.0;
                        FallingItem item = 
                        {
                            smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/hazelnut.b3d")),
                            move,
                            16.0 + RAND_F*6.0// acceleration due to gravity
                        };
                        item.node->setPosition(getPlayerPos());
                        item.node->updateAbsolutePosition();
                        item.node->setRotation(core::vector3df(rand()%360,rand()%360,rand()%360));
                        fallingItems.push_back(item);
                    }
                }
            }
            platforms[i].touched = true;
        }
    }
}

bool onPlatform(core::vector2df platPos)
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        if (platPos == toVector2d(platforms[i].node->getAbsolutePosition()))
        {
            return true;
        }
    }
    return false;
}

void setSelected(core::vector2df platPos)
{
    for (int i = 0; i < platforms.size(); i ++)
    {
        if (platPos == toVector2d(platforms[i].node->getAbsolutePosition()))
        {
            if (lastSelected != platforms[i].nodeSelectable)
            {
                //sengine->play2D("media/blip.wav");
            }
            platforms[i].nodeSelectable->getMaterial(0).EmissiveColor = ringEmissive;
            platforms[i].nodeSelectable->setScale(core::vector3df(1,1,1)*1.2);
            lastSelected = platforms[i].nodeSelectable;
        }
    }
}


