#include "CBulletPhysics.h"

#include "btBulletDynamicsCommon.h"
#include "btGImpactShape.h"
#include "btGImpactCollisionAlgorithm.h"

//#define USE_PARALLEL_DISPATCHER 1
#ifdef USE_PARALLEL_DISPATCHER
#include "../../Extras/BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#endif//USE_PARALLEL_DISPATCHER

//------------------------------------------------------------------------------
//! Ctor
//! Base initialization
CBulletPhysics::CBulletPhysics()
{
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CBulletPhysics::~CBulletPhysics()
{

}

//------------------------------------------------------------------------------
//! Do additional init work after irrlicht device was created
//! 
bool CBulletPhysics::Create()
{

#ifdef USE_PARALLEL_DISPATCHER
  iCollisionDispatcher = new	SpuGatheringCollisionDispatcher();
#else
  iCollisionDispatcher = new	btCollisionDispatcher();
#endif//USE_PARALLEL_DISPATCHER

  //register algorithm
  btGImpactCollisionAlgorithm::registerAlgorithm(iCollisionDispatcher);


  //btVector3 aWorldMin(-10000,-10000,-10000);
  //btVector3 aWorldMax(10000,10000,10000);
  //btOverlappingPairCache* aPairCache = new btAxisSweep3(aWorldMin,aWorldMax);

  btOverlappingPairCache* aPairCache = new btSimpleBroadphase();
            
  iConstraintSolver = new btSequentialImpulseConstraintSolver();
  iDynamicsWorld = new btDiscreteDynamicsWorld(iCollisionDispatcher, aPairCache, iConstraintSolver);
#ifdef USE_PARALLEL_DISPATCHER
  iDynamicsWorld->getDispatchInfo().m_enableSPU=true;
#endif //USE_PARALLEL_DISPATCHER

  //iDynamicsWorld->setGravity(btVector3(0,-9.81f,0));

  return true;
}

//------------------------------------------------------------------------------
//! Custom per tick update func
//! 
void CBulletPhysics::OnUpdate(u32 timeMs)
{
  if (iDynamicsWorld)
  {
    //remember, Bullet uses a fixed internal timestep and interpolates
    // if the delta time is different. Damn useful.

    float dt = iClock.getTimeMicroseconds() * 0.00001f;
    iClock.reset();

    iDynamicsWorld->stepSimulation(dt);

    // apply the new positions and rotations to the scenenodes
    iDynamicsWorld->updateAabbs();


    //// update positions
    //s32 numObjects = iDynamicsWorld->getNumCollisionObjects();
    //for (s32 i=0; i < numObjects; i++)
    //{
    //  scene::ISceneNode* node = iSceneNodes[i];
    //  btCollisionObject* colObj = iDynamicsWorld->getCollisionObjectArray()[i];
    //  btRigidBody* body = btRigidBody::upcast(colObj);

    //  if (node && body && body->getMotionState())
    //  {
    //    btPoint3 p = body->getCenterOfMassPosition();
    //    node->setPosition(core::vector3df(p.getX(), p.getY(), p.getZ()));

    //    const double GRAD_PI = 360./3.1415926535897932384626433832795;
    //    btVector3 btv;
    //    btQuaternion btq=body->getOrientation();
    //    QuaternionToEulerXYZ(btq, btv);
    //    core::vector3df v(btv.getX(), btv.getY(), btv.getZ());
    //    v *= GRAD_PI;
    //    node->setRotation(v);
    //  } 
    //}
  }
}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletPhysicsAnimator
scene::CBulletPhysicsAnimator* CBulletPhysics::createBulletPhysicsAnimator(
  scene::ISceneNode* sceneNode, scene::CBulletPhysicsAnimatorGeometry* geometry,
  scene::CBulletPhysicsAnimatorParams* physicsParams)
{
  scene::CBulletPhysicsAnimator* animator = 
    scene::CBulletPhysicsAnimator::CreateInstance(
      iDynamicsWorld, sceneNode, geometry, physicsParams
    );

  return animator;
}


//------------------------------------------------------------------------------
//! Adds scene node
//! 
bool CBulletPhysics::AddMeshSceneNode(scene::ISceneNode* pNode, scene::IMesh* pMesh, bool pIsStatic)
{
  //btTriangleIndexVertexArray* indexVertexArrays = ConvertIrrMeshToBulletTriangleArray(pMesh);
  //btTriangleMeshShape* trimeshShape = new btTriangleMeshShape(indexVertexArrays);
  btTriangleMesh* indexVertexArrays = ConvertIrrMeshToBulletTriangleMesh(pMesh);
  bool useQuantizedAabbCompression = true;
  btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(indexVertexArrays, useQuantizedAabbCompression);


  btTransform	startTransform;
  GetNodeTransform(pNode, startTransform);

  float mass = 0.f;
  if (pIsStatic)
  {
    mass = 0.f;
  }
  else
  {
    mass = 1.f;
  }
  btRigidBody* aRigidBody = AddRigidBody(mass, startTransform, trimeshShape);
  if (pIsStatic)
    aRigidBody->setCollisionFlags(aRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

  // add scene node to control
  iSceneNodes.push_back(pNode);

  return true;
}

//------------------------------------------------------------------------------
//! Adds scene node
//! 
bool CBulletPhysics::AddSphereSceneNode(scene::ISceneNode* pNode, f32 pRadius, bool pIsStatic)
{
  btSphereShape* aSphereShape = new btSphereShape(pRadius);

  btTransform	startTransform;
  GetNodeTransform(pNode, startTransform);

  float mass = 0.f;
  if (pIsStatic)
  {
    mass = 0.f;
  }
  else
  {
    mass = 1.f;
  }
  btRigidBody* aRigidBody = AddRigidBody(mass, startTransform, aSphereShape);
  if (pIsStatic)
    aRigidBody->setCollisionFlags(aRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

  // add scene node to control
  iSceneNodes.push_back(pNode);

  return true;
}

//------------------------------------------------------------------------------
//! Adds scene node
//! 
btRigidBody* CBulletPhysics::AddRigidBody(float pMass, const btTransform& pStartTransform, btCollisionShape* pShape)
{
  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  bool isDynamic = (pMass != 0.f);

  btVector3 localInertia(0,0,0);
  if (isDynamic)
    pShape->calculateLocalInertia(pMass, localInertia);
  btScalar  linearDumping = 0.0f;
  btScalar  angularDamping = 0.0f;
  btScalar  friction = 0.5f;
  btScalar  restitution = 0.3f;
  //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
  btDefaultMotionState* myMotionState = new btDefaultMotionState(pStartTransform);
  btRigidBody* body = new btRigidBody(
    pMass, 
    myMotionState, 
    pShape, 
    localInertia,
    linearDumping,
    angularDamping,
    friction,
    restitution
  );
  iDynamicsWorld->addRigidBody(body);

  return body;
}
