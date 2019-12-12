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
#include "MainFrame.h"
#include "RenderCanvas.h"
#include "dGlueToCollada.h"
#include "TargaToOpenGl.h"
#include "DebugDisplay.h"
#include "dMesh.h"



struct NEWTON_SDK_DEMOS
{
	const char *m_name;
	const char *m_description;
	LaunchSDKDemoCallback m_launchDemoCallback;
};

static NEWTON_SDK_DEMOS demosSelection[] = 
{
	{_T("spinning sphere"), _T("show a very simple rotating sphere"), BasicSphereSpin},
	{_T("Precessing tops"), _T("show natural precession"), PrecessingTops},
	
	{_T("convex cast"), _T("demonstrate convex castion diffrent shapes"), ConvexCast},
	{_T("closest distance"), _T("demonstrate closest distance to a conve shape"), ClosestDistance},
	{_T("coefficients of friction"), _T("demonstrate the effect of various coefficient of friction"), Friction},
	{_T("coefficients of restitution"), _T("demonstrate the effect of various coefficient of restitution"), Restitution},

	{_T("basic box stacking"), _T("demonstrate box stacking"), BasicBoxStacks},
	{_T("basic cylinder stacking"), _T("demonstrate cylinder stacking"), BasicCylinderStacks},
	{_T("basic convexhull stacking"), _T("demonstrate convexhull stacking"), BasicConvexStacks},

	{_T("basic unstable stacking"), _T("demonstrate stability stacking unstable objects"), UnstableStacks},
	{_T("UnstableStacks stacking"), _T("demonstrate stability stacking unstable objects"), UnstableStruture},

	{_T("Jenga stacking"), _T("demostrate Jenga game"), Jenga},
	{_T("Large Jenga stacking"), _T("demostrate Jenga game"), JengaTall},
	
	{_T("small pyramid stacking"), _T("demonstrate small pyramid stacking"), CreatePyramid},
	{_T("large pyramid stacking"), _T("demonstrate small pyramid stacking"), CreatePyramidTall},
	{_T("wall stacking"), _T("demonstrate wall stacking"), CreateWalls},

	{_T("small tower stacking"), _T("demonstrate tower stacking"), CreateTower},
	{_T("large tower stacking"), _T("demonstrate tower stacking"), CreateTowerTall},
	
	{_T("scaled collision shape"), _T("demonstrate scaling shape"), ScaledCollision},
	{_T("compound collision shape"), _T("demonstrate compound collision"), CompoundCollision},

	{_T("simple Plane collision"), _T("demonstrate simple Plane collision"), SimplePlaneCollision},
	{_T("simple polygon mesh collision"), _T("demonstrate simple polygon mesh collision"), SimpleMeshLevelCollision},
	{_T("optimized polygon mesh collision"), _T("demonstrate optimized polygon mesh collision"), OptimizedMeshLevelCollision},
	{_T("simple height field collision"), _T("demonstrate simple hightfield collision"), SimpleHeighMapCollision},
	{_T("user defined polygon static collision"), _T("demonstrate user defined polygon static collision"), UserHeighMapColliion},
	{_T("multi shape static level collision"), _T("demonstrate multi shape static level collision"), SceneCollision},

	{_T("actractive magnets force field"), _T("demonstrate atractive force field"), Magnets},
	{_T("repulsive magnets force field"), _T("demonstrate repulsive magnet force field"), Repulsive},
	{_T("archimedes buoyancy force field"), _T("demonstrate user define Archimedes as force field"), ArchimedesBuoyancy},

	{_T("legacy joints"), _T("demonstrate the build in joints"), LegacyJoints},
	{_T("custom joints"), _T("demonstrate custom joints"), BasicCustomJoints},
	{_T("Simple robots"), _T("demonstrate custom joints robot"), BasicRobots},

	{_T("motorized robots"), _T("demonstrate motorized custom joints robot"), TracktionJoints},
	
	{_T("raycast car"), _T("demonstrate simple ray cast behicle joint"), RayCastCar},

	{_T("descrete ragdoll"), _T("demonstrate simple rag doll"), DescreteRagDoll},
	{_T("skinned ragdoll"), _T("demonstrate simple rag doll"), SkinRagDoll},
	
//	{_T("player controller"), _T("demonstrate simple player joint"), PlayerController},
	

};


