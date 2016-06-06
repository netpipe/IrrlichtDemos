// coded by zet

#include "CBulletAnimatorManager.h"

#include "ISceneNodeAnimatorCollisionResponse.h"
#include "ISceneManager.h"


#include "CBulletWorldAnimator.h"
#include "CBulletObjectAnimator.h"


extern bool gDisableDeactivation;

namespace irr
{
namespace scene
{

//! Bullet animator types count
const s32 SceneNodeAnimatorTypeCount = 2;
//! Names for scene node types
struct CSceneNodeAnimatorTypeName { ESCENE_NODE_BULLET_ANIMATOR_TYPE type; const c8* name; };
CSceneNodeAnimatorTypeName const SceneNodeAnimatorTypeNames[] =
{
  { ESNAT_BULLET_WORLD, "bulletWorld" },
  { ESNAT_BULLET_OBJECT, "bulletObject" },
};


CBulletAnimatorManager::CBulletAnimatorManager(ISceneManager* mgr)
: sceneManager(mgr)
{
	// don't grab the scene manager here to prevent cyclic references

  // register factory
  sceneManager->registerSceneNodeAnimatorFactory(this);

  gDisableDeactivation = false;
}


CBulletAnimatorManager::~CBulletAnimatorManager()
{
}



//------------------------------------------------------------------------------
//! createBulletWorldAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletWorldAnimator* CBulletAnimatorManager::createBulletWorldAnimator(
  ISceneManager* sceneManager,
  ISceneNode* sceneNode, 
  CBulletWorldAnimatorParams* params)
{
  CBulletWorldAnimator* bulletWorld = 
    CBulletWorldAnimator::createInstance(sceneManager, sceneNode, this, params);

  if (bulletWorld != NULL)
    bulletWorldAnimators.push_back(bulletWorld);

  return bulletWorld;
}

//------------------------------------------------------------------------------
//! createEmptyBulletWorldAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletWorldAnimator* CBulletAnimatorManager::createEmptyBulletWorldAnimator(
  ISceneManager* sceneManager, 
  ISceneNode* sceneNode)
{
  CBulletWorldAnimator* bulletWorld = 
    CBulletWorldAnimator::createEmptyInstance(sceneManager, sceneNode, this);

  if (bulletWorld != NULL)
    bulletWorldAnimators.push_back(bulletWorld);

  return bulletWorld;
}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletObjectAnimator* CBulletAnimatorManager::createBulletObjectAnimator(
  ISceneManager* sceneManager, 
  ISceneNode* sceneNode, 
  s32 bulletWorldID,
  CBulletObjectAnimatorGeometry* geometry,
  CBulletObjectAnimatorParams* physicsParams)
{
  CBulletObjectAnimator* bulletObject = 
    CBulletObjectAnimator::createInstance(
      sceneManager, 
      sceneNode,
      this,
      geometry,
      physicsParams
    );

  bulletObject->AddToWorld(bulletWorldID);

  return bulletObject;
}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletCharaterAnimator

CBulletChracterAnimator* CBulletAnimatorManager::createBulletCharcterAnimator(
	ISceneManager* scenemanager, 
	ISceneNode* sceneNode, 
	s32 bulletWorldID,
	CBulletObjectAnimatorGeometry* geometry,
	CBulletObjectAnimatorParams* physicsParams
	)
{
	CBulletChracterAnimator* bulletObject = 
		(CBulletChracterAnimator*)CBulletChracterAnimator::createInstance(
		sceneManager, 
		sceneNode,
		this,
		geometry,
		physicsParams
		);

	bulletObject->AddToWorld(bulletWorldID);	

	return (CBulletChracterAnimator*)bulletObject;

}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletWorldAnimatorAnimator
CBulletObjectAnimator* CBulletAnimatorManager::createEmptyBulletObjectAnimator(
  ISceneManager* sceneManager, 
  ISceneNode* sceneNode)
{
  CBulletObjectAnimator* bulletObject = 
    CBulletObjectAnimator::createEmptyInstance(
      sceneManager, 
      sceneNode,
      this
    );

  return bulletObject;
}

//------------------------------------------------------------------------------
//! removeWorld
//! Remove world from the list
void CBulletAnimatorManager::removeWorld(CBulletWorldAnimator* world)
{
  core::list<CBulletWorldAnimator*>::Iterator it = bulletWorldAnimators.begin();
  for (; it != bulletWorldAnimators.end(); ++it)
    if ((*it) == world)
    {
      bulletWorldAnimators.erase(it);
      break;
    }
}

