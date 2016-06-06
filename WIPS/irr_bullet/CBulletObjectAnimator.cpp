#pragma warning (disable:4819)

//for 1.4.1 version

#include "CBulletObjectAnimator.h"
#include "btBulletDynamicsCommon.h"
#include "btGImpactShape.h"
#include "btGImpactCollisionAlgorithm.h"

#include "CBulletAnimatorManager.h"
#include "CBulletWorldAnimator.h"



namespace irr
{
namespace scene
{

//! Names for alignments
const c8* const CBulletObjectAnimatorGeometryTypesNames[] =
{
  "none",
  "sphere",
  "box",
  "cylinder",
  "capsule",
  "cone",
  "plane",
  "convex",
  "concave",
  "concave_gimpact",
  "concave_Terrain",
  0
};

//------------------------------------------------------------------------------
//! CreateInstance
//! Creates CBulletObjectAnimator or returns NULL
CBulletObjectAnimator* CBulletObjectAnimator::createInstance(
  ISceneManager* pSceneManager,
  ISceneNode* pSceneNode,
  CBulletAnimatorManager* pBulletMgr,
  CBulletObjectAnimatorGeometry* pGeom,
  CBulletObjectAnimatorParams* pPhysicsParam) 
{
  // get node scaling
  core::vector3df scaling = pSceneNode->getScale();

  btStridingMeshInterface* triangleMesh = NULL;
  // prepare collision shape
  btCollisionShape* collisionShape = 
    CreateBulletCollisionShape(pSceneManager, pGeom, scaling, triangleMesh);
  if (collisionShape == NULL)
    return NULL;

  CBulletObjectAnimator* bulletAnimator = new CBulletObjectAnimator();
  bulletAnimator->geometry = *pGeom;
  bulletAnimator->physicsParams = *pPhysicsParam;
  bulletAnimator->bulletMesh = triangleMesh;
  bulletAnimator->collisionShape = collisionShape;

  bulletAnimator->sceneNode = pSceneNode;
  bulletAnimator->sceneManager = pSceneManager;
  bulletAnimator->bulletMgr = pBulletMgr;

  bulletAnimator->InitPhysics();

  return bulletAnimator;
}

//------------------------------------------------------------------------------
//! CreateEmptyInstance
//! Creates empty CBulletObjectAnimator
CBulletObjectAnimator* CBulletObjectAnimator::createEmptyInstance(
  ISceneManager* pSceneManager,
  ISceneNode* pSceneNode,
  CBulletAnimatorManager* pBulletMgr) 
{
  // create empty bullet animator
  CBulletObjectAnimator* bulletAnimator = new CBulletObjectAnimator();
  bulletAnimator->sceneNode = pSceneNode;
  bulletAnimator->sceneManager = pSceneManager;
  bulletAnimator->bulletMgr = pBulletMgr;

  return bulletAnimator;
}

//------------------------------------------------------------------------------
//! Ctor
//! 
CBulletObjectAnimator::CBulletObjectAnimator() :
  sceneManager(NULL), rigidBody(NULL), sceneNode(NULL), 
  bulletMesh(NULL), collisionShape(NULL), motionState(NULL),
  bulletMgr(NULL), bulletWorldID(-1)
{
#ifdef _DEBUG
  setDebugName("CBulletObjectAnimator");
#endif
}

//------------------------------------------------------------------------------
//! Dtor
//! 
CBulletObjectAnimator::~CBulletObjectAnimator()
{
  RemoveFromWorld();
  SAFE_DELETE(bulletMesh);
  SAFE_DELETE(collisionShape);
  SAFE_DELETE(motionState);
  SAFE_DELETE(rigidBody);
}

//------------------------------------------------------------------------------
//! getType
//! Returns type of the scene node animator
ESCENE_NODE_ANIMATOR_TYPE CBulletObjectAnimator::getType() const 
{ 
  return (ESCENE_NODE_ANIMATOR_TYPE)ESNAT_BULLET_OBJECT; 
}

//------------------------------------------------------------------------------
//! InitializeEmptyInstance
//! Initialize empty animator. Only to use with serialization.
bool CBulletObjectAnimator::InitEmptyInstance()
{
  // get node scaling
  core::vector3df scaling = sceneNode->getScale();

  // prepare collision shape
  collisionShape = CreateBulletCollisionShape(sceneManager, &geometry, scaling, bulletMesh);
  if (collisionShape == NULL)
    return NULL;

  InitPhysics();

  AddToWorld(bulletWorldID);

  return true;
}


//------------------------------------------------------------------------------
//! GetMeshFromGeom
//! Helper function to extract mesh data from object geometry
IMesh* CBulletObjectAnimator::GetMeshFromGeom(ISceneManager* pSceneManager,
  CBulletObjectAnimatorGeometry* pGeom)
{
  IMesh* mesh = pGeom->mesh.irrMesh;
  if (mesh == NULL)
  {
    IAnimatedMesh* animMesh = pSceneManager->getMesh(pGeom->meshFile.c_str());
    if (animMesh != NULL)
      mesh = animMesh->getMesh(0);
  }

  return mesh;
}

//------------------------------------------------------------------------------
//! CreateBulletCollisionShape
//! Helper function to create bullet-compatible shape
btCollisionShape* CBulletObjectAnimator::CreateBulletCollisionShape(
  ISceneManager* pSceneManager,
  CBulletObjectAnimatorGeometry* pGeom, 
  const core::vector3df& pScaling,
  btStridingMeshInterface*& triangleMesh)
{
	triangleMesh = NULL;
	// prepare collision shape
	btCollisionShape* collisionShape = NULL;
	switch (pGeom->type)
	{
	case CBPAGT_CAPSULE:
		{
			collisionShape = new btCapsuleShape(pGeom->Capsule.radius,pGeom->Capsule.hight);
		}
		break;
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
			IMesh* mesh = GetMeshFromGeom(pSceneManager, pGeom);
			if (mesh != NULL)
			{
				triangleMesh = ConvertIrrMeshToBulletTriangleMesh(mesh, pScaling);
				//triangleMesh = ConvertIrrMeshToBulletTriangleArray(mesh, pScaling);
				collisionShape = new btConvexTriangleMeshShape(triangleMesh);
				//collisionShape = ConvertIrrMeshToBulletConvexHullShape(mesh);
			}
		} break;

	case CBPAGT_CONCAVE_MESH:
		{
			IMesh* mesh = GetMeshFromGeom(pSceneManager, pGeom);
			if (mesh != NULL)
			{
				triangleMesh = ConvertIrrMeshToBulletTriangleMesh(mesh, pScaling);
				bool useQuantizedAabbCompression = true;
				collisionShape = new btBvhTriangleMeshShape(triangleMesh, useQuantizedAabbCompression);
			}
		} break;

	case CBPAGT_CONCAVE_GIMPACT_MESH:
		{
			IMesh* mesh = GetMeshFromGeom(pSceneManager, pGeom);
			if (mesh != NULL)
			{
				triangleMesh = ConvertIrrMeshToBulletTriangleMesh(mesh, pScaling);
				//triangleMesh = ConvertIrrMeshToBulletTriangleArray(mesh, pScaling);
				btGImpactMeshShape* gimpactShape = new btGImpactMeshShape(triangleMesh);
				gimpactShape->updateBound();
				collisionShape = gimpactShape;
			}
		} break;
	case CBPAGT_CONCAVE_LOD_TERRAIN_MESH:
		{
			//메쉬컨버팅
			{
				btVector3 vertices[3];
				irr::u32 j,k,index,numVertices,numIndices;
				irr::u16* mb_indices;

				irr::scene::ITerrainSceneNode *pTerrain = 
					(irr::scene::ITerrainSceneNode *)pSceneManager->getSceneNodeFromName(pGeom->meshFile.c_str());

				irr::scene::SMeshBufferLightMap smb;
				pTerrain->getMeshBufferForLOD(smb,pGeom->terrain_mesh.meshLod);				

				numVertices = smb.getVertexCount ();//mb->getVertexCount();
				numIndices = smb.getIndexCount();//mb->getIndexCount();
				mb_indices = smb.getIndices();// mb->getIndices();
				irr::video::S3DVertex2TCoords* mb_vertices = 
					(irr::video::S3DVertex2TCoords*)smb.getVertices();// ->getVertices();

				{
					btTriangleMesh *pTriMesh = new btTriangleMesh();

					irr::core::vector3df scaling = pScaling;

					for(j=0;j<numIndices;j+=3)
					{ //index into irrlicht data
						for (k=0;k<3;k++) 
						{
							index = mb_indices[j+k];
							// we apply scaling factor directly to verticies
							vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X,
								mb_vertices[index].Pos.Y*scaling.Y,
								mb_vertices[index].Pos.Z*scaling.Z);
						}
						pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
					}
					triangleMesh = pTriMesh;
				}
			}
			
			bool useQuantizedAabbCompression = true;
			collisionShape = new btBvhTriangleMeshShape(triangleMesh, useQuantizedAabbCompression);
		}
		break;
	default:
		// unknown type
		break;
	}