enum MY_WXDHEDTS_IDS
{
	m_idStart = wxID_HIGHEST,
	m_idExportPhysics,
	m_idImportPhysics,
	m_idTooBar,
	m_idShowCollision,


	m_sdkDemosID
};


//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 0)
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 4)          // friction
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 20)
#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 21)			// mesh collision

//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 23)        // buoyancy
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 28)        // buoyancy
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 33)        // ray cast car
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 34)        // rag doll
//#define LOAD_DEFAULT_SCENE (m_sdkDemosID + 35)        // rag doll






IMPLEMENT_APP(NewtonSdkApp)

BEGIN_EVENT_TABLE(NewtonFrame, wxFrame)
	EVT_MENU(wxID_NEW, NewtonFrame::OnNewScene)

	EVT_MENU(wxID_OPEN, NewtonFrame::OnFileOpenVisualScene)
	EVT_MENU(wxID_SAVE, NewtonFrame::OnFileSaveVisualScene)

	EVT_MENU(m_idExportPhysics, NewtonFrame::OnFileSavePhysicsScene)
	EVT_MENU(m_idImportPhysics, NewtonFrame::OnFileOpenPhysicsScene)


	EVT_MENU(wxID_EXIT, NewtonFrame::OnExit)

	EVT_MENU(m_idShowCollision, NewtonFrame::OnShowCollision)
	

	EVT_MENU_RANGE (m_sdkDemosID, m_sdkDemosID + sizeof (demosSelection) / sizeof (demosSelection[0]), NewtonFrame::onSDKDemo)
END_EVENT_TABLE()





bool NewtonSdkApp::OnInit()
{
	// check for memory leaks
#if defined(_DEBUG) && defined(_MSC_VER)
 	// Track all memory leaks at the operating system level.
	// make sure no Newton tool or utility leaves leaks behind.
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));
#endif

// test collada exporter and importer
//dLoaderContext context;
//dSceneModelList xxx1;
//xxx1.ImportColladaScene("C:/Newton_200/NewtonSDK/samples/bin/jeep.dae", context, GetIdentityMatrix(), 1.0f);
//xxx1.ExportColladaScene("C:/Newton_200/NewtonSDK/samples/bin/xxx1.dae", GetIdentityMatrix(), 1.0f);
//dModel xxx2;
//xxx2.LoadCollada("C:/Newton_200/NewtonSDK/samples/bin/gymnast.dae", context, GetIdentityMatrix(), 1.0f);
//xxx2.SaveCollada("C:/Newton_200/NewtonSDK/samples/bin/xxx1.dae", GetIdentityMatrix(), 1.0f);


	NewtonFrame *frame = new NewtonFrame( "Newton 2.0 SDK demos", wxDefaultPosition, wxSize(1024, 768));
	frame->Show(true);
	SetTopWindow(frame);

	// load the default scene
	wxMenuEvent startScene (wxEVT_COMMAND_MENU_SELECTED, LOAD_DEFAULT_SCENE);
	frame->GetEventHandler()->ProcessEvent(startScene);

	return true;
}

void NewtonFrame::LoadIcon (char* name, wxBitmap& map)
{
	char fullPathName[2048];
	GetWorkingFileName (name, fullPathName);
	map = wxBitmap (_T(fullPathName), wxBITMAP_TYPE_BMP);
}


