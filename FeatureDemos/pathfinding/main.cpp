
#include <irrlicht.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <assert.h>

#include "navinode_control.h"
#include "npc_path_control.h"
#include "simple_util.h"

using namespace irr;

IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;

scene::ISceneNode* world = 0;
scene::IMeshSceneNode* worldMeshSceneNode = 0;

scene::ITriangleSelector* selector = 0;
scene::ISceneNodeAnimator* anim = 0;
scene::ISceneNodeAnimatorCollisionResponse* collisionResponse = 0;

NaviNodeControl* nnc = 0;



scene::ISceneNode* fairy = 0;
scene::IAnimatedMeshSceneNode* fairyAnimatedMeshSceneNode = 0;
f32 npcYOffset = 0.0f;

NpcPathControl* npcPathControl = 0;

s32 selectedNode = -1;
s32 startNode = -1;
s32 endNode = -1;

s32 guiId = -1;

bool showHelpScreen = true;

void drawHelpText
(
	IrrlichtDevice* device,
	const core::stringw& text,
	s32 sx,
	s32 sy
)
{
	gui::IGUIFont* font = device->getGUIEnvironment()->getFont("media/fonts/bh/bh.xml");
	if (!font)
		return;
	video::IVideoDriver* driver = device->getVideoDriver();

	core::dimension2d<u32> textSize = font->getDimension(text.c_str());
	s32 tw = textSize.Width;
	s32 th = textSize.Height;

	// draw bg:
	s32 border = 2;
	core::rect<s32> bgRect
	(
		sx, sy,
		sx+tw+border*2, sy+th+border*2
	);

	video::SColor bg(128, 255, 255, 255);
	driver->draw2DRectangle(bg, bgRect);

	// draw text:
	core::rect<s32> rect
	(
		sx+border, sy+border,
		sx+border+tw, sy+border+th
	);

	video::SColor color(0xFF000000);
	font->draw(text, rect, color, false, false);
}

bool loadHelpText(const core::stringc& filename, core::stringw& text)
{
	//printf("loadHelpText()...\n");
	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == 0)
		return false;

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp)/sizeof(char);
	//printf("size of file=%li bytes\n", size);
	if (size == 0)
	{
		fclose(fp);
		return false;
	}
	rewind(fp);

	// pass data to text:
	char* buf = new char[size];
	fread(buf, sizeof(char), size, fp);
	text = core::stringw(buf);

	// clear up:
	delete[] buf;
	fclose(fp);

	return true;
}

void findPath(u32 start, u32 end)
{
	core::array<core::stringw> ss;
	nnc->setStartNode(start);
	nnc->setEndNode(end);
	u32 ssCount = NaviNode::doAstarSearch
	(
		nnc->getNodes(),
		start,
		end,
		ss
	);

	if (ssCount > 0)
	{
		printf("no. of solutions=%i\n", ssCount);
		NaviNode::printSolutions(ss);

		core::stringw text = ss[0];
		core::array<u32> ns = NaviNode::stringwToU32s(text);

		core::array<NaviNode*> nodes;
		core::array<core::vector3df> points;
		for (u32 n=0; n<ns.size(); ++n)
		{
			NaviNode* node = nnc->getNode(ns[n]);
			if (node)
			{
				nodes.push_back(node);
				points.push_back(node->getPos());
			}
		}
		nnc->getView()->setPathNodes(nodes);

		npcPathControl->setWaypoints(points);
		npcPathControl->start();
	}
}


////////////////////////////////////////////////////////////////////////////////
enum
{
	GUI_ID_UNKONWN = -1,

	GUI_ID_LOAD_NAVINODES,
	GUI_ID_SAVE_NAVINODES,
	GUI_ID_SAVE_NAVINODES_AS,

	GUI_ID_QUIT
};

////////////////////////////////////////////////////////////////////////////////
void setupGui()
{
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	gui::IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font = guienv->getFont("media/fonts/bh/bh.xml");


	if (font)
		skin->setFont(font);

	bool enabled = true;
	bool hasSubMenu = true;

	gui::IGUIContextMenu* contextMenu = guienv->addMenu();
	u32 fileMenuId = contextMenu->addItem(L"File", -1, enabled, hasSubMenu);
	u32 helpMenuId = contextMenu->addItem(L"Help", -1, enabled, hasSubMenu);

	hasSubMenu = false;

	contextMenu->getSubMenu(0)->addItem(L"Load navinodes", GUI_ID_LOAD_NAVINODES, enabled, hasSubMenu);
	contextMenu->getSubMenu(0)->addItem(L"Save navinodes", GUI_ID_SAVE_NAVINODES, enabled, hasSubMenu);
	contextMenu->getSubMenu(0)->addItem(L"Quit", GUI_ID_QUIT, enabled, hasSubMenu);

	//contextMenu->getSubMenu(1)->addItem(...); // help submenu
}




