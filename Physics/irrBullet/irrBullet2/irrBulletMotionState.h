// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include <iostream>
#include "irrBulletCommon.h"
#include "irrBulletRigidBody.h"
#include <LinearMath/btDefaultMotionState.h>


/// This should be left for internal use by irrBullet.
/*!
    A motion state controls how to handle the visual representation of Bullet objects,
    and what to do on certain events such as setWorldTransform() and getWorldTransform().
*/
class IMotionState : public btDefaultMotionState
{
    public:
        IMotionState(const btTransform &initialPos);
        virtual ~IMotionState();


        void setWorldTransform(const btTransform &worldTrans);

        void setRigidBody(IRigidBody* const body) {RigidBody = body;};

        /// Makes it so that the node is aimed in the direction of the velocity of the object. (useful for realistic bombs, etc.)
        void setVelocityAsDirection(bool b) { VelocityAsRotation = b; };

        /// Makes it so that the motion state won't set the rotation of the node. (leaves it open for other code to rotate it)
        void setManualRotation(bool b) { ManualRotation = b; };

        void setManualPosition(bool b) { ManualPosition = b; };

        void getWorldTransform(btTransform &worldTrans) const;

        irr::core::matrix4& getWorldTransformationMatrix();

    private:
        IRigidBody* RigidBody;
        bool failed;
        bool VelocityAsRotation;
        bool ManualRotation;
        bool ManualPosition;

        btTransform worldTransform;
        irr::core::matrix4 TransformMatrix;
};
