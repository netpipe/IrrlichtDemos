// App.cpp: implementation of the CApp class.
// Setup, event handling, main execution loop etc.

//email me: nomad@sideshoot.com

#include "App.h"
#include "irrlicht.h"
#include "utils.h"
#include "BulletPhysicsWorld.h"
#include "BulletPhysicsObject.h"
#include "WorldObjectMgr.h"
#include "CollisionObjects.h"
#include "App.h"

#include "LinearMath/btQuickprof.h"

using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace gui;

CWorldObjectMgr* g_pWorldObjectMgr=0;
IrrlichtDevice* g_pIrrDevice=0;

#define MAX_TIMESTEP 0.2f	 //maximum time interval between physics updates

//set this to zero to run at maximum frame rate
#define MIN_STEP_SIZE 0.01666666f //minimum time before an update loop.

//the debug drawer used by Bullet to display debug info
//CMyBtDebugDraw	debugDrawer;

CApp::CApp()
{
	//some debug things
	m_bWireFrame=m_bAABBs=m_bPointCloud=false;
	m_bShadows=m_bShowStatues=m_bDebugWindow=m_bDeactivation=true;


	m_WindowDims.X=1024;
	m_WindowDims.Y=768;

	//Set the default key mapping for user-definable action keys
	//note, all keys are re-definable, including editor keys
	//also can have more than one action event assigned to same key

	m_KeyMap.clear();

	//NOTE - must have ALL _keyaction ENUMS in here, or m_ActionKeys will
	// not be correctly sized, so don't miss any out!
	//This code allows for detection of key release, repeat and changing
	// key mapping easily
	m_KeyMap.push_back(SCamKeyMap(KACTION_QUIT,			irr::KEY_ESCAPE));
	m_KeyMap.push_back(SCamKeyMap(KACTION_1,			irr::KEY_KEY_1));
	m_KeyMap.push_back(SCamKeyMap(KACTION_2,			irr::KEY_KEY_2));
	m_KeyMap.push_back(SCamKeyMap(KACTION_3,			irr::KEY_KEY_3));
	m_KeyMap.push_back(SCamKeyMap(KACTION_4,			irr::KEY_KEY_4));
	m_KeyMap.push_back(SCamKeyMap(KACTION_5,			irr::KEY_KEY_5));
	m_KeyMap.push_back(SCamKeyMap(KACTION_6,			irr::KEY_KEY_6));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_SHADOWS,			irr::KEY_KEY_S));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_WIREFRAME,			irr::KEY_KEY_W));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_POINTCLOUD,			irr::KEY_KEY_Q));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_DEACTIVATION,		irr::KEY_KEY_D));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_AABBS,			irr::KEY_KEY_A));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_STATUES,			irr::KEY_KEY_K));
	m_KeyMap.push_back(SCamKeyMap(KACTION_TOGGLE_DEBUGWINDOW,			irr::KEY_SPACE));

	m_numActionKeys = (s32) m_KeyMap.size();
	m_ActionKeys = new ActionKeys[m_numActionKeys]; //the array where key states are stored

	AllKeysUp();

	//create irrlicht devide
	g_pIrrDevice = createDevice(
	video::EDT_OPENGL,
	core::dimension2d<s32>(m_WindowDims.X,m_WindowDims.Y),
	32,		//bpp
	false,	//fullscreen
	true,	//stencilbuffer
	true,	//vsync - enabled, to prevent tearing and other anomolies in fullscreen (http://www.d-silence.com/feature.php?id=255)
	this);	//event receiver
	//should have option to turn vsync off for slow systems
	if (g_pIrrDevice == 0) {
		return;
	}

	GET_DRIVER;
	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	//set a font for the debug info window
	GET_GUIENV;
	IGUISkin* skin = guienv->createSkin(gui::EGST_WINDOWS_CLASSIC);
    guienv->setSkin(skin);
    skin->drop();
	IGUIFont* font = guienv->getFont("fonts/fonthaettenschweiler.bmp");
	if (font) skin->setFont(font);

	///////////////////////////////////////////////////////////////////////
	//add the game camera
	GET_SMGR;
	GET_SCENEROOT;

	m_pCamera = smgr->addCameraSceneNodeFPS();

	m_pCamera->setPosition(core::vector3df(-5,-10,-40));
	m_pCamera->setTarget(core::vector3df(-5,-10,0));
	m_pCamera->setFarValue(1200.0f);


	// disable mouse cursor
	g_pIrrDevice->getCursorControl()->setVisible(false);
	///////////////////////////////////////////////////////////////////////
	//add a directional light
	m_pLight = smgr->addLightSceneNode(NULL, vector3df(0,1,1), SColorf(1.0, 1.0, 1.0, 1.0), 6000.f);

	SLight & l = m_pLight->getLightData();
	l.AmbientColor = SColorf(0.15f,0.15f,0.15f); //or is very dark on back faces
	//l.SpecularColor = SColorf(0,0,0);

	//add parent node to rotate directional light
	ISceneNode* pNode = smgr->addEmptySceneNode();

	m_pLight->setPosition(vector3df(0,0,1000.f)); //put at some distance
	m_pLight->setParent(pNode);

	pNode->setRotation(vector3df(-120,0,-30));

	//only set the red component to change shadow intensity for some reason
	smgr->setShadowColor(video::SColor(64,0,0,0));
	///////////////////////////////////////////////////////////////////////

   	// create skybox
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	ISceneNode* pSky = smgr->addSkyBoxSceneNode(
		driver->getTexture("sky/up.jpg"),
		driver->getTexture("sky/down.jpg"), //down
		driver->getTexture("sky/forward.jpg"),
		driver->getTexture("sky/back.jpg"),
		driver->getTexture("sky/left.jpg"),
		driver->getTexture("sky/right.jpg"));

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	///////////////////////////////////////////////////////////////////////

	g_pWorldObjectMgr = new CWorldObjectMgr();
	//create the physical world
	m_pPhysicsWorld = new CBulletPhysicsWorld(g_pWorldObjectMgr);

	CBulletPhysicsObject::s_pPhysicsWorld = m_pPhysicsWorld;
