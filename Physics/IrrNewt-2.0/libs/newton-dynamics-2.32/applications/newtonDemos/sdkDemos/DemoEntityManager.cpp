#include "toolbox_stdafx.h"
#include "NewtonDemos.h"
#include "DemoEntityManager.h"
#include "SkyBox.h"
#include "DemoMesh.h"
#include "DemoEntity.h"
#include "DemoCamera.h"
#include "PhysicsUtils.h"
#include "DebugDisplay.h"
#include "dRuntimeProfiler.h"
#include "OpenGlUtil.h"




#define MAX_PHYSICS_LOOPS 1
#define MAX_PHYSICS_FPS	  120.0f





DemoEntityManager::DemoEntityManager(QWidget* const parent)
	:QGLWidget (QGLFormat(QGL::SampleBuffers), parent)
	,dList <DemoEntity*>() 
	,dSimulationThread()
	,m_camera(NULL)
	,m_world(NULL)
	,m_physicsUpdate(true) 
	,m_reEntrantUpdate (false)
	,m_microsecunds (0)
	,m_timer()
	,m_profiler (620 * 0 / 8 + 45, 40)
	,m_font()
{
	// Create the main Camera
	m_camera = new DemoCamera();

	// initialized the physics world for the new scene
	Cleanup ();

	// Start rendering after the system is set properly
	ContinueExecution();

	doubleBuffer(); 
	setAutoFillBackground(false);
}

DemoEntityManager::~DemoEntityManager(void)
{
	delete m_camera;

	// suspend simulation before making changes to the physics world
	StopsExecution ();

	Cleanup ();
	if (m_world) {
		NewtonDestroy (m_world);
		m_world = NULL;
	}
	_ASSERTE (NewtonGetMemoryUsed () == 0);

	// Start rendering after the system is set properly
	ContinueExecution();

}

//	GLContext& GetGL() {return m_glContext;}
NewtonWorld* DemoEntityManager::GetNewton() const 
{
	return m_world;
}

DemoCamera* DemoEntityManager::GetCamera() const 
{
	return m_camera;
}

void DemoEntityManager::initializeGL()
{
	QGLWidget::initializeGL();


	m_camera->SetProjectionMode(width(), height());
}


void DemoEntityManager::resizeGL(int w, int h)
{
	QGLWidget::resizeGL(w, h);
	m_camera->SetProjectionMode(width(), height());
}

void DemoEntityManager::Print (QPainter& painter, Qt::GlobalColor color, int x, int y, const char *fmt, ... ) const
{
	va_list argptr;
	char string[2048];

	va_start (argptr, fmt);
	vsprintf (string, fmt, argptr);
	va_end( argptr );

	painter.setPen(Qt::black);
	painter.drawText(x, y, tr(string));
}


void DemoEntityManager::Cleanup ()
{
	// suspend simulation before making changes to the physics world
	StopsExecution ();

	// destroy all remaining visual objects
	//	demo.CleanUp();
	while (GetFirst()) {
		DemoEntity* const entity = GetFirst()->GetInfo();
		entity->Release();
		Remove(GetFirst());
	}

	// destroy the Newton world
	if (m_world) {
		NewtonDestroy (m_world);
		m_world = NULL;
	}

	//	memset (&demo, 0, sizeof (demo));
	// check that there are no memory leak on exit
	_ASSERTE (NewtonGetMemoryUsed () == 0);


	newtonDemos* const mainWindow = (newtonDemos*) parent();

	// create the newton world
	m_world = NewtonCreate();

	// set the default parameters for the newton world
	// set the simplified solver mode (faster but less accurate)
	NewtonSetSolverModel (m_world, 1);

	// Set the number of internal micro threads
	NewtonSetThreadsCount (m_world, mainWindow->m_threadCount);

	// use the standard x87 floating point model  
	if (mainWindow->m_usesSimdInstructions) {
		NewtonSetPlatformArchitecture (m_world, 3);  //best hardware (SSE at this time)
	} else {
		NewtonSetPlatformArchitecture (m_world, 0);  //x87 mode
	}

	// set the parallel solve on single island or or off
	NewtonSetMultiThreadSolverOnSingleIsland (m_world, mainWindow->m_solveIslandOnSingleThread ? 1 : 0);

	// set a fix world size
	dVector minSize (-500.0f, -500.0f, -500.0f);
	dVector maxSize ( 500.0f,  500.0f,  500.0f);
	NewtonSetWorldSize (m_world, &minSize[0], &maxSize[0]); 

	// set the performance track function
	NewtonSetPerformanceClock (m_world, dRuntimeProfiler::GetTimeInMicrosecunds);

	// Set performance counters off
	memset (m_showProfiler, 1, sizeof (m_showProfiler));

	// clean up all caches the engine have saved
	NewtonInvalidateCache (m_world);

	// Set the Newton world user data
	NewtonWorldSetUserData(m_world, this);

	// Start rendering after the system is set properly
	ContinueExecution();
}



