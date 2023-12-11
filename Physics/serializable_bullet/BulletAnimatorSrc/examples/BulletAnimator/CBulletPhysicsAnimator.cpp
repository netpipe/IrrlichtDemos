#include "CBulletPhysicsAnimator.h"

#include "btBulletDynamicsCommon.h"
#include "btGImpactShape.h"
#include "btGImpactCollisionAlgorithm.h"


namespace irr
{
namespace scene
{

//------------------------------------------------------------------------------
//! CreateInstance
//! Creates CBulletPhysicsAnimator or returns NULL
CBulletPhysicsAnimator* CBulletPhysicsAnimator::CreateInstance(
  btDynamicsWorld* pDynamicWorld, 
  scene::ISceneNode* pSceneNode, CBulletPhysicsAnimatorGeometry* pGeom,
  CBulletPhysicsAnimatorParams* pPhysicsParam) 
{
  // get node scaling
  irr::core::vector3df scaling = pSceneNode->getScale();

  // prepare collision shape
  btCollisionShape* collisionShape = NULL;
  switch (pGeom->type)
  {
    case CBPAGT_SPHERE:
    {
      collisionShape = new btSphereShape(pGeom->sphere.radius);
    } break;

    case CBPAGT_BOX:
    {
      collisionShape = new btBoxShape(btVector3(pGeom->box.X, pGeom->box.Y, pGeom->box.Z));
    } break;

    case CBPAGT_CONVEX_MESH:
    {
      btTriangleMesh* triangleMesh = 
        ConvertIrrMeshToBulletTriangleMesh(pGeom->mesh.irrMesh, scaling);
      //btTriangleIndexVertexArray* triangleMesh = 
      //  ConvertIrrMeshToBulletTriangleArray(pGeom->mesh.irrMesh, scaling);
      collisionShape = new btConvexTriangleMeshShape(triangleMesh);

      //collisionShape = ConvertIrrMeshToBulletConvexHullShape(pGeom->mesh.irrMesh);
    } break;

    case CBPAGT_CONCAVE_MESH:
    {
      btTriangleMesh* triangleMesh = 
        ConvertIrrMeshToBulletTriangleMesh(pGeom->mesh.irrMesh, scaling);
      bool useQuantizedAabbCompression = true;
      collisionShape = new btBvhTriangleMeshShape(triangleMesh, useQuantizedAabbCompression);
    } break;

    case CBPAGT_CONCAVE_GIMPACT_MESH:
    {
      btTriangleMesh* triangleMesh = 
        ConvertIrrMeshToBulletTriangleMesh(pGeom->mesh.irrMesh, scaling);
      //btTriangleIndexVertexArray* triangleMesh = ConvertIrrMeshToBulletTriangleArray(pGeom->mesh.irrMesh);
      btGImpactMeshShape* gimpactShape = new btGImpactMeshShape(triangleMesh);
      gimpactShape->updateBound();
      collisionShape = gimpactShape;
    } break;

    default:
      // unknown type
      break;
  }
  if (collisionShape == NULL)
  {
    return NULL;
  }


  CBulletPhysicsAnimator* bulletAnimator = new CBulletPhysicsAnimator();
  bulletAnimator->physicsParams = *pPhysicsParam;
  bulletAnimator->sceneNode = pSceneNode;
  bulletAnimator->collisionShape = collisionShape;
  bulletAnimator->dynamicWorld = pDynamicWorld;

  bulletAnimator->InitPhysics();

  return bulletAnimator;
}

//------------------------------------------------------------------------------
//! Ctor
//! 
CBulletPhysicsAnimator::CBulletPhysicsAnimator() :
  rigidBody(NULL), sceneNode(NULL), collisionShape(NULL), 
  dynamicWorld(NULL), motionState(NULL)
{
#ifdef _DEBUG
  setDebugName("CBulletPhysicsAnimator");
#endif
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CBulletPhysicsAnimator::~CBulletPhysicsAnimator()
{

}


//------------------------------------------------------------------------------
//! animateNode
//! animates a scene node
void CBulletPhysicsAnimator::animateNode(scene::ISceneNode* node, u32 timeMs)
{
  _IRR_DEBUG_BREAK_IF(node != sceneNode);

  if (physicsParams.mass != 0.0f && rigidBody && rigidBody->getMotionState())
  {
    // set pos
    btPoint3 p = rigidBody->getCenterOfMassPosition();
    sceneNode->setPosition(core::vector3df(p.getX(), p.getY(), p.getZ()));

    // set rot
    core::vector3df rot;
    btQuaternion btq = rigidBody->getOrientation();
    QuaternionToEulerXYZ(btq, rot);
    rot *= BPU_180_PI;
    sceneNode->setRotation(rot);
  } 

}

//------------------------------------------------------------------------------
//! serializeAttributes
//! Writes attributes of the scene node animator.
void CBulletPhysicsAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/)
{

}

//------------------------------------------------------------------------------
//! deserializeAttributes
//! Reads attributes of the scene node animator.
void CBulletPhysicsAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/)
{

}

//------------------------------------------------------------------------------
//! InitPhysics
//! Adds body to btDynamicsWorld
void CBulletPhysicsAnimator::InitPhysics()
{
  //rigidbody is dynamic if and only if mass is non zero, otherwise static
  bool isDynamic = (physicsParams.mass != 0.0f);

  // calculate inertia vector
  btVector3 localInertia(0,0,0);
  if (isDynamic)
    collisionShape->calculateLocalInertia(physicsParams.mass, localInertia);

  // calculate value for ccdThreshold
  core::aabbox3d<f32> box = sceneNode->getBoundingBox();
  core::vector3df ext = box.getExtent();
  f32 ret=0;
  if (ext.X>ret) ret=ext.X;
  if (ext.Y>ret) ret=ext.Y;
  if (ext.Z>ret) ret=ext.Z;
  physicsParams.ccdThreshold = ret/2;

  // evaluate start transform in terms of btTransform 
  // and set offset of center of mass
  btTransform	startTransform, centerOfMassOffset;
  GetNodeTransform(sceneNode, startTransform);
  centerOfMassOffset.setIdentity();
  centerOfMassOffset.setOrigin(btVector3(physicsParams.centerOfMassOffset.X, 
    physicsParams.centerOfMassOffset.Y, physicsParams.centerOfMassOffset.Z));

  //using motionstate is recommended, it provides interpolation capabilities, 
  //and only synchronizes 'active' objects
  motionState = new btDefaultMotionState(startTransform, centerOfMassOffset);
  // create body
  rigidBody = new btRigidBody(
    physicsParams.mass, 
    motionState, 
    collisionShape, 
    localInertia,
    physicsParams.linearDamping,
    physicsParams.angularDamping,
    physicsParams.friction,
    physicsParams.restitution
  );

  // add body to the world
  dynamicWorld->addRigidBody(rigidBody);

  // Only do CCD if  motion in one timestep (1.f/60.f) exceeds half dims
  rigidBody->setCcdSquareMotionThreshold(physicsParams.ccdThreshold);
  //Experimental: better estimation of CCD Time of Impact:
  rigidBody->setCcdSweptSphereRadius(0.2*physicsParams.ccdThreshold);
}

//------------------------------------------------------------------------------
//! EndPhysics
//! Remove body from btDynamicsWorld, cleanup
void CBulletPhysicsAnimator::EndPhysics()
{
  if (dynamicWorld) 
    dynamicWorld->removeRigidBody(rigidBody);

  SAFE_DELETE(motionState);
  SAFE_DELETE(rigidBody);
}


//------------------------------------------------------------------------------
//! setPosition
//! Set animator position
void CBulletPhysicsAnimator::setPosition(const core::vector3df& v) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  btTransform t = rigidBody->getWorldTransform();
  btVector3 btv(v.X, v.Y, v.Z);
  t.setOrigin(btv);
  rigidBody->setWorldTransform(t);
}

