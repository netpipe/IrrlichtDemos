// Copyright (C) 2014- Danyal Zia
// Copyright (C) 2009-2013 Josiah Hartzell (Skyreign Software)
// This file is part of the "irrBullet" Bullet physics wrapper.
// For conditions of distribution and use, see copyright notice in irrBullet.h
// The above copyright notice and its accompanying information must remain here.

#pragma once

#include "irrBulletCommon.h"
#include "irrBulletCollisionShape.h"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include "irrBulletCollisionObjectAffector.h"
#include <IAttributes.h>
#include <vector>

enum class EActivationState
{
    EAS_ACTIVE = 1,
    EAS_SLEEPING = 2,
    EAS_WANTS_DEACTIVATION = 3,
    EAS_DISABLE_DEACTIVATION = 4,
    EAS_DISABLE_SIMULATION = 5
};


enum class ECollisionObjectType
{
  ECOT_COLLISION_OBJECT = 1,
  ECOT_RIGID_BODY,
  ECOT_GHOST_OBJECT,
  ECOT_SOFT_BODY,
  ECOT_HF_FLUID
};


enum class ECollisionFlag
{
    ECF_STATIC_OBJECT = 1,
    ECF_KINEMATIC_OBJECT = 2,
    ECF_NO_CONTACT_RESPONSE = 4,
    ECF_CUSTOM_MATERIAL_CALLBACK = 8,
    ECF_CHARACTER_OBJECT = 16
};

class ICollisionObject;

struct SCollisionObjectIdentification
{
    SCollisionObjectIdentification()
    {
        name = "IdentificationInvalid";
        object = 0;
    }

    void setCollisionObject(ICollisionObject* const obj) { object = obj; };

    void setName(const irr::core::stringc& newName) { name = newName; };

    const irr::core::stringc& getName() const { return name; };

    ICollisionObject* getCollisionObject() const { return object; };

private:
    ICollisionObject* object;
    irr::core::stringc name;
};

class irrBulletWorld;

/// The base class of all world objects.
class ICollisionObject
{
    public:
        ICollisionObject();

		ICollisionObject(const ICollisionObject& other) = default;

		ICollisionObject& operator=(const ICollisionObject& other) = default;

		// Move constructor
		ICollisionObject(const ICollisionObject&& other)
		{
			*this = std::move(other);
		}

		// Move assignment operator
		ICollisionObject& operator=(ICollisionObject&& other)
		{
			if (this != &other)
			{
				delete object;
				delete attributes;
				delete collID;

				dynamicsWorld = other.dynamicsWorld;
				object = other.object;
				uniqueID = other.uniqueID;
				objectType = other.objectType;
				attributes = other.attributes;
				affectors = other.affectors;
				IncludeNodeOnRemoval = other.IncludeNodeOnRemoval;
				LiquidSimulationEnabled = other.LiquidSimulationEnabled;
				internalTransform = other.internalTransform;
				worldTransform = other.worldTransform;
				collID = other.collID;

				object = nullptr;
				attributes = nullptr;
				collID = nullptr;
			}
			return *this;
		}

        virtual ~ICollisionObject();

        void updateObject();

        /*!
            Sets the Continuous Collision Detection values of the world object.
            This helps to avoid tunneling (when an object "shoots" through
            another object when it is not supposed to, due to traveling
            too far in one step, sometimes actually passing by a collision
            face before it is stepped again, where it should have collided) at high speeds.
            @param motionThreshold The simulation doesn't do CCD unless the motion (in one step) is more than motionThreshold.
            @param radius Swept sphere radius. (0.0 by default)
        */
        void setCcdValues(irr::f32 motionThreshold, irr::f32 radius);

        /// Sets the new world transform of this object. (rotation and position)
        void setWorldTransform(const irr::core::matrix4& irrmat);

        void setAnisotropicFriction(const irr::core::vector3df& friction);

        /*!
            The constraint solver can discard solving contacts if the distance is above this threshold.

            0 by default. Note that using contacts with positive distance can improve stability.
            It increases, however, the chance of colliding with degerate contacts, such as 'interior' triangle edges
        */
        void setContactProcessingThreshold(irr::f32 cpt);

        void setActivationState(EActivationState newState);

        void forceActivationState(EActivationState newState);

        /// If sleeping, this will "wake" the object up.
        void activate(bool forceActivation=false);

        /// Sets how "bouncy" the object is. Higher values will make it more bouncy.
        void setRestitution(irr::f32 rest);