NewtonFrame::NewtonFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	:wxFrame(NULL, -1, title, pos, size)
{

	// add load and save file options
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_NEW, _T("&New scene"), _T("Create a blank new scene"));
	fileMenu->AppendSeparator();

	fileMenu->Append(wxID_OPEN, _T("&Open visual scene"), _T("Open visual scene in collada format"));
	fileMenu->Append(wxID_SAVE, _T("&Save visual scene"), _T("Save visual scene in collada format"));
	fileMenu->AppendSeparator();

	fileMenu->Append(m_idImportPhysics, _T("&Open physics scene"), _T("Open physics scene in collada format"));
	fileMenu->Append(m_idExportPhysics, _T("&Save physics scene"), _T("Save physics in collada format"));
	fileMenu->AppendSeparator();

	fileMenu->Append(wxID_EXIT, _T("E&xit\tAlt-X"), _T("Quit SDK sample") );

	wxMenu *sdkDemos = new wxMenu;
	for (int i = 0; i < int (sizeof (demosSelection) / sizeof (demosSelection[0])); i ++) {
		sdkDemos->Append (m_sdkDemosID + i, _T (demosSelection[i].m_name), _T (demosSelection[i].m_description));
	}

	wxMenuBar *menuBar  = new wxMenuBar;
	menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(sdkDemos, _T("SDK demos"));
//	menuBar->Append(options, _T("EngineOptions"));

	// these items should be initially checked
//	menuBar->Check(Menu_MenuBar_Toggle, true);
//	menuBar->Check(Menu_MenuBar_Enable, true);
//	menuBar->Check(Menu_Menu_Enable, true);
//	menuBar->Check(Menu_Menu_Check, false);

	// associate the menu bar with the frame
	SetMenuBar(menuBar);

	// add a status bar to display the engine settings
	CreateStatusBar();
	SetStatusText(_T("Engine settings:"));

	// create the option toolbar
	int style = wxTB_TOP | wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;
	m_toolBar = CreateToolBar(style, m_idTooBar);

	// Set up toolbar
	enum
	{
		m_showCollision,
//		Tool_new,
//		Tool_open,
//		Tool_save,
//		Tool_copy,
//		Tool_cut,
//		Tool_paste,
//		Tool_print,
//		Tool_help,
		m_toolMax
	};
	wxBitmap toolBarBitmaps[m_toolMax];

	LoadIcon ("showCollision.bmp", toolBarBitmaps[0]);

//	int w = toolBarBitmaps[0].GetWidth() * 2;
//	int h = toolBarBitmaps[0].GetHeight() * 2;
//	for ( size_t n = m_showCollision; n < WXSIZEOF(toolBarBitmaps); n++ )
//	{
//		toolBarBitmaps[n] = wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
//	}

	m_toolBar->AddTool(m_idShowCollision, _T(""), toolBarBitmaps[m_showCollision], wxNullBitmap, wxITEM_NORMAL, _T(""), _T(""));



	// after adding the buttons to the toolbar, must call Realize() to reflect the changes
	m_toolBar->Realize();

	
	// Note: In Linux the default gl_canvas window does not uses GL_DOUBLEBUFFER, 
	// this results int a terrible flicker when rendering in real time 
	// the solution is to explicitly force DL_DOUBLEBUFFER on all platform
	//int attributes [] = {WX_GL_DOUBLEBUFFER, WX_GL_RGBA, WX_GL_DEPTH_SIZE, 32, 0};
	int attributes [] = {WX_GL_DOUBLEBUFFER, WX_GL_RGBA, WX_GL_DEPTH_SIZE, 16, 0};
	// add the render window	
	m_canvas = new RenderCanvas(this, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxSUNKEN_BORDER, attributes);
}

NewtonFrame::~NewtonFrame()
{
}


DGCamera* NewtonFrame::GetDGCamera ()
{
	return m_canvas->GetDGCamera();
}

