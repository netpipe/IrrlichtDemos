
#include "Person.h"

Person::Person(IrrlichtDevice *device) : Target(device)
{
    sn = smgr->addAnimatedMeshSceneNode(smgr->getMesh("person.b3d"));
    ((scene::IAnimatedMeshSceneNode *)sn)->setAnimationSpeed(6000 + rand()%3000);
    sn->setRotation(core::vector3df(-90 - rand()%140,0,0));
    bbExtent = sn->getBoundingBox().getExtent();

    // choose start position
    core::vector3df positions[3] =
    {
        core::vector3df(-4.4,7,-2),
        core::vector3df(0,7,-2),
        core::vector3df(4.4,7,-2)
    };
    core::vector3df pos = positions[rand()%3];
    pos.Y += bbExtent.Y/2.0;
    sn->setPosition(pos);

    // movement
    vel.X = ((float)(rand()%1000 * ((rand()%2) ? -1 : 1))) / 10000.0;
    vel.Z = 2.0;
    vel.Y = 3.0;

    acc.Y = -4.0;

    rvel.X = rand()%5 * ((rand()%2) ? -1 : 1);
    rvel.Y = rand()%5 * ((rand()%2) ? -1 : 1);
    rvel.Z = rand()%5 * ((rand()%2) ? -1 : 1);

    stickAge = 0;

    bigSprayed = false;
}

void Person::secondUpdate(double dt)
{
    core::vector3df pos = sn->getPosition();

    if (inactive)
    {
        if (stickAge && age-stickAge > 1.0 && !slidingDown)
        {
            slidingDown = true;
            stickPos = pos;
            acc.Y = -0.25;//-1.0;

            const static char *bloodStains[] =
            {
                "bloodstain0.tga","bloodstain1.tga","bloodstain2.tga"
            };

            bloodStainTexture = driver->getTexture(bloodStains[rand()%3]);
        }

        // blood stain on wall
        if (pos.Y > 0.01 && slidingDown)
        {
            Splatter bloodSplat;
            bloodSplat.pos = pos;
            bloodSplat.texture = bloodStainTexture;
            splatters.push_back(bloodSplat);
        }
    }

    if (pos.Y < 0)
    {
        if (!inactive)
        {
            emptySceneNode();
            bigSpray();
//            sengine->play2D("sfx/squelch0.wav");
            sn->setPosition(pos);
            failedToHit = true;
            fullnessIncrease += 10;

            if (rand()%100 < 20)
            {
                shellsGiven ++;
            }

            // blood splat on ground
            Splatter bloodSplat;
            bloodSplat.pos = pos;
            bloodSplat.texture = driver->getTexture("blood_dark.tga");
            splatters.push_back(bloodSplat);
        }
        inactive = true;
        removeSoon();
    }
}

void Person::hitScore()
{
    score += 200;
}

void Person::hitEffects(core::line3df ray)
{
    inactive = true;

    stickAge = age;
    slidingDown = false;

    vel = core::vector3df(0,0,0);
    acc = core::vector3df(0,0,0);
    rvel = core::vector3df(0,0,0);

    core::vector3df rot = sn->getRotation();
    rot.Y = 0;
    rot.X = 0;
    rot.Z = rand()%360;
    sn->setRotation(rot);

    core::vector3df pos = sn->getPosition();

    if (pos.Y + bbExtent.Y > 7)
    {
//        sengine->play2D("sfx/squelch0.wav");

        vel = ray.getVector().normalize()*40.0;
        acc.Y = -4.0;
        emptySceneNode();
        bigSpray();
        removeSoon();
        return;
    }

    f32 wallZ = 0.1;
    core::vector3df vec = (pos-smgr->getActiveCamera()->getPosition()).normalize();
    pos = smgr->getActiveCamera()->getPosition() + vec * -(smgr->getActiveCamera()->getPosition().Z-wallZ) / vec.Z;
    pos.Z = wallZ;
    sn->setPosition(pos);

    scene::IAnimatedMeshSceneNode *n = smgr->addAnimatedMeshSceneNode(smgr->getMesh("interior.b3d"), sn);
    n->setMaterialFlag(video::EMF_LIGHTING, false);

//    sengine->play2D("sfx/squelch2.wav");
}

void Person::bigSpray()
{
    if (bigSprayed) return;
    bigSprayed = true;

    scene::IAnimatedMesh *meshes[3] =
    {
        smgr->getMesh("person_gib0.b3d"),
        smgr->getMesh("person_gib1.b3d"),
        smgr->getMesh("person_gib2.b3d")
    };
    particleSpray(
            driver->getTexture("blood.tga"),
            meshes,
            3,
            core::vector3df(0,0,0),
            core::vector3df(0,0.005,0),
            4000, // particles per second
            100, // milliseconds duration of initial spray
            5000 // life time
            );

    particleSpray(
            driver->getTexture("tubing0.tga"),
            NULL,
            0,
            core::vector3df(0,0,0),
            core::vector3df(0,0.007,0),
            100, // particles per second
            50, // milliseconds duration of initial spray
            1000 // life time
            );
    particleSpray(
            driver->getTexture("tubing1.tga"),
            NULL,
            0,
            core::vector3df(0,0,0),
            core::vector3df(0,0.007,0),
            100, // particles per second
            50, // milliseconds duration of initial spray
            1000 // life time
            );
}

