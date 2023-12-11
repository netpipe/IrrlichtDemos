////////////////////////////////////////////////////////////
// Author: Tanner Mickelson
// Description: Class managing multiple tracks of animation data.
// License: ZLIB
// Implementation file:CAnimatedHuman.cpp
////////////////////////////////////////////////////////////
#ifndef ANIMATED_HUMAN_H
#define ANIMATED_HUMAN_H
#include <Irrlicht.h>
#include "SAnimation.h"

class CAnimatedHuman : public irr::scene::IAnimationEndCallBack
{
 public:
 
 CAnimatedHuman(irr::scene::IAnimatedMesh* mesh, irr::scene::ISceneManager* smgr);
 ~CAnimatedHuman();

 //New animation iterface
 void addAnimation(SAnimation anim);
 void changeAnimation(irr::core::stringc name, SAnimation newAnim);
 void removeAnimation(irr::core::stringc name);
 SAnimation& getAnimationByName(irr::core::stringc name);
 irr::core::stringc getNameOfCurrentAnim(){return m_currentAnim;};

 void playAnimation(irr::core::stringc name);
 void playAnimation(SAnimation anim);//Plays a seperate animation.

 void setNextAnimation(irr::core::stringc name){m_nextAnim = name;}

 void update(float dt);

 irr::scene::IAnimatedMeshSceneNode* getSceneNode(){return m_meshNode;};

 //Callback for looping
 void OnAnimationEnd(irr::scene::IAnimatedMeshSceneNode* node);

 private:

 //Node to actually render
 irr::scene::IAnimatedMeshSceneNode* m_meshNode;
 
 //Array of animations that this character can play
 irr::core::array<SAnimation> m_animations;

 //Name of the next animation to play.
 irr::core::stringc m_currentAnim;
 irr::core::stringc m_nextAnim;

};

#endif