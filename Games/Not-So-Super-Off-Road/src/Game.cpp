#include "Game.h"

#include <irrlicht.h>
#include <iostream>
#include <exception>
#include <ctime>

#include "GameObject.h"
#include "Camera.h"
#include "EventReciever.h"

Game::Game()
{
	object = new vector<GameObject*>();
	checkpoint = new deque<IAnimatedMeshSceneNode*>();
	arrow = new vector<IAnimatedMeshSceneNode*>();
	nr_of_laps = 0;
	max_laps = 4;
	current_state = GS_START;
	last_time = clock();
	countdown = NULL;
}

Game::~Game()
{
}

void Game::init()
{
#ifdef WIN32
	device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<s32>(640, 480));
#else
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480));
#endif

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();

	device->setWindowCaption(L"Not So Much Super Offroad - By Andreas Kröhnke");

	IMeshSceneNode *node = smgr->addMeshSceneNode( smgr->getMesh("data/car3.3ds")->getMesh(0) );
	GameObject *car = new GameObject();
	car->setSceneNode(node);
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setScale(vector3df(0.09,0.09,0.09));
	node->setRotation(vector3df(0,0,0));
	node->setPosition(vector3df(3454,500,1256));
	//node->setDebugDataVisible(EDS_BBOX);
	object->push_back(car);

	ITerrainSceneNode *terrain = smgr->addTerrainSceneNode("data/level1.bmp");
	GameObject *go_terrain = new GameObject();
	go_terrain->setSceneNode(terrain);
	terrain->setMaterialFlag(EMF_LIGHTING, false);
	terrain->setScale(core::vector3df(18, 3.0f, 18));
	terrain->setMaterialFlag(video::EMF_LIGHTING, false);
	terrain->setMaterialTexture(0, driver->getTexture("data/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("data/detailmap3.jpg"));
	terrain->setMaterialType(video::EMT_DETAIL_MAP);
	terrain->scaleTexture(1.0f, 20.0f);
	terrain->setDebugDataVisible(EDS_BBOX);
	object->push_back(go_terrain);

	// Camera
	Camera *cam = new Camera();
	cam->setSceneNode(smgr->addCameraSceneNode());
	object->push_back(cam);
	cam->followNode(car->getSceneNode());

	reciever = new EventReciever();
	reciever->setSteer(car->getSceneNode());
	device->setEventReceiver(reciever);

	// create triangle selector for the terrain
	ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);
	selector->drop();

	// create collision response animator and attach it to the camera
	ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, car->getSceneNode(), core::vector3df(10,10,10),
																			core::vector3df(0,-5.0f,0),
																			core::vector3df(0,0,0)
	);

	car->getSceneNode()->addAnimator(anim);
	anim->drop();

	// Skybox
	smgr->addSkyBoxSceneNode(
							 driver->getTexture("data/irrlicht2_up.jpg"),
							 driver->getTexture("data/irrlicht2_dn.jpg"),
							 driver->getTexture("data/irrlicht2_lf.jpg"),
							 driver->getTexture("data/irrlicht2_rt.jpg"),
							 driver->getTexture("data/irrlicht2_ft.jpg"),
							 driver->getTexture("data/irrlicht2_bk.jpg"));

	// Checkpoints
	pair<vector3df, vector3df> cp1(vector3df(3112,393,1234), vector3df(90,90,0));
	addCheckPoint(cp1.first, cp1.second);
	pair<vector3df, vector3df> cp2(vector3df(2531,281,1389), vector3df(90,120,0));
	addCheckPoint(cp2.first, cp2.second);
	addCheckPoint(vector3df(2304,160,1826), vector3df(90,140,0));
	addCheckPoint(vector3df(2132,111,2672), vector3df(90,120,0));
	addCheckPoint(vector3df(1130,415,3313), vector3df(90,75,0));
	addCheckPoint(vector3df(746,471,1753), vector3df(90,0,0));
	addCheckPoint(vector3df(1985,269,1457), vector3df(90,-120,0));
	addCheckPoint(vector3df(2475,146,2868), vector3df(90,-120,0));
	addCheckPoint(vector3df(3707,417,2915), vector3df(90,-60,0));

	// Arrows
	addArrow(vector3df(3012,320,1234), vector3df(100,-55,0));
	addArrow(vector3df(2531,220,1389), vector3df(100,-10,0));
	//addArrow(vector3df(2304,110,1826), vector3df(90,10,0));
	addArrow(vector3df(2232,20,2272), vector3df(90,-20,0));

	// HUD
	info = guienv->addStaticText(L"USE ARROW KEYS TO PLAY",
								 rect<int>(10,10,200,60), true);
	info->setOverrideColor(SColor(255, 255, 255, 255));

	//IGUIStaticText *quick_info = guienv->addStaticText(L"Arrow keys to play\n", rect<int>(10,50,200,50), true);
	//quick_info->setOverrideColor(SColor(255,255,255,255));
	initSound();
}

