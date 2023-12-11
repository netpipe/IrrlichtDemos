// A game application wrapper class

#ifndef __C_SCENE_NODE_ANIMATOR_BULLET_WORLD_H_INCLUDED__
#define __C_SCENE_NODE_ANIMATOR_BULLET_WORLD_H_INCLUDED__

//! Bullet
#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"
class btOverlappingPairCache;
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


#include "CBulletObjectAnimator.h"



namespace irr
{
namespace scene
{

  //------------------------------------------------------------------------------
  //! Helper structure to configure animator physics parameters
  struct CBulletWorldAnimatorParams 
  {
    CBulletWorldAnimatorParams() : gravity(0, -9.81f, 0) {}

    core::vector3df gravity;
  };

  //!	CBulletWorldAnimator
  /** 
  */
  class CBulletWorldAnimator : public ISceneNodeAnimator
  {
  public:

    //! Dtor
    ~CBulletWorldAnimator();


    //! Create animator
    static CBulletWorldAnimator* createInstance(
      ISceneManager* scenemanager,
      ISceneNode* sceneNode,
      CBulletAnimatorManager* bulletMgr,
      CBulletWorldAnimatorParams* params
    );
    //! Create animator
    static CBulletWorldAnimator* createEmptyInstance(
      ISceneManager* scenemanager,
      ISceneNode* sceneNode,
      CBulletAnimatorManager* bulletMgr
    );

    //! World's gravity
    void setGravity(const core::vector3df& gravity);
    core::vector3df getGravity() const;



    //! Update
    void OnUpdate(u32 timeMs);

    //!  
    void RegisterBody(btRigidBody* rigidBody);
    void UnregisterBody(btRigidBody* rigidBody);

    //! Returns world's ID
    s32  getID() { return iID; }



  protected:

    //! Ctor
    CBulletWorldAnimator(CBulletAnimatorManager* bulletMgr);

    //! animates a scene node
    void animateNode(ISceneNode* node, u32 timeMs);

    //! Writes attributes of the scene node animator.
    void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options=0);

    //! Reads attributes of the scene node animator.
    void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options=0);

    //! Returns type of the scene node animator
    ESCENE_NODE_ANIMATOR_TYPE getType() const;


    //! Physics clocks
    btClock                iClock;

    //! 
    btOverlappingPairCache*iPairCache;
    //! 
    btConstraintSolver*    iConstraintSolver;
    //! 
    btCollisionDispatcher* iCollisionDispatcher;
    //! this is the most important class
    btDynamicsWorld*		   iDynamicsWorld;
    //! constraint for mouse picking
    btTypedConstraint*	   iPickConstraint;
    //! Global world's gravity
    btVector3              iGravity;

    //! World's identifier
    CBulletAnimatorManager*iBulletManager;
    s32                    iID;
  };

} // end namespace scene
} // end namespace irr

#endif //__C_SCENE_NODE_ANIMATOR_BULLET_WORLD_H_INCLUDED__