void NewtonFrame::OnFileSaveVisualScene(wxCommandEvent& event)
{

	// get the file name to be save
	wxString filename = wxFileSelector(_T("Save collada visual scene"), _T(""), _T(""), _T(""), _T("Collada files (*.dae)|*.dae"), wxFD_SAVE);
	if (!filename.IsEmpty())
	{
		//dLoaderContext context;
		dSceneModelList modelList;
		for (SceneManager::dListNode* node = GetFirst(); node; node = node->GetNext()) {
			modelList.AddModel(node->GetInfo());
		}

		dMatrix matrix (dYawMatrix (3.14159265f * 0.5f));
		modelList.ExportVisualScene(filename, matrix, 1.0f);
	}

	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}



void NewtonFrame::OnFileSavePhysicsScene(wxCommandEvent& event)
{
	// get the file name to be save
	if (m_world) {
		wxString filename = wxFileSelector(_T("Save collada physics scene"), _T(""), _T(""), _T(""), _T("Collada files (*.dae)|*.dae"), wxFD_SAVE);
		if (!filename.IsEmpty())
		{
			//dLoaderContext context;
			dSceneModelList modelList;
	//		for (SceneManager::dListNode* node = GetFirst(); node; node = node->GetNext()) {
	//			modelList.AddModel(node->GetInfo());
	//		}
	//dSceneModelList xxx1;
	//xxx1.ImportColladaScene("C:/Newton_200/NewtonSDK/samples/bin/jeep.dae", context, GetIdentityMatrix(), 1.0f);
	//xxx1.ExportPhysicsScene("C:/Newton_200/NewtonSDK/samples/bin/xxx1.dae", m_world, GetIdentityMatrix(), 1.0f);
	//xxx1.ExportPhysicsScene("D:/collada/coherencytest_0_1a/xxx1.dae", m_world, GetIdentityMatrix(), 1.0f);
	//dLoaderContext context;
	//xxx1.ImportPhysicsScene("D:/collada/coherencytest_0_1a/xxx1.dae", m_world, context, GetIdentityMatrix(), 1.0f);

			dMatrix matrix (dYawMatrix (3.14159265f * 0.5f));
			modelList.ExportPhysicsScene(filename, m_world, matrix, 1.0f);
		}

		wxPaintEvent paintEvent;
		m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
	}
}


void NewtonFrame::OnNewScene(wxCommandEvent& event)
{
	// close the current scene playing
	CloseScene ();

	// send a display refresh event in case the runtime update is stopped bu the user.
	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}


void NewtonFrame::OnFileOpenVisualScene(wxCommandEvent& event)
{
	// get the file name to be loaded
	wxString filename = wxFileSelector(_T("Open collada scene"), _T(""), _T(""), _T(""), _T("Collada files (*.dae)|*.dae"), wxFD_OPEN);
	if (!filename.IsEmpty())
	{
		OGLLoaderContext context;
		dSceneModelList modelList;

		dMatrix matrix (dYawMatrix (-3.14159265f * 0.5f));
		modelList.ImportVisualScene (filename, context, matrix, 1.0f);

		// add all models to the scene
		for (dSceneModelList::dListNode* node = modelList.GetFirst(); node; node = node->GetNext()) {
			AddModel___((OGLModel*)node->GetInfo());
		}
	}

	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}

void NewtonFrame::OnFileOpenPhysicsScene(wxCommandEvent& event)
{
	// get the file name to be loaded
	wxString filename = wxFileSelector(_T("Open collada scene"), _T(""), _T(""), _T(""), _T("Collada files (*.dae)|*.dae"), wxFD_OPEN);
	if (!filename.IsEmpty())
	{
		OGLLoaderContext context;
		dSceneModelList modelList;

		IniWorld ();
		dMatrix matrix (dYawMatrix (-3.14159265f * 0.5f));
		modelList.ImportPhysicsScene(filename, m_world, context, matrix, 1.0f);

		// add all models to the scene
		for (dSceneModelList::dListNode* node = modelList.GetFirst(); node; node = node->GetNext()) {
			AddModel___((OGLModel*)node->GetInfo());
		}
	}

	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}