	return collisionShape;
}

//------------------------------------------------------------------------------
//! InitPhysics
//! Adds body to btDynamicsWorld
void CBulletObjectAnimator::InitPhysics()
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
  /*
  rigidBody = new btRigidBody(
    physicsParams.mass, 
    motionState, 
    collisionShape, 
    localInertia,
    physicsParams.linearDamping,
    physicsParams.angularDamping,
    physicsParams.friction,
    physicsParams.restitution
  );*/

  rigidBody = new btRigidBody(
    physicsParams.mass, 
    motionState, 
    collisionShape, 
    localInertia);
  rigidBody->setDamping(physicsParams.linearDamping,physicsParams.angularDamping);
  rigidBody->setFriction(physicsParams.friction);


  // Only do CCD if  motion in one timestep (1.f/60.f) exceeds half dims
  rigidBody->setCcdSquareMotionThreshold((btScalar)(physicsParams.ccdThreshold));
  //Experimental: better estimation of CCD Time of Impact:
  rigidBody->setCcdSweptSphereRadius((btScalar)(.2*physicsParams.ccdThreshold));
}

//------------------------------------------------------------------------------
//! AddToWorld
//! Adds body to Bullet World
void CBulletObjectAnimator::AddToWorld(s32 worldID)
{
  bulletWorldID = worldID;

  if (bulletWorldID != -1 && rigidBody)
  {
    CBulletWorldAnimator* bulletWorld = bulletMgr->getBulletWorldByID(worldID);
    bulletWorld->RegisterBody(rigidBody);

    // apply additional constant force
    core::vector3df newGravity = bulletWorld->getGravity() + physicsParams.gravity;
    // set gravity
    rigidBody->setGravity(btVector3(newGravity.X, newGravity.Y, newGravity.Z));

    bulletWorld->grab();
  }
}

