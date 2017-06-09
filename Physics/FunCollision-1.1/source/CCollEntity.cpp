// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollManager.h"
#include "CCollObject.h"
#include "CCollEntity.h"
#include "CCollSampler.h"


namespace COLL
{

//! setSampler
void CCollEntity::setSampler(ICollSampler* Sampler)
{
    // detach old sampler
    if (sampler) sampler->drop();

    // grab the new one
    CCollSampler* s = NULL;
    if (Sampler)
    {
        s = dynamic_cast<CCollSampler *>(Sampler);
        s->grab();
    }

    // update
    update();
    sampler = s;
    spawn(pos);
}

//! setNode
void CCollEntity::setNode(scene::ISceneNode* Node)
{
    node = Node;
    spawn(pos);
}

//! setPos
void CCollEntity::setPos(const core::vector3df& Pos)
{
    core::vector3df origin(sampler ? sampler->getOrigin() : core::vector3df());
    pos = Pos - origin;
}


//! getScale
core::vector3df CCollEntity::getScale() const
{
    return sampler ? sampler->getScale() : core::vector3df();
}

//! getBBoxOS
core::aabbox3df CCollEntity::getBBoxOS() const
{
    return sampler ? sampler->getBBoxOS() : core::aabbox3df();
}

//! getBBoxWS
core::aabbox3df CCollEntity::getBBoxWS() const
{
    return sampler ? sampler->getBBoxWS(pos) : core::aabbox3df();
}

//! getTunnel
core::aabbox3df CCollEntity::getTunnel(const core::line3df& Ray) const
{
    if (!sampler) return core::aabbox3df();
    core::aabbox3df ret = sampler->getBBoxWS(Ray.start);
    ret.addInternalBox(sampler->getBBoxWS(Ray.end));
    ret.repair();
    ret.MinEdge -= core::vector3df(COLL_MIN_LENGTH_SQ);
    ret.MaxEdge += core::vector3df(COLL_MIN_LENGTH_SQ);
    return ret;
}

//! getCollector
core::aabbox3df CCollEntity::getCollector(const core::line3df& Ray, const core::matrix4& InvMatrix) const
{
    core::aabbox3df ret = getTunnel(Ray);
    InvMatrix.transformBoxEx(ret);
    ret.repair();
    return ret;
}

//! addForce
void CCollEntity::addForce(const core::vector3df& Force, bool WaitSync)
{
    if (manager && ((WaitSync && manager->getTimePeriodEnded()) || !WaitSync)) force += Force;
}

//! addImpulse
void CCollEntity::addImpulse(const core::vector3df& Impulse, bool WaitSync)
{
    if (manager && ((WaitSync && manager->getTimePeriodEnded()) || !WaitSync)) impulse += Impulse;
}

//! clean
void CCollEntity::clean()
{
    // debug:
    debugNumSelectedTris = 0;
    debugNumIterations = 0;

    // clear
    collided = false;
    contacts.clear();
    normalResultant = standResultant = core::vector3df();
}

//! teleport
void CCollEntity::teleport(const core::vector3df& Pos)
{
    clean();

    setPos(Pos);

    prevPos = pos;
    path = core::line3df(pos, pos);

    setAwake();
}

//! spawn
void CCollEntity::spawn(const core::vector3df& Pos)
{
    if (!manager) return;

    teleport(Pos);

    velocity = force = impulse = gravity = core::vector3df();
    state = ECES_FALLING;

    setAwake();
}

//! prepare
void CCollEntity::prepare()
{
    if (!manager) return;

    float minLengthSQ = manager->getMinLengthSQ();

    // reset pos
    pos = prevPos;

    // gravity vector
    gravity = (state == ECES_STANDING) ? core::vector3df() : gravity;
    core::vector3df gv = (state < ECES_STANDING) ? manager->getGravityVect() : core::vector3df();

    // update physics variables:
    for (u32 periods = 0; periods < manager->getNumTimePeriods(); ++periods)
    {
        gravity += gv;
        velocity += force + impulse + gravity;

        pos += velocity;

        velocity *= manager->getBaseFriction();
        impulse = core::vector3df();
    }
    force = core::vector3df();

    // deal with sleeping:
    sleeping = sleeping > COLL_DEF_AUTO_AWAKE ? 0 : sleeping + manager->getNumTimePeriods();
    // if we've been standing at the same position for a long time
    if (samePoint(prevPos, pos, minLengthSQ)) pos = prevPos;
    else setAwake(); // for slower computers; otherwise it could fall asleep while being in motion

    // save path
    path = core::line3df(prevPos, pos);
    pos = prevPos;
}


//! update
void CCollEntity::update()
{
    if (node)
    {
        core::vector3df origin(sampler ? sampler->getOrigin() : core::vector3df());
        node->setPosition(pos + origin);
        node->updateAbsolutePosition();
    }

    prevPos = pos;
}


//! collect
bool CCollEntity::collect(const core::line3df& RayOS, const core::list<CCollObject*>& SelectedObjects,
                          const core::list<CCollEntity*>& SelectedEntities, SCollContact& ClosestContact)
{
    if (!sampler || !manager) return false;

    SCollContact contact;
    core::line3df rayWS = core::line3df(sampler->transformPointWS(RayOS.start), sampler->transformPointWS(RayOS.end));
    core::vector3df normOS = RayOS.getVector().normalize();
    float closest = COLL_MAX_DISTANCE;
    bool ret = false;

    // find closest contact
    core::list<CCollEntity*>::ConstIterator eit;
    core::list<CCollObject*>::ConstIterator oit;
    core::list<CCollTriangloid>::Iterator tit;
    core::list<CCollTriangloid> temp;

    for (oit = SelectedObjects.begin(); oit != SelectedObjects.end(); ++oit)
    {
        temp.clear();
        if (!(*oit)->collect(temp, getCollector(rayWS, (*oit)->getInvMatrix()))) continue;

        for (tit = temp.begin(); tit != temp.end(); ++tit)
        {
            sampler->prepareTriangloid(*tit, (*oit)->getMatrix());

            // TODO: kell a frontFacing teszt?
            // if it's front facing
            if ((*tit).frontFacingDir(normOS)) continue;

            // if there was no collision
            if (!sampler->collideTriangloid(RayOS, *tit, contact, manager->getMinLengthSQ())) continue;

            // select if it was the closest
            if (contact.distSQ < closest)
            {
                closest = contact.distSQ;

                // fill closest contact
                ClosestContact.object = *oit;
                ClosestContact.entity = NULL;
                ClosestContact.tri = *tit;
                ClosestContact.impact = contact.impact;
                ClosestContact.normal = contact.normal;
                ClosestContact.closest = contact.closest;
            }

            // there was a contact
            ret = true;

            // debug:
            ++debugNumSelectedTris;
        }
    }

    core::aabbox3df tunnel = getTunnel(rayWS);
    for (eit = SelectedEntities.begin(); eit != SelectedEntities.end(); ++eit)
    {
        if ((*eit) == this) continue;

        if (!isectBoxes(tunnel, (*eit)->getBBoxWS())) continue;

        (*eit)->setAwake(); // wake him up

        if (!sampler->collideEntity(RayOS, *eit, contact)) continue;  // if there was no collision

        // select if it was the closest
        if (contact.distSQ < closest)
        {
            closest = contact.distSQ;

            // fill closest contact
            ClosestContact.object = NULL;
            ClosestContact.entity = *eit;
            ClosestContact.tri = CCollTriangloid();
            ClosestContact.impact = contact.impact;
            ClosestContact.normal = contact.normal;
            ClosestContact.closest = contact.closest;
        }

        // there was a contact
        ret = true;
    }

    return ret;
}

//! collide
void CCollEntity::collide(const core::list<CCollObject*>& SelectedObjects, const core::list<CCollEntity*>& SelectedEntities)
{
    if (!sampler || !manager) return;

    core::line3df rayOS(sampler->transformPointOS(path.start), sampler->transformPointOS(path.end));
    core::line3df travel = rayOS, prevTravel = rayOS, prevTravel2 = rayOS, helpLine;
    core::vector3df tpos = travel.end;
    core::array<SCollContact> tempContacts;
    tempContacts.reallocate(COLL_MAX_ITERATIONS);
    SCollContact contact;
    bool stop = false;
    u32 numIters = 0;

    // loop
    while (!stop)
    {
        ++numIters;

        // save travel vectors
        prevTravel2 = prevTravel;
        prevTravel = travel;

        // find the closest contact
        if (collect(travel, SelectedObjects, SelectedEntities, contact))
        {
            // save result to travel.Start
            travel.start = contact.impact;

            // if there are at least one more plane and if they intersects
            u32 cnt = tempContacts.size();
            if (cnt > 0 && isectPlanes(core::plane3df(contact.impact, contact.normal),
                core::plane3df(tempContacts[cnt - 1].impact, tempContacts[cnt - 1].normal), helpLine))
            {
                // maintenance vector (see below why at dealing with bouncing)
                core::vector3df push = sampler->transformPointOS(COLL_DEF_PUSH_OUT *
                    (contact.normal + tempContacts[cnt - 1].normal).normalize());

                // projection
                travel.end = pointToLine(travel.end,
                    core::line3df(contact.impact + push, contact.impact + helpLine.getVector() + push));
            }
            else // if not projected yet
            {
                // maintenance vector (see below why at dealing with bouncing)
                core::vector3df push = sampler->transformPointOS(COLL_DEF_PUSH_OUT * contact.normal);

                // project travel.End to the plane
                lineToPlane(travel.end, contact.normal,
                    core::plane3df(contact.impact + push, contact.normal), travel.end);
            }

            // save the collision state and the contact
            collided = true;
            tempContacts.push_back(contact);

            // save resultants
            core::vector3df trn(sampler->transformPointOS(contact.normal));
            if (frontFacingDir(manager->getGravityNorm(), -trn)) standResultant += trn;
            normalResultant += contact.normal;
        }
        else // if there was no collision this time
        {
            stop = true;
            tpos = travel.end;
        }

        if  (!stop && (
            // travel vector too short?
            (samePoint(travel.start, travel.end, manager->getMinLengthSQ())) ||
            // turned back? (according to the original ray)
            (!frontFacingDir(rayOS.getVector(), travel.getVector())) ||
            // turned back? (according to the previous dir)
            (!frontFacingDir(prevTravel.getVector(), travel.getVector())) ||
            // circular? (where 3 or more planes meet)
            (numIters > 2 && (
                (samePoint(travel.end, prevTravel2.start, manager->getMinLengthSQ())) ||
                (samePoint(travel.start, prevTravel.start, manager->getMinLengthSQ()) &&
                samePoint(prevTravel.start, prevTravel2.start, manager->getMinLengthSQ())) ) ) ||
            // reached limit?
            (numIters >= COLL_MAX_ITERATIONS) ) )
        {
            stop = true;
            tpos = travel.start;
        }
    }

    // transform pos back to world space
    pos = sampler->transformPointWS(tpos);

    // normalize resultants
    normalResultant.normalize();
    standResultant.normalize();

    // is the entity standing / sliding / falling?
    float standDistSQ = standResultant.getDistanceFromSQ(-manager->getGravityNorm());
    if (standResultant == core::vector3df() || standDistSQ > sampler->getSlideValue()) state = ECES_FALLING;
    else if (standDistSQ > sampler->getStandValue()) state = ECES_SLIDING;
    else state = ECES_STANDING;

    // deal with contacts
    contacts.reallocate(contacts.size() + tempContacts.size());
    core::vector3df vel(velocity - gravity);
    for (u32 i = 0; i < tempContacts.size(); ++i)
    {
        // transform saved contacts
        tempContacts[i].impact = sampler->transformPointWS(tempContacts[i].impact);
        tempContacts[i].normal = sampler->transformPointWS(tempContacts[i].normal).normalize();
        tempContacts[i].closest = sampler->transformPointWS(tempContacts[i].closest);
        tempContacts[i].tri = sampler->transformTriWS(tempContacts[i].tri);
        contacts.push_back(tempContacts[i]);

        // bouncing
        /* If hitting an entity, we need a small push-back for proper mass-simulation,
           but otherwise bouncing causes a minimal unwanted jittering effect.
           However this small jittering is our friend: removing it will cause a lot of
           mean problems: e.g. being able to fly slowly into the geometry in complicated
           situations, or blocking smooth movement on the edges (e.g. terrain), etc.
           It took me hours to find out and debug these issues. */
        core::plane3df bouncePlane(core::vector3df(), tempContacts[i].normal);
        float adv = core::abs_(bouncePlane.getDistanceTo(vel));
        core::vector3df bounceVect(tempContacts[i].normal * adv);
        addImpulse(bounceVect * sampler->getInverseMass() * (1 + sampler->getBounceFactor()), true);

        // if we've hit an other entity
        if (tempContacts[i].entity)
        {
            tempContacts[i].entity->addImpulse(-bounceVect * tempContacts[i].entity->getSampler()->getInverseMass()
                                                * (1 + tempContacts[i].entity->getSampler()->getBounceFactor()), true);

            // TODO: kell ez igy ???????
            CCollEntity* e = dynamic_cast<CCollEntity *>(tempContacts[i].entity);
            e->setCollided();
        }
    }

    // debug:
    debugNumIterations += numIters;
}

//! rayIntersect
bool CCollEntity::rayIntersect(const core::line3df& Ray, SCollContact& Contact)
{
    return sampler ? sampler->rayIntersect(pos, Ray, Contact) : false;
}


//! getDebugData
void CCollEntity::getDebugData(int& DebugNumSelectedTris, int& DebugNumIterations)
{
    DebugNumSelectedTris = debugNumSelectedTris;
    DebugNumIterations = debugNumIterations;
}

//! drawDebugData
void CCollEntity::drawDebugData(video::IVideoDriver* Driver, float NormalLength)
{
    if (!Driver) return;

    for (u32 i = 0; i < contacts.size(); ++i)
    {
        Driver->draw3DTriangle(contacts[i].tri.getTri(), video::SColor(255, 255, 0, 0));
        Driver->draw3DLine(contacts[i].closest, contacts[i].closest + contacts[i].normal * NormalLength, video::SColor(255, 0, 255, 0));
    }
}

} // end namespace COLL
