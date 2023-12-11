    /*
    irrDynamics - Light-weight Bullet Physics wrapper for the irrlicht graphics engine
    Copyright (C) 2014  Otto Naderer - otto.naderer@aec.at

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
    */

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "irrDynamics.h"

using namespace irr;

irrDynamics* irrDynamics::instance = NULL;

irrDynamics::irrDynamics() : lastStep(0)
{
// Initialize bullet
    collisionConfiguration = new btDefaultCollisionConfiguration();
    broadPhase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
}

irrDynamics* irrDynamics::getInstance()
{
    if (!instance)
    {
        instance = new irrDynamics();
    }
    return instance;
}

void irrDynamics::simStep(const u32& curTimeStamp)
{
    irrDynamics* inst = getInstance();
    if (inst->lastStep == 0)
        inst->lastStep = curTimeStamp;

    inst->world->stepSimulation((curTimeStamp - inst->lastStep) * 0.001f, 5);
    inst->updateObjects();
    inst->lastStep = curTimeStamp;
}

void irrDynamics::shutdown()
{
    irrDynamics* inst = getInstance();
    inst->clearObjects();
    delete inst->world;
    delete inst->solver;
    delete inst->dispatcher;
    delete inst->broadPhase;
    delete inst->collisionConfiguration;
}

void irrDynamics::addTerrain(scene::ITerrainSceneNode* terrain, u32 lodLevel)
{
    irrDynamics* inst = getInstance();
    terrain->updateAbsolutePosition();
    core::vector3df irrPos = terrain->getPosition();
    btVector3 btPos(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(btPos);

    btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

    btTriangleMesh * mTriMesh = new btTriangleMesh();
    scene::CDynamicMeshBuffer* buffer = 0;
    btVector3 vertices[3];
    s32 j, k;
    buffer = new scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_32BIT);
    (terrain)->getMeshBufferForLOD(*buffer, lodLevel);

    core::vector3df terrScale = terrain->getScale();
    //Build the triangleMesh
    const u32 indexCount = buffer->getIndexCount();
    printf("indeces: %u\n", indexCount);
    video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*) buffer->getVertexBuffer().getData();
    u32* mb_indices = (u32*) buffer->getIndices();
    for (j = 0; (u32) j < indexCount; j += 3)
    {
        for (k = 0; k < 3; k++)
        {
            s32 index = mb_indices[j + k];
            vertices[k] = btVector3(mb_vertices[index].Pos.X * terrScale.X, mb_vertices[index].Pos.Y * terrScale.Y, mb_vertices[index].Pos.Z * terrScale.Z);
        }
        mTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
    }

    buffer->drop();
    //Add the terrain collision shape to the world
    btBvhTriangleMeshShape* mShape = new btBvhTriangleMeshShape(mTriMesh, true);
    btRigidBody* rbody = new btRigidBody(0.f, MotionState, mShape);
    inst->world->addRigidBody(rbody);
    inst->objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(terrain, rbody));

    terrain->grab();
}

void irrDynamics::debugDraw()
{
//    getInstance()->bWorld->debugDrawWorld(true);
}

