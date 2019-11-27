/*

Flag - header file

*/

#ifndef __Flag_h
#define __Flag_h

#include <irrlicht.h>
#include <BMaths.h>

using namespace irr;


class cFlag {
public:
	
	// constructor
	cFlag();
	
	// destructor
	~cFlag();
	
	// Initialization
	bool Initialize(IrrlichtDevice *device,
					 core::vector3df position=core::vector3df(0,0,0),
					 core::vector3df rotation=core::vector3df(0,0,0),
					 core::vector3df scale=core::vector3df(1,1,1));
	
	// draw flag
	void drawFlag();
	
	
private:
	
	scene::IAnimatedMeshSceneNode *flagNode;
	
	core::array<core::vector3df> bonesOsci;

	// storing bone names
	core::array<irr::c8*> boneName;
	// storing bone scene nodes
	core::array<scene::IBoneSceneNode*> bones;
	// storing bone rotations for stand position
	core::array<core::vector3df> bonesRot;
	
	// motion variable
	irr::u32 i;
	// force of the wind
	irr::u32 speed;

	// enumeration for bones	
	enum SKT_BONE {
		SKT_BONE01,
		SKT_BONE02,
		SKT_BONE03,
		SKT_BONE04,
		SKT_BONE05,
		SKT_BONE06,
		SKT_BONE07,
		
		// number of bones
		SKT_BONE_COUNT
	};
	
};

#endif
