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

// NewtonDemos.cpp : Defines the entry point for the application.
//


#include <toolbox_stdafx.h>
#include "NewtonDemos.h"
#include "DemoCamera.h"
#include "DemoEntityManager.h"

#define DEFAULT_SCENE	0						// friction test
//#define DEFAULT_SCENE	1						// closest distance
//#define DEFAULT_SCENE	2						// Box stacks
//#define DEFAULT_SCENE	3						// simple level mesh collision
//#define DEFAULT_SCENE	4						// optimized level mesh collision
//#define DEFAULT_SCENE	5						// Scene Collision



int newtonDemos::m_totalMemoryUsed = 0;

void *operator new (size_t size) 
{ 
	void* const ptr = malloc (size);

	//unsigned xxx = unsigned (ptr);
	//xxx &= 0xffff;
	//_ASSERTE (xxx != 0x6FF0);
	//	dTrace (("%d %x\n", xxxx, ptr))
	return ptr; 
}                                          

void operator delete (void *ptr) 
{ 
	free (ptr); 
}


typedef void (*LaunchSDKDemoCallback) (DemoEntityManager* scene);

class SDKDemos
{
	public:
	const char *m_name;
	const char *m_description;
	LaunchSDKDemoCallback m_launchDemoCallback;
};



//void BasicSphereSpin (DemoEntityManager* scene);
void Friction (DemoEntityManager* const scene);
void ClosestDistance (DemoEntityManager* const scene);
void BasicBoxStacks (DemoEntityManager* const scene);
void SimpleMeshLevelCollision (DemoEntityManager* const scene);
void OptimizedMeshLevelCollision (DemoEntityManager* const scene);
void SceneCollision (DemoEntityManager* const scene);

static SDKDemos demosSelection[] = 
{
	{"coefficients of friction", "demonstrate the effect of various coefficient of friction", Friction},
	{"closest distance", "demonstrate closest distance to a convex shape", ClosestDistance},
	{"Simple Box Stacks", "show simple stack of Boxes", BasicBoxStacks},
	{"Unoptimized Mesh collision", "show simple level mesh", SimpleMeshLevelCollision},
	{"Optimized Mesh collision", "show optimized level mesh", OptimizedMeshLevelCollision},
	{"multi geometry collision", "show static mesh with teh avility of moving internal parts", SceneCollision},

/*
	{"spinning sphere", "show a very simple rotating sphere", BasicSphereSpin},
	{"Precessing tops", "show natural precession", PrecessingTops},
	{"convex cast", "demonstrate convex cast different shapes", ConvexCast},
	{"basic box stacking", "demonstrate box stacking", BasicBoxStacks},

	
	{"coefficients of restitution", "demonstrate the effect of various coefficient of restitution", Restitution},
	{"basic cylinder stacking", "demonstrate cylinder stacking", BasicCylinderStacks},
	{"basic convex hull stacking", "demonstrate convex hull stacking", BasicConvexStacks},
	{"basic unstable stacking", "demonstrate stability stacking unstable objects", UnstableStacks},
	{"UnstableStacks stacking", "demonstrate stability stacking unstable objects", UnstableStruture},
	{"Jenga stacking", "demonstrate Jenga game", Jenga},
	{"Large Jenga stacking", "demonstrate Jenga game", JengaTall},
	{"small pyramid stacking", "demonstrate small pyramid stacking", CreatePyramid},
	{"large pyramid stacking", "demonstrate small pyramid stacking", CreatePyramidTall},
	{"wall stacking", "demonstrate wall stacking", CreateWalls},
	{"small tower stacking", "demonstrate tower stacking", CreateTower},
	{"large tower stacking", "demonstrate tower stacking", CreateTowerTall},
	{"scaled collision shape", "demonstrate scaling shape", ScaledCollision},
	{"compound collision shape", "demonstrate compound collision", CompoundCollision},
	{"convex approximation shape", "demonstrate convex approximation of close complex meshes", ConvexApproximation},
	{"simple Plane collision", "demonstrate simple Plane collision", SimplePlaneCollision},
	{"simple polygon mesh collision", "demonstrate simple polygon mesh collision", SimpleMeshLevelCollision},
	{"optimized polygon mesh collision", "demonstrate optimized polygon mesh collision", OptimizedMeshLevelCollision},
	{"simple height field collision", "demonstrate simple hight field collision", SimpleHeighMapCollision},
	{"user defined polygon static collision", "demonstrate user defined polygon static collision", UserHeighMapColliion},
	{"multi shape static level collision", "demonstrate multi shape static level collision", SceneCollision},
	{"attractive magnets force field", "demonstrate attractive force field", Magnets},
	{"repulsive magnets force field", "demonstrate repulsive magnet force field", Repulsive},
	{"Archimedes buoyancy force field", "demonstrate user define Archimedes as force field", ArchimedesBuoyancy},
	{"legacy joints", "demonstrate the build in joints", LegacyJoints},
	{"custom joints", "demonstrate custom joints", BasicCustomJoints},
	{"Simple robots", "demonstrate custom joints robot", BasicRobots},
	{"motorized robots", "demonstrate motorized custom joints robot", TracktionJoints},
	//	{"raycast car", "demonstrate simple ray cast vehicle joint", RayCastCar},
	{"discrete rag doll", "demonstrate simple rag doll", DescreteRagDoll},
	{"skinned rag doll", "demonstrate simple rag doll", SkinRagDoll},
*/
	//	{"player controller", "demonstrate simple player joint", PlayerController},
};




