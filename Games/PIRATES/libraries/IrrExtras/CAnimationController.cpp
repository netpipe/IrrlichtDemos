////////////////////////////////////////////////////////////
// Author: Tanner Mickelson
// Description: Class manging multiple tracks of animation data.
// License: ZLIB
// Header file:CAnimatedHuman.h
////////////////////////////////////////////////////////////
#include "CAnimationController.h"
//==========================================================
//Constructor/destructor.
//==========================================================
CAnimatedHuman::CAnimatedHuman(irr::scene::IAnimatedMesh* mesh, irr::scene::ISceneManager* smgr)
{
 //Store mesh
 //m_finalBlend = smgr->addAnimatedMeshSceneNode(mesh);

// m_currentWalkTrack = 0;
 m_currentAnim = "";
 m_nextAnim = "";

 m_meshNode = smgr->addAnimatedMeshSceneNode(mesh);
 m_meshNode->setAnimationEndCallback(this);
 //m_walkTrack->setVisible(false);
}
CAnimatedHuman::~CAnimatedHuman()
{
 m_meshNode->remove();
// m_finalBlend->remove();
}
//==========================================================
//Add/remove animations
//==========================================================
void CAnimatedHuman::addAnimation(SAnimation anim)
{
 if ( anim.m_name == "" )
 {
  printf("Error: Animation has no name. Animation was not added.\n");
  return;
 }

 if ( !anim.m_anim )
 {
  printf("Error: No animation data in animation: ");
  printf(anim.m_name.c_str());
  printf(".\n");
  return;
 }

 m_animations.push_back(anim);

 printf("Added animation: ");
 printf(anim.m_name.c_str());
 printf(".\n");
}

void CAnimatedHuman::removeAnimation(irr::core::stringc name)
{
 for ( irr::u32 i=0; i<m_animations.size(); ++i )
 {
  if ( m_animations[i].m_name == name )
  {
	  printf("Removed animation: ");
	  printf(name.c_str());
	  printf(".\n");

	  m_animations.erase(i);
	  return;
  }
 }

 printf("Warning: No animation matching this name exists\n");//If control falls through then we couldn't find any to remove.
}
//==========================================================
//Change animation
//==========================================================
void CAnimatedHuman::changeAnimation(irr::core::stringc name, SAnimation newAnim)
{
 printf("Warning: Change animation is unimplemented.\n");

 if ( name == "" ) return;

}
//==========================================================
//Play animation
//==========================================================
void CAnimatedHuman::playAnimation(irr::core::stringc name)
{
 for ( irr::u32 i=0; i<m_animations.size(); ++i )
 {
  if ( m_animations[i].m_name == name )
  {
   SAnimation& anim = m_animations[i];

   if ( m_meshNode->getMesh()->getMeshType() == irr::scene::EAMT_SKINNED && anim.m_anim )
   {
    ( (irr::scene::ISkinnedMesh*)m_meshNode->getMesh() )->useAnimationFrom(anim.m_anim);//Play animation.
	m_meshNode->setFrameLoop(0,m_meshNode->getMesh()->getFrameCount());
   }
   else
   {
	printf("Error: Not a skinned mesh.\n");
   }

   m_meshNode->setLoopMode(anim.m_loop);

   m_meshNode->setTransitionTime(anim.m_transitionTime);

   m_meshNode->setAnimationSpeed(anim.m_animSpeed);

   //Copy anim name
   m_currentAnim = anim.m_name;
   if ( anim.m_nextAnimName != "" )
	   m_nextAnim = anim.m_nextAnimName;

   return;
  }
 }
  printf("Warning: No animation matching this name exists\n");//If control falls through then we couldn't find any to play.
}
void CAnimatedHuman::playAnimation(SAnimation anim)
{
 printf("Warning: playAnim(SAnimation anim) is untested and animation chaining is not implemented.\n");

 if ( m_meshNode->getMesh()->getMeshType() == irr::scene::EAMT_SKINNED && anim.m_anim )
   {
    ( (irr::scene::ISkinnedMesh*)m_meshNode->getMesh() )->useAnimationFrom(anim.m_anim);//Play animation.
	m_meshNode->setFrameLoop(0,m_meshNode->getMesh()->getFrameCount());
   }
   else
   {
	printf("Error: Not a skinned mesh.\n");
   }

   m_meshNode->setLoopMode(anim.m_loop);

   m_meshNode->setTransitionTime(anim.m_transitionTime);

   m_meshNode->setAnimationSpeed(anim.m_animSpeed);

   //Copy anim name
   m_currentAnim = anim.m_name;
}
//==========================================================
//Get animations
//==========================================================
SAnimation& CAnimatedHuman::getAnimationByName(irr::core::stringc name)
{
 for ( irr::u32 i=0; i<m_animations.size(); ++i )
 {
  if ( m_animations[i].m_name == name )
  {
   return m_animations[i];
  }
 }
 printf("Error: No animation exists with this name.");
 return SAnimation();//Return empty animation.
}
//==========================================================
//Update
//==========================================================
void CAnimatedHuman::update(float dt)
{
 m_meshNode->animateJoints();
}
//==========================================================
//End callback for animation chaining
//==========================================================
void CAnimatedHuman::OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node)
{
 if ( m_nextAnim == "" )
 {
  printf("Warning: Next animation name is empty.\n");
  return;
 }

 playAnimation(m_nextAnim);
 m_nextAnim = "";
}
