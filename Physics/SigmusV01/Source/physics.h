#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "globals.h"
#include "main.h"
#include "map.h"

class CPhysics
{
    public:
    CPhysics(ISceneManager* smgr, IVideoDriver* driver, ITimer* Timer, btVector3 Gravity, CMap* Map);
    void Update();
    bool UpdateRender(btRigidBody* TObject);
    btTriangleMesh* ConvertIrrMeshToBulletTriangleMesh(IMesh* pMesh,const vector3df& scaling);
    void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler);
    void getConvexHull(IMesh *collMesh, btConvexHullShape *hullShape, IAnimatedMeshSceneNode* node);
    list<btRigidBody *> getObjectList();

    btRigidBody* CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);
    btRigidBody* loadConvex(std::string filename, const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);
    btRigidBody* CreateConvex(IAnimatedMeshSceneNode* Node, const btVector3 &TPosition, const vector3df &TScale, btScalar TMass);

    btDiscreteDynamicsWorld* getWorld();

    list<btRigidBody *> Objects;

    void setFullDir(core::stringc fulldir);
    void loadMapObjects();

    void remove();

    u32 TimeStamp;
    u32 DeltaTime;

    private:
    core::stringc fulldir;
    ISceneManager* smgr;
    IVideoDriver* driver;
    ITimer* Timer;
    btDiscreteDynamicsWorld* World;
    btVector3 Gravity;
};

#endif
