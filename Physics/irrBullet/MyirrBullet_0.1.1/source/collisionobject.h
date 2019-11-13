#ifndef __COLLISION_OBJECT_H_INCLUDED__
#define __COLLISION_OBJECT_H_INCLUDED__

#include "common.h"
#include "collisionshape.h"

enum EActivationState
{
    EAS_SLEEPING = 0,
    EAS_ACTIVE = 1,
};


class ICollisionObject
{
    public:
        ICollisionObject();

        ~ICollisionObject();


        //////////////////////////
        // The 'dos' and 'sets' //
        //////////////////////////
        void setCollisionShape(ICollisionShape* const shape);

        void setCcdValues(const irr::f64 mt, const irr::f64 r);

        void autoCalculateCcdValues();

        void setAnisotropicFriction(const irr::core::vector3df &friction);

        void setContactProcessingThreshold(irr::f64 cpt);

        void setActivationState(EActivationState newState);

        void forceActivationState(EActivationState newState);

        void activate(bool forceActivation=false);

        void setRestitution(irr::f64 rest);

        void setFriction(irr::f64 frict);

        void setInterpolationLinearVelocity(const irr::core::vector3df &linvel);

        void setInterpolationAngularVelocity(const irr::core::vector3df &angvel);

        void setIslandTag(irr::u32 tag);

        void setCompanionId(irr::u32 id);

        void setHitFraction(irr::f64 hitFraction);

        void setCollisionFlags(ECollisionFlag flags);




        ////////////////
        // The 'gets' //
        ////////////////
        btCollisionObject *getPointer() const;

        ICollisionShape *getCollisionShape() const;

        bool mergesSimulationIslands() const;

        irr::core::vector3df getAnisotropicFriction() const;

        bool hasAnisotropicFriction() const;

        irr::f64 getContactProcessingThreshold() const;

        bool isStaticObject() const {return object->isStaticObject();};
        bool isKinematicObject() const {return object->isKinematicObject();};
        bool isStaticOrKinematicObject() const {return object->isStaticOrKinematicObject();};
        bool hasContactResponse() const;

        EActivationState getActivationState() const;

        bool isActive() const;

        irr::f64 getRestitution() const;

        irr::f64 getFriction() const;

        irr::core::vector3df getInterpolationLinearVelocity() const;

        irr::core::vector3df getInterpolationAngularVelocity() const;

        irr::u32 getIslandTag() const;

        irr::u32 getCompanionId() const;

        irr::f64 getHitFraction() const;

        irr::u32 getCollisionFlags() const;



    protected:
        ICollisionShape *shape;
        btCollisionObject *object;
        irr::scene::ISceneNode *node;
};

#endif // __COLLISION_OBJECT_H_INCLUDED__
