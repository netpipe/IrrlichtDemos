#include "CPhysicsManager.h"

namespace agEngine
{
    namespace physics
    {
        CPhysicsManager::CPhysicsManager()
        : simulationSpeed(1.0f)
        {
            btVector3 worldAabbMin(-2000, -2000, -2000);
            btVector3 worldAabbMax(2000, 2000, 2000);

            s32 maxProxies = 32766;

            collisionConfiguration = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(collisionConfiguration);
            solver = new btSequentialImpulseConstraintSolver();
            broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
            dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

            dynamicsWorld->getDispatchInfo().m_enableSPU = true;
            dynamicsWorld->setGravity(btVector3(0, -60, 0));
        }

        CPhysicsManager::~CPhysicsManager()
        {
            for (u32 i = 0; i < managedEntities.size(); ++i)
            {
                static_cast<CBaseEntity*>(managedEntities[i]->getUserPointer())->drop();
                delete managedEntities[i];
            }

            managedEntities.clear();

            delete dynamicsWorld;
            delete broadphase;
            delete solver;
            delete dispatcher;
            delete collisionConfiguration;
        }

        void CPhysicsManager::registerPhysicsEntity(CBaseEntity* newEnt)
        {
            btDefaultMotionState* defaultMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(newEnt->getFloatParam(entities::xPosition), newEnt->getFloatParam(entities::yPosition), newEnt->getFloatParam(entities::zPosition))));
            btCollisionShape* defaultShape = new btSphereShape(1);

            btRigidBody::btRigidBodyConstructionInfo defaultConstructionInfo(0, defaultMotionState, defaultShape, btVector3(0, 0, 0));

            btRigidBody* rigidBody = new btRigidBody(defaultConstructionInfo);
            dynamicsWorld->addRigidBody(rigidBody, newEnt->getIntParam(entities::physicsCollisionType), newEnt->getIntParam(entities::physicsCollideWith));
            newEnt->grab();
            newEnt->setPhysicsRigidBody(rigidBody);

            delete defaultShape;
            delete defaultMotionState;
        }

        void CPhysicsManager::registerPhysicsEntity(CBaseEntity* newEnt, btRigidBody* rigidBody)
        {
            rigidBody->setUserPointer(newEnt);
            dynamicsWorld->addRigidBody(rigidBody);
            newEnt->grab();
            newEnt->setPhysicsRigidBody(rigidBody);
        }

        void CPhysicsManager::unregisterPhysicsEntity(CBaseEntity* oldEnt)
        {
            if (!oldEnt)
                return;

            for (u32 i = 0; i < managedEntities.size(); ++i)
            {
                if (static_cast<CBaseEntity*>(managedEntities[i]->getUserPointer()) == oldEnt)
                {
                    oldEnt->drop();
                    dynamicsWorld->removeRigidBody(managedEntities[i]);
                    delete managedEntities[i];
                    managedEntities.erase(i);
                    break;
                }
            }
        }

        void CPhysicsManager::updatePhysics(u32 deltaTime)
        {
            dynamicsWorld->stepSimulation(deltaTime * 0.001f * simulationSpeed, 2);

            for (u32 i = 0; i < managedEntities.size(); ++i)
                updateRender(managedEntities[i]);
        }

        void CPhysicsManager::updateRender(btRigidBody* body)
        {
            CBaseEntity* entity = static_cast<CBaseEntity*>(body->getUserPointer());

            btVector3 point = body->getCenterOfMassPosition();
            entity->setPosition((f32)point[0], (f32)point[1], (f32)point[2]);

            btVector3 eulerRot;
            quaternionToEuler(body->getOrientation(), &eulerRot);
            entity->setRotation((f32)eulerRot[0], (f32)eulerRot[1], (f32)eulerRot[2]);
        }

        void CPhysicsManager::quaternionToEuler(const btQuaternion& quat, btVector3* euler)
        {
            btScalar w = quat.getW(),
                     x = quat.getX(),
                     y = quat.getY(),
                     z = quat.getZ();

            f32 wSquared = w * w,
                xSquared = x * x,
                ySquared = y * y,
                zSquared = z * z;

            euler->setX(atan2f(2.0f * (y * z + x * w), -xSquared - ySquared + zSquared + wSquared));
            euler->setY(asinf(-2.0f * (x * z - y * w)));
            euler->setZ(atan2f(2.0f * (x * y + z * w), xSquared - ySquared - zSquared + wSquared));

            (*euler) *= core::RADTODEG;
        }
    }
}