//	m_pPhysicsWorld->m_pDynamicsWorld->setDebugDrawer(&debugDrawer);
//	debugDrawer.setDebugMode(btIDebugDraw::DBG_ProfileTimings);



	///////////////////////////////////////////////////////////////////////
	//create any other static geometry
	//we have an 'altar' which is a static, non-optimized concave object
	//loaded as a .3ds mesh
	m_pTerrainObject = new CTerrain("meshes/scene1.MY3D"); //this is an optimized type

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector
		= smgr->createTriangleSelector(m_pTerrainObject->m_pMesh->getMesh(0), m_pTerrainObject->m_pNode);
	m_pTerrainObject->m_pNode->setTriangleSelector(selector);
	selector->drop();

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
		selector, m_pCamera, vector3df(2,2,2), vector3df(0,0,0));
	m_pCamera->addAnimator(anim);
	anim->drop();

	//a static object for collision testing
	m_pAltar=0;
	m_pAltar = new CConcave("meshes/altar.3DS"); // this is not, perhaps more suitable for simple static objects?
	m_pAltar->SetPosition(vector3df(-2.8f,-11.8f,-22.2f));

	//finally some objects with no attached collision geometry (due to high poly count)
	// although could do these as compounds, non-uniform scaled spheres (elipsoids) etc.
	m_pStatueNode1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("meshes/statue_l.MY3D"));
	m_pStatueNode2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("meshes/statue_r.MY3D"));
	m_pStatueNode1->setPosition(vector3df(-5.25,-12.25,-23.5));//(13,99,135));
	m_pStatueNode2->setPosition(vector3df(-0.5,-12.25,-23.5));

	m_pLaserArray.clear();

	ShowDebugWindow(true);

}
CApp::~CApp()
{
	//don't need to delete the camera since it is a scene node
	SAFE_DELETE(m_pTerrainObject);
	SAFE_DELETE(m_pAltar);
	//delete all other objects here

	SAFE_DELETE(g_pWorldObjectMgr);
	SAFE_DELETE(m_pPhysicsWorld);
	CBulletPhysicsObject::s_pPhysicsWorld = 0;

 	SAFE_DELETE_ARRAY(m_ActionKeys);
	g_pIrrDevice->closeDevice();
	g_pIrrDevice->drop();
	g_pIrrDevice=0;
}
void CApp::Run()
{
	if (g_pIrrDevice == 0) return;
	g_pIrrDevice->setWindowCaption(L"Bullet-Irrlicht demo: SPACE to show / hide debug info");

	GET_DRIVER;
	GET_SMGR;
	GET_GUIENV;

	s32 lastTime = g_pIrrDevice->getTimer()->getTime();
	f32 count = 0.0f;
	bool loop=true;
	f32 accumulator = 0.0f;

	while (loop) {

		if (!g_pIrrDevice->run()) break; //or it will hang when switching out to another application

		if (g_pIrrDevice->isWindowActive())	{

            s32 now = g_pIrrDevice->getTimer()->getTime();
			s32 timeDiff = now - lastTime;
			lastTime = now;
			f32 deltaTime = timeDiff / 1000.f;

			accumulator += deltaTime;
			if (accumulator > MAX_TIMESTEP) accumulator = MAX_TIMESTEP;

			//not much point in having an unlimited frame rate. Using a time 'accumulator'
			// allows for a minimum deltaTime before performing a step.
			if (accumulator >= MIN_STEP_SIZE) {

				//must call run() in the main loop, or there will be a delay on event handling on slower systems
				// also ensures tick() is called internally each update loop

				if (!(loop=g_pIrrDevice->run())) break;

				Update(accumulator);
#ifdef USE_QUICKPROF
				btProfiler::beginBlock(PROF_RENDER);
#endif
				driver->beginScene(true, true, video::SColor(0,0,0,0));
				if (smgr) smgr->drawAll();
				if (guienv) guienv->drawAll();

				driver->endScene();
#ifdef USE_QUICKPROF
				btProfiler::endBlock(PROF_RENDER);
#endif

				UpdateProfiling(accumulator); //do FPS, CPU usage display etc.
				accumulator=0;
			}


			//quit if escape hit
			if (IsActionKeyPressed(KACTION_QUIT)) {
				loop=false;
			}
		}
	}
}


