
#include "Target.h"
#include <assert.h>

Target::Target(IrrlichtDevice *device) : Effects(device)
{
//    this->sengine = sengine;
    age = 0;
    removable = false;
    inactive = false;
    removeSoonAge = 0;
    score = 0;
    failedToHit = false;
    shellsGiven = 0;
    fullnessIncrease = 0;
}

Target::~Target()
{
    assert(sn);
    sn->remove();
}

bool Target::update(double dt)
{
    if (!removable)
    {
        age += dt;

        core::vector3df pos = sn->getPosition() + vel*dt;
        sn->setPosition(pos);
        vel += acc*dt;

        sn->setRotation(sn->getRotation() + rvel*dt);

        secondUpdate(dt);

        if (removeSoonAge && age-removeSoonAge > 6.0)
        {
            removable = true;
        }
    }

    return !removable;
}

void Target::secondUpdate(double dt)
{
    // do nothing...
}

bool Target::shoot(core::line3df ray)
{
    if (inactive) return false;

    //if (sn == smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray))
    // (removed as it didn't seem to work with Duck. Because Duck was rescaled?
    if (
        ray.getClosestPoint(sn->getPosition()).getDistanceFrom(sn->getPosition())
        < 0.5
        // this removed as seems to be other problems with Duck's BB box...
        //bbExtent.getLength()/3.0
        )
    {
        hit(ray);
        return true;
    }
    return false;
}

int Target::takeScore()
{
    if (score > 0) flyText((core::stringw)score);
    int temp = score;
    score = 0;
    return temp;
}

bool Target::takeUnhitFlag()
{
    bool temp = failedToHit;
    failedToHit = false;
    return temp;
}

int Target::takeShellsGiven()
{
    if (shellsGiven > 0)
    {
        scene::IAnimatedMesh **meshes = new scene::IAnimatedMesh*[shellsGiven];
        for (int i = 0; i < shellsGiven; i ++)
        {
            meshes[i] = smgr->getMesh("shell.b3d");
        }
        core::vector3df rvel;
        rvel.X = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Y = rand()%100 * ((rand()%2) ? -1 : 1);
        rvel.Z = rand()%100 * ((rand()%2) ? -1 : 1);
        core::vector3df vec = (smgr->getActiveCamera()->getPosition()-sn->getPosition()).normalize();
        vec *= 4.0;
        vec.Y = 4.5;
        meshSpray(meshes, shellsGiven,
            core::vector3df(0,0,0),//rand()%200-100,0,rand()%200-100)/100.0,
            vec,
            core::vector3df(0,-5.0,0),
            rvel);
    }
    int temp = shellsGiven;
    shellsGiven = 0;
    return temp;
}

int Target::takeFullnessIncrease()
{
    int temp = fullnessIncrease;
    fullnessIncrease = 0;
    return temp;
}

std::vector <Splatter> Target::getNewSplatters()
{
    std::vector <Splatter> temp = splatters;
    splatters.clear();
    return temp;
}

void Target::emptySceneNode()
{
    core::vector3df pos = sn->getPosition();
    sn->remove();
    sn = smgr->addEmptySceneNode();
    sn->setPosition(pos);
}

void Target::removeSoon()
{
    if (!removeSoonAge) removeSoonAge = age;
}

void Target::hit(core::line3df ray)
{

    // blood splat on wall
    Splatter bloodSplat;
    if (core::plane3df(0,0,0, 0,0,1).getIntersectionWithLine(ray.start, ray.getVector(), bloodSplat.pos))
    {
        bloodSplat.texture = driver->getTexture("blood_dark.tga");
        splatters.push_back(bloodSplat);
    }

    // scoring?
    hitScore();
    hitEffects(ray);
}

