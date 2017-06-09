/** 
 * \RunOn: Irrlicht Engine 
 * \Summary: Third Person Camera 
 * \CreatedBy: FOSP Team - NamVoDang(kiemkhachchilo) 
 * \EditedBy: Strong99
 **/ 

#ifndef _THIRD_PERSON_CAMERA_H_ 
#define _THIRD_PERSON_CAMERA_H_ 

#include "include.h" 
class ThirdPersonCamera
{ 
public: 
	//! constructor
	ThirdPersonCamera(irr::scene::ISceneNode* targetNode, island::CCore* core); 

	//! return camera node
	irr::scene::ICameraSceneNode* getPointer() const; 

	//! event listener
	void onMouse(const irr::SEvent & event);

	//! Update camera / render / animate
	void update();
private: 
	float Zoom; 
	
	irr::s32 Rotation,nRotationX,nRotationY; 
	
	bool rotating; 
	bool Mouse[1];//Left 0, Right 1. 
	
	irr::gui::ICursorControl* cursor; 
	irr::core::position2d<irr::s32> mousePos; 
	irr::core::position2d<irr::s32> curMousePos; 
	irr::core::vector3df camPos;//Camera position. 
	irr::core::vector3df target;//Look at. 
	irr::core::vector3df rotation; 
	irr::f32 RotateStartX, RotateStartY, curentRotateStartX, curentRotateStartY; 
	irr::scene::ISceneNode* targetNode;//Character. 
	irr::scene::ICameraSceneNode* camera;//Camera. 

	irr::s32 lastUpdateTime;

	void onRemove(); 
	island::CCore* core;
}; 
#endif 