void CApp::AllKeysUp()
{
	for (s32 i=0; i<m_numActionKeys; i++) {
		m_ActionKeys[i].Clear();

	}
	for (int i=0; i<3; ++i)
		m_MouseKeys[i].Clear();
}


void CApp::Update(f32 elapsedTime)
{


	m_pPhysicsWorld->Update(elapsedTime);
	for (std::vector<Laser*>::iterator iter = m_pLaserArray.begin(); iter != m_pLaserArray.end(); iter++) {
		Laser* pLaser = *iter;
		if (pLaser->Update(elapsedTime)) {
			pLaser->Cleanup();
			delete pLaser;
			m_pLaserArray.erase(iter);
			break;
		}
	}
	if (IsMouseKeyPressed(0)) {
		//cast a ray from (slightly below) camera to some point in the distance
		vector3df up(0,1,0);
		vector3df campos = m_pCamera->getPosition();
		vector3df camrot = m_pCamera->getRotation();

		//get camera direction
		matrix4 m;
		m.setRotationDegrees(camrot);
		vector3df camdir(0,0,1);
		m.rotateVect(camdir);

		//work out fire pos and target pos
		vector3df camoffset = up.crossProduct(camdir).crossProduct(camdir) * 2.f + up.crossProduct(camdir);
		vector3df target = camdir * 100;
		vector3df farpos = campos + target;
		vector3df firepos = campos + camoffset;
		m_pLaserArray.push_back(new Laser(firepos, farpos));

		//see if the ray hits anything
		CWorldObject* pObj = m_pPhysicsWorld->RayTest(firepos,farpos);
		if (pObj) g_pWorldObjectMgr->Remove(pObj); //just destroy object
	}
#ifdef USE_QUICKPROF
	btProfiler::endBlock(PROF_GAME);
#endif
}

