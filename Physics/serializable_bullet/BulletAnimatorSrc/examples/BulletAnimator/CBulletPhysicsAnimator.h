// A game application wrapper class

#ifndef __C_BULLET_PHYSICS_ANIMATOR_H_INCLUDED__
#define __C_BULLET_PHYSICS_ANIMATOR_H_INCLUDED__

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
class btTriangleMesh;
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

  //------------------------------------------------------------------------------
  //!	CBulletPhysicsAnimatorGeometryTypes
  //! All available bullet's geometry types
  enum CBulletPhysicsAnimatorGeometryTypes
  {
    CBPAGT_SPHERE = 0,
    CBPAGT_BOX,
    CBPAGT_CYLINDER,
    CBPAGT_CAPSULE,
    CBPAGT_CONE,
    CBPAGT_STATIC_PLANE,

    CBPAGT_CONVEX_MESH,
    CBPAGT_CONCAVE_MESH,
    CBPAGT_CONCAVE_GIMPACT_MESH,

    //! This enum is never used, it only forces the compiler to 
    //! compile these enumeration values to 32 bit.
    CBPAGT_FORCE_32_BIT = 0x7fffffff
  };

  //------------------------------------------------------------------------------
  //! Helper structure to configure animator geometry
  struct CBulletPhysicsAnimatorGeometry 
  {
    CBulletPhysicsAnimatorGeometryTypes type;
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
    
      struct CMesh 
      {
        IMesh* irrMesh;  //! mesh interface
      } mesh;
    };
  };

  //------------------------------------------------------------------------------
  //! Helper structure to configure animator physics parameters
  struct CBulletPhysicsAnimatorParams 
  {
    CBulletPhysicsAnimatorParams() : mass(1.0f), ccdThreshold(0.0f), 
      linearDamping(0.0f), angularDamping(0.0f), friction(0.5f),
      restitution(0.0f), centerOfMassOffset() {}

    f32 mass;
    f32 ccdThreshold;
    f32 linearDamping;
    f32 angularDamping;
    f32 friction;
    f32 restitution;
    core::vector3df centerOfMassOffset;
  };


  //------------------------------------------------------------------------------
  //!	CBulletPhysicsAnimator
  /** Performs synchronization task between Bullet's object and corresponding
  *   scene node, also applies external forces to Bullet's object
  */
  class CBulletPhysicsAnimator : public ISceneNodeAnimator
  {
  public:


    //! CreateInstance
    static CBulletPhysicsAnimator* CreateInstance(
      btDynamicsWorld* pDynamicWorld, 
      scene::ISceneNode* pSceneNode, CBulletPhysicsAnimatorGeometry* pGeom,
      CBulletPhysicsAnimatorParams* pPhysicsParam
    );
    //! Dtor
    virtual ~CBulletPhysicsAnimator();


    //! animates a scene node
    void animateNode(scene::ISceneNode* node, u32 timeMs);

    //! Writes attributes of the scene node animator.
    void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

    //! Reads attributes of the scene node animator.
    void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

    //! Returns type of the scene node animator
    ESCENE_NODE_ANIMATOR_TYPE getType() const { return ESNAT_COLLISION_RESPONSE; }

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
    void zeroForces();
    void setActivationState(bool active);
    void activate(bool force = false);


  protected:


    //! Ctor
    CBulletPhysicsAnimator();

    //! 
    void InitPhysics();
    void EndPhysics();



    CBulletPhysicsAnimatorParams physicsParams;
    btRigidBody*                 rigidBody;
    btDefaultMotionState*        motionState;
    btCollisionShape*            collisionShape;
    scene::ISceneNode*           sceneNode;
    btDynamicsWorld*             dynamicWorld;
  };

} // end namespace scene
} // end namespace irr

#endif //__C_BULLET_PHYSICS_ANIMATOR_H_INCLUDED__
