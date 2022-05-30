#ifndef _CPHYSICSMANAGER_H_
#define _CPHYSICSMANAGER_H_

#include <irrlicht.h>
#include "CBaseEntity.h"

#include <btBulletDynamicsCommon.h>

using namespace irr;

namespace agEngine
{
    namespace physics
    {
        using namespace entities;

#define BIT(x) (1<<(x))

        enum collisionTypes
        {
            COL_0 = 0,
            COL_1 = BIT(1),
            COL_2 = BIT(2),
            COL_3 = BIT(3),
            COL_4 = BIT(4),
            COL_5 = BIT(5),
            COL_6 = BIT(6),
            COL_7 = BIT(7),
        };

        class CPhysicsManager : public virtual IReferenceCounted
        {
        public:
            CPhysicsManager();
            virtual ~CPhysicsManager();

            virtual void registerPhysicsEntity(CBaseEntity* newEnt = 0);
            virtual void registerPhysicsEntity(CBaseEntity* newEnt, btRigidBody* rigidBody);
            virtual void unregisterPhysicsEntity(CBaseEntity* oldEnt = 0);

            virtual void updatePhysics(u32 deltaTime);
        private:
            virtual void updateRender(btRigidBody* body);
            virtual void quaternionToEuler(const btQuaternion& quat, btVector3* euler);

            core::array<btRigidBody*> managedEntities;

            bool firstTick;

            btDiscreteDynamicsWorld* dynamicsWorld;
            btDefaultCollisionConfiguration* collisionConfiguration;
            btCollisionDispatcher* dispatcher;
            btBroadphaseInterface* broadphase;
            btSequentialImpulseConstraintSolver* solver;

            f32 simulationSpeed;
        };
    }
}

#endif
