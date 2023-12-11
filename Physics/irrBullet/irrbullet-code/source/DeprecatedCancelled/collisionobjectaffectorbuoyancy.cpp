// Copyright (C) 2009-2010 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics extension library and wrapper.
// For conditions of distribution and use, see license in irrbullet.h
// The above copyright notice and its accompanying information may not be removed.

#include "collisionobjectaffectorbuoyancy.h"
#include "bulletworld.h"
#include "collisionobject.h"
#include <vector3d.h>
#include <IrrlichtDevice.h>
#include <IVideoDriver.h>
#include <SColor.h>
#include <IMeshSceneNode.h>

ICollisionObjectAffectorBuoyancy::ICollisionObjectAffectorBuoyancy(const irr::core::aabbox3df& waterExtents, irr::f32 force)
{
    WaterExtents = waterExtents;
    PullForce = force;
    points.push_back(SBuoyancyPoint(irr::core::vector3df(0,0,0),PullForce));
    EnableWaves = false;
}

ICollisionObjectAffectorBuoyancy::ICollisionObjectAffectorBuoyancy(const irr::core::array<SBuoyancyPoint>& floatPoints, const irr::core::aabbox3df& waterExtents, irr::f32 force)
{
    WaterExtents = waterExtents;
    PullForce = force;
    points = floatPoints;
    EnableWaves = false;
}

void ICollisionObjectAffectorBuoyancy::affectObject(ICollisionObject* object, irr::u32 timeMS)
{
    irr::core::vector3df CurrentPosition = irr::core::vector3df(0,0,0);
    irr::core::vector3df ForceDirection = irr::core::vector3df(0,0,0);

    if(object->getObjectType() == ECOT_RIGID_BODY)
        CurrentPosition = static_cast<IRigidBody*>(object)->getCollisionShape()->getSceneNode()->getAbsolutePosition();
    else
        CurrentPosition = static_cast<ISoftBody*>(object)->getSceneNode()->getAbsolutePosition();

    if(DebugDraw)
    {
        object->getDynamicsWorld()->getIrrlichtDevice()->getVideoDriver()->setMaterial(object->getDynamicsWorld()->getDebugMaterial());
        object->getDynamicsWorld()->getIrrlichtDevice()->getVideoDriver()->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

        object->getDynamicsWorld()->getIrrlichtDevice()->getVideoDriver()->draw3DBox(WaterExtents, irr::video::SColor(255, 0, 0, 100));
    }

    irr::f32 y = 0.0f;
    if(EnableWaves)
    {
        y = WaterExtents.MaxEdge.Y;

        irr::f32 waveHeight = rand() / 3 - 1;
        WaterExtents.MaxEdge.Y += (waveHeight*0.0005);
    }

    for(irr::u32 i=0; i < points.size(); i++)
    {
        ForceDirection = irr::core::vector3df(0,PullForce*points[i].buoyancy,0);

        if(object->getObjectType() == ECOT_RIGID_BODY)
        {
            irr::core::matrix4 mat = static_cast<IRigidBody*>(object)->getCollisionShape()->getSceneNode()->getAbsoluteTransformation();
            const irr::core::matrix4 mat2 = mat;

            irr::core::vector3df offset(points[i].localPoint);

            mat.transformVect(offset);

            mat.setTranslation(offset);

            const irr::core::matrix4 w2n(mat2, irr::core::matrix4::EM4CONST_INVERSE);

            mat = (w2n*mat);

            if(WaterExtents.isPointInside(offset))
            {
                if(DebugDraw)
                {
                    object->getDynamicsWorld()->getIrrlichtDevice()->getVideoDriver()->draw3DLine(
                        offset, offset+irr::core::vector3df(0,10,0), irr::video::SColor(255, 0, 0, 255));
                }

                static_cast<IRigidBody*>(object)->applyForce(-(static_cast<IRigidBody*>(object)->getVelocityInLocalPoint(mat.getTranslation())*0.25), mat.getTranslation());
                static_cast<IRigidBody*>(object)->applyForce(ForceDirection, mat.getTranslation());
            }
        }
    }

    if(EnableWaves)
        WaterExtents.MaxEdge.Y = y;
}

ICollisionObjectAffectorBuoyancy::~ICollisionObjectAffectorBuoyancy()
{
    //dtor
}
