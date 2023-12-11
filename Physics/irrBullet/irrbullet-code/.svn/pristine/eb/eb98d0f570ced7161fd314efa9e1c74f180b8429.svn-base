// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see license in irrbullet.h
// The above copyright notice and its accompanying information may not be removed.

#ifndef __COLLISION_OBJECT_AFFECTOR_BUOYANCY_H__
#define __COLLISION_OBJECT_AFFECTOR_BUOYANCY_H__

#include "collisionobjectaffector.h"
#include <irrTypes.h>
#include <vector3d.h>
#include <ISceneNode.h>
#include <aabbox3d.h>
#include <irrArray.h>

class ICollisionObject;

/*!
    This struct contains information about a point in a buoyancy affector
    that helps the object using the affector to stay afloat.
*/
struct SBuoyancyPoint
{
    SBuoyancyPoint(const irr::core::vector3df& point, irr::f32 pointBuoyancy)
    {
        localPoint = point;
        buoyancy = pointBuoyancy;
    }

    irr::core::vector3df localPoint;
    irr::f32 buoyancy;
};

/*!
    The buoyancy affector simulates a simple water body effect for a given rigid body (soft bodies not supported).

    This is more or less an example of how one can do his own buoyancy system with irrBullet,
    since different games may need different approaches depending on level of complexity.

    For instance, if the water simulation was needed for a naval simulation, it would need to be a bit different
    from this and might include particle effects and such, whereas a game just using the buoyancy affector
    for visual reasons could use this with no modifications.
*/
class ICollisionObjectAffectorBuoyancy : public ICollisionObjectAffector
{
    public:
        /*!
            @param waterExtents The bounding box that defines the water body's volume
            @param force The primary force that is applied to each point on a floating object (it's multiplied by the point's buoyancy)
        */
        ICollisionObjectAffectorBuoyancy(const irr::core::aabbox3df& waterExtents, irr::f32 force);

        /*!
            @param floatPoints An array of SBuoyancyPoint's that will help the object stay afloat
            @param waterExtents The bounding box that defines the water body's volume
            @param force The primary force that is applied to each point on a floating object (it's multiplied by the point's buoyancy)
        */
        ICollisionObjectAffectorBuoyancy(const irr::core::array<SBuoyancyPoint>& floatPoints, const irr::core::aabbox3df& waterExtents, irr::f32 force);
        virtual ~ICollisionObjectAffectorBuoyancy();

        virtual void affectObject(ICollisionObject* object, irr::u32 timeMS);

        /// @param b If true, fake waves will act on objects floating (it looks a bit more natural)
        void setWavesEnabled(bool b) { EnableWaves = b; };


    protected:
        irr::f32 PullForce;
        irr::core::aabbox3df WaterExtents;
        irr::core::array<SBuoyancyPoint> points;
        bool FirstRun;
        bool EnableWaves;
};

#endif // __COLLISION_OBJECT_BUOYANCY_DELETE_H__