////////////////////////////////////////////////////////////////////////////////
class EventReceiver: public IEventReceiver
{

public:
bool OnEvent(const SEvent& event)
{
	if
	(
		(event.EventType == EET_GUI_EVENT)
		&& (event.GUIEvent.EventType == gui::EGET_MENU_ITEM_SELECTED)
	)
	{
		gui::IGUIContextMenu* menu = (gui::IGUIContextMenu*)event.GUIEvent.Caller;
		s32 id = menu->getItemCommandId(menu->getSelectedItem());
		guiId = id;

		if (id == GUI_ID_QUIT)
		{
			device->closeDevice();
			device->run();
			return true;
		}
		else if
		(
			(id == GUI_ID_LOAD_NAVINODES)
			|| (id == GUI_ID_SAVE_NAVINODES)
		)
		{
			gui::IGUIFileOpenDialog* dialog = device->getGUIEnvironment()->addFileOpenDialog(L"Please select a navi nodes file");
			dialog->setRelativePositionProportional(core::rect<f32>(0.1f, 0.1f, 0.9f, 0.9f));
			//dialog->setMinSize(core::dimension2du(800, 600));
		}

		return false;
	}

	if
	(
		(event.EventType == EET_GUI_EVENT)
		&& (event.GUIEvent.EventType == gui::EGET_FILE_SELECTED)
	)
	{
		gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
		core::stringc filename = core::stringc(dialog->getFileName());

		if (guiId == GUI_ID_LOAD_NAVINODES)
		{
			printf("loading navi nodes from filename=%s\n", filename.c_str());
			nnc->setFilename(filename);
			core::array<u32> unused = NaviNodeControl::getUnusedIds(nnc->getNodes());
			for (u32 i = 0; i < unused.size(); i++)
			{
				printf("unused id=%u\n", unused[i]);
			}

			camera->setInputReceiverEnabled(true);
			device->getCursorControl()->setVisible(false);
		}
		else if (guiId == GUI_ID_SAVE_NAVINODES)
		{
			printf("saving navi nodes to filename=%s\n", filename.c_str());
			nnc->save(device, nnc->getNodes(), filename);
			camera->setInputReceiverEnabled(true);
			device->getCursorControl()->setVisible(false);
		}

		return false;
	}

	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == true)
	)
	{
		if (event.KeyInput.Key == irr::KEY_ESCAPE)
		{
			nnc->save(device, nnc->getNodes(), "backup.xml");

			device->closeDevice();
			device->run();
			return true;
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_W)
		{
			endNode = selectedNode;
			if (endNode > -1)
				nnc->setEndNode(u32(endNode));
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_S)
		{
			startNode = selectedNode;
			if (startNode > -1)
				nnc->setStartNode(u32(startNode));
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_A)
		{
			nnc->addNode(device);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_D)
		{
			NaviNode* n = nnc->getNode(selectedNode);
			if (!n)
				return false;
			NpcPathControl::teleportTo(camera, n->getPos(), npcYOffset*2.0f);
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_Q)
		{
			NaviNode* n = nnc->getNode(selectedNode);
			if (!n)
				return false;
			core::vector3df pos = n->getPos();
			npcPathControl->moveNodeTo(pos);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_E)
		{
			nnc->setNodeReserved(selectedNode, false);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_Z)
		{
			nnc->addNode(device, false); // testLineOfSight = false
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_C)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_N)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_M)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_R)
		{
			nnc->setNodeReserved(selectedNode, true);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_T)
		{
			nnc->unsetAllNodesReserved();
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_X)
		{
			if (selectedNode > -1)
				nnc->removeNode(u32(selectedNode));
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_G)
		{
			startNode = endNode;
			if (startNode > -1)
				nnc->setStartNode(u32(startNode));

			endNode = selectedNode;
			if (endNode > -1)
				nnc->setEndNode(u32(endNode));

			if ((startNode > -1) && (endNode > -1))
				findPath(u32(startNode), u32(endNode));
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_B)
		{
			if ((startNode > -1) && (endNode > -1))
				findPath(u32(endNode), u32(startNode));
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_I)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_K)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_O)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_U)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_J)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_L)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_F1)
		{
			showHelpScreen = !showHelpScreen;
		}
		else if (event.KeyInput.Key == irr::KEY_F2)
		{
			if (fairy)
				SimpleUtil::rotateCameraTowardsNodeAroundYAxis(camera, fairy, 5.0f);
		}
		else if (event.KeyInput.Key == irr::KEY_F3)
		{
			bool enabled = camera->isInputReceiverEnabled();
			camera->setInputReceiverEnabled(!enabled);
			device->getCursorControl()->setVisible(enabled);
		}
		else if (event.KeyInput.Key == irr::KEY_F4)
		{
			SimpleUtil::print(fairy->getPosition());
			SimpleUtil::print(camera->getPosition());
			SimpleUtil::print(camera->getTarget());
		}
		else if (event.KeyInput.Key == irr::KEY_F5)
		{
			if ((startNode > -1) && (endNode > -1))
				findPath(u32(startNode), u32(endNode));
		}
		else if (event.KeyInput.Key == irr::KEY_F6)
		{
			static u32 screenShotNum = 1;
			SimpleUtil::makeScreenshot(device, L"screenshot_", screenShotNum);
			++screenShotNum;
		}
		else if (event.KeyInput.Key == irr::KEY_F7)
		{
			SimpleUtil::changeCameraFPSMoveSpeed(camera, 0.5f);
		}
		else if (event.KeyInput.Key == irr::KEY_F8)
		{
			SimpleUtil::changeCameraFPSMoveSpeed(camera, 2.0f);
		}
		else if (event.KeyInput.Key == irr::KEY_F9)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F10)
		{
		}
		else if (event.KeyInput.Key == KEY_F11)
		{
			f32 dsq = nnc->getMaxDistSQ();
			dsq *= 0.75f;
			nnc->setMaxDistSQ(dsq);

		}
		else if (event.KeyInput.Key == KEY_F12)
		{
			f32 dsq = nnc->getMaxDistSQ();
			dsq *= 1.5f;
			nnc->setMaxDistSQ(dsq);

		}


		else if (event.KeyInput.Key == irr::KEY_UP)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_DOWN)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_LEFT)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_RIGHT)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_PRIOR)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_NEXT)
		{
		}

	}


	if
	(
		event.EventType == EET_MOUSE_INPUT_EVENT
	)
	{
		if (event.MouseInput.isMiddlePressed())
		{
			nnc->setMaxDistSQ(100000.0f);
			return false;
		}

		f32 wheel = event.MouseInput.Wheel;
		if ((wheel < -1.0f) || (wheel > 1.0f))
			return false;

		//printf("wheel=%f\n", wheel);

		f32 dsq = nnc->getMaxDistSQ();
		if (wheel <= -1.0f)
			dsq *= 1.5f;
		else if (wheel >= 1.0f)
			dsq *= 0.75f;

		nnc->setMaxDistSQ(dsq);
		return false;
	}
	return false;
}

};


