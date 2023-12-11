#include "StdAfx.h"

void CGame::Init()
{
	// Init the Irrlicht engine
	device = createDevice(EDT_OPENGL, dimension2d<s32>(1024, 768), 16, false, true, this);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	// Init newton
	nWorld = NewtonCreate(NULL, NULL);

	// Set up default material properties for newton
	int i = NewtonMaterialGetDefaultGroupID(nWorld);
	NewtonMaterialSetDefaultFriction   (nWorld, i, i, 0.8f, 0.4f);
	NewtonMaterialSetDefaultElasticity (nWorld, i, i, 0.3f);
	NewtonMaterialSetDefaultSoftness   (nWorld, i, i, 0.05f);
	NewtonMaterialSetCollisionCallback (nWorld, i, i, NULL, NULL, NULL, NULL);

	// add the camera
	cam = smgr->addCameraSceneNodeFPS();
	cam->setPosition(vector3df(0, 50, 0));

	lasttick = 0;

	currentCube = 0;
}

void CGame::Prepare()
{
	// add skybox
	smgr->addSkyBoxSceneNode(driver->getTexture("data/irrlicht2_up.bmp"),driver->getTexture("data/irrlicht2_dn.bmp"),driver->getTexture("data/irrlicht2_lf.bmp"),driver->getTexture("data/irrlicht2_rt.bmp"),driver->getTexture("data/irrlicht2_ft.bmp"),driver->getTexture("data/irrlicht2_bk.bmp"));

	// load map
	g_map = smgr->getMesh("data/physicstest.bsp");
	g_mapnode = smgr->addOctTreeSceneNode(g_map->getMesh(0));

	//////////////////////////////////////////////////////////////////////////
	//
	// Create the newton collision tree from the map mesh
	//
	// Remember to use (video::S3DVertex) if you are loading a mesh without lightmaps
	// for your level. (Like a .x or .3ds level)
	//
	//////////////////////////////////////////////////////////////////////////
	g_newtonmap = NewtonCreateTreeCollision(nWorld, NULL);
	NewtonTreeCollisionBeginBuild(g_newtonmap);
	int cMeshBuffer, j;
	int v1i, v2i, v3i;
	IMeshBuffer *mb;

	float vArray[9]; // vertex array (3*3 floats)

	int tmpCount = 0;

	for (cMeshBuffer=0; cMeshBuffer<g_map->getMesh(0)->getMeshBufferCount(); cMeshBuffer++)
	{	
		mb = g_map->getMesh(0)->getMeshBuffer(cMeshBuffer);

		video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*)mb->getVertices();

		u16* mb_indices  = mb->getIndices();

		// add each triangle from the mesh
		for (j=0; j<mb->getIndexCount(); j+=3)
		{
			v1i = mb_indices[j];
			v2i = mb_indices[j+1];
			v3i = mb_indices[j+2];
	
			vArray[0] = mb_vertices[v1i].Pos.X;
			vArray[1] = mb_vertices[v1i].Pos.Y;
			vArray[2] = mb_vertices[v1i].Pos.Z;
			vArray[3] = mb_vertices[v2i].Pos.X;
			vArray[4] = mb_vertices[v2i].Pos.Y;
			vArray[5] = mb_vertices[v2i].Pos.Z;
			vArray[6] = mb_vertices[v3i].Pos.X;
			vArray[7] = mb_vertices[v3i].Pos.Y;
			vArray[8] = mb_vertices[v3i].Pos.Z;

			NewtonTreeCollisionAddFace(g_newtonmap, 3, (float*)vArray, 12, 1);
		}

	}
	NewtonTreeCollisionEndBuild(g_newtonmap, 0);
	g_newtonmapbody = NewtonCreateBody(nWorld, g_newtonmap);

	
	// set the newton world size based on the bsp size
	float boxP0[3]; 
	float boxP1[3]; 
	float matrix[4][4]; 
	NewtonBodyGetMatrix (g_newtonmapbody, &matrix[0][0]); 
	NewtonCollisionCalculateAABB (g_newtonmap, &matrix[0][0],  &boxP0[0], &boxP1[0]); 
	// you can pad the box here if you wish
	//boxP0.y -= somevalue; 
	//boxP1.y += somevaluef; 
	NewtonSetWorldSize (nWorld, (float*)boxP0, (float*)boxP1);
	
	// hide cursor
	device->getCursorControl()->setVisible(false);

	// Make some cubes
	int xo = 500, yo = 500;
	int x, y;
	for (x=0; x<10; x++)
	{
		for (y=0; y<10; y++)
		{
			MakeCube(vector3df((float)(x*100)-xo, 500.0f, (float)(y*100)-yo));
		}
	}

}