void Game::initSound()
{
//	FMOD::System *system = NULL;
//	FMOD::Sound *sound = NULL;
//	FMOD::Channel *channel;

//	FMOD::System_Create(&system);
//	system->init(32, FMOD_INIT_NORMAL, 0);
//	system->createSound("data/drumloop.wav", FMOD_SOFTWARE, 0, &sound);
//	system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

void Game::addArrow(const vector3df& position, const vector3df& rotation)
{
	string<char> str = "arrow" + arrow->size();
	IAnimatedMesh *a = smgr->addArrowMesh(str.c_str(), SColor(255,255,255,255), SColor(255,255,255,255));
	IAnimatedMeshSceneNode *arrow_sn = smgr->addAnimatedMeshSceneNode(a);
	arrow_sn->setPosition(position);
	arrow_sn->setScale(vector3df(150,150,0));
	arrow_sn->setRotation(rotation);
	arrow_sn->setMaterialType(EMT_SOLID);
	arrow->push_back(arrow_sn);
}

void Game::addCheckPoint(const vector3df& position, const vector3df& rotation)
{
	string<char> str = "checkpoint" + checkpoint->size();
	IAnimatedMesh *cp1 = smgr->addHillPlaneMesh(str.c_str(), dimension2d<f32>(1000,1000), dimension2d<u32>(1,1));
	IAnimatedMeshSceneNode *c = smgr->addAnimatedMeshSceneNode(cp1);
	c->setRotation(rotation);
	c->setPosition(position);
	//c->setDebugDataVisible(EDS_BBOX);
	checkpoint->push_back(c);

	if (checkpoint->size() == 1)
	{
		first_checkpoint = c;
	}

	c->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	//c->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
}

void Game::update()
{
	switch (current_state)
	{
		case GS_START:
		{
			if (!countdown)
			{
//				countdown = smgr->addBillboardTextSceneNode(guienv->getBuiltInFont(), L"READY!", 0, dimension2d<f32>(200.0f, 200.0f));
//				countdown->setText(L"READY!");
//				countdown->setTextColor(SColor(255,230,100,0));
//				countdown->setPosition(vector3df(3454,500,1256));
				//countdown->setScale(vector3df(10,10,10));
				(*object->begin())->getSceneNode()->setPosition(vector3df(3454,500,1256));
				(*object->begin())->getSceneNode()->setRotation(vector3df(0,260,0));
				nr_of_laps = 0;
				last_time = clock();
			}

			u32 time = clock();
			if ((time - last_time)/CLOCKS_PER_SEC >= 1)
			{
				current_state = GS_COUNTING_DOWN;
			}
			break;
		}
		case GS_COUNTING_DOWN:
		{
			u32 time = clock();

			if ((time - last_time)/CLOCKS_PER_SEC >= 3)
			{
				current_state = GS_RUNNING;
				countdown->remove();
				countdown = NULL;
				last_time = clock();
			}
			else
			{
				stringw str = "";
				str += (3 - (time - last_time)/CLOCKS_PER_SEC);
				countdown->setText(str.c_str());
			}

			break;
		}
		case GS_RUNNING:
		{
			u32 time = clock();
			time = time - last_time;

			// Update the car
			reciever->dispatch();

			stringw str;
			str += "LAP ";
			str += nr_of_laps;
			str += " / ";
			str += max_laps;
			str += "\nTOTAL TIME: ";
			str += time/CLOCKS_PER_SEC;
			str += " seconds";
			str += "\nUSE ARROW KEYS TO PLAY";
			info->setText(str.c_str());

			// Transition
			if (nr_of_laps > max_laps)
			{
				current_state = GS_GAME_OVER;

//				countdown = smgr->addBillboardTextSceneNode(guienv->getBuiltInFont(), L"GOAL!", 0, dimension2d<f32>(150.0f, 150.0f));
//				countdown->setText(L"GOAL!");
//				countdown->setTextColor(SColor(255,230,100,0));
//				vector3df v = (*object->begin())->getSceneNode()->getPosition();
//				v.Y = 500.0f;
//				countdown->setPosition(v);

				IAnimatedMeshSceneNode *cp = checkpoint->back();
				checkpoint->pop_back();
				checkpoint->push_front(cp);
			}

			break;
		}
		case GS_GAME_OVER:
		{
			if (reciever->getNextState() == GS_QUIT)
			{
				device->closeDevice();
			}
			else if (reciever->getNextState() == GS_START)
			{
				current_state = GS_START;
				countdown->remove();
				countdown = NULL;
			}
			else
			{
				stringw str = info->getText();
				str += "\nPRESS ENTER TO CONTINUE\nESC TO QUIT";
				info->setText(str.c_str());
			}

			break;
		}
	}

	reciever->setCurrentState(current_state);

	// Update everything else
	vector<GameObject*>::iterator i = object->begin();
	for (; i != object->end(); i++)
	{
		(*i)->update();
	}

	if (current_state == GS_RUNNING)
	{
		// Car intersects checkpoint
		i = object->begin();
		for (; i != object->end(); i++)
		{
			if ((*i)->getID() == 1)
			{
				// Intersects, front and push_back
				IAnimatedMeshSceneNode *cp = checkpoint->front();
				aabbox3d<f32> box = (*i)->getSceneNode()->getTransformedBoundingBox();
				if (box.intersectsWithBox(cp->getTransformedBoundingBox()))
				{
					checkpoint->pop_front();
					checkpoint->push_back(cp);

					// First?
					if (cp == first_checkpoint)
					{
						nr_of_laps++;
					}
				}
			}
		}
	}
}

void Game::run()
{
current_state = GS_RUNNING;
	// Main loop
	while (device->run())
	{
		update();

		driver->beginScene(true, true, SColor(255,100,101,140));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
	}

	device->drop();
}