//------------------------------------------------------------------------------
//! setRotation
//! Set animator rotation
core::vector3df CBulletPhysicsAnimator::getPosition() const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  btPoint3 p = rigidBody->getCenterOfMassPosition();
  return core::vector3df(p.getX(), p.getY(), p.getZ());
}

//------------------------------------------------------------------------------
//! setRotation
//! Set animator rotation (v measured in radians)
void CBulletPhysicsAnimator::setRotation(const core::vector3df& v) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  btTransform t = rigidBody->getWorldTransform();
  btQuaternion btq;
  EulerXYZToQuaternion(v, btq);
  t.setRotation(btq);
  rigidBody->setWorldTransform(t);
}

//------------------------------------------------------------------------------
//! getRotation
//! Get animator rotation
core::vector3df CBulletPhysicsAnimator::getRotation() const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  core::vector3df rot;
  btQuaternion btq=rigidBody->getOrientation();
  QuaternionToEulerXYZ(btq, rot);
  rot *= BPU_360_PI2;
  return rot;
}

//------------------------------------------------------------------------------
//! setLinearVelocity
//! Set animator linear vel
void CBulletPhysicsAnimator::setLinearVelocity(const core::vector3df& vel) const
{	
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  //bullet hangs if attempt to set lin. or ang. velocity on static object
  if (physicsParams.mass != 0.0f) 
  {
    rigidBody->setLinearVelocity(btVector3(vel.X, vel.Y, vel.Z));
  }
  //note - apparently, need to use motion state on kinematic objects, and rigid body functions
  // on dynamic rigid bodies to change pos / rot etc.
}

