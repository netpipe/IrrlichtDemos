
#include "rabbit.h"
#include "platforms.h"
#include <stdlib.h>
//#include <irrKlang.h>
#include "stringaling.h"
#include "event.h"
#include "text.h"

//using namespace audio;

//extern ISoundEngine *sengine;
extern scene::ISceneManager *smgr;
extern core::vector3df firstCameraPosition;
extern core::vector3df firstCameraTarget;
extern video::IVideoDriver *driver;

scene::IAnimatedMeshSceneNode *node = NULL;
scene::IParticleSystemSceneNode *nodePS = NULL;

f32 rabbitRadius;               // used for collisions. half rabbit bounding box height
const f32 yVelMax = 100.0;     // max fall velocity
const f32 yInc = 100.0;         // fall acceleration (due to gravity)
const f32 jumpVelMax = 100.0;    // first jump acceleration
f32 jumpVel;                    // current jump acceleration
f32 yVel;                       // current fall velocity
const f32 bumpFactor = 5.0;
const f32 fallDistance = 200.0; // distance of no return... i.e. cannot jump and recover.
f32 startHeight = 20.0;

bool rabbitLetLoose;    // gravity enabled...
bool rabbitFallen;      // falling to death :-(

void createRabbit()
{
    node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/rabbit.b3d"));
    node->setRotation(core::vector3df(90,0,0));
    //f32 s = 2.0;
    //node->setScale(core::vector3df(s,s,s));
    node->setAnimationSpeed(12000);
    rabbitRadius = node->getBoundingBox().getExtent().Y/2.0;
    yVel = 0.0;
    jumpVel = jumpVelMax;
    rabbitLetLoose = false;
    rabbitFallen = false;
    node->setPosition(core::vector3df(0,startHeight,0));
    node->updateAbsolutePosition();

    nodePS = smgr->addParticleSystemSceneNode(
        false, node, -1, core::vector3df(1,1,2)
        );

    // create and set emitter
    scene::IParticleEmitter *em = nodePS->createBoxEmitter(
        core::aabbox3d<f32>(-1,-1,-1,1,1,1),
        core::vector3df(0,-1.0,-1.0)*0.025,
		40,50,
		video::SColor(255,255,255,255), video::SColor(255,255,255,255),
		// min, max lifetime of particles
		1000,1000,
		// max angle
        20);
	nodePS->setEmitter(em);
	em->drop();

	// adjust some material settings
    nodePS->setMaterialFlag(video::EMF_LIGHTING, false);
	nodePS->setMaterialTexture(0, driver->getTexture("media/star.png"));
	nodePS->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
}

void updateRabbit(double dt)
{
    core::vector3df pos = node->getAbsolutePosition();

    //node->setRotation(node->getRotation()+core::vector3df(0,2,0));

    core::aabbox3df box;

    // collisions above and below, front and back of platforms
    if (getPlatformCollision(pos+core::vector3df(0,rabbitRadius,-rabbitRadius), &box)
        || getPlatformCollision(pos+core::vector3df(0,-rabbitRadius,-rabbitRadius), &box)
        || getPlatformCollision(pos+core::vector3df(0,rabbitRadius,rabbitRadius), &box)
        || getPlatformCollision(pos+core::vector3df(0,-rabbitRadius,rabbitRadius), &box)
        ||
            (
            //getPlatformCollision(pos+core::vector3df(0,-rabbitRadius-startHeight,0), &box)
            getClosestPlatformDistance2D(pos) < 40.0
            && !rabbitLetLoose
            )
        )
    {
        if (!rabbitLetLoose)
        {
            core::stringw str = L"GO!";
            shoutMessage(str, screenHH());
            rabbitLetLoose = true;
        }

        f32 overY = box.getCenter().Y + box.getExtent().Y/2.0 + rabbitRadius;
        f32 underY = box.getCenter().Y - box.getExtent().Y/2.0 - rabbitRadius;

        if (pos.Y > box.getCenter().Y)
        {
            // on platform
            jumpVel = jumpVelMax;

            if (yVel > 0)
            {
                yVel = 0.0;
                pos.Y += (overY-pos.Y)*bumpFactor * dt;
            }
        }
        else
        {
            // below platform
            if (yVel < 0)
            {
                yVel = 0.0;
                pos.Y -= (pos.Y-underY)*bumpFactor * dt;
            }
        }
    }


    if (rabbitLetLoose) pos.Y -= yVel*dt;

    node->setPosition(pos);
    if (!rabbitFallen)
    {
        smgr->getActiveCamera()->setPosition(pos+firstCameraPosition);//+core::vector3df(100,0,30));

        if (yVel >= 0)
        {
            node->setRotation(core::vector3df(90, 0,0));
        }
        else
        {
            node->setRotation(core::vector3df(45, 0,0));
        }
        node->setLoopMode(yVel >= 0);
        //nodePS->setVisible(yVel < 0);
    }
    else
    {
        //smgr->getActiveCamera()->setPosition(pos+core::vector3df(0,20.0-pos.Y,-30.0));
        node->setRotation(node->getRotation()+core::vector3df(2,2,2)*60.0*dt);
    }
    smgr->getActiveCamera()->setTarget(pos+firstCameraTarget);

    if (rabbitLetLoose) yVel += yInc*dt;
    if (yVel > yVelMax) yVel = yVelMax;

    node->updateAbsolutePosition(); // fix1
}

void jumpRabbit()
{
    if (rabbitFallen || !rabbitLetLoose) return;

    if (jumpVel > 1.0)
    {
        yVel = -jumpVel;
        jumpVel /= 2.0;
    }
//    sengine->play2D(strparty("media/la", rand()%3, ".wav").c_str());
}

void removeRabbit()
{
    if (node)
    {
        node->remove();
        node = NULL;
    }
}

bool rabbitHitNode(scene::ISceneNode *otherNode)
{
    core::vector2df pos1 = core::vector2df(node->getAbsolutePosition().Y, node->getAbsolutePosition().Z);
    core::vector2df pos2 = core::vector2df(otherNode->getAbsolutePosition().Y, otherNode->getAbsolutePosition().Z);

    f32 otherRadius = otherNode->getBoundingBox().getExtent().getLength();// / 2.0;

    return (pos1.getDistanceFrom(pos2) < rabbitRadius+otherRadius);
}

bool rabbitDied()
{
    f32 diff = node->getAbsolutePosition().Y-getLowestPlatformPoint().Y;

    if (diff < -fallDistance && !rabbitFallen)
    {
//        sengine->play2D(strparty("media/die", rand()%3, ".wav").c_str());
        rabbitFallen = true;

        freezePlatforms();
    }

    return (diff < -fallDistance-400.0);
}

