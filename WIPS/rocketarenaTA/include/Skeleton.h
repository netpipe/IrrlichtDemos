/*

Skeletal human animation class - header-file

Created 2007 (modified 2008) for 3d model animations in Irrlicht 3d engine version 1.4
For more information about Irrlicht visit:
http://irrlicht.sourceforge.net

Use and/or modify it to your needs.
If you have any questions or problems, ask "xray" on Irrlicht forum

*/

#ifndef __Skeleton_h
#define __Skeleton_h

#include <irrlicht.h>

using namespace irr;

#include <BMaths.h>


// enumeration for animation types
enum CSK_ANIM {
	CSK_ANIM_STAND,
	CSK_ANIM_WALK,
	CSK_ANIM_RUN,
	CSK_ANIM_JUMP,
	CSK_ANIM_TURN,
	CSK_ANIM_ROCKET,
	CSK_ANIM_JUMP_OFF_ROCKET,
	CSK_ANIM_COUNT
};

class cSkeleton {
public:
	
	// constructor
	cSkeleton();
			
	// destructor
	~cSkeleton();
	
	// Initialisation
	// node = player scene node with bones
	// setSpeed = how fast should the animation run
	// caution: you should only take numbers between 2 and 50
	void Initialize(scene::IAnimatedMeshSceneNode *node, u32 setSpeed=6);
	
	// set the type of animation
	void setAnimType(CSK_ANIM setAnimType);
	
	// get the current animation type
	CSK_ANIM getAnimType() const;
	
	// animates the skeleton
	void animSkeleton();
	
	// returns skeleton scene node
	scene::IAnimatedMeshSceneNode *getSkeletonSceneNode();
	
	// returns motion variable
	u32 getMotionVariable();


private:
	
	scene::IAnimatedMeshSceneNode *skeletonNode;
	
	core::array<core::vector3df> bonesOsci;
	core::array<core::vector3df> bonesOsciPos;
	
	CSK_ANIM animType;
	CSK_ANIM lastAnimType;
	
	core::array<bool> startMode;
	core::array<bool> normalise;
	
	// storing bone names
	core::array<irr::c8*> boneName;
	// storing bone scene nodes
	core::array<scene::IBoneSceneNode*> bones;
	// storing bone rotations for stand position
	core::array<core::vector3df> bonesRot;
	// storing bone positions
	core::array<core::vector3df> bonesPos;
	
	// stored animation data
	core::array<core::vector3df> bonesAnimRunOsci;
	
	// motion variable
	u32 i;
	s32 k;
	// motion variable for better blending from run to walk
	u32 a_temp;
	bool endup;
	
	// speed variable
	u32 speed;
	
	enum SKT_BONE {
		
		// main items
		SKT_BONE_TORSO,
		SKT_BONE_STOMACH,
		SKT_BONE_BODY,
		SKT_BONE_NECK,
		SKT_BONE_HEAD,
		
		// right arm items
		SKT_BONE_ARM_R,
		SKT_BONE_UARM_R,
		SKT_BONE_HAND_R,
		SKT_BONE_FINGER_R,
		
		// left arm items
		SKT_BONE_ARM_L,
		SKT_BONE_UARM_L,
		SKT_BONE_HAND_L,
		SKT_BONE_FINGER_L,
		
		// right leg items
		SKT_BONE_LEG_R,
		SKT_BONE_SHIN_R,
		SKT_BONE_FEET_R,
		SKT_BONE_TOES_R,
		
		// left leg items
		SKT_BONE_LEG_L,
		SKT_BONE_SHIN_L,
		SKT_BONE_FEET_L,
		SKT_BONE_TOES_L,
		
		// number of bones
		SKT_BONE_COUNT
	};
	
};

#endif
