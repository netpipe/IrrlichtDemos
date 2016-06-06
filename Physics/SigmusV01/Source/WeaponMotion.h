/*-----------------------------------------------------------------------------* 
| weaponMotion.h                                                               | 
|                                                                              | 
| last edit date: (5.5. 2009)                                                  | 
|                                                                              | 
| author:  Martin Cmar                                                         | 
|                                                                              | 
*-----------------------------------------------------------------------------*/ 

#ifndef WEAPON_MOTION_H 
#define WEAPON_MOTION_H 

#include "main.h"

class weaponMotion 
{ 
	ICameraSceneNode * camera; 
	vector3df lastPos; 
public: 
	weaponMotion(ICameraSceneNode * cam) 
	{ 
		camera = cam; 
		lastPos = camera->getPosition(); 
	} 
	void weaponMotion::update(int time,IAnimatedMeshSceneNode * node, vector3df pos) 
	{ 
		vector3df diff = camera->getPosition() - lastPos; 
		float scale = sqrt( diff.X * diff.X   + diff.Z * diff.Z); 
		if(scale > 8) 
		scale = 8; 
		float fracsin = sin(2 * time * 0.005); 
		float fracsin2 = sin( time * 0.005); 
		pos.Y += scale * fracsin * 0.01; 
		pos.X += scale * fracsin2 * 0.01; 
		node->setPosition(pos);    
		lastPos = camera->getPosition(); 
	} 
};
#endif 