//------------------------------------------------------------------------------
//! getBulletWorldByID
//! Get Bullet world by identifier
CBulletWorldAnimator* CBulletAnimatorManager::getBulletWorldByID(s32 id)
{
  core::list<CBulletWorldAnimator*>::Iterator it = bulletWorldAnimators.begin();
  for (; it != bulletWorldAnimators.end(); ++it)
    if ((*it)->getID() == id)
      return (*it);

  return NULL;
}

//------------------------------------------------------------------------------
//! createBulletPhysicsAnimator
//! Create CBulletWorldAnimatorAnimator
void CBulletAnimatorManager::OnUpdate(u32 timeMs)
{
  core::list<CBulletWorldAnimator*>::Iterator it = bulletWorldAnimators.begin();
  for (; it != bulletWorldAnimators.end(); ++it)
  {
    (*it)->OnUpdate(timeMs);
  }
}


//! creates a scene node animator based on its type id
ISceneNodeAnimator* CBulletAnimatorManager::createSceneNodeAnimator(ESCENE_NODE_ANIMATOR_TYPE type, ISceneNode* target)
{
  ESCENE_NODE_BULLET_ANIMATOR_TYPE animType = (ESCENE_NODE_BULLET_ANIMATOR_TYPE)type;

  ISceneNodeAnimator* anim = NULL;
  switch (animType)
  {
    case ESNAT_BULLET_WORLD:
    {
      // create bullet world animator
      //if (BulletWorldAnimator != NULL) { BulletWorldAnimator->drop(); BulletWorldAnimator = NULL; } 
      anim = createEmptyBulletWorldAnimator(sceneManager, target);
    } break;

    case ESNAT_BULLET_OBJECT:
    {
      // create object
      anim = createEmptyBulletObjectAnimator(sceneManager, target);
    } break;
  }

  if (anim && target)
  {
    target->addAnimator(anim);
  }

  return anim;
}

//! creates a scene node animator based on its type name
ISceneNodeAnimator* CBulletAnimatorManager::createSceneNodeAnimator(const c8* typeName, ISceneNode* target)
{
  ESCENE_NODE_BULLET_ANIMATOR_TYPE animType = ESNAT_BULLET_UNKNOWN;
  s32 i;
  for (i=0; i < SceneNodeAnimatorTypeCount; ++i)
  {
    if (!strcmp(typeName, SceneNodeAnimatorTypeNames[i].name) )
    {
      animType = SceneNodeAnimatorTypeNames[i].type;
      break;
    }
  }

  return createSceneNodeAnimator((ESCENE_NODE_ANIMATOR_TYPE)animType, target);
}


//! returns amount of scene node animator types this factory is able to create
#ifdef  IRR_OLD_13X
s32 CBulletAnimatorManager::getCreatableSceneNodeAnimatorTypeCount()
#else
u32 CBulletAnimatorManager::getCreatableSceneNodeAnimatorTypeCount() const
#endif
//s32 CBulletAnimatorManager::getCreatableSceneNodeAnimatorTypeCount()
{
	return SceneNodeAnimatorTypeCount;
}


//! returns type of a createable scene node animator type
#ifdef  IRR_OLD_13X
ESCENE_NODE_ANIMATOR_TYPE CBulletAnimatorManager::getCreateableSceneNodeAnimatorType(s32 idx)
#else
ESCENE_NODE_ANIMATOR_TYPE CBulletAnimatorManager::getCreateableSceneNodeAnimatorType(u32 idx) const
#endif
//ESCENE_NODE_ANIMATOR_TYPE CBulletAnimatorManager::getCreateableSceneNodeAnimatorType(s32 idx)
{
  if (idx>=0 && idx<SceneNodeAnimatorTypeCount)
    return (ESCENE_NODE_ANIMATOR_TYPE)SceneNodeAnimatorTypeNames[idx].type;

	return ESNAT_UNKNOWN;
}


//! returns type name of a createable scene node animator type 
#ifdef  IRR_OLD_13X
     const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(s32 idx)
#else
	 const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(u32 idx) const
#endif	
//const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(s32 idx)
{
	if (idx>=0 && idx<SceneNodeAnimatorTypeCount)
		return SceneNodeAnimatorTypeNames[idx].name;

	return 0;
}

//! returns type name of a createable scene node animator type 
#ifdef  IRR_OLD_13X
     const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type)
#else
	 const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type) const
#endif	
//const c8* CBulletAnimatorManager::getCreateableSceneNodeAnimatorTypeName(ESCENE_NODE_ANIMATOR_TYPE type)
{
  s32 i;
  for (i = 0; i < SceneNodeAnimatorTypeCount; i++)
  {
    if ((ESCENE_NODE_BULLET_ANIMATOR_TYPE)type == SceneNodeAnimatorTypeNames[i].type)
      return SceneNodeAnimatorTypeNames[i].name;
  }

	return 0;
}

} // end namespace scene
} // end namespace irr