bool CApp::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT) {

		switch(event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				m_MouseKeys[0].Press();
				break;
			case EMIE_MMOUSE_PRESSED_DOWN:
				m_MouseKeys[1].Press();
				break;
			case EMIE_RMOUSE_PRESSED_DOWN:
				m_MouseKeys[2].Press();
				break;
			case EMIE_LMOUSE_LEFT_UP:
				m_MouseKeys[0].Release();
				break;
			case EMIE_RMOUSE_LEFT_UP:
				m_MouseKeys[2].Release();
				break;
			case EMIE_MMOUSE_LEFT_UP:
				m_MouseKeys[1].Release();
				break;
			case EMIE_MOUSE_MOVED:
				break;
		}
	}
	if (event.EventType == EET_KEY_INPUT_EVENT) { //mouse event also sent with this for some reason

		//first save any mapped key actions
		for (s32 i=0; i<m_numActionKeys; ++i) {
			//get a key mapping
			SCamKeyMap &keymap = m_KeyMap.at(i);
			s32 action = keymap.action;
			s32 keycode = keymap.keycode;
			if (keycode == event.KeyInput.Key)	{//this mapping corresponds to the key event

				//so we need to block the key repeat (for event-driven behaviours, like weapon selection)
				// and save the state (for update-driven behavious, like movement
				//This is done in the ActionKeys class.
				if (event.KeyInput.PressedDown)
					m_ActionKeys[action].Press();
				else
					m_ActionKeys[action].Release();

			}
		}

		//now we can look for specific key presses
		const vector3df initpos(-2.8f,-10.0f, -22.5f);
		if (IsActionKeyPressed(KACTION_1)) {
			AddObject(0, initpos);
		}
		if (IsActionKeyPressed(KACTION_2)) {
			AddObject(1, initpos);
		}
		if (IsActionKeyPressed(KACTION_3)) {
			AddObject(2, initpos);
		}
		if (IsActionKeyPressed(KACTION_4)) {
			AddObject(3, initpos);
		}
		if (IsActionKeyPressed(KACTION_5)) {
			AddObject(4, initpos);
		}
		if (IsActionKeyPressed(KACTION_6)) {
			AddObject(5, initpos);
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_DEBUGWINDOW)) {
			m_bDebugWindow = !m_bDebugWindow;
			ShowDebugWindow(m_bDebugWindow);
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_SHADOWS)) {
			m_bShadows = !m_bShadows;
			SetText(m_bShadows, m_pShadowText);
			UpdateDebugDisplay();
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_WIREFRAME)) {
			m_bWireFrame=!m_bWireFrame;
			m_bPointCloud=false;
			SetText(m_bPointCloud, m_pPointCloudText);
			SetText(m_bWireFrame, m_pWireframeText);

			UpdateDebugDisplay();
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_AABBS)) {
			m_bAABBs = !m_bAABBs;
			SetText(m_bAABBs, m_pAABBtext);
			UpdateDebugDisplay();
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_POINTCLOUD)) {
			m_bPointCloud=!m_bPointCloud;
			m_bWireFrame=false;
			SetText(m_bPointCloud, m_pPointCloudText);
			SetText(m_bWireFrame, m_pWireframeText);
			UpdateDebugDisplay();
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_DEACTIVATION)) {
			m_bDeactivation = !m_bDeactivation;
//			if (m_bDeactivation) debugDrawer.setDebugMode(debugDrawer.getDebugMode() & ~btIDebugDraw::DBG_NoDeactivation);
//			else debugDrawer.setDebugMode(debugDrawer.getDebugMode() | btIDebugDraw::DBG_NoDeactivation);
			SetText(m_bDeactivation, m_pDeactivationText);
			UpdateDebugDisplay();
		}
		if (IsActionKeyPressed(KACTION_TOGGLE_STATUES)) {
			m_bShowStatues = !m_bShowStatues;
			SetText(m_bShowStatues, m_pShowStatuesText);
			UpdateDebugDisplay();
		}
	}
	return 0;
}



