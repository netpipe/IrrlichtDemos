////////////////////////////////////////////////////////////
// Author: Tanner Mickelson
// Description: Structure to hold animation sequence data.
// License: ZLIB
// Implementation file:none
////////////////////////////////////////////////////////////
#ifndef S_ANIMATION_H
#define S_ANIMATION_H

#include <irrlicht.h>

struct SAnimation
{
 SAnimation()
 {
  m_name = "";
  m_nextAnimName = "";
  m_anim = 0;
  m_loop = false;
  m_animSpeed = 30.0f;
  m_transitionTime = 0.0f;
 }

 irr::core::stringc m_name;

 irr::core::stringc m_nextAnimName;

 irr::scene::ISkinnedMesh* m_anim;

 float m_animSpeed;

 float m_transitionTime;

 bool m_loop;
};

#endif