void irrDynamics::updateObjects()
{
    for (std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
    {
        btVector3 Point = iter->second->getCenterOfMassPosition();
        iter->first->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

        // Set rotation
        core::vector3df euler;
        const btQuaternion& quat = iter->second->getOrientation();
        core::quaternion q(quat.getX(), quat.getY(), quat.getZ(), quat.getW());
        q.toEuler(euler);
        euler *= core::RADTODEG;
        iter->first->setRotation(euler);
    }
}

void irrDynamics::removeObject(scene::ISceneNode* node)
{
    irrDynamics* inst = getInstance();
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = inst->objects.find(node);
    if (iter != inst->objects.end())
    {
        inst->removeConstraints(iter->second);
        inst->world->removeRigidBody(iter->second);

        // Free memory
        delete iter->second->getMotionState();
        delete iter->second->getCollisionShape();
        delete iter->second;
        iter->first->drop();
        inst->objects.erase(iter);
    }
    else
    {
        printf("irrdynamics: object not found in map!\n");
    }
}

void irrDynamics::clearObjects()
{
    for (std::map<scene::ISceneNode*, btRigidBody*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
    {
        removeConstraints(iter->second);
        world->removeRigidBody(iter->second);

        // Free memory
        delete iter->second->getMotionState();
        delete iter->second->getCollisionShape();
        delete iter->second;
        iter->first->drop();
    }
    objects.clear();
}

btRigidBody* irrDynamics::addSphericalObject(scene::ISceneNode* node, f32 radius, f32 mass)
{
    irrDynamics* inst = getInstance();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(tPosition);

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);

    // Create the shape
    btCollisionShape *shape = new btSphereShape(radius);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(mass, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, shape, localInertia);

    // Add it to the world
    inst->world->addRigidBody(rigidBody);
    inst->objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    node->grab();
    return rigidBody;
}

btRigidBody* irrDynamics::addBoxObject(scene::ISceneNode* node, f32 mass)
{
    irrDynamics* inst = getInstance();
    node->updateAbsolutePosition();
    core::vector3df irrPos = node->getAbsolutePosition();
    btVector3 tPosition(irrPos.X, irrPos.Y, irrPos.Z);



    const core::aabbox3df aabox = node->getTransformedBoundingBox();
    core::vector3df fullExtent = aabox.getExtent();
    btVector3 halfExtent(fullExtent.X * .5f, fullExtent.Y * .5f, fullExtent.Z * .5f);

    //get rotation to quaternion
    core::vector3df rotationRadians = node->getRotation();
    rotationRadians *= core::DEGTORAD;
    core::quaternion quat(rotationRadians);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(tPosition);
    transform.setRotation(btQuaternion(quat.X, quat.Y, quat.Z, quat.W));

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);

    // Create the shape
    btCollisionShape *shape = new btBoxShape(halfExtent);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(mass, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(mass, motionState, shape, localInertia);

    // Add it to the world
    inst->world->addRigidBody(rigidBody);
    inst->objects.insert(std::pair<scene::ISceneNode*, btRigidBody*>(node, rigidBody));
    node->grab();
    return rigidBody;
}

bool irrDynamics::createHingeConstraint(scene::ISceneNode* nodeA, scene::ISceneNode* nodeB, const core::vector3df& pivotInA, const core::vector3df& pivotInB, const core::vector3df& axisInA, const irr::core::vector3df& axisInB)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = inst->objects.find(nodeA);
    iterB = inst->objects.find(nodeB);

    if (iterA == inst->objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == inst->objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btHingeConstraint* constraint = new btHingeConstraint(*(iterA->second), *(iterB->second), btVector3(pivotInA.X, pivotInA.Y, pivotInA.Z), btVector3(pivotInB.X, pivotInB.Y, pivotInB.Z), btVector3(axisInA.X, axisInA.Y, axisInA.Z), btVector3(axisInB.X, axisInB.Y, axisInB.Z));
    inst->world->addConstraint(constraint);
    return true;
}

bool irrDynamics::createPoint2PointConstraint(scene::ISceneNode* nodeA, scene::ISceneNode* nodeB, const core::vector3df& pivotInA, const core::vector3df& pivotInB)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = inst->objects.find(nodeA);
    iterB = inst->objects.find(nodeB);

    if (iterA == inst->objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == inst->objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*(iterA->second), *(iterB->second), btVector3(pivotInA.X, pivotInA.Y, pivotInA.Z), btVector3(pivotInB.X, pivotInB.Y, pivotInB.Z));
    inst->world->addConstraint(constraint);
    return true;
}

