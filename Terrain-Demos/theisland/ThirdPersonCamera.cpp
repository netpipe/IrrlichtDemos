/** 
 * \RunOn: SGF(Simple Game Framework) - Irrlicht Engine 
 * \Summary: Third Person Camera 
 * \Filename: ThirdPersonCamera.cpp 
 * \Encoding: UTF-8 
 * \CreatedDate: 2:17:PM 2008/06/15 
 * \CreatedBy: FOSP Team - NamVoDang(kiemkhachchilo) 
 * \Copyright: Permission is granted to anyone to use this software/code 
 * \ for any purpose, including commercial applications, and to alter it 
 * \ and redistribute it freely 
 **/ 

#include "CCore.h" 
#include "ThirdPersonCamera.h" 
using namespace irr; 
using namespace scene; 
using namespace core; 
ThirdPersonCamera::ThirdPersonCamera(ISceneNode* targetNode, island::CCore* core) 
{
	this->core = core;
	this->targetNode = targetNode; 
	Zoom = 75; 
	nRotationX = 0; 
	nRotationY = 0; 
	rotating = false; 
	Mouse[0] = false; 
	Mouse[1] = false; 
	rotation = vector3df(0,15,-25);//Default rotation on level enter. 

	cursor = core->getCursor(); 
	ISceneManager* smgr = core->getSceneManager(); 
	camera = smgr->getActiveCamera();//add camera. 

	lastUpdateTime = core->getRealTime();
} 

void ThirdPersonCamera::update() 
{
	irr::f32 deltaTime = core->getRealTime() - lastUpdateTime;
	lastUpdateTime = core->getRealTime();
	if (Mouse[1])//Rotation process. 
	{ 
		if (!rotating) 
		{
			mousePos = cursor->getPosition(); 
			curMousePos = mousePos; 
			rotating = true; 

		}
		//rotate
		else 
		{
			rotation = vector3df(0,15,-25); 
			mousePos = cursor->getPosition(); 

			if (nRotationY + mousePos.Y - curMousePos.Y > 80)
			{
				nRotationY = 80;
			}
			else if (nRotationY + mousePos.Y - curMousePos.Y < -220)
			{
				nRotationY = -220;
			}
			else
			{
				nRotationX += mousePos.X - curMousePos.X; 
				nRotationY += mousePos.Y - curMousePos.Y; 
			}

			rotation.rotateYZBy((nRotationY)*0.25,vector3df(0,0,0)); 
			rotation.rotateXZBy((nRotationX)*-0.25,vector3df(0,0,0));//The value (mousePos.X - curMousePos.X) determine camera rotation. 
			curMousePos = mousePos;
			
			//reset rotation to easy use
			if (rotation.X > 180) rotation.X-=360;
			if (rotation.Z > 180) rotation.Y-=360;
			if (rotation.X < -180) rotation.X+=360;
			if (rotation.Z < -180) rotation.Y+=360;
		}
	} 

	target = targetNode->getPosition(); 
	camPos = rotation; 
	camPos *= Zoom;//Zoom process. 
	camPos += target; 

	//Update camera. 
	camera->setTarget(target); 
	camera->setPosition(camPos); 
} 

void ThirdPersonCamera::onRemove() 
{ 
      camera->remove();//remove object 
} 

void ThirdPersonCamera::onMouse(const irr::SEvent & event) 
{	
      if(event.MouseInput.Event==EMIE_MOUSE_WHEEL)//On Wheel Event. 
      { 
		 Zoom  += event.MouseInput.Wheel;//change zoom speed by  wheel value 
          
         //Dont zoom in or out in max and min zoom value is reached 
         if (Zoom < 25) 
            Zoom = 25; 
         if (Zoom > 150) 
            Zoom = 150; 
      } 
      else if(event.MouseInput.Event==EMIE_RMOUSE_PRESSED_DOWN) 
      {    
          Mouse[1] = true; 
      }
      else if(event.MouseInput.Event==EMIE_RMOUSE_LEFT_UP) 
      { 
          Mouse[1] = false; 
          rotating = false; 
      } 
} 

ICameraSceneNode* ThirdPersonCamera::getPointer() const 
{ 
   return camera; 
} 