// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include <irrTypes.h>

class ICollisionObject;

enum class ECollisionObjectAffectorType
{
    ECOAT_DELETE_AFFECTOR,
    ECOAT_ATTRACT_AFFECTOR,
    ECOAT_AFFECTOR_COUNT
};


/*!
    The base class for all collision object affectors. Derive from this class to create your own affectors.
*/
class ICollisionObjectAffector
{
    public:
        ICollisionObjectAffector();
        virtual ~ICollisionObjectAffector();

        virtual void affectObject(ICollisionObject* object, irr::u32 timeMS) = 0;

        void setDebugDrawing(bool b) { DebugDraw = b; };

		bool hasFinished() const { return HasFinished; };

		ECollisionObjectAffectorType getAffectorType() const { return Type; };

    protected:
        ECollisionObjectAffectorType Type;
        bool HasFinished;
        bool DebugDraw;
};
