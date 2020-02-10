
#include "Duck.h"

Duck::Duck(IrrlichtDevice *device) : Target(device)
{
    sn = smgr->addAnimatedMeshSceneNode(smgr->getMesh("duck.x"));
    ((scene::IAnimatedMeshSceneNode *)sn)->setAnimationSpeed(100);
    bbExtent = sn->getBoundingBox().getExtent();

    // choose start position
    int side = rand()%2;
    core::vector3df positions[2] =
    {
        core::vector3df(-9,0,2),
        core::vector3df(9,0,2)
    };
    core::vector3df pos = positions[side];
    pos.Y = 0.08;
    pos.Z = 1 + rand()%5;
    sn->setPosition(pos);

    // movement
    moveDir = side ? -1 : 1;
    vel.X = side ? -1 : 1;
    sn->setRotation(core::vector3df(90,-90*vel.X,0));
    vel.X *= 1.0;
    vel.X += ((float)(rand()%1000)) / 10000.0;

    shotOnce = false;
}

void Duck::secondUpdate(double dt)
{
    core::vector3df pos = sn->getPosition();

    if (fabs(pos.X) > 10)
    {
        removable = true;
    }
}

void Duck::hitScore()
{
    score += 50;
    shellsGiven += 2;
}

void Duck::hitEffects(core::line3df ray)
{
    // blood splat on ground
    Splatter bloodSplat;
    bloodSplat.pos = sn->getPosition();
    bloodSplat.pos.Y = -1;
    bloodSplat.texture = driver->getTexture("blood_dark.tga");
    splatters.push_back(bloodSplat);

    if (!shotOnce)
    {
        shotOnce = true;

        vel.X *= 5;
        ((scene::IAnimatedMeshSceneNode *)sn)->setAnimationSpeed(200);

//        sengine->play2D("sfx/squawk.wav");

        scene::IParticleSystemSceneNode *ps = particleSpray(
            driver->getTexture("blood.tga"),
            NULL,
            0,
            core::vector3df(0,0,0),
            core::vector3df(0,0,-0.003),
            100, // particles per second
            10000, // milliseconds duration of initial spray
            1000 // life time
            );

        if (moveDir > 0)
        {
            ps->setPosition(core::vector3df(0.1,-0.15,-0.4)); // z,x,y??? (as node is rotated)
        }
        else
        {
            ps->setPosition(core::vector3df(-0.1,-0.1,-0.4));
        }
    }
    else
    {

        score += 50;
        shellsGiven += 3;

        inactive = true;
//        sengine->play2D("sfx/squelch0.wav");

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
            driver->getTexture("feather.tga"),
            NULL,
            0,
            core::vector3df(0,0,0),
            core::vector3df(0,0.007,0),
            1000, // particles per second
            50, // milliseconds duration of initial spray
            1000 // life time
            );

        // stop moving
        vel = core::vector3df(0,0,0);

        // explode + empty node
        removeSoon();
    }
}
