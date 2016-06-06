// The code is distributed under the terms of the GNU General Public License
// Part of the OSRPG project (http://osrpgcreation.sf.net)
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "camera.h"

Camera::Camera () {
	//pos = core::vector3df (12.7, -31.4, 95.8);
	//target = core::vector3df (12.8, -31.5, 94.8);
}

void Camera::PrintCurrentPos () {
	SavePosition ();
	PrintVector (pos);
	PrintVector (target);
}

void Camera::Create (ISceneManager *smgr, ISceneNode *parent) {
	// Make camera be controlled with arrow keys as well as with
	// W, S, A, D
	SKeyMap key_map[8];

	key_map[0].Action = EKA_MOVE_FORWARD;
	key_map[0].KeyCode = KEY_UP;
	key_map[1].Action = EKA_MOVE_FORWARD;
	key_map[1].KeyCode = KEY_KEY_W;

	key_map[2].Action = EKA_MOVE_BACKWARD;
	key_map[2].KeyCode = KEY_DOWN;
	key_map[3].Action = EKA_MOVE_BACKWARD;
	key_map[3].KeyCode = KEY_KEY_S;

	key_map[4].Action = EKA_STRAFE_LEFT;
	key_map[4].KeyCode = KEY_LEFT;
	key_map[5].Action = EKA_STRAFE_LEFT;
	key_map[5].KeyCode = KEY_KEY_A;

	key_map[6].Action = EKA_STRAFE_RIGHT;
	key_map[6].KeyCode = KEY_RIGHT;
	key_map[7].Action = EKA_STRAFE_RIGHT;
	key_map[7].KeyCode = KEY_KEY_D;
	
	cam = smgr->addCameraSceneNodeFPS (parent, 100, 100, -1, 0, 0);
	//cam = smgr->addCameraSceneNodeFPS (parent, 100, 100, -1, key_map, 8);
	RestorePos ();
}

void Camera::SetTarget (core::vector3df target) {
	cam->setTarget (target);
}

core::vector3df Camera::GetForceVec () {
	SavePosition ();
	return (target - pos);
}

void Camera::SavePosition () {
	pos = cam->getPosition ();
	target = cam->getTarget ();
}

void Camera::SetPosition (core::vector3df pos) {
	cam->setPosition (pos);
}

core::vector3df Camera::GetPosition () {
	SavePosition ();
	return pos;
}

core::vector3df Camera::GetTarget () {
	SavePosition ();
	return target;
}

ICameraSceneNode* Camera::GetCamera () {
	return cam;
}

void Camera::RestorePos () {
	cam->setPosition (pos);
	cam->setTarget (target);
}

float Camera::GetXZProportion () {
	SavePosition ();
	core::vector3df dir_vec (target - pos);
	return ((float)dir_vec.X / (float)dir_vec.Z);
}