void NewtonFrame::OnExit(wxCommandEvent& event)
{
	// close destroy and terminate the main frame
	Close(true);
}

void NewtonFrame::IniWorld ()
{
	// if a scene was found launch it a break the search
	//	LoadScene (demosSelection[i].m_launchDemoCallback);

	//	NewtonWorld* world;
	//	TimeLastFrame = GetTickCount();
	if (!m_world) {

		// create the newton world
		m_world = NewtonCreate ();

		// attach a callback to handle body going out of the world 
		//	NewtonSetBodyLeaveWorldEvent (world, BodyLeaveWorld); 

		// set a collision destructor call back
		//	NewtonSetCollisionDestructor(world, CollisionDestructor);

		// set performance counter
		//	NewtonSetPerformanceClock (world, CHiResTimer::GetTimeInMicrosenconds);


		// select instruction set mode
		//	hardwareMode->set_int_val (1);
		//	hardwareMode->set_int_val (0);
		//	ChangeHardwareMode (0);

		// set the solver mode
		//setSolverMode->set_int_val (2);
		//	ChangeSolverMode (0);
		NewtonSetSolverModel (m_world, 1);

		// set the Max Thread
		//	selectNumberOfthreads->set_int_val (1);
		//selectNumberOfthreads->set_int_val (4);
		//	SetThreadCount (0);

		// set auto sleep mode on
		//autoSleep->set_int_val (1);
		//	ChangeSleepMode (0);

		//showContacts->set_int_val(1);
		//	ChangeShowContacts (0);

		//	ChangeChangeIslandTrading (0);

		// set the adaptive friction model for faster speed 
		//	NewtonSetFrictionModel (nWorld, 1);

		// organize the world before beginning updates (optional)
		NewtonInvalidateCache (m_world);

		// set the z axis as the vertical axis (1 is the default witch map y axis)
		//	NewtonSetBroadPhaseVerticalAxis (world, 2);
	}
}


void NewtonFrame::onSDKDemo (wxCommandEvent& event)
{
	// close the current scene playing
	CloseScene ();

	// find the selected scene
	for (int i = 0; i < int (sizeof (demosSelection) / sizeof (demosSelection[0])); i ++) {
		if (event.GetId() == (m_sdkDemosID + i)) {
			IniWorld ();

			// load this demo into world
			demosSelection[i].m_launchDemoCallback (*this);

//dSceneModelList xxx1;
//xxx1.ImportColladaScene("C:/Newton_200/NewtonSDK/samples/bin/jeep.dae", context, GetIdentityMatrix(), 1.0f);
//xxx1.ExportPhysicsScene("C:/Newton_200/NewtonSDK/samples/bin/xxx1.dae", m_world, GetIdentityMatrix(), 1.0f);
//xxx1.ExportPhysicsScene("D:/collada/coherencytest_0_1a/xxx1.dae", m_world, GetIdentityMatrix(), 1.0f);
//dLoaderContext context;
//xxx1.ImportPhysicsScene("D:/collada/coherencytest_0_1a/xxx1.dae", m_world, context, GetIdentityMatrix(), 1.0f);
			break;
		}
	}

	// send a display refresh event in case the runtime update is stopped bu the user.
	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}


void NewtonFrame::OnShowCollision(wxCommandEvent& event)
{
	m_canvas->m_showCollision = !m_canvas->m_showCollision; 
	DebugDisplaySetMode(m_canvas->m_showCollision);

//	_ASSERTE (demo.m_setMeshCollision);
	m_setMeshCollision(*this, m_canvas->m_showCollision);

	// send a display refresh event in case the runtime update is stopped bu the user.
	wxPaintEvent paintEvent;
	m_canvas->GetEventHandler()->ProcessEvent(paintEvent);
}