int main()
{
	device = createDevice
	(
		video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
		32,
		false, false, false
	);

	if (device == 0)
		return 1;

	EventReceiver receiver;
	device->setEventReceiver(&receiver);

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	io::IFileSystem* fs = device->getFileSystem();


	////////////////////////////////////////////////////////////////////////////
	// setup city:
	fs->addFileArchive("./data/");
	smgr->loadScene("scenes/CityDriving.irr");
	world = smgr->getSceneNodeFromName("world1_1");
	if (!world)
		return -1;

	world->setDebugDataVisible(scene::EDS_BBOX);
	if (world->getType() == scene::ESNT_MESH)
	{
		worldMeshSceneNode = static_cast<scene::IMeshSceneNode*>(world);
	}
	/***
	printf("type=%i\n", type);
	printf("scene::ESNT_ANIMATED_MESH=%i\n", scene::ESNT_ANIMATED_MESH);
	printf("scene::ESNT_MESH=%i\n", scene::ESNT_MESH);
	***/

	if (worldMeshSceneNode)
	{
		selector = smgr->createOctreeTriangleSelector(worldMeshSceneNode->getMesh(), world, 128);
		world->setTriangleSelector(selector);
	}

	////////////////////////////////////////////////////////////////////////////


	nnc = new NaviNodeControl(device);
	nnc->Selector = selector;
	//nnc->setFilename("navi_nodes_29-03-2012.xml");

	core::aabbox3d<f32> box = world->getTransformedBoundingBox();
	core::vector3df extent = box.getExtent();
	f32 radius = sqrt(extent.X * extent.Z * 0.005f);
	nnc->setMaxDistSQ(radius*radius+1000.0f);

	////////////////////////////////////////////////////////////////////////////
	std::vector<core::vector3df> waypoints;

	scene::ISceneCollisionManager* cmgr = device->getSceneManager()->getSceneCollisionManager();

	core::line3df ray;
	core::vector3df pos;
	core::triangle3df tri;

	//bool withCenters = true;
	//std::vector<core::vector3df> rayStarts = SimpleUtil::getHexGridPoints(world, radius, 0.5f, withCenters);
	/***
	srand(time(0));
	std::vector<core::vector3df> rayStarts = SimpleUtil::getRandomGridPoints(world, 0.5f, 500);
	for (int n=0; n<int(rayStarts.size()); ++n)
	{
		core::vector3df rayStart = rayStarts[n];
		core::vector3df rayEnd = rayStart;
		rayEnd.Y -= 1000000.0f;

		ray.start = rayStart;
		ray.end = rayEnd;

		if (cmgr->getSceneNodeAndCollisionPointFromRay(ray, pos, tri))
			if ((pos.Y > 150.0f) && (pos.Y < 200.0f))
				waypoints.push_back(pos);
	}

	nnc->setWaypoints(waypoints);
	***/

	nnc->setMaxDistSQ(100000.0f);
	////////////////////////////////////////////////////////////////////////////


	//nnc->setWaypointsWithCollision(waypoints);
	//nnc->save("navi_nodes_29-03-2012.xml");

	////////////////////////////////////////////////////////////////////////////
	f32 camRotateSpeed = 100.0f;
	f32 camMoveSpeed = 1.5f;
	f32 camJumpSpeed = 3.0f;

	camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);
	camera->setFarValue(40000.0f);

	//camera->bindTargetAndRotation(true);
	//camera->bindTargetAndRotation(false);

	camera->setPosition(core::vector3df(4972.64,556.69,-2106.97));
	camera->setTarget(core::vector3df(9426.58,-2352.16,-1021.91));

	////////////////////////////////////////////////////////////////////////////
	fairy = smgr->addEmptySceneNode();
	scene::IAnimatedMesh* amesh = smgr->getMesh("media/faerie.md2");
	fairyAnimatedMeshSceneNode = smgr->addAnimatedMeshSceneNode(amesh, fairy);

	if (!fairyAnimatedMeshSceneNode)
	{
		printf("Failed creating fairy scene node!\n");
		return -1;
	}

	fairyAnimatedMeshSceneNode->setLoopMode(true);
	fairyAnimatedMeshSceneNode->setJointMode(scene::EJUOR_READ);
	//fairyAnimatedMeshSceneNode->setJointMode(scene::EJUOR_CONTROL);

	fairyAnimatedMeshSceneNode->setRotation(core::vector3df(0.0f, -90.0f, 0.0f));

	fairyAnimatedMeshSceneNode->setMaterialTexture(0, driver->getTexture("media/faerie2.bmp"));
	fairyAnimatedMeshSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
	if (fairyAnimatedMeshSceneNode)
		fairyAnimatedMeshSceneNode->setMD2Animation(scene::EMAT_RUN);
	//fairyAnimatedMeshSceneNode->setDebugDataVisible(scene::EDS_BBOX);

	//f32 yoffset = fairyAnimatedMeshSceneNode->getBoundingBox().getExtent().Y * 0.5f;
	//fairy->setPosition(core::vector3df(125.68,-418.00+yoffset,-1148.17));
	f32 scale = 2.0f;
	fairy->setScale(core::vector3df(scale));

	fairy->setPosition(core::vector3df(5402.30,240.77,-1737.70));
	////////////////////////////////////////////////////////////////////////////
	npcPathControl = new NpcPathControl(device, fairy);
	npcPathControl->setSpeed(200.0f);

	fairyAnimatedMeshSceneNode->updateAbsolutePosition();
	//f32 yoffset = fairyAnimatedMeshSceneNode->getTransformedBoundingBox().getExtent().Y * 0.5f * scale;
	//printf("yoffset=%f\n", yoffset);
	//f32 yoffset = fairyAnimatedMeshSceneNode->getBoundingBox().getExtent().Y * 0.5f;
	npcYOffset = NpcPathControl::getAnimatedMeshSceneNodeYOffset(fairyAnimatedMeshSceneNode);

	npcPathControl->setYOffset(npcYOffset);
	////////////////////////////////////////////////////////////////////////////
	// create skybox
	/***
	fs->addFileArchive("/home/smso/ex/irrlicht/sky/");
	fs->addFileArchive("/home/smso/ex/irrlicht/sky/snowmountain/");


	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	video::ITexture* t1 = driver->getTexture("winter_sky_1.jpg");
	video::ITexture* t2 = driver->getTexture("winter_sky_2.jpg");
	video::ITexture* t3 = driver->getTexture("winter_sky_3.jpg");
	video::ITexture* t4 = driver->getTexture("winter_sky_4.jpg");
	video::ITexture* t5 = driver->getTexture("winter_sky_5.jpg");
	video::ITexture* t6 = driver->getTexture("winter_sky_6.jpg");

	scene::ISceneNode* sky = smgr->addSkyBoxSceneNode(t6,t5,t2,t4,t3,t1);


	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
	***/

	////////////////////////////////////////////////////////////////////////////
	// setup gui:
	setupGui();

	////////////////////////////////////////////////////////////////////////////
	// draw legend:
	core::rect<s32> legendSourceRect;
	core::position2d<s32> legendTopLeft;

	video::ITexture* legend = driver->getTexture("./data/media/legend.png");
	if (legend)
	{
		core::dimension2d<u32> size = legend->getSize();
		s32 w = (s32)(size.Width);
		s32 h = (s32)(size.Height);
		legendSourceRect = core::rect<s32>(0, 0, w, h);
		//printf("(w,h)=(%i,%i)\n", w, h);

		u32 sh = driver->getScreenSize().Height;
		legendTopLeft = core::position2d<s32>(0, s32(sh)-h);
	}

	////////////////////////////////////////////////////////////////////////////
	// draw reticle:
	core::rect<s32> reticleSourceRect;
	core::position2d<s32> reticleTopLeft;

	video::ITexture* reticle = driver->getTexture("./data/media/reticle.png");
	if (reticle)
	{
		core::dimension2d<u32> size = reticle->getSize();
		s32 w = (s32)(size.Width);
		s32 h = (s32)(size.Height);
		reticleSourceRect = core::rect<s32>(0, 0, w, h);
		//printf("(w,h)=(%i,%i)\n", w, h);

		u32 sw = driver->getScreenSize().Width;
		u32 sh = driver->getScreenSize().Height;
		reticleTopLeft = core::position2d<s32>(s32((sw-w)*0.5f), s32((sh-h)*0.5f));
	}

	////////////////////////////////////////////////////////////////////////////
	core::stringw helpText;
	if (!loadHelpText("help.txt", helpText))
		printf("Error loading help text!\n");

	device->getCursorControl()->setVisible(false);

	// ----------------------------------------------------

	while (device->run())
	if (device->isWindowActive())
	{

		npcPathControl->update();

		driver->beginScene(true, true, 0);


		smgr->drawAll();

		nnc->draw();

		u32 id = 0;
		if (nnc->getView()->findSelectedNode(id, 50))
		{
			nnc->getView()->drawSelectedNode(id);
			selectedNode = s32(id);
		}
		else
		{
			selectedNode = -1;
		}
		SimpleUtil::drawTrajectory(device, npcPathControl->SplinePoints, 0xFFEECC00);


		//SimpleUtil::drawHexGrid(device, world, sqrt(maxDistSQ), 0.5f, 0xFF00FF00);
		SimpleUtil::drawBBoxTop(device, world, 0xFFFF0000);

		guienv->drawAll();
		if (legend)
			driver->draw2DImage
			(
				legend,
				legendTopLeft,
				legendSourceRect,
				0, //clipRect
				0xFFFFFFFF,
				true
			);

		if (reticle)
			driver->draw2DImage
			(
				reticle,
				reticleTopLeft,
				reticleSourceRect,
				0, //clipRect
				0xFFFFFFFF,
				true
			);

		if (showHelpScreen)
			drawHelpText(device, helpText, 50, 50);


		driver->endScene();

		//show info on title bar:
		core::stringw text;
		text += L"No. of nodes: ";
		text += nnc->getNodeCount();
		text += L", DistSQ: ";
		text += NaviNodeControl::stringFromF32(nnc->getMaxDistSQ());

		device->setWindowCaption(text.c_str());

	}
	else { device->yield(); }

	if (nnc) { delete nnc; nnc = 0; }
	if (npcPathControl) { delete npcPathControl; npcPathControl = 0; }

	if (device) { device->drop(); }

	return 0;
}