bool newtonDemosEventFilter::eventFilter(QObject *obj, QEvent *ev)   
{    
	// I want to know what code is the one for want the application is Idle so that I can send one even to update my WGLWidget
	//	if (ev->type() == QEvent::What_is_the_Idle_Evene?) {

//	QEvent::Type type = ev->type();
//	if (type == QEvent::MenubarUpdated) {
		// how to send an Event to trigger in Update?
//		_ASSERTE (0);
//	}

	bool state = QObject::eventFilter(obj, ev);   
	return state;
} 


int main(int argc, char *argv[])
{
	// Enable run-time memory check for debug builds.
#ifdef _MSC_VER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Set the memory allocation function before creation the newton world
	// this is the only function that can be called before the creation of the newton world.
	// it should be called once, and the the call is optional 
	NewtonSetMemorySystem (newtonDemos::PhysicsAlloc, newtonDemos::PhysicsFree);


	QApplication aplication(argc, argv);
	newtonDemosEventFilter filter;

	newtonDemos demos;
	demos.show();
	aplication.installEventFilter(&filter); 

	// load the first scene
	demos.LoadDemo (DEFAULT_SCENE);

	return aplication.exec();
}


newtonDemos::newtonDemos(QWidget *parent, Qt::WFlags flags)
	:QMainWindow (parent, flags)
	,m_threadCount(1)
	,m_autoSleepState(true)
	,m_usesSimdInstructions(false)
	,m_solveIslandOnSingleThread(false)
	,m_debugDisplayState(false)
	,m_showPhysicProfiler(false)
	,m_showThreadProfiler(false)
	,m_showStatistics(true)
	,m_doVisualUpdates(true)
{
	setObjectName(QString::fromUtf8("newtonMain"));
	resize(1024, 768);
	setWindowTitle (QApplication::translate("newtonMain", "newton sdk demos", 0, QApplication::UnicodeUTF8));

	// create all menu and toolbars	
	QMenuBar* const menuBar = new QMenuBar(this);
	menuBar->setObjectName(QString::fromUtf8("menubar"));
	menuBar->setGeometry(QRect(0, 0, 1024, 26));
	setMenuBar(menuBar);

	// add the tool bars
	//m_toolBar = new QToolBar(this);
	//m_toolBar->setObjectName(QString::fromUtf8("editToolBar"));
	//addToolBar(Qt::TopToolBarArea, m_toolBar);

	// adding the file menu
	{
		QMenu* const subMenu = new QMenu(menuBar);
		subMenu->setTitle(QApplication::translate("newtonMain", "File", 0, QApplication::UnicodeUTF8));
		menuBar->addAction(subMenu->menuAction());

		// file sub menus
		{
			// file new, open, save
			QAction* action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Load", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnLoad()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Save", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnSave()));

			// load plug ins
			subMenu->addSeparator();

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Exit", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), qApp, SLOT (quit()));
		}
	}


	// Add edit Menu
	{
		QMenu* const subMenu = new QMenu(menuBar);
		subMenu->setObjectName(QString::fromUtf8("demos"));
		subMenu->setTitle(QApplication::translate("newtonMain", "Demos", 0, QApplication::UnicodeUTF8));
		menuBar->addAction(subMenu->menuAction());
		
		// prefab demos
		{

			//HMENU hMenu = GetMenu(mainWindow);  
			//int count = GetMenuItemCount(hMenu);
			//for (int i = 0; i < count; i ++) {
			//	char name[256];
			//	GetMenuString(hMenu, i, name, sizeof (name), MF_BYPOSITION);
			//	if (!stricmp (name, "demos")) {
			//		hMenu = GetSubMenu(hMenu, i);
			//		break;
			//	}
			//}
			//DeleteMenu(hMenu, 0, MF_BYPOSITION);
			for (int i = 0; i < sizeof (demosSelection) / sizeof demosSelection[0]; i ++) {
				//AppendMenu(hMenu, MF_STRING, IDM_DEMOS + i, demosSelection[i].m_name);

				//char actionName[256];
				//sprintf (actionName, "actionImportDll_%03d", id_importIndex) ;
				//dImportPlugin* const importPlugin = (dImportPlugin*) plugin;
				//const char* const menuName = importPlugin->GetMenuName ();
				//const char* const description = importPlugin->GetFileDescription ();

				QAction* const action = new QAction(this);

				//QVariant::Int value = i;
				action->setData(QVariant (i));
				action->setText(QApplication::translate("Alchemedia", demosSelection[i].m_name, 0, QApplication::UnicodeUTF8));
				subMenu->addAction(action);
				connect (action, SIGNAL (triggered(bool)), this, SLOT (OnRunDemo()));
			}

		}
	}


	// Add edit Menu
	{
		QMenu* const subMenu = new QMenu(menuBar);
		subMenu->setObjectName(QString::fromUtf8("options"));
		subMenu->setTitle(QApplication::translate("newtonMain", "Options", 0, QApplication::UnicodeUTF8));
		menuBar->addAction(subMenu->menuAction());

		// sub menu options
		{
			// file new, open, save
			QAction* action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Autosleep dissabled", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			// file new, open, save
			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Show debug display", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Hide physics profiler", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Show thread profiler", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Hide statistics", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Use simd", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Run Physics in main Thread", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "select number of physics micro threads", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));

			action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "Use parallel solve", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));
		}
	}

	// Add edit Menu
	{
		QMenu* const subMenu = new QMenu(menuBar);
		subMenu->setObjectName(QString::fromUtf8("help"));
		subMenu->setTitle(QApplication::translate("newtonMain", "Help", 0, QApplication::UnicodeUTF8));
		menuBar->addAction(subMenu->menuAction());

		// help options
		{
			QAction* action = new QAction(this);
			action->setText(QApplication::translate("newtonMain", "About", 0, QApplication::UnicodeUTF8));
			subMenu->addAction(action);
			connect (action, SIGNAL (triggered(bool)), this, SLOT (OnNotUsed()));
		}
	}

	// create the render window
	m_canvas = new DemoEntityManager (this);
	setCentralWidget(m_canvas);

	connect(QAbstractEventDispatcher::instance(), SIGNAL(aboutToBlock()), this, SLOT(OnIdle())); 
	//connect(QAbstractEventDispatcher::instance(), SIGNAL(awake ()), this, SLOT(OnIdle())); 

