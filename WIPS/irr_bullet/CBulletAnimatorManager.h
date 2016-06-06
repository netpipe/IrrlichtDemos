// A game application wrapper class
// 1.4.1 support!

#ifndef __C_BULLET_SCENE_NODE_ANIMATOR_FACTORY_H_INCLUDED__
#define __C_BULLET_SCENE_NODE_ANIMATOR_FACTORY_H_INCLUDED__

#include "irrlicht.h"

#include "CBulletWorldAnimator.h"
#include "CBulletObjectAnimator.h"
#include "BulletChracterAnimator.h"

namespace irr
{
namespace scene
{
  class  ISceneNodeAnimator;
  class  ISceneManager;

  //! An enumeration for all types of built-in scene node animators
  enum ESCENE_NODE_BULLET_ANIMATOR_TYPE
  {
    ESNAT_BULLET_WORLD = 777,
    ESNAT_BULLET_OBJECT,
    ESNAT_BULLET_UNKNOWN
  };

  //!	Interface making it possible to dynamicly create scene nodes animators 
  class CBulletAnimatorManager : public ISceneNodeAnimatorFactory
  {
  public:

    CBulletAnimatorManager(ISceneManager* mgr);
    ~CBulletAnimatorManager();

    //! Create animator
    CBulletWorldAnimator* createBulletWorldAnimator(
      ISceneManager* scenemanager,
      ISceneNode* sceneNode, 
      CBulletWorldAnimatorParams* params
    );
    //! Create animator
    CBulletObjectAnimator* createBulletObjectAnimator(
      ISceneManager* scenemanager, 
      ISceneNode* sceneNode, 
      s32 bulletWorldID,
      CBulletObjectAnimatorGeometry* geometry,
      CBulletObjectAnimatorParams* physicsParams
    );

	//! Create animator
    CBulletChracterAnimator* createBulletCharcterAnimator(
      ISceneManager* scenemanager, 
      ISceneNode* sceneNode, 
      s32 bulletWorldID,
      CBulletObjectAnimatorGeometry* geometry,
      CBulletObjectAnimatorParams* physicsParams
    );

    //! Remove world from the list
    void removeWorld(CBulletWorldAnimator* world);

    //! Get world by id
    CBulletWorldAnimator* getBulletWorldByID(s32 id);

    //! Update
    void OnUpdate(u32 timeMs);


  private:


    //! creates a scene node animator based on its type id
    /** \param type: Type of the scene node animator to add.
    \param target: Target scene node of the new animator.
    \return Returns pointer to the new scene node animator or null if not successful. You need to
    drop this pointer after calling this, see IUnknown::drop() for details. */
    virtual ISceneNodeAnimator* createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target);

    //! creates a scene node animator based on its type name
    /** \param typeName: Type of the scene node animator to add.
    \param target: Target scene node of the new animator.
    \return Returns pointer to the new scene node animator or null if not successful. You need to
    drop this pointer after calling this, see IUnknown::drop() for details. */
    virtual ISceneNodeAnimator* createSceneNodeAnimator(const char* typeName, ISceneNode* target);

    //! returns amount of scene node animator types this factory is able to create
#ifdef  IRR_OLD_13X
    virtual s32 getCreatableSceneNodeAnimatorTypeCount();
#else
	virtual u32 getCreatableSceneNodeAnimatorTypeCount() const;
#endif

    //! returns type of a createable scene node animator type
    /** \param idx: Index of scene node animator type in this factory. Must be a value between 0 and
    getCreatableSceneNodeTypeCount() */
#ifdef  IRR_OLD_13X
    virtual ESCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(s32 idx);
#else
	virtual ESCENE_NODE_ANIMATOR_TYPE getCreateableSceneNodeAnimatorType(u32 idx) const;
#endif
	

    //! returns type name of a createable scene node animator type 
    /** \param idx: Index of scene node animator type in this factory. Must be a value between 0 and
    getCreatableSceneNodeAnimatorTypeCount() */
#ifdef  IRR_OLD_13X
    virtual const c8* getCreateableSceneNodeAnimatorTypeName(s32 idx);
#else
	virtual const c8* getCreateableSceneNodeAnimatorTypeName(u32 idx) const;
#endif
	

    //! returns type name of a createable scene node animator type 
    /** \param type: Type of scene node animator. 
    \return: Returns name of scene node animator type if this factory can create the type, otherwise 0. */
#ifdef  IRR_OLD_13X
    virtual const c8* getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type);
#else
	virtual const c8* getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const;
#endif
	


    //! Create empty animators for deserialization
    CBulletWorldAnimator* createEmptyBulletWorldAnimator(
      ISceneManager* scenemanager,
      ISceneNode* sceneNode
    );
    CBulletObjectAnimator* createEmptyBulletObjectAnimator(
      ISceneManager* scenemanager, 
      ISceneNode* sceneNode
    );


    core::list<CBulletWorldAnimator*> bulletWorldAnimators;
    ISceneManager* sceneManager;
  };


} // end namespace scene
} // end namespace irr


#endif //__C_BULLET_SCENE_NODE_ANIMATOR_FACTORY_H_INCLUDED__
