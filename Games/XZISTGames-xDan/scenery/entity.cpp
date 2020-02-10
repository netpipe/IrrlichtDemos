
#include "entity.h"

Entity::Entity(irrInfo *info)
{
    this->info = info;
    this->dWorld = info->dWorld;
    this->dSpace = info->dSpace;
    this->allEntities = info->entities;
    allEntities->push_back(this);
    AI_controlled = false;
}

Entity::~Entity()
{
}

void Entity::setAuto(bool flag)
{
    this->AI_controlled = flag;
}

void Entity::attack(Entity *enemy)
{
    // break this down into smaller functions? e.g. findHighestAttacker()...
    bool found = false;
    int highAttack = 0;
    int highAttraction = 0;

    // through all entities.
    for (i = 0; i < allEntities->size(); i ++)
    {
        // ignore own pointer
        if ( (*allEntities) [i] == this) continue;

        // get this entity's opinion of the other entity
        EntityAnalysis opinionOfOther = (*allEntities) [i]->getOpinion(this);

        // remember highest attack values etc.
        if (opinionOfOther.attack > highAttack) highAttack = opinionOfOther.attack;
        if (opinionOfOther.attraction > highAttraction) highAttraction = opinionOfOther.attraction;

        // flag the enemy actually found in allEntities list.
        if ( (*allEntities) [i] == enemy)
        {
            found = true;
        }
    }

    // if enemy is still there, set this entity's opinion of it to be just higher than all others
    // (opinion being in this case the urgeto attack)
    if (found)
    {
        // get this entity's opinion of other
        EntityAnalysis opinionOfOther = enemy->getOpinion(this);
        // set it to highest attack priorities etc...
        opinionOfOther.attack = highAttack + 1;
        opinionOfOther.attraction = highAttraction + 1;
        opinionOfOther.fear = 0;
        // send back the modified version
        enemy->setOpinion(opinionOfOther);
    }
}

void Entity::RefreshAI()
{
    if (!AI_controlled) return;

    dReal nextPos[3] = {0,0,0};

    int highAttack = 0;

    // through all entities.
    for (i = 0; i < allEntities->size(); i ++)
    {
        // ignore own pointer
        if ( (*allEntities) [i] == this) continue;

        // get this entity's opinion of the other entity
        EntityAnalysis opinionOfOther = (*allEntities) [i]->getOpinion(this);

        if (opinionOfOther.attack > highAttack)
        {
            highAttack = opinionOfOther.attack;
            // overkill using functions, optimise!
            nextPos[0] = (*allEntities) [i]->getPosition() [0];
            nextPos[1] = (*allEntities) [i]->getPosition() [1];
            nextPos[2] = (*allEntities) [i]->getPosition() [2];
        }
    }

    if (highAttack > 0)
    {
        // attack!
        // will need move up/down code when more advanced ships are created...or hovercraft can go up/down.

        dReal *myPos = getPosition();

        const dReal *currentAimMatrix = getRotationMatrix();

        vector3df currentAim = vector3df(currentAimMatrix[0], currentAimMatrix[4], currentAimMatrix[8]);
        vector3df desiredAim = vector3df(nextPos[0]-myPos[0], nextPos[1]-myPos[1], nextPos[2]-myPos[2]);

        currentAim.normalize();
        desiredAim.normalize();

        double angle = acos(currentAim.dotProduct(desiredAim));

        if (angle < 1.57)
        {
            goLeft();
        }
        else
        {
            goRight();
        }

        goForwards();
    }

    // here will go through all other entities (like in attack() code)
    // for simple testing, use movement functions to move towards the one
    // with highest attack priority (with all weighting etc... but not in testing!)
}

void Entity::setOpinion(EntityAnalysis opinion)
{
    // find if already has one, if so replace.
    for (i = 0; i < opinionsOfMe.size(); i ++)
    {
        if (opinionsOfMe[i].whose == opinion.whose)
        {
            opinionsOfMe[i] = opinion;
            return;
        }
    }
    // add new opinion
    opinionsOfMe.push_back(opinion);
}

EntityAnalysis Entity::getOpinion(Entity *you)
{
    // enable when needed, but check it works first.
    for (i = 0; i < opinionsOfMe.size(); i ++)
    {
        if (opinionsOfMe[i].whose == you)
        {
            return opinionsOfMe[i];
        }
    }
    // create an opinion.
    EntityAnalysis newOpinion(you);
    opinionsOfMe.push_back(newOpinion);
    return newOpinion;
}

// stolen function
void Entity::QuaternionToEuler(const dQuaternion quaternion, vector3df &euler)
{
    dReal w,x,y,z;
    w=quaternion[0];
    x=quaternion[1];
    y=quaternion[2];
    z=quaternion[3];
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;
    euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*irr::core::RADTODEG);
    euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*irr::core::RADTODEG);
    euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*irr::core::RADTODEG);
}

void Entity::addShadows(IAnimatedMeshSceneNode *node)
{
    if (info->Options.Screen.Stencilbuffer)
    {
        //node->addShadowVolumeSceneNode(-1, false);
        node->addShadowVolumeSceneNode();
    }
    // random stuffo
    node->setMaterialFlag(EMF_LIGHTING, true);
    node->setMaterialFlag(EMF_FOG_ENABLE, true);
}
