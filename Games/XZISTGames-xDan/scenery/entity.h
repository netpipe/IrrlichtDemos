
#ifndef __ENTITY_H
#define __ENTITY_H

#include <ode/ode.h>
#include "irrInfo.h"
#include "collision.h"
#include <vector>

// AI, FIXED state of the entity (although may be individual to each - its personality)
struct EntityState
{
    EntityState()
    {
        agression = 1;
        defense = 1;
        flee = 1;
        follow = 1;
        accuracy = 1;
    }
    // these are all likelyhoods, in no way certainties.
    float agression;    // attack with no regard to safety (e.g. frontal attack into gunfire)
    float defense;      // defensiveness (e.g. distance maintained from enemy)
    float flee;         // run away when damaged?
    float follow;       // may follow another entity
    float accuracy;     // mixing value
};

// AI, entities analysis of another entity (each entity will maintain this for all others).
// whereas states are likelyhoods, the analysis is certain
// might be mixed a set amount by the state, but this will occur equally with respect
// to all other entities.
struct EntityAnalysis
{
    EntityAnalysis(Entity *whose)
    {
        this->whose = whose;
        attack = 0;
        attraction = 0;
        fear = 0;
    }
    // the entity whose opinion it is...
    Entity *whose;
    // > 0 == potential target. will hunt the entity with greatest.
    // may shoot all potential targets on passing (with unlimited ammo weapons at least).
    int attack;
    // > 0 == may follow this entity provided no other pressing engagements.
    // entity attack() function sets attack and attraction to highest.
    int attraction;
    // > 0 may run away... as long as attraction is not greater.
    int fear;
};

class Entity
{
public:
    Entity(irrInfo *);
    ~Entity();
    
    // these functions do not just apply to spaceships. ground turrets?
    void setAuto(bool);
    // yup, even boxes could be hostile. Towards other boxes even...
    void attack(Entity *enemy);
    
    void RefreshAI();
    
    virtual void Refresh() = 0;
    virtual void setPosition(dReal, dReal, dReal) = 0;
    virtual dReal *getPosition() = 0;
    virtual vector3df getRotation() = 0;
    virtual const dReal *getRotationMatrix() = 0;
    
    // movement functions. used by both AI and player control.
    // if entity is immobile they just won't be used.
    virtual void goForwards() = 0;
    virtual void goLeft() = 0;
    virtual void goRight() = 0;
    
    // AI, other entities' opinions of this one.
    void setOpinion(EntityAnalysis);
    EntityAnalysis getOpinion(Entity *);
    
protected:
    irrInfo *info;
    dWorldID dWorld;
    dSpaceID dSpace;
    
    // list of all other entities. may not modify it.
    // list may be dynamically modified at top level, so every refresh should check other exists before accessing it.
    std::vector <Entity *> *allEntities;
    
    void QuaternionToEuler(const dQuaternion, vector3df &);
    
    // used by derived classes, will only add shadow scene node if they are enabled.
    void addShadows(IAnimatedMeshSceneNode *);
    
private:
    int i;
    // AI stuffo!
    bool AI_controlled;
    EntityState myState;
    
    // potential problem: will keep all opinions until this entity dies.
    std::vector <EntityAnalysis> opinionsOfMe;
};

#endif