void DemoEntityManager::LoadScene (const char* const fileName)
{
	dScene database (GetNewton());
	
	// stops simulation
	StopsExecution (); 

	database.Deserialize(fileName);

	// Load the Visual Scene
	EntityDictionary entDictionary;
	LoadVisualScene(&database, entDictionary);

	//Load the physics world
	dList<NewtonBody*> bodyList;
	database.SceneToNewtonWorld(m_world, bodyList);

	// bind every rigidBody loaded to the scene entity
	for (dList<NewtonBody*>::dListNode* bodyNode = bodyList.GetFirst(); bodyNode; bodyNode = bodyNode->GetNext()) {
		// find the user data and set to the visual entity in the scene
		NewtonBody* const body = bodyNode->GetInfo();
		dScene::dTreeNode* const sceneNode = (dScene::dTreeNode*)NewtonBodyGetUserData(body);
		DemoEntity* const entity = entDictionary.Find(sceneNode)->GetInfo();
		NewtonBodySetUserData(body, entity);

		// see if this body have some special setups
		dScene::dTreeNode* const node = database.FindChildByType(sceneNode, dRigidbodyNodeInfo::GetRttiType());
		_ASSERTE (node);
		dRigidbodyNodeInfo* const bodyData = (dRigidbodyNodeInfo*) database.GetInfoFromNode(node);
		dVariable* bodyType = bodyData->FindVariable("rigidBodyType");

		// set the default call backs
		if (!bodyType || !strcmp (bodyType->GetString(), "default gravity")) {
			NewtonBodySetTransformCallback(body, DemoEntity::SetTransformCallback);
			NewtonBodySetForceAndTorqueCallback(body, PhysicsApplyGravityForce);
			NewtonBodySetDestructorCallback (body, PhysicsBodyDestructor);
		}
	}


	// clean up all caches the engine have saved
	NewtonInvalidateCache (m_world);

	// resume the simulation
	ContinueExecution();
}

void DemoEntityManager::SaveScene (const char* const name)
{
	_ASSERTE (0);
/*
	// stops simulation
	StopsExecution (); 

	dScene database (GetNewton());
	database.NewtonWorldToScene (GetNewton());

	char fileName[2048];
	GetWorkingFileName (name, fileName);
	database.Serialize(fileName);

	// resume the simulation
	ContinueExecution();
*/
}


void DemoEntityManager::LoadVisualScene(dScene* const scene, EntityDictionary& dictionary)
{
	// load all meshes into a Mesh cache for reuse
	dTree<DemoMesh*, dScene::dTreeNode*> meshDictionary;
	for (dScene::dTreeNode* node = scene->GetFirstNode (); node; node = scene->GetNextNode (node)) {
		dNodeInfo* info = scene->GetInfoFromNode(node);
		if (info->GetTypeId() == dMeshNodeInfo::GetRttiType()) {
			DemoMesh* mesh = new DemoMesh(scene, node);
			meshDictionary.Insert(mesh, node);
		}
	}


	// create an entity for every root node in the mesh
	dScene::dTreeNode* const root = scene->GetRootNode();
	for (void* child = scene->GetFirstChild(root); child; child = scene->GetNextChild (root, child)) {
		dScene::dTreeNode* node = scene->GetNodeFromLink(child);
		dNodeInfo* info = scene->GetInfoFromNode(node);
		if (info->GetTypeId() == dSceneNodeInfo::GetRttiType()) {
			// Load this model and add it to the Scene
			DemoEntity* const entityRoot = new DemoEntity (*this, scene, node, meshDictionary, dictionary);
			Append(entityRoot);
		}
	}

	// release all meshes before exiting
	dTree<DemoMesh*, dScene::dTreeNode*>::Iterator iter (meshDictionary);
	for (iter.Begin(); iter; iter++) {
		DemoMesh* const mesh = iter.GetNode()->GetInfo();
		mesh->Release();
	}
}


void DemoEntityManager::SetAutoSleepState (bool state)
{
	int autosleep = state ? 1 : 0;
	for (NewtonBody* body = NewtonWorldGetFirstBody(m_world); body; body = NewtonWorldGetNextBody(m_world, body)) {
		NewtonBodySetAutoSleep(body, autosleep);
	}
}


void DemoEntityManager::InterpolateMatrices ()
{
	unsigned64 timeStep = m_timer.GetTimeInMicrosenconds () - m_microsecunds;		
	dFloat step = (dFloat (timeStep) * MAX_PHYSICS_FPS) / 1.0e6f;
	_ASSERTE (step >= 0.0f);
	if (step > 1.0f) {
		step = 1.0f;
	}

	for (NewtonBody* body = NewtonWorldGetFirstBody(m_world); body; body = NewtonWorldGetNextBody(m_world, body)) {
		DemoEntity* entity = (DemoEntity*)NewtonBodyGetUserData(body);
		entity->InterpolateMatrix (*this, step);
	}
}



