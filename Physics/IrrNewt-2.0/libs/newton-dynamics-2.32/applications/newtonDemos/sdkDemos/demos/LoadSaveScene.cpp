/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include <toolbox_stdafx.h>
#include "../DemoEntityManager.h"
#include "../DemoCamera.h"
#include "../PhysicsUtils.h"



void LoadScene (DemoEntityManager* const scene, char* const fileName)
{

	// suspend simulation before making changes to the physics world
	scene->StopsExecution ();

	// load the scene from and alchemedia file format
	scene->LoadScene (fileName);

	// place camera into position
	dVector origin (-40.0f, 10.0f, 0.0f, 0.0f);
	scene->GetCamera()->m_upVector = dVector (0.0f, 1.0f, 0.0f);
	scene->GetCamera()->m_origin = origin;
	scene->GetCamera()->m_pointOfInterest = origin + dVector (1.0f, 0.0f, 0.0f);

	// resume the simulation
	scene->ContinueExecution();
}
