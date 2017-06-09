// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollManager.h"
#include "CCollObject.h"
#include "CCollEntity.h"


namespace COLL
{

//! createCollisionManager
ICollManager* createCollisionManager(IrrlichtDevice* Device, float GravityStrength, float BaseFriction)
{
    CCollManager* manager = new CCollManager(Device, GravityStrength, BaseFriction);

    return manager;
}

//! addSelector
ICollSelector* CCollManager::addSelector()
{
    CCollSelector* selector = new CCollSelector();
    selectors.push_back(selector);
    return selector;
}

ICollSelector* CCollManager::addSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
{
    CCollSelector* selector = new CCollSelector(MeshBuffer, Matrix);
    selectors.push_back(selector);
    return selector;
}

ICollSelector* CCollManager::addSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
{
    CCollSelector* selector = new CCollSelector(Mesh, Matrix);
    selectors.push_back(selector);
    return selector;
}


//! addBBoxSelector
ICollBBoxSelector* CCollManager::addBBoxSelector()
{
    CCollBBoxSelector* selector = new CCollBBoxSelector();
    selectors.push_back(selector);
    return selector;
}

ICollBBoxSelector* CCollManager::addBBoxSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
{
    CCollBBoxSelector* selector = new CCollBBoxSelector(MeshBuffer, Matrix);
    selectors.push_back(selector);
    return selector;
}

ICollBBoxSelector* CCollManager::addBBoxSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
{
    CCollBBoxSelector* selector = new CCollBBoxSelector(Mesh, Matrix);
    selectors.push_back(selector);
    return selector;
}


//! addOctreeSelector
ICollOctreeSelector* CCollManager::addOctreeSelector()
{
    CCollOctreeSelector* selector = new CCollOctreeSelector();
    selectors.push_back(selector);
    return selector;
}

ICollOctreeSelector* CCollManager::addOctreeSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
{
    CCollOctreeSelector* selector = new CCollOctreeSelector(MeshBuffer, Matrix);
    selectors.push_back(selector);
    return selector;
}

ICollOctreeSelector* CCollManager::addOctreeSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
{
    CCollOctreeSelector* selector = new CCollOctreeSelector(Mesh, Matrix);
    selectors.push_back(selector);
    return selector;
}


//! addTerrainSelector
ICollTerrainSelector* CCollManager::addTerrainSelector()
{
    CCollTerrainSelector* selector = new CCollTerrainSelector();
    selectors.push_back(selector);
    return selector;
}

ICollTerrainSelector* CCollManager::addTerrainSelector(const scene::IMeshBuffer* MeshBuffer, const core::matrix4& Matrix)
{
    CCollTerrainSelector* selector = new CCollTerrainSelector(MeshBuffer, Matrix);
    selectors.push_back(selector);
    return selector;
}

ICollTerrainSelector* CCollManager::addTerrainSelector(const scene::IMesh* Mesh, const core::matrix4& Matrix)
{
    CCollTerrainSelector* selector = new CCollTerrainSelector(Mesh, Matrix);
    selectors.push_back(selector);
    return selector;
}

ICollTerrainSelector* CCollManager::addTerrainSelector(const scene::ITerrainSceneNode* TerrainNode, int LODLevel, const core::matrix4& Matrix)
{
    CCollTerrainSelector* selector = new CCollTerrainSelector(TerrainNode, LODLevel, Matrix);
    selectors.push_back(selector);
    return selector;
}


//! addSphereSampler
ICollSphereSampler* CCollManager::addSphereSampler(const core::vector3df& Scale, const core::vector3df& Origin,
                                                   float StandValue, float SlideValue, float InverseMass, float BounceFactor)
{
    CCollSphereSampler* sampler = new CCollSphereSampler(Scale, Origin, StandValue, SlideValue, InverseMass, BounceFactor);
    samplers.push_back(sampler);
    return sampler;
}

//! addEllipsoidSampler
ICollEllipsoidSampler* CCollManager::addEllipsoidSampler(const core::vector3df& Scale, const core::vector3df& Origin,
                                                         float StandValue, float SlideValue, float InverseMass, float BounceFactor)
{
    CCollEllipsoidSampler* sampler = new CCollEllipsoidSampler(Scale, Origin, StandValue, SlideValue, InverseMass, BounceFactor);
    samplers.push_back(sampler);
    return sampler;
}


//! addObject
ICollObject* CCollManager::addObject(ICollSelector* Selector, scene::ISceneNode* Node)
{
    if (!Selector) return NULL;

    CCollSelector* selector = dynamic_cast<CCollSelector *>(Selector);
    CCollObject* object = new CCollObject(this, selector, Node);
    objects.push_back(object);
    return object;
}

//! addEntity
ICollEntity* CCollManager::addEntity(const core::vector3df& Pos, ICollSampler* Sampler, scene::ISceneNode* Node)
{
    if (!Sampler) return NULL;

    CCollSampler* sampler = dynamic_cast<CCollSampler *>(Sampler);
    CCollEntity* entity = new CCollEntity(this, Pos, sampler, Node);
    entities.push_back(entity);
    return entity;
}


//! removeSelector
void CCollManager::removeSelector(ICollSelector *Selector)
{
    if (!Selector) return;

    // detach binded selectors from objects
    core::list<CCollObject*>::Iterator oit = objects.begin();
    for (; oit != objects.end(); ++oit) if ((*oit)->getSelector() == Selector) (*oit)->setSelector(NULL);

    // remove from list and drop
    core::list<CCollSelector*>::Iterator sit = selectors.begin();
    for (; sit != selectors.end(); ++sit) if ((*sit) == Selector) { selectors.erase(sit); break; }

    Selector->drop();
}

//! removeSampler
void CCollManager::removeSampler(ICollSampler *Sampler)
{
    if (!Sampler) return;

    // detach binded samplers from entities
    core::list<CCollEntity*>::Iterator eit = entities.begin();
    for (; eit != entities.end(); ++eit) if ((*eit)->getSampler() == Sampler) (*eit)->setSampler(NULL);

    // remove from list and drop
    core::list<CCollSampler*>::Iterator sit = samplers.begin();
    for (; sit != samplers.end(); ++sit) if ((*sit) == Sampler) { samplers.erase(sit); break; }

    Sampler->drop();
}

//! removeObject
void CCollManager::removeObject(ICollObject *Object)
{
    if (!Object) return;

    // remove from list and drop
    core::list<CCollObject*>::Iterator oit = objects.begin();
    for (; oit != objects.end(); ++oit) if ((*oit) == Object) { objects.erase(oit); break; }

    Object->drop();
}

//! removeEntity
void CCollManager::removeEntity(ICollEntity *Entity)
{
    if (!Entity) return;

    // remove from list and drop
    core::list<CCollEntity*>::Iterator eit = entities.begin();
    for (; eit != entities.end(); ++eit) if ((*eit) == Entity) { entities.erase(eit); break; }

    Entity->drop();
}

//! removeSelectors
void CCollManager::removeSelectors()
{
    // detach binded selectors from objects
    core::list<CCollObject*>::Iterator oit = objects.begin();
    for (; oit != objects.end(); ++oit) if ((*oit)->getSelector()) (*oit)->setSelector(NULL);

    // remove from list and drop
    core::list<CCollSelector*>::Iterator sit = selectors.begin();
    for (; sit != selectors.end(); ++sit) (*sit)->drop();

    selectors.clear();
}

//! removeSamplers
void CCollManager::removeSamplers()
{
    // detach binded samplers from entities
    core::list<CCollEntity*>::Iterator eit = entities.begin();
    for (; eit != entities.end(); ++eit) if ((*eit)->getSampler()) (*eit)->setSampler(NULL);

    // remove from list and drop
    core::list<CCollSampler*>::Iterator sit = samplers.begin();
    for (; sit != samplers.end(); ++sit) (*sit)->drop();

    samplers.clear();
}

//! removeObjects
void CCollManager::removeObjects()
{
    // remove from list and drop
    core::list<CCollObject*>::Iterator oit = objects.begin();
    for (; oit != objects.end(); ++oit) (*oit)->drop();

    objects.clear();
}

//! removeEntities
void CCollManager::removeEntities()
{
    // remove from list and drop
    core::list<CCollEntity*>::Iterator eit = entities.begin();
    for (; eit != entities.end(); ++eit) (*eit)->drop();

    entities.clear();
}

//! removeAll
void CCollManager::removeAll()
{
    removeObjects();
    removeEntities();
    removeSelectors();
    removeSamplers();
}


//! update
void CCollManager::update(u32 ElapsedTime)
{
    elapsedTime = ElapsedTime;

    // debug:
    debugSimulationTime = 0;
    u32 ts = 0;
    if (device) ts = device->getTimer()->getRealTime();


    // TODO: bbox Irrlicht hiba! A transzformacio soran megno!
    // TODO: itt csak 1x valogatok, a collide soran meg tobbszor.... igeny szerint legyen (ha tullepi a tunneljet)!

    // syncronization:
    timeSpent += elapsedTime;

    minLengthSQ = COLL_MIN_LENGTH_SQ * (timeSpent / 1000.0f);

    numTimePeriods = 0;
    if (timeSpent >= COLL_DEF_TIME_PERIOD)
    {
        while (timeSpent >= COLL_DEF_TIME_PERIOD)
        {
            timeSpent -= COLL_DEF_TIME_PERIOD;
            ++numTimePeriods;
        }
    }

    // collision tests when a period is ended:
    if (numTimePeriods > 0)
    {
        core::list<CCollEntity*>::Iterator eit, pit;
        core::list<CCollObject*>::Iterator oit;

        for (eit = entities.begin(); eit != entities.end(); ++eit) (*eit)->prepare();

        core::list<CCollEntity*> selEntities;
        core::list<CCollObject*> selObjects;
        for (eit = entities.begin(); eit != entities.end(); ++eit)
        {
            if (!(*eit)->getActive() || (*eit)->getSleeping()) continue;

            (*eit)->clean();
            core::line3df path = (*eit)->getPath();
            core::aabbox3df tunnel = (*eit)->getTunnel(path);
            selEntities.clear();
            selObjects.clear();

            for (pit = entities.begin(); pit != entities.end(); ++pit)
            {
                if ((*pit) != (*eit) && (*pit)->getActive() && isectBoxes(tunnel, (*pit)->getBBoxWS())) selEntities.push_back(*pit);
            }
            for (oit = objects.begin(); oit != objects.end(); ++oit)
            {
                if ((*oit)->getActive() && isectBoxes(tunnel, (*oit)->getBBoxWS())) selObjects.push_back(*oit);
            }

            (*eit)->collide(selObjects, selEntities);
        }

        for (eit = entities.begin(); eit != entities.end(); ++eit) (*eit)->update();
    }


    // debug
    if (device) debugSimulationTime = device->getTimer()->getRealTime() - ts;
}


//! rayIntersect
bool CCollManager::rayIntersect(const core::vector3df& RayStart, const core::vector3df& RayDir, SCollContact& Contact,
                                float RayLength, int RaySegments, ICollEntity* EmitterEntity, ICollObject* EmitterObject)
{
    core::vector3df norm = core::vector3df(RayDir).normalize();
    core::vector3df rs = RayStart, re = rs + (norm * RayLength);
    core::line3df st(rs, re);
    core::aabbox3df stBB(st.start, st.end);
    stBB.repair();

    // number and length of segments
    int raySegments = (RaySegments < 1) ? 1 : RaySegments;
    float stlen = (re - rs).getLength() / raySegments;

    // select all the items the segment can hit
    core::list<CCollObject*> selObjects;
    core::list<CCollObject*>::Iterator oit;

    for (oit = objects.begin(); oit != objects.end(); ++oit)
    {
        if (EmitterObject && (*oit) ==  EmitterObject) continue;
        if ((*oit)->getActive() && isectBoxes(stBB, (*oit)->getBBoxWS())) selObjects.push_back(*oit);
    }

    core::list<CCollEntity*> selEntities;
    core::list<CCollEntity*>::Iterator eit;

    for (eit = entities.begin(); eit != entities.end(); ++eit)
    {
        if (EmitterEntity && (*eit) ==  EmitterEntity) continue;
        if ((*eit)->getActive() && isectBoxes(stBB, (*eit)->getBBoxWS())) selEntities.push_back(*eit);
    }

    // loop through the segments
    for (int i = 0; i < raySegments; ++i)
    {
        st.end = st.start + ((re - st.start).normalize() * stlen); // deal with possible inaccurancy issues...

        if (segmentIntersect(st, Contact, selObjects, selEntities)) return true;

        st.start = st.end;
    }

    return false;
}

//! segmentIntersect
bool CCollManager::segmentIntersect(const core::line3df& Segment, SCollContact& Contact,
                                    const core::list<CCollObject*>& SelectedObjects, const core::list<CCollEntity*>& SelectedEntities)
{
    SCollContact temp;
    bool foundIsect = false;
    core::line3df st(Segment);
    float closestIsect = COLL_MAX_DISTANCE;
    core::aabbox3df stBB(st.start, st.end);
    stBB.repair();

    // loop through the selected objects
    core::list<CCollObject*>::ConstIterator oit;
    for (oit = SelectedObjects.begin(); oit != SelectedObjects.end(); ++oit)
    {
        if (!isectBoxes(stBB, (*oit)->getBBoxWS())) continue;

        if ((*oit)->rayIntersect(st, temp))
        {
            float dcp = st.start.getDistanceFromSQ(temp.impact);
            if (dcp < closestIsect)
            {
                closestIsect = dcp;
                Contact = SCollContact(temp);
                Contact.object = (*oit);
                foundIsect = true;
            }
        }
    }

    // loop through the selected entities
    core::list<CCollEntity*>::ConstIterator eit;
    for (eit = SelectedEntities.begin(); eit != SelectedEntities.end(); ++eit)
    {
        if (!isectBoxes(stBB, (*eit)->getBBoxWS())) continue;

        if ((*eit)->rayIntersect(st, temp))
        {
            float dcp = st.start.getDistanceFromSQ(temp.impact);
            if (dcp < closestIsect)
            {
                closestIsect = dcp;
                Contact = SCollContact(temp);
                Contact.entity = (*eit);
                foundIsect = true;
            }
        }
    }

    return foundIsect;
}

} // end namespace COLL