void CApp::AddObject(s32 type, core::vector3df pos)
{
	static bool once = true;
	CWorldObject* pObj=0;

	GET_DRIVER;
	switch(type)
	{
		case 0:
			pObj = new CBox("meshes/box.3DS");
			break;
		case 1:
			{
			pObj = new CSphere("meshes/sphere.X");
			//OK, why are we setting the texture here?
			//because MAX is exporting an absolute path for the texture, and we need .X files for the exported normals!
			//This needs fixing in the loader.
			pObj->m_pNode->setMaterialTexture(0,driver->getTexture("meshes/black.jpg"));
			}

			break;
		case 2:
			{
			pObj = new CCylinder("meshes/log.X");
			pObj->m_pNode->setMaterialTexture(0,driver->getTexture("meshes/log.jpg"));

			}
			break;
		case 3:
			{
			pObj = new CCone("meshes/cone.X");
			pObj->m_pNode->setMaterialTexture(0,driver->getTexture("meshes/greystone.jpg"));
			}
			break;
		case 4:
			{
			pObj = new CConvex("meshes/rock.X");
			pObj->m_pNode->setMaterialTexture(0,driver->getTexture("meshes/rock.jpg"));
			//as an example, 'register' this object for collision with the altar
			if (m_pAltar) m_pPhysicsWorld->RegisterCollision(m_pAltar, pObj);
			}
			break;
		case 5:
			pObj = new CCompound("meshes/smallball.3DS");
			break;
		default:
			return;
	}

	if (pObj) {
		if (pObj->m_pNode->getMaterialCount()>0) {
			//due to the messy way I've handled compounds, these won't
			// have an accessible material so won't show when deactivated
			video::SMaterial& mat = pObj->m_pNode->getMaterial(0);
			mat.EmissiveColor = video::SColor(0,0,0,0);
			mat.AmbientColor = video::SColor(160,160,160,160);
			mat.DiffuseColor = video::SColor(160,160,160,160);
		}
		g_pWorldObjectMgr->Add(pObj);
		pObj->SetPosition(pos);
		SetDisplayOptions(pObj);
	}
}
//The rest is just to display the stats like FPS
void CApp::ShowDebugWindow(bool show)
{
	GET_GUIENV;
	if (!show) {
		IGUIElement* e = g_pIrrDevice->getGUIEnvironment()->getRootGUIElement()->getElementFromId(101, true);
		if (e) e->remove();
		return;
	}
	//set up a window for debug info

	const s32 toolWindowW=300;
	s32 winH = m_WindowDims.Y - 20;
	s32 winX = m_WindowDims.X - toolWindowW-10;
	s32 winY = 10;

	IGUIWindow* toolWindow =guienv->addWindow(rect<s32>(winX, winY, winX+toolWindowW, winY+winH), false,L"Debug info", 0, 101);

	s32 x=10;
	s32 y=20;

	 s32 wl=100; //width of label
	 s32 hl=20; //height of label
	 s32 wt=30; //width of on / off text box
	guienv->addStaticText(L"Shadows:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pShadowText = guienv->addEditBox(L"on", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"AABB's:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pAABBtext = guienv->addEditBox(L"off", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Wireframe:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pWireframeText = guienv->addEditBox(L"off", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Hide Geometry:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pPointCloudText= guienv->addEditBox(L"off", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Show Statues:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pShowStatuesText = guienv->addEditBox(L"on", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Auto Deactivation:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pDeactivationText = guienv->addEditBox(L"on", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=30;
	guienv->addStaticText(L"FPS:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pFPSText = guienv->addEditBox(L"", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Num Objects:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pNumObjectsText = guienv->addEditBox(L"", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=16;
	guienv->addStaticText(L"Polys Rendered:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	m_pNumPolysText = guienv->addEditBox(L"", rect<s32>(x+wl, y, x+wl+wt, y+hl), false, toolWindow);
	y+=30;
	guienv->addStaticText(L"Profiling:", rect<s32>(x, y, x+wl, y+hl), false, false, toolWindow);
	y+=16;
	m_pBTProfText = guienv->addListBox(rect<s32>(x, y, x+toolWindowW-20, y+250), toolWindow);

	y+=260;

	guienv->addStaticText(L"INSTRUCTIONS:", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 20;
	guienv->addStaticText(L"Arrow keys to move, click to shoot", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"1 - Add Box", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"2 - Add Sphere", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"3 - Add Cylinder", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"4 - Add Cone", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"5 - Add Convex Shape", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"6 - Add Compound Shape", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"W - Wireframe", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"Q - Hide geometry (as pointcloud)", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"S - Toggle Shadows", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"K - Toggle Statues (reduces polycount)", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"SPACE - Hide this window", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"D - Toggle Auto Deactivation", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	y+= 16;
	guienv->addStaticText(L"ESC - Quit", rect<s32>(x, y, x+toolWindowW, y+hl), false, false, toolWindow);
	UpdateDebugDisplay();
}


//make changes to debug display here
//this is a bit messy, this should go in the CWorldObject class
//but keeping it all in the CApp class since it is just for debugging
void CApp::UpdateDebugDisplay()
{

	CWorldObject* pObj;
	//first the world objects (dynamic things)
	for (pObj = g_pWorldObjectMgr->GetFirst(); pObj; pObj = g_pWorldObjectMgr->GetNext()) {
		SetDisplayOptions(pObj);
	}
	//now the sundry graphic objects
	m_pTerrainObject->m_pNode->setMaterialFlag(video::EMF_WIREFRAME, m_bWireFrame);
	m_pTerrainObject->m_pNode->setMaterialFlag(video::EMF_POINTCLOUD, m_bPointCloud);
	//m_pTerrainObject->m_pNode->setDebugDataVisible(m_bAABBs);
	m_pAltar->m_pNode->setMaterialFlag(video::EMF_WIREFRAME, m_bWireFrame);
	m_pAltar->m_pNode->setMaterialFlag(video::EMF_POINTCLOUD, m_bPointCloud);
	m_pAltar->m_pNode->setDebugDataVisible((m_bAABBs) ? scene::EDS_BBOX : scene::EDS_OFF);
	m_pStatueNode1->setMaterialFlag(video::EMF_WIREFRAME, m_bWireFrame);
	m_pStatueNode1->setMaterialFlag(video::EMF_POINTCLOUD, m_bPointCloud);
	m_pStatueNode1->setDebugDataVisible((m_bAABBs) ? scene::EDS_BBOX : scene::EDS_OFF);
	m_pStatueNode1->setVisible(m_bShowStatues);
	m_pStatueNode2->setMaterialFlag(video::EMF_WIREFRAME, m_bWireFrame);
	m_pStatueNode2->setMaterialFlag(video::EMF_POINTCLOUD, m_bPointCloud);
	m_pStatueNode2->setDebugDataVisible((m_bAABBs) ? scene::EDS_BBOX : scene::EDS_OFF);
	m_pStatueNode2->setVisible(m_bShowStatues);

	SLight & l = m_pLight->getLightData();
	l.CastShadows=m_bShadows;
}

void CApp::UpdateProfiling(f32 elapsedTime)
{
	GET_GUIENV;
	//look for the debug display window first
	if (!guienv->getRootGUIElement()->getElementFromId(101, true)) return;

	static f32 count=0;
	count += elapsedTime;
	if (count < 0.5) return; //updating display every 1/2 second
	count=0;

	GET_DRIVER;
	stringw s1(driver->getFPS());
	stringw s2((s32)g_pWorldObjectMgr->m_WorldObjectArray.size());
	stringw s3((s32)driver->getPrimitiveCountDrawn());
	m_pFPSText->setText(s1.c_str());
	m_pNumObjectsText->setText(s2.c_str());
	m_pNumPolysText->setText(s3.c_str());
	m_pBTProfText->clear();
#ifdef USE_QUICKPROF
	std::map<std::string, hidden::ProfileBlock*>::iterator iter;
	for (iter = btProfiler::mProfileBlocks.begin(); iter != btProfiler::mProfileBlocks.end(); ++iter)
	{
		char blockTime[128];
		if (strcmp(&((*iter).first[0]), PROF_BULLET_PERCENT)==0)
			sprintf(blockTime, "%s: %lf",&((*iter).first[0]),btProfiler::getBlockTime((*iter).first, btProfiler::BLOCK_TOTAL_PERCENT));
		else
			sprintf(blockTime, "%s: %lf",&((*iter).first[0]),btProfiler::getBlockTime((*iter).first, btProfiler::BLOCK_CYCLE_SECONDS));//BLOCK_TOTAL_PERCENT));
		m_pBTProfText->addItem(stringw(blockTime).c_str());
	}
#endif
}
void CApp::SetDisplayOptions(CWorldObject* pObj)
{
	pObj->m_pNode->setMaterialFlag(video::EMF_WIREFRAME, m_bWireFrame);
	pObj->m_pNode->setMaterialFlag(video::EMF_POINTCLOUD, m_bPointCloud);
	pObj->m_pNode->setDebugDataVisible((m_bAABBs) ? scene::EDS_BBOX : scene::EDS_OFF);

}
void CApp::SetText(bool on, IGUIEditBox* pBox)
{
	GET_GUIENV;
	if (!guienv->getRootGUIElement()->getElementFromId(101, true)) return;

	if (on) {
		if (pBox) pBox->setText(L"on");
	}else{
		if (pBox) pBox->setText(L"off");
	};
}
