// A game application wrapper class

#ifndef __C_SCENE_NODE_ANIMATOR_BULLET_OBJECT_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_BULLET_OBJECT_H_INCLUDED__

//! Bullet
#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"
class btConstraintSolver;
class btCollisionDispatcher;
class btDynamicsWorld;
class btTypedConstraint;
class btTriangleIndexVertexArray;
class btStridingMeshInterface;
class btCollisionShape;
class btRigidBody;
struct btDefaultMotionState;


//! Irrlicht
#include "irrlicht.h"

#include "CBulletPhysicsUtils.h"

namespace irr
{
namespace scene
{
  class CBulletAnimatorManager;

  //------------------------------------------------------------------------------
  //!	CBulletObjectAnimatorGeometryTypes
  //! All available bullet's geometry types
  enum CBulletObjectAnimatorGeometryTypes
  {
    CBPAGT_NONE = 0,
    CBPAGT_SPHERE,
    CBPAGT_BOX,
    CBPAGT_CYLINDER,
    CBPAGT_CAPSULE,
    CBPAGT_CONE,
    CBPAGT_STATIC_PLANE,

    CBPAGT_CONVEX_MESH,
    CBPAGT_CONCAVE_MESH,
    CBPAGT_CONCAVE_GIMPACT_MESH,

	//gbox patch
	CBPAGT_CONCAVE_LOD_TERRAIN_MESH,
	/////////////////

    //! This enum is never used, it only forces the compiler to 
    //! compile these enumeration values to 32 bit.
    CBPAGT_FORCE_32_BIT = 0x7fffffff
  };

  //------------------------------------------------------------------------------
  //! Helper structure to configure animator geometry
  struct CBulletObjectAnimatorGeometry 
  {
	  CBulletObjectAnimatorGeometry() 
	  { type = CBPAGT_NONE; mesh.irrMesh = NULL; }
	  CBulletObjectAnimatorGeometry(const CBulletObjectAnimatorGeometry& obj) 
	  { type = obj.type; meshFile = obj.meshFile; sphere = obj.sphere; box = obj.box; mesh = obj.mesh; }

	  CBulletObjectAnimatorGeometryTypes type;
	  union
	  {
		  struct CSphereGeom 
		  {
			  f32 radius;             //! sphere radius
		  } sphere;

		  struct CBoxGeom 
		  { 
			  f32 X, Y, Z;            //! boxHalfExtents
		  } box;

		  struct CCapsuleGeom 
		  { 
			  f32 radius;            //! capsule radius
			  f32 hight;            //!capsule hight
		  } Capsule;

		  struct CMesh 
		  {
			  IMesh* irrMesh;         //! mesh interface
		  } mesh;

		  struct CTerrainMesh
		  { 
			  //ITerrainSceneNode *pTerrain;
			  s32 meshLod;
			  //f32 hight;            //!capsule hight
		  } terrain_mesh;
	  };
	  core::stringc meshFile; //! mesh file
  };

  //------------------------------------------------------------------------------
  //! Helper structure to configure animator physics parameters
  struct CBulletObjectAnimatorParams 
  {
	  CBulletObjectAnimatorParams() : mass(1.0f), ccdThreshold(0.0f), 
		  linearDamping(0.0f), angularDamping(0.0f), friction(0.5f),
		  restitution(0.0f), centerOfMassOffset(), gravity(0, 0, 0) {}

	  f32 mass;
	  f32 ccdThreshold;
	  f32 linearDamping;
	  f32 angularDamping;
	  f32 friction;
	  f32 restitution;
	  core::vector3df gravity;
	  core::vector3df centerOfMassOffset;
  };


  //------------------------------------------------------------------------------
  //!	CBulletObjectAnimator
  /** Performs synchronization task between Bullet's object and corresponding
  *   scene node, also applies external forces to Bullet's object
  */
  class CBulletObjectAnimator : public ISceneNodeAnimator
  {
  public:

    //! CreateInstance
    static CBulletObjectAnimator* createInstance(
      ISceneManager* pSceneManager,
      ISceneNode* pSceneNode,
      CBulletAnimatorManager* pBulletMgr,
      CBulletObjectAnimatorGeometry* pGeom,
      CBulletObjectAnimatorParams* pPhysicsParam
    );
    //! CreateEmptyInstance
    static CBulletObjectAnimator* createEmptyInstance(
      ISceneManager* pSceneManager,
      ISceneNode* pSceneNode,
      CBulletAnimatorManager* pBulletMgr
    );


    //! Dtor
    virtual ~CBulletObjectAnimator();


    virtual void AddToWorld(s32 worldID);
    void RemoveFromWorld();


    //! Various setters/getters
    void setPosition(const core::vector3df& v = core::vector3df(0,0,0)) const; 
    void setRotation(const core::vector3df& v = core::vector3df(0,0,0)) const; 
    core::vector3df getRotation() const;
    core::vector3df getPosition() const;

    void setLinearVelocity(const core::vector3df& vel = core::vector3df(0,0,0)) const;
    void setAngularVelocity(const core::vector3df& vel = core::vector3df(0,0,0)) const;
    core::vector3df getAngularVelocity() const;
    core::vector3df getLinearVelocity() const;

    void applyImpulse(const core::vector3df& impulse, 
      const core::vector3df& rel_pos = core::vector3df(0,0,0)) const;

    void applyForce(const core::vector3df& force, 
      const core::vector3df& rel_pos = core::vector3df(0,0,0)) const;
    void zeroForces() const;
    void setActivationState(bool active) const;
    void activate(bool force = false) const;

	//gbox patch
	btRigidBody* getRigidBody()
	{		
		return rigidBody;
	}

	ISceneNode* const getSceneNode()
	{
		return sceneNode;
	}

	///////////////////////

  protected:

    //! Ctor
    CBulletObjectAnimator();

    //! 
    void InitPhysics();

    //! Initialize empty animator
    bool InitEmptyInstance();


    //!
    static btCollisionShape* CreateBulletCollisionShape(ISceneManager* pSceneManager,
      CBulletObjectAnimatorGeometry* pGeom, 
      const core::vector3df& pScaling,
      btStridingMeshInterface*& triangleMesh);

    static IMesh* GetMeshFromGeom(ISceneManager* pSceneManager,
      CBulletObjectAnimatorGeometry* pGeom);

    //! Helper function for irrlicht mesh data serialization 
    void SerializeIrrMesh(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);
    void DeserializeIrrMesh(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);


    //! animates a scene node
    virtual void animateNode(ISceneNode* node, u32 timeMs);

    //! Writes attributes of the scene node animator.
    void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

    //! Reads attributes of the scene node animator.
    void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

    //! Returns type of the scene node animator
    ESCENE_NODE_ANIMATOR_TYPE getType() const;



    ISceneManager*               sceneManager;
    ISceneNode*                  sceneNode;

    CBulletObjectAnimatorGeometry geometry;
    CBulletObjectAnimatorParams  physicsParams;
    btRigidBody*                 rigidBody;
    btDefaultMotionState*        motionState;
    btStridingMeshInterface*     bulletMesh;
    btCollisionShape*            collisionShape;

    CBulletAnimatorManager*      bulletMgr;
    s32                          bulletWorldID;
  };

} // end namespace scene
} // end namespace irr

#endif //__C_SCENE_NODE_ANIMATOR_BULLET_OBJECT_H_INCLUDED__