//------------------------------------------------------------------------------
//! RemoveFromWorld
//! Remove body from btDynamicsWorld
void CBulletObjectAnimator::RemoveFromWorld()
{
  if (bulletWorldID != -1 && rigidBody) 
  {
    CBulletWorldAnimator* bulletWorld = bulletMgr->getBulletWorldByID(bulletWorldID);
    bulletWorld->UnregisterBody(rigidBody);
    bulletWorld->drop();
  }

  bulletWorldID = -1;
}


//------------------------------------------------------------------------------
//! setPosition
//! Set animator position
void CBulletObjectAnimator::setPosition(const core::vector3df& v) const
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
core::vector3df CBulletObjectAnimator::getPosition() const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  btPoint3 p = rigidBody->getCenterOfMassPosition();
  return core::vector3df(p.getX(), p.getY(), p.getZ());
}

//------------------------------------------------------------------------------
//! setRotation
//! Set animator rotation (v measured in radians)
void CBulletObjectAnimator::setRotation(const core::vector3df& v) const
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
core::vector3df CBulletObjectAnimator::getRotation() const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  core::vector3df rot;
  btQuaternion btq=rigidBody->getOrientation();
  QuaternionToEulerXYZ(btq, rot);
  rot *= (f32)BPU_360_PI2;
  return rot;
}