//	m_animationTimer.setSingleShot(false);
//	connect(&m_animationTimer, SIGNAL(timeout()), this, SLOT(OnIdle()));
//	m_animationTimer.start(0);

}

newtonDemos::~newtonDemos()
{
}


// memory allocation for Newton
void* newtonDemos::PhysicsAlloc (int sizeInBytes)
{
	m_totalMemoryUsed += sizeInBytes;
	return malloc (sizeInBytes);
}

// memory de-allocation for Newton
void newtonDemos::PhysicsFree (void *ptr, int sizeInBytes)
{
	m_totalMemoryUsed -= sizeInBytes;
	free (ptr);
}

void newtonDemos::OnNotUsed()
{
	_ASSERTE (0);
}


void newtonDemos::OnLoad()
{
	m_doVisualUpdates = false;

	m_doVisualUpdates = true;
}

void newtonDemos::OnSave()
{
	m_doVisualUpdates = false;

	m_doVisualUpdates = true;
}

void newtonDemos::LoadDemo (int index)
{
	m_canvas->Cleanup();
	demosSelection[index].m_launchDemoCallback (m_canvas);
	m_canvas->SetAutoSleepState (m_autoSleepState);
}

void newtonDemos::OnRunDemo()
{
	m_doVisualUpdates = false;
	QAction* const action = (QAction*)sender();
	int index = (INT) action->data().toInt();
	LoadDemo (index);
	m_doVisualUpdates = true;
}



void newtonDemos::OnIdle()
{
	if (m_doVisualUpdates) {
		m_canvas->update();
	}
}