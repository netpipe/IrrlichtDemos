
#include "Demon.h"

Demon::Demon(IrrlichtDevice *device) : Target(device)
{
    sn = smgr->addAnimatedMeshSceneNode(smgr->getMesh("demon.b3d"));
    ((scene::IAnimatedMeshSceneNode *)sn)->setAnimationSpeed(6000);
    bbExtent = sn->getBoundingBox().getExtent();

    // choose start position
    int side = rand()%2;
    core::vector3df positions[2] =
    {
        core::vector3df(-9,0,2),
        core::vector3df(9,0,2)
    };
    core::vector3df pos = positions[side];
    pos.Y = 2 + rand()%2;
    pos.Z = 2 + (rand()%200)/100.0;
    sn->setPosition(pos);

    // movement
    moveDir = side ? -1 : 1;
    vel.X = side ? -1 : 1;
    sn->setRotation(core::vector3df(0,-90*vel.X,0));
    vel.X *= ((float)(100+rand()%400)) / 100.0;

    acc.Y = ((float)(200+rand()%300)) / 100.0;

    const static char *sfx[] = {"sfx/demon0.wav", "sfx/demon1.wav"};
//    sengine->play2D(sfx[rand()%2]);
}

void Demon::secondUpdate(double dt)
{
    core::vector3df pos = sn->getPosition();

    f32 maxVel = ((float)(100+rand()%500)) / 100.0;

    if (vel.Y > maxVel)
    {
        acc.Y = -acc.Y;
        vel.Y = maxVel;
    }
    if (vel.Y < -maxVel)
    {
        acc.Y = -acc.Y;
        vel.Y = -maxVel;
    }

    if (fabs(pos.X) > 10)
    {
        removable = true;
    }
}

void Demon::hitScore()
{
    score += 100;
    fullnessIncrease -= 50;
    shellsGiven += rand()%5;
}

void Demon::hitEffects(core::line3df ray)
{
    inactive = true;
//    sengine->play2D("sfx/demondie.wav");

    emptySceneNode();

    particleSpray(
        driver->getTexture("blood.tga"),
        NULL,
        0,
        core::vector3df(0,0,0),
        core::vector3df(0,0.0025,0.005),
        4000, // particles per second
        100, // milliseconds duration of initial spray
        2000 // life time
        );

    particleSpray(
        driver->getTexture("spider.tga"),
        NULL,
        0,
        core::vector3df(0,0,0),
        core::vector3df(0,0.007,0),
        1000, // particles per second
        50, // milliseconds duration of initial spray
        1000, // life time
        0.5 // particle size
        );

    // stop moving
    vel = core::vector3df(0,0,0);

    // explode + empty node
    removeSoon();
}