//------------------------------------------------------------------------------
//! setLinearVelocity
//! Set animator linear vel
void CBulletObjectAnimator::setLinearVelocity(const core::vector3df& vel) const
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
core::vector3df CBulletObjectAnimator::getLinearVelocity() const
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
void CBulletObjectAnimator::setAngularVelocity(const core::vector3df & vel) const
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
core::vector3df CBulletObjectAnimator::getAngularVelocity() const
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
void CBulletObjectAnimator::applyImpulse(const core::vector3df& force, 
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
void CBulletObjectAnimator::applyForce(const core::vector3df& force, 
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
void CBulletObjectAnimator::zeroForces() const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->setLinearVelocity(btVector3(0,0,0));
  rigidBody->setAngularVelocity(btVector3(0,0,0));
}

//------------------------------------------------------------------------------
//! setActivationState
//! Zero all forces
void CBulletObjectAnimator::setActivationState(bool active) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->setActivationState((active) ? 1 : 0);
}


//------------------------------------------------------------------------------
//! activate
//! Zero all forces
void CBulletObjectAnimator::activate(bool force) const
{
  _IRR_DEBUG_BREAK_IF(rigidBody == NULL);

  rigidBody->activate(force);
}



//------------------------------------------------------------------------------
//! animateNode
//! animates a scene node
void CBulletObjectAnimator::animateNode(ISceneNode* node, u32 timeMs)
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
    rot *= (f32)BPU_180_PI;

	//rot.Y = 0-rot.Y; //왼손좌표계로 변환(may bullet use right hande axies?)

    sceneNode->setRotation(rot);
  } 
}

//------------------------------------------------------------------------------
//! serializeAttributes
//! Writes attributes of the scene node animator.
void CBulletObjectAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/)
{
  // store world's id
  out->addInt("worldID", bulletWorldID);

  core::vector3df ip;
  btPoint3 bp;
  btQuaternion bq;

  // save rigid body position
  bp = rigidBody->getCenterOfMassPosition();
  out->addVector3d("pos", core::vector3df(bp.getX(), bp.getY(), bp.getZ()));

  // save rigid body rotation
  bq = rigidBody->getOrientation();
  QuaternionToEulerXYZ(bq, ip);
  ip *= (f32)BPU_180_PI;
  out->addVector3d("rot", ip);

  // linear velocity
  bp = rigidBody->getLinearVelocity();
  out->addVector3d("lvel", core::vector3df(bp.getX(), bp.getY(), bp.getZ()));
  // angular velocity
  bp = rigidBody->getAngularVelocity();
  out->addVector3d("avel", core::vector3df(bp.getX(), bp.getY(), bp.getZ()));

  // save physics parameters
  //out->addBinary("physics", &physicsParams, sizeof(physicsParams));
  out->addFloat("m", physicsParams.mass);
  out->addFloat("ldamp", physicsParams.linearDamping);
  out->addFloat("adamp", physicsParams.angularDamping);
  out->addFloat("rest", physicsParams.restitution);
  out->addFloat("fric", physicsParams.friction);
  out->addVector3d("grav", physicsParams.gravity);
  out->addVector3d("cmof", physicsParams.centerOfMassOffset);

  // save geometry
  out->addEnum("geom_type", geometry.type, CBulletObjectAnimatorGeometryTypesNames);
  switch (geometry.type)
  {
    case CBPAGT_SPHERE:
    {
      out->addFloat("radius", geometry.sphere.radius);
    } break;
    case CBPAGT_BOX:
    {
      out->addVector3d("halfExtents", core::vector3df(geometry.box.X, geometry.box.Y, geometry.box.Z));
    } break;
    case CBPAGT_CONVEX_MESH:
    case CBPAGT_CONCAVE_MESH:
    case CBPAGT_CONCAVE_GIMPACT_MESH:
    {
      if (geometry.mesh.irrMesh != NULL)
      {
        SerializeIrrMesh(out, options);
      }
      else
      {
        out->addString("meshFile", geometry.meshFile.c_str());
      }
    } break;
  }
}