//------------------------------------------------------------------------------
//! getLinearVelocity
//! Get animator linear vel
core::vector3df CBulletPhysicsAnimator::getLinearVelocity() const
{	
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  if (physicsParams.mass == 0.0f) 
    return core::vector3df(0,0,0);
  btVector3 btV = rigidBody->getLinearVelocity();
  return core::vector3df(btV.getX(), btV.getY(), btV.getZ());
}

//------------------------------------------------------------------------------
//! setLinearVelocity
//! Set animator linear vel
void CBulletPhysicsAnimator::setAngularVelocity(const core::vector3df & vel) const
{	
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  //bullet hangs if attempt to set lin. or ang. velocity on static object
  if (physicsParams.mass != 0.0f) 
  {
    rigidBody->setAngularVelocity(btVector3(vel.X, vel.Y, vel.Z));
  }
}

//------------------------------------------------------------------------------
//! getAngularVelocity
//! Get animator angular vel
core::vector3df CBulletPhysicsAnimator::getAngularVelocity() const
{	
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  if (physicsParams.mass == 0.0f) 
    return core::vector3df(0,0,0);

  btVector3 btV = rigidBody->getAngularVelocity();
  return core::vector3df(btV.getX(), btV.getY(), btV.getZ());
}

//------------------------------------------------------------------------------
//! applyImpulse
//! Apply impulse to object
void CBulletPhysicsAnimator::applyImpulse(const core::vector3df& force, 
  const core::vector3df& rel_pos/* = core::vector3df(0,0,0)*/) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->applyImpulse(btVector3(force.X, force.Y, force.Z), 
    btVector3(rel_pos.X, rel_pos.Y, rel_pos.Z));
  rigidBody->activate();
}

//------------------------------------------------------------------------------
//! applyForce
//! Apply force to object
void CBulletPhysicsAnimator::applyForce(const core::vector3df& force, 
  const core::vector3df& rel_pos/* = core::vector3df(0,0,0)*/) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->applyForce(btVector3(force.X, force.Y, force.Z), 
    btVector3(rel_pos.X, rel_pos.Y, rel_pos.Z));
  rigidBody->activate();
}

//------------------------------------------------------------------------------
//! zeroForces
//! Zero all forces
void CBulletPhysicsAnimator::zeroForces()
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->setLinearVelocity(btVector3(0,0,0));
  rigidBody->setAngularVelocity(btVector3(0,0,0));
}

//------------------------------------------------------------------------------
//! setActivationState
//! Zero all forces
void CBulletPhysicsAnimator::setActivationState(bool active)
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->setActivationState((active) ? 1 : 0);
}


//------------------------------------------------------------------------------
//! activate
//! Zero all forces
void CBulletPhysicsAnimator::activate(bool force)
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->activate(force);
}


} // end namespace scene
} // end namespace irr