        /*!
            Sets the friction of the object. Objects with higher friction values tend
            to tumble and roll more, and will not slide as far as it would with
            lower friction values.
        */
        void setFriction(irr::f32 friction);

        void setInterpolationLinearVelocity(const irr::core::vector3df &linvel);

        void setInterpolationAngularVelocity(const irr::core::vector3df &angvel);

        void setIslandTag(irr::u32 tag);

        void setCompanionId(irr::u32 id);

        void setHitFraction(irr::f32 hitFraction);

        void setCollisionFlags(ECollisionFlag flags);

        /// Adds a new collision object animator to the list of animators to affect this object
        void addAffector(ICollisionObjectAffector* const affector);

        /// Removes a collision object animator at the given index
        void removeAffector(ICollisionObjectAffector* const affector);

        void removeAffectors();

        /// @param b If this parameter is true, then the node will be included in the removal process.
        void includeNodeOnRemoval(bool b) { IncludeNodeOnRemoval = b; };

        /// Sets the name of this object to newName
        void setName(const irr::core::stringc& newName) { collID->setName(newName); };

        /// Removes the object from the dynamics world without explicit use of irrBulletWorld's removal functions.
        void remove();

        /// If enabled, liquid bodies will affect this object.
        void setLiquidSimulationEnabled(bool b) { LiquidSimulationEnabled = b; };


        // Operators
        bool operator==(const ICollisionObject& other) const { return (this->uniqueID == other.getUniqueID()); };

        bool operator!=(const ICollisionObject& other) const { return !(*this == other); }



        /// @return Underlaying btCollisionObject pointer.
        btCollisionObject* getPointer() const;

        /// @return The irrBullet world that this object is being simulated in
        irrBulletWorld* getDynamicsWorld() const { return dynamicsWorld; };

        /*!
        In irrBullet, irr::core::matrix4 takes the place of Bullet's btTransform.
        getWorldTransform() will return a btTransform in Bullet, but returns a matrix4 in irrBullet.
        This matrix includes rotation and position.
        */
        irr::core::matrix4 getWorldTransform()
        {
            btTransformToIrrlichtMatrix(getPointer()->getWorldTransform(), worldTransform);
            return worldTransform;
        }

        bool mergesSimulationIslands() const;

        const irr::core::vector3df getAnisotropicFriction() const;

        bool hasAnisotropicFriction() const;

        irr::f32 getContactProcessingThreshold() const;

        bool isStaticObject() const { return object->isStaticObject(); };
        bool isKinematicObject() const { return object->isKinematicObject(); };
        bool isStaticOrKinematicObject() const { return object->isStaticOrKinematicObject(); };
        bool hasContactResponse() const;

        EActivationState getActivationState() const;

        bool isActive() const;

        /// @return How "bouncy" the object is.
        irr::f32 getRestitution() const;

        irr::f32 getFriction() const;

        const irr::core::vector3df getInterpolationLinearVelocity() const;

        const irr::core::vector3df getInterpolationAngularVelocity() const;

        irr::u32 getIslandTag() const;

        irr::u32 getCompanionId() const;

        irr::f32 getHitFraction() const;

        ECollisionFlag getCollisionFlags() const;

        irr::u32 getUniqueID() const { return uniqueID; };

        ECollisionObjectType getObjectType() const { return objectType; };

        bool checkCollideWith(ICollisionObject* const collObj) const;

        irr::io::IAttributes* getAttributes() const { return attributes; };

        irr::u32 getNumAffectors() const;

        ICollisionObjectAffector* getAffector(irr::u32 index);

        //bool hasCollidedWithAttribute(irr::c8 *attributeName) const;

        bool isNodeIncludedInRemoval() const { return IncludeNodeOnRemoval; };

        // @return The name of this collision object
        //const irr::core::stringc& getName() const { return collID->getName(); };

        /// The SCollisionObjectIdentification struct holds the collision object pointer and also the name of the object
        SCollisionObjectIdentification* getIdentification() const { return collID; };

        bool isLiquidSimulationEnabled() const { return LiquidSimulationEnabled; };



    protected:
        irrBulletWorld *dynamicsWorld;
        btCollisionObject *object;
        irr::u32 uniqueID;
        ECollisionObjectType objectType;
        irr::io::IAttributes *attributes;
        std::vector<ICollisionObjectAffector*> affectors;
        bool IncludeNodeOnRemoval;
        bool LiquidSimulationEnabled;
        btTransform internalTransform;
        irr::core::matrix4 worldTransform;

        SCollisionObjectIdentification *collID;
};