void DemoEntityManager::UpdatePhysics()
{
	if (m_world) {
		dFloat timestepInSecunds = 1.0f / MAX_PHYSICS_FPS;
		unsigned64 timestepMicrosecunds = unsigned64 (timestepInSecunds * 1000000.0f);

		unsigned64 currentTime = m_timer.GetTimeInMicrosenconds ();
		unsigned64 nextTime = currentTime - m_microsecunds;
		int loops = 0;
		while ((nextTime >= timestepMicrosecunds) && (loops < MAX_PHYSICS_LOOPS)) {
			loops ++;

			// run the newton update function
			if (!m_reEntrantUpdate) {
				m_reEntrantUpdate = true;
				if (m_physicsUpdate && m_world) {
					NewtonUpdate (m_world, timestepInSecunds);
				}
				m_reEntrantUpdate = false;
			}

			nextTime -= timestepMicrosecunds;
			m_microsecunds += timestepMicrosecunds;
		}

		if (loops) {
			m_physicsTime = dFloat (m_timer.GetTimeInMicrosenconds () - currentTime) / 1000000.0f;

			if (m_physicsTime >= MAX_PHYSICS_LOOPS * (1.0f / MAX_PHYSICS_FPS)) {
				m_microsecunds = currentTime;
			}
		}
	}
}



void DemoEntityManager::paintEvent(QPaintEvent* ev)
{
	newtonDemos* const mainWindow = (newtonDemos*) parent();

	if (!m_asycronousUpdate) {
		UpdatePhysics();
	}

	makeCurrent();
	InterpolateMatrices ();

	// Our shading model--Goraud (smooth). 
	glShadeModel (GL_SMOOTH);

	// Culling. 
	glCullFace (GL_BACK);
	glFrontFace (GL_CCW);
	glEnable (GL_CULL_FACE);

	//	glEnable(GL_DITHER);

	// z buffer test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	glClearColor (0.5f, 0.5f, 0.5f, 0.0f );
	//glClear( GL_COLOR_BUFFER_BIT );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	m_camera->SetProjectionMode(width(), height());

	// set default lightning
	glDisable(GL_BLEND);
	glEnable (GL_LIGHTING);
	dFloat cubeColor[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, cubeColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cubeColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// set just one directional light
	GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 0.0 };
	GLfloat lightAmbientColor[] = { 0.7f, 0.7f, 0.7f, 0.0 };
	GLfloat lightPosition[] = { 500.0f, 200.0f, 500.0f, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glEnable(GL_LIGHT0);

	// update Camera
	m_camera->Update();

	// render all entities
	dFloat timestep = m_timer.GetElapsedSeconds();	
	for (dListNode* node = GetFirst(); node; node = node->GetNext()) {
		DemoEntity* entity = node->GetInfo();
		glPushMatrix();	
		entity->Render(timestep);
		glPopMatrix();
	}


	if (mainWindow->m_debugDisplayState) {
		DebugRenderDebugInfo ();
		DebugRenderWorldCollision (GetNewton());
	}

	// do all 2d drawing
	glShadeModel(GL_FLAT);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	QPainter painter(this);
	//QFontMetrics metrics = QFontMetrics(m_font);
	//int border = qMax(4, metrics.leading());
	//painter.setRenderHint(QPainter::Antialiasing);
	//renderText (4, 4, "This is a test", m_font);
	if (mainWindow->m_showPhysicProfiler) {
		int profileFlags = 0;
		for (int i = 0; i < int (sizeof (m_showProfiler) / sizeof (m_showProfiler[0])); i ++) {
			profileFlags |=  m_showProfiler[i] ? (1 << i) : 0;
		}

		//profileFlags = 1;
		if (profileFlags) {
			//m_profiler.Render (m_world, profileFlags, m_glContext);
		}
	}

	if (mainWindow->m_showThreadProfiler) {
		//m_profiler.ReanderThreadPerformace (m_world, m_glContext);
	}

	if (mainWindow->m_showStatistics) {
		dFloat fps = 1.0f / timestep;
		//m_glContext.Print (color, 4,  4, "FPS %6.2f", fps);
		Print (painter, Qt::black, 14, 14, "FPS %6.2f", fps);

		Print (painter, Qt::black, 14, 30, "Physics time ms %6.3f", m_physicsTime * 1000.0f);
		Print (painter, Qt::black, 14, 46, "Body count %d",  NewtonWorldGetBodyCount(m_world));
		if (m_asycronousUpdate) {
			Print (painter, Qt::black, 14, 62, "physics running asynchronous");
		}
	}
	painter.end();

	// draw everything and swap the display buffer
	//glFlush();
	//glFinish();
	//swapBuffers();
}