//------------------------------------------------------------------------------
//! deserializeAttributes
//! Reads attributes of the scene node animator.
void CBulletObjectAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/)
{
  // get world's id
  bulletWorldID = in->getAttributeAsInt("worldID");

  core::vector3df ip;

  // load physics parameters
  //in->getAttributeAsBinaryData("physics", &physicsParams, sizeof(physicsParams));
  physicsParams.mass = in->getAttributeAsFloat("m");
  physicsParams.linearDamping = in->getAttributeAsFloat("ldamp");
  physicsParams.angularDamping = in->getAttributeAsFloat("adamp");
  physicsParams.restitution = in->getAttributeAsFloat("rest");
  physicsParams.friction = in->getAttributeAsFloat("fric");
  physicsParams.gravity = in->getAttributeAsVector3d("grav");
  physicsParams.centerOfMassOffset = in->getAttributeAsVector3d("cmof");


  // detect geometry type
  geometry.type = 
    (CBulletObjectAnimatorGeometryTypes)in->getAttributeAsEnumeration("geom_type", CBulletObjectAnimatorGeometryTypesNames);
  switch (geometry.type)
  {
    case CBPAGT_SPHERE:
    {
      geometry.sphere.radius = in->getAttributeAsFloat("radius");
    } break;
    case CBPAGT_BOX:
    {
      ip = in->getAttributeAsVector3d("halfExtents");
      geometry.box.X = ip.X;
      geometry.box.Y = ip.Y;
      geometry.box.Z = ip.Z;
    } break;
    case CBPAGT_CONVEX_MESH:
    case CBPAGT_CONCAVE_MESH:
    case CBPAGT_CONCAVE_GIMPACT_MESH:
    {
      geometry.meshFile = in->getAttributeAsString("meshFile");
      if (geometry.meshFile == "")
      {
        DeserializeIrrMesh(in, options);
      }
    } break;
  }

  // get pos & rot
  ip = in->getAttributeAsVector3d("pos");
  sceneNode->setPosition(ip);
  ip = in->getAttributeAsVector3d("rot");
  sceneNode->setRotation(ip);

  InitEmptyInstance();

  // set vels
  if (physicsParams.mass != 0.0)
  {
    ip = in->getAttributeAsVector3d("lvel");
    rigidBody->setLinearVelocity(btVector3(ip.X, ip.Y, ip.Z));
    ip = in->getAttributeAsVector3d("avel");
    rigidBody->setAngularVelocity(btVector3(ip.X, ip.Y, ip.Z));
  }
}

//------------------------------------------------------------------------------
//! SerializeIrrMesh
//! Helper function for irrlicht mesh data serialization 
void CBulletObjectAnimator::SerializeIrrMesh(io::IAttributes* out, io::SAttributeReadWriteOptions* options/*=0*/)
{

}

//------------------------------------------------------------------------------
//! DeserializeIrrMesh
//! Helper function for irrlicht mesh data serialization 
void CBulletObjectAnimator::DeserializeIrrMesh(io::IAttributes* in, io::SAttributeReadWriteOptions* options/*=0*/)
{

}

} // end namespace scene
} // end namespace irr


