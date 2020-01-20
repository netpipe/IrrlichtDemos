#include "StdAfx.h"

#if !defined(GAME_H)
#define GAME_H

struct NewtonCube {
	IAnimatedMesh *mesh;
	ISceneNode *node;
	NewtonBody *body;
	NewtonCollision *collision;
};

class CGame : public IEventReceiver {
protected:
	// misc
	int lastFPS;
	u32 lasttick;


public:
	// Irrlicht  vars
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	IrrlichtDevice *device;

	// Newton vars
	NewtonWorld *nWorld;

	// our level
	scene::IAnimatedMesh* g_map;
	scene::ISceneNode* g_mapnode;
	scene::ITriangleSelector *g_selector;
	NewtonCollision* g_newtonmap;
	NewtonBody* g_newtonmapbody;

	// Scene vars
	ICameraSceneNode *cam;


	//////////////////////////////////////////////////////////////////////////
	// Irrlicht Callbacks
	virtual bool OnEvent(SEvent event);

	//////////////////////////////////////////////////////////////////////////
	// Newton Callbacks
	static void _cdecl SetMeshTransformEvent(const NewtonBody* , const float*);
	static void _cdecl ApplyForceAndTorqueEvent (const NewtonBody* body);

	//////////////////////////////////////////////////////////////////////////
	// Game Functions
	void Init();
	void Prepare();

	void MainLoop();

	void Render();

	void Finish();

	//////////////////////////////////////////////////////////////////////////
	// Game functions
	NewtonCube *cubes[512];
	int currentCube;
	NewtonCube *MakeCube(vector3df loc);

};

#endif