void CGame::MainLoop()
{
	Render();
	
	// Update newton 100 times / second
	if (device->getTimer()->getTime() > lasttick + 10) {	
		lasttick = device->getTimer()->getTime();
		NewtonUpdate(nWorld, 0.01f);
	}
}

void CGame::Render()
{
	driver->beginScene(true, true, video::SColor(0,220,220,255));

	// render the scene
	smgr->drawAll();

	driver->endScene();

	// Draw fps counter
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		wchar_t tmp[1024];
		swprintf(tmp, 1024, L"Newton Example [fps:%d] [triangles:%d]", 
		fps, driver->getPrimitiveCountDrawn());
		device->setWindowCaption(tmp);
		lastFPS = fps;
	}
}

void CGame::Finish()
{
	// release the collision tree
	NewtonReleaseCollision(nWorld, g_newtonmap);

	// release the box primitives
	for (int i=0; i<currentCube; i++)
		NewtonReleaseCollision(nWorld, cubes[i]->collision);

	// finish newton & irrlicht
	NewtonDestroy(nWorld);
	device->drop();
}

//////////////////////////////////////////////////////////////////////////
// Irrlicht event receiver
//////////////////////////////////////////////////////////////////////////

bool CGame::OnEvent(SEvent event)
{
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
	{
		// make a cube where the camera is and set its velocity to follow the target
		NewtonCube *tmp = MakeCube(cam->getPosition());
		if (!tmp) return false;
		vector3df camvec = (cam->getTarget() - cam->getPosition()).normalize() * 500;
		float newpos[3] = { camvec.X, camvec.Y, camvec.Z };
		NewtonBodySetVelocity(tmp->body, (float*)newpos);
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// Adds a cube into the world.
//////////////////////////////////////////////////////////////////////////

NewtonCube *CGame::MakeCube(vector3df loc)
{
	NewtonCube *tmp = new NewtonCube;

	tmp->mesh = smgr->getMesh("data/smallcube.3ds");
	tmp->node = smgr->addAnimatedMeshSceneNode(tmp->mesh);
	tmp->node->setMaterialTexture(0, driver->getTexture("data/crate.jpg"));

	// Create a box primitive. 38 is just an estimated value of the size of the model,
	tmp->collision = NewtonCreateBox(nWorld, 38, 38, 38, NULL);
	tmp->body = NewtonCreateBody(nWorld, tmp->collision);

	// Set user data pointer to the scene node
	NewtonBodySetUserData(tmp->body, tmp->node);

	// Set body mass & inertia matrix
	NewtonBodySetMassMatrix (tmp->body, 10.0f, 150.0f, 150.0f, 150.0f);

	// Set the freeze threshhold to 1 unit (default is 0.01 but irrlight uses a large unit scale)
	NewtonBodySetFreezeTreshold(tmp->body, 1.0, 1.0, 1.0, 1.0); 

	// Set callback functions for the body
	NewtonBodySetTransformCallback(tmp->body, SetMeshTransformEvent);
	NewtonBodySetForceAndTorqueCallback(tmp->body, ApplyForceAndTorqueEvent);

	// Set the position of the body
	matrix4 mat;
	mat.setTranslation(loc);
	NewtonBodySetMatrix(tmp->body, &mat.M[0]);
	
	if (currentCube == 512)
	{
		printf("* Too many cubes!");
		return NULL;
	}

	cubes[currentCube] = tmp;
	currentCube ++;

	return tmp;
}

//////////////////////////////////////////////////////////////////////////
//
// The last 2 functions are callbacks from newton
//
//////////////////////////////////////////////////////////////////////////

void _cdecl CGame::SetMeshTransformEvent(const NewtonBody* body, const float* matrix)
{
	// copy the matrix into an irrlicht matrix4
	matrix4 mat;
	memcpy(mat.M, matrix, sizeof(float)*16);

	// Retreive the user data attached to the newton body
	ISceneNode *tmp = (ISceneNode *)NewtonBodyGetUserData(body);
	if (tmp)
	{
		// Position the node
		tmp->setPosition(mat.getTranslation());		// set position
		tmp->setRotation(mat.getRotationDegrees());	// and rotation
	}
}

void _cdecl CGame::ApplyForceAndTorqueEvent (const NewtonBody* body) 
{ 
   float mass; 
   float Ixx; 
   float Iyy; 
   float Izz; 
   float force[3]; 
   float torque[3]; 

   NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz); 

   force[0] = 0.0f; 
   force[1] = NEWTON_GRAVITY * mass; 
   force[2] = 0.0f; 

   torque[0] = 0.0f; 
   torque[1] = 0.0f; 
   torque[2] = 0.0f; 

   NewtonBodyAddForce (body, force); 
   NewtonBodyAddTorque (body, torque); 
}