bool irrDynamics::createSliderConstraint(scene::ISceneNode* nodeA,
                                         scene::ISceneNode* nodeB,
                                         const core::vector3df& posInA,
                                         const core::vector3df& rotInA,
                                         const core::vector3df& posInB,
                                         const core::vector3df& rotInB)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid bodies:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iterA, iterB;
    iterA = inst->objects.find(nodeA);
    iterB = inst->objects.find(nodeB);

    if (iterA == inst->objects.end())
    {
        printf("irrdynamics: Unable to find first node for constraint!\n");
        return false;
    }

    if (iterB == inst->objects.end())
    {
        printf("irrdynamics: Unable to find second node for constraint!\n");
        return false;
    }

    btTransform matA, matB;
    core::vector3df rotationRadians = rotInA;
    rotationRadians *= core::DEGTORAD;
    core::quaternion rotA(rotationRadians);

    rotationRadians = rotInB;
    rotationRadians *= core::DEGTORAD;
    core::quaternion rotB(rotationRadians);

    matA.setIdentity();
    matA.setOrigin(btVector3(posInA.X, posInA.Y, posInA.Z));
    matA.setRotation(btQuaternion(rotA.X, rotA.Y, rotA.Z, rotA.W));

    matB.setIdentity();
    matB.setOrigin(btVector3(posInB.X, posInB.Y, posInB.Z));
    matB.setRotation(btQuaternion(rotB.X, rotB.Y, rotB.Z, rotB.W));


    btSliderConstraint* constraint = new btSliderConstraint(*(iterA->second), *(iterB->second), matA, matB, true);
    inst->world->addConstraint(constraint);
    return true;
}

void irrDynamics::applyCentralForce(scene::ISceneNode* node, const core::vector3df& force)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = inst->objects.find(node);
    if (iter == inst->objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Force application aborted.\n");
        return;
    }

    iter->second->applyCentralForce(btVector3(force.X, force.Y, force.Z));
}

btRigidBody* irrDynamics::addFloor(const core::vector3df& normal, const core::vector3df& offset)
{
    irrDynamics* inst = getInstance();
    btVector3 tPosition(offset.X, offset.Y, offset.Z);
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(tPosition);

    btDefaultMotionState *motionState = new btDefaultMotionState(transform);

    // Create the shape
    btCollisionShape *shape = new btStaticPlaneShape(btVector3(normal.X, normal.Y, normal.Z), 0);

    // Add mass
    btVector3 localInertia;
    shape->calculateLocalInertia(0.f, localInertia);

    // Create the rigid body object
    btRigidBody *rigidBody = new btRigidBody(0.f, motionState, shape, localInertia);

    // Add it to the world
    inst->world->addRigidBody(rigidBody);
    return rigidBody;
}

void irrDynamics::setDamping(scene::ISceneNode* node, f32 linearDamping, f32 angularDamping)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = inst->objects.find(node);
    if (iter == inst->objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Damping application aborted.\n");
        return;
    }

    iter->second->setDamping(linearDamping, angularDamping);
}

void irrDynamics::setPosition(scene::ISceneNode* node, const core::vector3df& newPos)
{
    irrDynamics* inst = getInstance();
    //find the corresponding rigid body:
    std::map<scene::ISceneNode*, btRigidBody*>::iterator iter;
    iter = inst->objects.find(node);
    if (iter == inst->objects.end())
    {
        printf("irrdynamics: Unable to find node in list. Position update aborted.\n");
        return;
    }

    btTransform transform = iter->second->getCenterOfMassTransform();
    transform.setOrigin(btVector3(newPos.X, newPos.Y, newPos.Z));
    iter->second->setCenterOfMassTransform(transform);
}

void irrDynamics::removeConstraints(btRigidBody* rigidBody)
{
    const int len = rigidBody->getNumConstraintRefs();
    //first, remove constraint reference at the other node
    for (int i = 0; i < len; i++)
    {
        btTypedConstraint* constraint = rigidBody->getConstraintRef(i);
        btRigidBody* bodyB = &(constraint->getRigidBodyB());
        if (bodyB == rigidBody)
            bodyB = &(constraint->getRigidBodyA());

        bodyB->removeConstraintRef(constraint);
    }
    //then remove/delete at our side
    while (rigidBody->getNumConstraintRefs() > 0)
    {
        btTypedConstraint* constraint = rigidBody->getConstraintRef(0);
        world->removeConstraint(constraint);
        rigidBody->removeConstraintRef(constraint);
        delete constraint;
    }
}
