// FunCollision -- Collision detection and response system -- Created by Peter Szollosi, 2009-2010.
// For conditions of distribution and use, see copyright notice in license.txt or FunCollision.h

#include <irrlicht.h>
using namespace irr;

#include "CCollSampler.h"
#include "CCollEntity.h"


namespace COLL
{

//! CCollSampler --------------------------------------------------------------------------------------------------------------------------
//! setScale
void CCollSampler::setScale(const core::vector3df& Scale)
{
    // repair shape
    scale = Scale;
    scale.X = (scale.X < 0) ? -scale.X : scale.X;
    scale.Y = (scale.Y < 0) ? -scale.Y : scale.Y;
    scale.Z = (scale.Z < 0) ? -scale.Z : scale.Z;
    scale.X = (scale.X == 0) ? 1 : scale.X;
    scale.Y = (scale.Y == 0) ? 1 : scale.Y;
    scale.Z = (scale.Z == 0) ? 1 : scale.Z;

    sphereRadius = 1;
}


//! spherePoint
bool CCollSampler::spherePoint(const core::line3df& S, const core::vector3df& P, SCollContact& Contact, float ToleranceSQ)
{
    core::vector3df CP = pointToLine(P, S);

    float rad2 = sphereRadius * sphereRadius;
    float d2SS_P = S.start.getDistanceFromSQ(P);
    if (rad2 > d2SS_P) rad2 = d2SS_P;   // repair rad2

    float d2PCP = P.getDistanceFromSQ(CP);

    if (d2PCP >= rad2) return false;

    float back = sqrtf(rad2 - d2PCP);

    Contact.impact = -S.getVector().normalize();
    Contact.impact = CP + Contact.impact * back;

    Contact.closest = P;

    Contact.normal = core::vector3df(Contact.impact - P).normalize();
    Contact.distSQ = Contact.impact.getDistanceFromSQ(S.start);

    return pointOnSegment(Contact.impact, S, ToleranceSQ);
}

//! sphereEdge
bool CCollSampler::sphereEdge(const core::line3df& S, const core::line3df& E, SCollContact& Contact, float ToleranceSQ)
{
    core::vector3df PS, CPS;

    if (!lineToLine(S.start, S.end, E.start, E.end, PS, CPS)) return false;

    core::vector3df CSS = pointToLine(S.start, E);
    float d2SS_CSS = S.start.getDistanceFromSQ(CSS);
    float rad2 = sphereRadius * sphereRadius;
    if (rad2 > d2SS_CSS) rad2 = d2SS_CSS;   // repair rad2

    float d2PS_CPS = PS.getDistanceFromSQ(CPS);

    if (d2PS_CPS >= rad2) return false;

    float m2 = d2SS_CSS - d2PS_CPS;

    float mrad2 = rad2 - d2PS_CPS;

    float dv = (m2 == 0) ? 1 : (mrad2 / m2);

    float d2PS_SS = PS.getDistanceFromSQ(S.start);
    float d2CPS_CSS = CPS.getDistanceFromSQ(CSS);

    float backS = sqrtf(d2PS_SS * dv);
    float backE = sqrtf(d2CPS_CSS * dv);

    Contact.impact = -S.getVector().normalize();
    Contact.impact = PS + Contact.impact * backS;

    core::vector3df CFE = (CSS - CPS).normalize();
    CFE = CPS + CFE * backE;

    if (!pointOnSegment(CFE, E, ToleranceSQ)) return false;

    Contact.closest = CFE;
    Contact.normal = core::vector3df(Contact.impact - CFE).normalize();
    Contact.distSQ = Contact.impact.getDistanceFromSQ(S.start);

    return pointOnSegment(Contact.impact, S, ToleranceSQ);
}

//! sphereTriangloid
bool CCollSampler::sphereTriangloid(const core::line3df& S, const CCollTriangloid& T, SCollContact& Contact, float ToleranceSQ)
{
    core::vector3df PS, PT;

    if (!T.intersectPlane(S.start, S.getVector(), PS)) return false;

    T.projectToPlane(S.start, PT);

    float rad2 = sphereRadius * sphereRadius;
    float d2SS_PT = S.start.getDistanceFromSQ(PT);
    if (rad2 > d2SS_PT) rad2 = d2SS_PT;     // repair rad2

    float dv = (d2SS_PT == 0) ? 1 : (rad2 / d2SS_PT);
    float CPBack = sqrtf(PS.getDistanceFromSQ(PT) * dv);
    float FEBack = sqrtf(PS.getDistanceFromSQ(S.start) * dv);

    core::vector3df CP = (PT - PS).normalize();
    CP = PS + CP * CPBack;

    if (!T.pointInside(CP)) return false;

    Contact.impact = -S.getVector().normalize();
    Contact.impact = PS + Contact.impact * FEBack;
    Contact.closest = CP;
    Contact.normal = core::vector3df(Contact.impact - CP).normalize();
    Contact.distSQ = Contact.impact.getDistanceFromSQ(S.start);

    return pointOnSegment(Contact.impact, S, ToleranceSQ);
}

//! collideTriangloid
bool CCollSampler::collideTriangloid(const core::line3df& S, const CCollTriangloid& T, SCollContact& Contact, float ToleranceSQ)
{
    Contact.distSQ = COLL_MAX_DISTANCE;
    SCollContact cr(Contact);
    bool ret = false;

    // triangle
    if (sphereTriangloid(S, T, Contact, ToleranceSQ)) return true;

    // edges
    if (sphereEdge(S, T.getEdgeAB(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }
    if (sphereEdge(S, T.getEdgeBC(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }
    if (sphereEdge(S, T.getEdgeCA(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }

    // points
    if (spherePoint(S, T.getPointA(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }
    if (spherePoint(S, T.getPointB(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }
    if (spherePoint(S, T.getPointC(), cr, ToleranceSQ))
        { Contact = SCollContact(ret ? (cr.distSQ < Contact.distSQ ? cr : Contact ) : cr); ret = true; }

    return ret;
}

//! collideEntity
bool CCollSampler::collideEntity(const core::line3df& S, CCollEntity* Entity, SCollContact& Contact)
{
    if (!Entity || !Entity->getSampler()) return false;

    core::vector3df scaleCS = scale + Entity->getScale();
    float radiusCS = core::max_(scaleCS.X, scaleCS.Y, scaleCS.Z);
    core::vector3df mpsCS = core::vector3df(radiusCS) / scaleCS;
    core::vector3df spmCS = scaleCS / core::vector3df(radiusCS);

    core::vector3df entPosWS(Entity->getPos());
    core::vector3df entPosCS(entPosWS * mpsCS);
    core::line3df segmWS(transformPointWS(S.start), transformPointWS(S.end));
    core::line3df segmCS(segmWS.start * mpsCS, segmWS.end * mpsCS);

    core::vector3df proj = pointToLine(entPosCS, segmCS);
    float radSQ = radiusCS * radiusCS;
    core::vector3df isect;
    if (segmCS.start.getDistanceFromSQ(entPosCS) <= radSQ)
    {
        isect = entPosCS + (segmCS.start - entPosCS).normalize() * radiusCS;
    }
    else
    {
        float bSQ = proj.getDistanceFromSQ(entPosCS);
        if (bSQ > radSQ) return false;

        core::vector3df back = (segmCS.start - segmCS.end).normalize();
        if (bSQ == radSQ)   isect = proj;
        else if (bSQ > 0)   isect = proj + back * sqrtf(radSQ - bSQ);
        else                isect = entPosCS + back * radiusCS;

        if (!segmCS.isPointBetweenStartAndEnd(isect)) return false;
    }

    isect *= spmCS;
    // the ellipsoid is a bitch!!!
    Contact.normal = (transformPointOS(segmWS.start - isect).normalize() +
                      Entity->getSampler()->transformPointOS(isect - entPosWS).normalize()).normalize();
    Contact.impact = transformPointOS(isect + Contact.normal * COLL_DEF_PUSH_OUT);
    Contact.closest = Contact.impact - (Contact.normal * sphereRadius);
    Contact.distSQ = Contact.impact.getDistanceFromSQ(S.start);

    return true;
}

//! rayIntersect
bool CCollSampler::rayIntersect(const core::vector3df& Pos, const core::line3df& Ray, SCollContact& Contact)
{
    core::vector3df tpos = transformPointOS(Pos);
    core::vector3df trs = transformPointOS(Ray.start);
    core::vector3df trd = transformPointOS(Ray.getVector()).normalize();

    core::vector3df q = tpos - trs;
    float c = q.getLength();
    float v = q.dotProduct(trd);
    float d = sphereRadius * sphereRadius - (c * c - v * v);

    if (d <= core::ROUNDING_ERROR_f32) return false;

    float dist = v - sqrtf(d);
    if (dist <= core::ROUNDING_ERROR_f32) return false;

    core::vector3df ti = trs + (trd * dist);
    Contact.impact = transformPointWS(ti);
    Contact.normal = transformPointOS(Contact.impact - Pos).normalize();
    Contact.tri.setID(0);

    return true;
}


//! CCollSphereSampler --------------------------------------------------------------------------------------------------------------------
//! setScale
void CCollSphereSampler::setScale(const core::vector3df& Scale)
{
    CCollSampler::setScale(Scale);

    // set the largest compontent
    sphereRadius = core::max_(scale.X, scale.Y, scale.Z);
    scale = core::vector3df(sphereRadius);
}

//! prepareTriangloid
void CCollSphereSampler::prepareTriangloid(CCollTriangloid& Tri, const core::matrix4& Matrix)
{
    Tri.transform(Matrix);
    Tri.recalcPlane();
}

//! CCollEllipsoidSampler ---------------------------------------------------------------------------------------------------------------
//! setScale
void CCollEllipsoidSampler::setScale(const core::vector3df& Scale)
{
    CCollSampler::setScale(Scale);

    // calculate distorted vectors
    sphereRadius = core::min_(scale.X, scale.Y, scale.Z);
    mps = core::vector3df(sphereRadius) / scale;
    spm = scale / core::vector3df(sphereRadius);
}

//! prepareTriangloid
void CCollEllipsoidSampler::prepareTriangloid(CCollTriangloid& Tri, const core::matrix4& Matrix)
{
    Tri.transform(Matrix);
    Tri.setFrom(transformTriOS(Tri.getTri()));
    Tri.recalcPlane();
}

} // end namespace COLL
