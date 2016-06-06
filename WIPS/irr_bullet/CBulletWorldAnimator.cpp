#pragma warning(disable:949)
#pragma warning(disable:4819)
#include "CBulletWorldAnimator.h"

#include "btBulletDynamicsCommon.h"
#include "btGImpactShape.h"
#include "btGImpactCollisionAlgorithm.h"

//#define USE_PARALLEL_DISPATCHER 1
#ifdef USE_PARALLEL_DISPATCHER
#include "SpuGatheringCollisionDispatcher.h"
#endif//USE_PARALLEL_DISPATCHER


#include "CBulletAnimatorManager.h"


namespace irr
{
namespace scene
{

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletWorldAnimator* CBulletWorldAnimator::createInstance(
  ISceneManager* scenemanager,
  ISceneNode* sceneNode,
  CBulletAnimatorManager* bulletMgr,
  CBulletWorldAnimatorParams* params)
{
  CBulletWorldAnimator* animator = new CBulletWorldAnimator(bulletMgr);

  // set params
  animator->setGravity(params->gravity);

  // set world's ID
  static s32 IDCounter = 1;
  animator->iID = IDCounter++;

  return animator;
}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletWorldAnimator* CBulletWorldAnimator::createEmptyInstance(
  ISceneManager* scenemanager,
  ISceneNode* sceneNode,
  CBulletAnimatorManager* bulletMgr)
{
  CBulletWorldAnimator* animator = new CBulletWorldAnimator(bulletMgr);

  return animator;
}


//------------------------------------------------------------------------------
//! Ctor
//! Base initialization
CBulletWorldAnimator::CBulletWorldAnimator(CBulletAnimatorManager* bulletMgr)
: iBulletManager(bulletMgr), iID(-1)
{
#ifdef _DEBUG
  setDebugName("CBulletWorldAnimator");
#endif

#ifdef USE_PARALLEL_DISPATCHER
  iCollisionDispatcher = new	SpuGatheringCollisionDispatcher();
#else

  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  iCollisionDispatcher = new	btCollisionDispatcher(collisionConfiguration);

#endif//USE_PARALLEL_DISPATCHER

  //register algorithm
  btGImpactCollisionAlgorithm::registerAlgorithm(iCollisionDispatcher);


  btVector3 aWorldMin(-10000,-10000,-10000);
  btVector3 aWorldMax(10000,10000,10000);
  iPairCache = new btAxisSweep3(aWorldMin,aWorldMax);
  //btOverlappingPairCache* aPairCache = new btAxisSweep3(aWorldMin,aWorldMax);

  //iPairCache = new btSimpleBroadphase(); 

  iConstraintSolver = new btSequentialImpulseConstraintSolver();
  iDynamicsWorld = new btDiscreteDynamicsWorld(
	  iCollisionDispatcher, 
	  iPairCache,	  
	  iConstraintSolver,
	  collisionConfiguration
	  );

#ifdef USE_PARALLEL_DISPATCHER
  iDynamicsWorld->getDispatchInfo().m_enableSPU=true;
#endif //USE_PARALLEL_DISPATCHER
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CBulletWorldAnimator::~CBulletWorldAnimator()
{
  iBulletManager->removeWorld(this);

  delete iDynamicsWorld;
  delete iConstraintSolver;
  delete iPairCache;
  delete iCollisionDispatcher;
}

//------------------------------------------------------------------------------
//! getType
//! Returns type of the scene node animator
ESCENE_NODE_ANIMATOR_TYPE CBulletWorldAnimator::getType() const 
{ 
  return (ESCENE_NODE_ANIMATOR_TYPE)ESNAT_BULLET_WORLD; 
}


//------------------------------------------------------------------------------
//! setGravity
//! World's gravity
void CBulletWorldAnimator::setGravity(const core::vector3df& gravity)
{
  iGravity.setValue(gravity.X, gravity.Y, gravity.Z);
  iDynamicsWorld->setGravity(iGravity);
}

//------------------------------------------------------------------------------
//! getGravity
//! World's gravity
core::vector3df CBulletWorldAnimator::getGravity() const
{
  return core::vector3df(iGravity.x(), iGravity.y(), iGravity.z());
}


//------------------------------------------------------------------------------
//! Custom per tick update func
//! 
void CBulletWorldAnimator::OnUpdate(u32 timeMs)
{
  if (iDynamicsWorld)
  {
    //remember, Bullet uses a fixed internal timestep and interpolates
    // if the delta time is different. Damn useful.
    iDynamicsWorld->stepSimulation(timeMs/1000.f, 3, 1.f/60.f);
	//static float ftick =0;
	//ftick += (1/60.f);
	// float dt = float(getDeltaTimeMicroseconds()) * 0.000001f;
	//iDynamicsWorld->stepSimulation(dt);

    // apply the new positions and rotations to the scenenodes
    iDynamicsWorld->updateAabbs();
  }
}

//------------------------------------------------------------------------------
//! RegisterBody
//! 
void CBulletWorldAnimator::RegisterBody(btRigidBody* rigidBody)
{
  iDynamicsWorld->addRigidBody(rigidBody);
}

//------------------------------------------------------------------------------
//! UnregisterBody
//! 
void CBulletWorldAnimator::UnregisterBody(btRigidBody* rigidBody)
{
  iDynamicsWorld->removeRigidBody(rigidBody);
}



//------------------------------------------------------------------------------
//! animateNode
//! animates a scene node
void CBulletWorldAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
}

//------------------------------------------------------------------------------
//! serializeAttributes
//! Writes attributes of the scene node animator.
void CBulletWorldAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/)
{
  // store world's id
  out->addInt("id", iID);

  core::vector3df ip;
  btPoint3 bp;
  btQuaternion bq;

  // save global gravity
  out->addVector3d("gravity", core::vector3df(iGravity.getX(), iGravity.getY(), iGravity.getZ()));
}

//------------------------------------------------------------------------------
//! deserializeAttributes
//! Reads attributes of the scene node animator.
void CBulletWorldAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/)
{
  // set world's id
  iID = in->getAttributeAsInt("id");

  core::vector3df ip;
  btPoint3 bp;
  btQuaternion bq;

  // save global gravity
  ip = in->getAttributeAsVector3d("gravity");
  iGravity = btVector3(ip.X, ip.Y, ip.Z);
  iDynamicsWorld->setGravity(iGravity);
}

} // end namespace scene
} // end namespace irr
