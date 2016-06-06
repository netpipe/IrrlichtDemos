// A game application wrapper class

#ifndef __C_BULLET_PHYSICS_H_INCLUDED__
#define __C_BULLET_PHYSICS_H_INCLUDED__

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


//! Irrlicht
#include "irrlicht.h"
using namespace irr;


#include "CBulletPhysicsAnimator.h"

//struct 
//{
//};


//!	CBulletPhysics
/** 
*/
class CBulletPhysics
{
public:

  //! Ctor
  CBulletPhysics();
  //! Dtor
  ~CBulletPhysics();


  //! Base initialization
  bool Create();
  //! Update
  void OnUpdate(u32 timeMs);

  //! Create animator
  scene::CBulletPhysicsAnimator* createBulletPhysicsAnimator(
    scene::ISceneNode* sceneNode, scene::CBulletPhysicsAnimatorGeometry* geometry,
    scene::CBulletPhysicsAnimatorParams* physicsParams
  );

  //! Adds mesh scene node
  bool AddMeshSceneNode(scene::ISceneNode* pNode, scene::IMesh* pMesh, bool pIsStatic);

  //! Adds sphere scene node
  bool AddSphereSceneNode(scene::ISceneNode* pNode, f32 pRadius, bool pIsStatic);


private:

  //! 
  btRigidBody* AddRigidBody(float pMass, const btTransform& pStartTransform, btCollisionShape* pShape);


  //! Physics clocks
  btClock                iClock;

  //! 
  btConstraintSolver*    iConstraintSolver;
  //! 
  btCollisionDispatcher* iCollisionDispatcher;
  //! this is the most important class
  btDynamicsWorld*		   iDynamicsWorld;
  //! constraint for mouse picking
  btTypedConstraint*	   iPickConstraint;


  btAlignedObjectArray<scene::ISceneNode*> iSceneNodes;
};

#endif //__C_BULLET_PHYSICS_H_INCLUDED__
