#ifndef RUNNINGSTATE_H_
#define RUNNINGSTATE_H_

#include "../scene/grass/CGrassGeneratorNode.h"
#include "../scene/grass/CWindGenerator.h"
#include "../scene/TeleporterSceneNode.h"
#include "../scene/Forest.h"
#include "../scene/lensflare/CLensFlareSceneNode.h"
#include "../gui/Compass.h"
#include "../Flock.h"

class RunningState : public State<Demo, irr::SEvent>
{

public:

	RunningState();

	virtual ~RunningState();

	virtual void onEnter(Demo* const demo);

	virtual void onLeave(Demo* const demo);

	virtual const bool onEvent(Demo* const demo, const irr::SEvent& event);

	virtual void onUpdate(Demo* const demo);

private:

	Flock* flock;

	Compass* pgCompass;

	irr::gui::IGUIStaticText* infoText;

	irr::scene::ISceneNodeAnimatorCollisionResponse* anim;

	irr::scene::ISceneNode* skybox;

	irr::scene::CLensFlareSceneNode* flare;

	irr::scene::ITerrainSceneNode* terrain;

	Forest* forest;

	irr::scene::IAnimatedMesh* waterMesh;

	irr::scene::ISceneNode* waterNode;

	irr::scene::IVolumeLightSceneNode* cursor;

	irr::scene::IBillboardSceneNode* crosshair;

	irr::scene::TeleporterSceneNode* teleporter;

	irr::scene::CGrassGeneratorNode* grassGeneratorNode;

	irr::scene::IWindGenerator* wind;

	///Array of keys pressed.
	bool keys[irr::KEY_KEY_CODES_COUNT];

	irr::core::position2di mousePos;

	bool rightMouseButton;

	bool pauseFlock;

	bool freezeTarget;
};

RunningState::RunningState() :
	flock(0),
	pgCompass(0),
	infoText(0),
	anim(0), skybox(0), flare(0), terrain(0), forest(0), waterMesh(0), waterNode(0),
	cursor(0), crosshair(0), teleporter(0), grassGeneratorNode(0), wind(0),
	mousePos(0, 0), rightMouseButton(false),
	pauseFlock(false), freezeTarget(false)
	{

	}

RunningState::~RunningState()
{
	if (anim)
		anim->drop();

	if (pgCompass)
	{
		pgCompass->remove();
		pgCompass->drop();
	}

	if (this->infoText)
	{
		this->infoText->remove();
		this->infoText->drop();
	}

	if (this->skybox)
	{
		this->skybox->remove();
		this->skybox->drop();
	}

	if (this->flare)
	{
		this->flare->remove();
		this->flare->drop();
	}

	if (this->terrain)
	{
		this->terrain->remove();
		this->terrain->drop();
	}

	if (this->forest)
		delete this->forest;

	if (this->waterMesh)
		this->waterMesh->drop();

	if (this->waterNode)
	{
		this->waterNode->remove();
		this->waterNode->drop();
	}

	if (this->cursor)
	{
		this->cursor->remove();
		this->cursor->drop();
	}

	if (this->crosshair)
	{
		this->crosshair->remove();
		this->crosshair->drop();
	}

	if (this->teleporter)
	{
		this->teleporter->drop();
	}

	if (grassGeneratorNode)
	{
		grassGeneratorNode->remove();
		grassGeneratorNode->drop();
	}


	delete wind;

	delete this->flock;
}

void RunningState::onEnter(Demo* const demo)
{
	//clear key array and mouse bool
	memset(this->keys, 0, irr::KEY_KEY_CODES_COUNT);

	// change gui transparency
	irr::gui::IGUISkin* currentSkin = demo->getGuiEnvironment()->getSkin();

	irr::u32 c;
	for (c = 0; c < irr::gui::EGDC_COUNT; ++c)
	{
		irr::video::SColor col = currentSkin->getColor(irr::gui::EGUI_DEFAULT_COLOR(c));
		col.setAlpha(demo->getConfiguration()->getGuiTransparency());
		currentSkin->setColor(irr::gui::EGUI_DEFAULT_COLOR(c), col);
	}

	//add info text
	this->infoText = demo->getGuiEnvironment()->addStaticText(
			L"", irr::core::rect<irr::s32>(SX(20), SY(20), SX(450), SY(520)), false, true, 0, -1, true);
	this->infoText->setTextAlignment(irr::gui::EGUIA_UPPERLEFT, irr::gui::EGUIA_CENTER);
	this->infoText->setOverrideColor(irr::video::SColor(255, 0, 255, 255));
	this->infoText->grab();

	// add a compass
	const irr::core::dimension2di compassSize(128, 128);

	irr::core::rect<irr::s32> gCompassRect;
	gCompassRect.UpperLeftCorner.X = SX(880);
	gCompassRect.UpperLeftCorner.Y = SY(10);
	gCompassRect.LowerRightCorner.X = SX(880) + compassSize.Width;
	gCompassRect.LowerRightCorner.Y = SY(10) + compassSize.Height;

	this->pgCompass = new Compass(gCompassRect, demo->getGuiEnvironment(), demo->getGuiEnvironment()->getRootGUIElement());
	this->pgCompass->SetCompassBodyTexture(demo->getVideoDriver()->getTexture("media/images/compass/compass_body.png"));
	this->pgCompass->SetCompassNeedleTexture(demo->getVideoDriver()->getTexture("media/images/compass/compass_needle.png"));


	irr::scene::ISceneManager* smgr = demo->getSceneManager();

	//add camera
	irr::SKeyMap keyMap[9];
	keyMap[0].Action = irr::EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = irr::KEY_UP;
	keyMap[1].Action = irr::EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = irr::KEY_KEY_W;
	keyMap[2].Action = irr::EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = irr::KEY_DOWN;
	keyMap[3].Action = irr::EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = irr::KEY_KEY_S;
	keyMap[4].Action = irr::EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = irr::KEY_LEFT;
	keyMap[5].Action = irr::EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = irr::KEY_KEY_A;
	keyMap[6].Action = irr::EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = irr::KEY_RIGHT;
	keyMap[7].Action = irr::EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = irr::KEY_KEY_D;
	keyMap[8].Action = irr::EKA_JUMP_UP;
	keyMap[8].KeyCode = irr::KEY_SPACE;

	const irr::f32 moveSpeed = demo->getConfiguration()->getCameraSpeed();
	demo->setCamera(smgr->addCameraSceneNodeFPS(0, 100.0f, moveSpeed, -1, keyMap, 9, true, 30.f));
	demo->getCamera()->setPosition(irr::core::vector3df(830.0f, 300.0f, 340.0f));
	demo->getCamera()->setTarget(irr::core::vector3df(1870.0f, 290.0f, 3300.0f));
	demo->getCamera()->setFarValue(12000.0f);


	//create skybox
	demo->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	this->skybox = smgr->addSkyBoxSceneNode(
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_up.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_dn.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_lf.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_rt.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_ft.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_bk.png"));
	demo->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
	this->skybox->grab();

	irr::scene::ISceneNodeAnimator* const skyAnim = smgr->createRotationAnimator(irr::core::vector3df(0.0f, 0.005f, 0.0f));
	this->skybox->addAnimator(skyAnim);
	skyAnim->drop();


	//add terrain scene node
	this->terrain = smgr->addTerrainSceneNode(
			"media/images/terrain/terrain-heightmap.bmp",
			0,											// parent node
			-1,											// node id
			irr::core::vector3df(0.0f, 0.0f, 0.0f),		// position
			irr::core::vector3df(0.0f, 0.0f, 0.0f),		// rotation
			irr::core::vector3df(40.0f, 4.0f, 40.0f),	// scale
			irr::video::SColor(255, 255, 255, 255),		// vertexColor
			5,											// maxLOD
			irr::scene::ETPS_17,						// patchSize
			4);											// smoothFactor
	this->terrain->grab();
	//this->terrain->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	this->terrain->setMaterialTexture(0, demo->getVideoDriver()->getTexture("media/images/terrain/terrain-texture.jpg"));
	this->terrain->setMaterialTexture(1, demo->getVideoDriver()->getTexture("media/images/terrain/detailmap3.jpg"));
	this->terrain->setMaterialType(irr::video::EMT_DETAIL_MAP);
	this->terrain->scaleTexture(1.0f, 20.0f);
	this->terrain->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	//	this->terrain->setMaterialFlag(irr::video::EMF_WIREFRAME, true);

	// create triangle selector for the terrain
	irr::scene::ITriangleSelector* const sel = smgr->createTerrainTriangleSelector(terrain, 0);
	demo->addSelector(sel);
	//	this->terrain->setTriangleSelector(sel);
	sel->drop();

	// create collision response animator and attach it to the camera
	this->anim = smgr->createCollisionResponseAnimator(
			demo->getSelector(), demo->getCamera(), irr::core::vector3df(60.0f, 100.0f, 60.0f),
			irr::core::vector3df(0.0f, -9.81f, 0.0f), //gravity
			irr::core::vector3df(0.0f, 50.0f, 0.0f));
	demo->getCamera()->addAnimator(this->anim);
	//don't grab it because we need to drop it later anyway


	//create lens flare node
	demo->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

	this->flare = new irr::scene::CLensFlareSceneNode(smgr->getRootSceneNode(), smgr);
	this->flare->setPosition(irr::core::vector3df(terrain->getTerrainCenter().X, 5500, terrain->getTerrainCenter().Z));
	// attach the flare images texture, needs to be n quad sized textures from left to right
	this->flare->getMaterial(0).setTexture(0, demo->getVideoDriver()->getTexture("media/images/flares.jpg"));
	//this->flare->grab();

	//add some kind of cursor
	this->cursor = smgr->addVolumeLightSceneNode(
			0, -1,
			32,                              // Subdivisions on U axis
			32,                              // Subdivisions on V axis
			irr::video::SColor(0, 255, 255, 255), // foot color
			irr::video::SColor(0, 0, 0, 0));      // tail color
	this->cursor->grab();
	this->cursor->setScale(irr::core::vector3df(56.0f, 56.0f, 56.0f));
	this->cursor->setMaterialFlag(irr::video::EMF_LIGHTING, false);

	// load textures for animation
	irr::core::array<irr::video::ITexture*> textures;
	for (irr::u32 g = 7; g > 0; --g)
	{
		irr::core::stringc tmp = "media/images/portal/portal";
		tmp += g;
		tmp += ".bmp";

		textures.push_back(demo->getVideoDriver()->getTexture(tmp.c_str()));
	}

	// create texture animator
	irr::scene::ISceneNodeAnimator* const glow = smgr->createTextureAnimator(textures, 100);
	this->cursor->addAnimator(glow);
	glow->drop();


	this->crosshair = smgr->addBillboardSceneNode(
			demo->getCamera(), irr::core::dimension2df(4.0f, 4.0f), irr::core::vector3df(0.0f, 0.0f, 50.0f));
	this->crosshair->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	this->crosshair->setMaterialTexture(0, demo->getVideoDriver()->getTexture("media/images/crosshair/cross.bmp"));
	this->crosshair->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	this->crosshair->setMaterialFlag(irr::video::EMF_ZBUFFER, false);
	this->crosshair->grab();


	// add lights
	irr::scene::ILightSceneNode* light1 = smgr->addLightSceneNode(
			0,
			irr::core::vector3df(terrain->getTerrainCenter().X, 2500, terrain->getTerrainCenter().Z),
			irr::video::SColorf(5.0f, 5.0f, 5.0f, 5.0f), 800.0f);
	light1->setRadius(2000);

		smgr->setAmbientLight(irr::video::SColorf(0.2f, 0.2f, 0.2f));

	// create wind
	const irr::f32 strength = 30.0f;
	const irr::f32 regularity = 3.0f;
	this->wind = irr::scene::createWindGenerator(strength, regularity);

	//add grass
	const irr::video::IImage* const heightMap = demo->getVideoDriver()->createImageFromFile("media/images/terrain/terrain-heightmap.bmp");
	const irr::video::IImage* const textureMap = demo->getVideoDriver()->createImageFromFile("media/images/terrain/terrain-grasscol.bmp");
	const irr::video::IImage* const grassMap = demo->getVideoDriver()->createImageFromFile("media/images/terrain/terrain-grassmap.png");

	this->grassGeneratorNode = new irr::scene::CGrassGeneratorNode(demo->getDevice());
	this->grassGeneratorNode->addGrassToTerrain(demo, wind, terrain, heightMap, textureMap, grassMap);
	this->grassGeneratorNode->increaseDrawDistance(10000.0f);
	this->grassGeneratorNode->setVisible(false);

	heightMap->drop();
	textureMap->drop();
	grassMap->drop();


	//create flock
	const irr::core::aabbox3df& tb = terrain->getBoundingBox();
	const irr::f32 borders[4] = { 0.0f, tb.getExtent().X, 0.0f, tb.getExtent().Z }; //Xmin, Xmax, Zmin, Zmax

	this->flock = new Flock(demo, 20, irr::core::vector3df(1870.0f, 290.0f, 3300.0f), borders);


	//add teleporter

	//bounds of placeA
	irr::core::vector3df minEdgeA(1100.0f, 100.0f, 2500.0f);
	irr::core::vector3df maxEdgeA(1300.0f, 300.0f, 2700.0f);
	irr::core::aabbox3d<irr::f32> placeA(minEdgeA, maxEdgeA);

	//bounds of placeB
	irr::core::vector3df minEdgeB(4000.0f, 1000.0f, 6500.0f);
	irr::core::vector3df maxEdgeB(4200.0f, 1200.0f, 6700.0f);
	irr::core::aabbox3d<irr::f32> placeB(minEdgeB, maxEdgeB);

	//true if teleport from B to A is possible, too
	const bool viceversa = true;

	this->teleporter = new irr::scene::TeleporterSceneNode(demo, placeA, placeB, viceversa);
	//this->teleporter->setDebugDataVisible(irr::scene::EDS_BBOX); //show bounds of placaA and placeB

	//add scene nodes which are effected by the teleporter
	this->teleporter->addNodeToWatchList(demo->getCamera());


	const irr::core::array<irr::scene::BoidSceneNode*>& boids = this->flock->getBoids();
	for(irr::u32 i = 0; i < boids.size(); ++i)
		this->teleporter->addNodeToWatchList((irr::scene::ISceneNode*)boids[i]);



	//add trees
	irr::core::array<irr::core::aabbox3df> exceptions; //places where not to place any trees

	//player start
	irr::core::aabbox3df b(1, -1500, 1, 5000, 1000, 5000);
	exceptions.push_back(b);
	//teleporters
	exceptions.push_back(placeA);
	exceptions.push_back(placeB);

	this->forest = new Forest(demo->getDevice(), terrain, exceptions);
	this->forest->randomlyPlaceVegetation(10);

	//add water

	this->waterMesh = smgr->addHillPlaneMesh("myHill",
			irr::core::dimension2d<irr::f32>(40.0f, 40.0f), //tile size
			irr::core::dimension2d<irr::u32>(25, 30), //tile count
			0, 0,
			irr::core::dimension2d<irr::f32>(0.0f, 0.0f),
			irr::core::dimension2d<irr::f32>(10.0f, 10.0f));
	this->waterMesh->grab();

	this->waterNode = smgr->addWaterSurfaceSceneNode(this->waterMesh->getMesh(0), 3.0f, 300.0f, 30.0f);
	this->waterNode->setPosition(irr::core::vector3df(9100.0f, 40.0f, 2600.0f));
	this->waterNode->setMaterialTexture(0, demo->getVideoDriver()->getTexture("media/images/water/stones.jpg"));
	this->waterNode->setMaterialTexture(1, demo->getVideoDriver()->getTexture("media/images/water/water.jpg"));
	this->waterNode->setMaterialType(irr::video::EMT_REFLECTION_2_LAYER);
	this->waterNode->grab();

#ifdef _SOUND
	if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
	{
		demo->getSoundEngine()->play2D("media/sounds/wind.ogg", true);
	}
#endif
}

void RunningState::onLeave(Demo* const demo)
{
	this->anim->drop();
	this->anim = 0;

	this->pgCompass->remove();
	this->pgCompass->drop();
	this->pgCompass = 0;

	this->infoText->remove();
	this->infoText->drop();
	this->infoText = 0;

	this->skybox->remove();
	this->skybox->drop();
	this->skybox = 0;

	this->flare->remove();
	this->flare->drop();
	this->flare = 0;

	this->terrain->remove();
	this->terrain->drop();
	this->terrain = 0;

	delete this->forest;
	this->forest = 0;

	this->waterMesh->drop();
	this->waterMesh = 0;

	this->waterNode->remove();
	this->waterNode->drop();
	this->waterNode = 0;

	this->cursor->remove();
	this->cursor->drop();
	this->cursor = 0;

	this->crosshair->remove();
	this->crosshair->drop();
	this->crosshair = 0;

	this->teleporter->remove();
	this->teleporter->drop();
	this->teleporter = 0;

	wind->drop();
	this->wind = 0;

	this->grassGeneratorNode->remove();
	this->grassGeneratorNode->drop();
	this->grassGeneratorNode = 0;

	delete this->flock;
	this->flock = 0;

	demo->removeSelectors();
	demo->getCamera()->remove();
	demo->setCamera(0);
}

const bool RunningState::onEvent(Demo* const demo, const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if (!event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_ESCAPE: demo->getDevice()->closeDevice(); return true;
			case irr::KEY_F1: this->terrain->setMaterialFlag(irr::video::EMF_WIREFRAME, !this->terrain->getMaterial(0).Wireframe); return true;
			case irr::KEY_F2: this->grassGeneratorNode->setVisible(!this->grassGeneratorNode->isVisible()); return true;
			case irr::KEY_F3:
				this->forest->randomlyPlaceVegetation(1);
#ifdef _SOUND
				if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
					demo->getSoundEngine()->play2D("media/sounds/button.wav");
#endif
				return true;

			case irr::KEY_F4:
				this->forest->removeTree();
#ifdef _SOUND
				if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
					demo->getSoundEngine()->play2D("media/sounds/button.wav");
#endif
				return true;

#ifdef _SOUND
			case irr::KEY_F5:
			{
				//toggle sounds
				const bool sound = demo->getConfiguration()->isSoundEnabled();
				demo->getConfiguration()->setSoundEnabled(!sound);

				if (sound)
					demo->getSoundEngine()->stopAllSounds();
				else
				{
					//start music again
					if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
					{
						demo->getSoundEngine()->play2D("media/sounds/runwithme.ogg", true);
						demo->getSoundEngine()->play2D("media/sounds/wind.ogg", true);
					}
				}

				return true;
			}
#endif

			case irr::KEY_F6: demo->makeScreenshot(); return true;


			case irr::KEY_PAUSE:
			case irr::KEY_KEY_P: this->pauseFlock = !this->pauseFlock; return true;
			case irr::KEY_PLUS:
			{
				irr::scene::BoidSceneNode* const boid = this->flock->addBoid();
				this->teleporter->addNodeToWatchList((irr::scene::ISceneNode*)boid);

#ifdef _SOUND
				if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
					demo->getSoundEngine()->play2D("media/sounds/button.wav");
#endif
				return true;
			}
			case irr::KEY_MINUS:
			{
				irr::scene::BoidSceneNode* const boid = this->flock->getBoids().getLast();

				this->teleporter->removeNodeFromWatchList((irr::scene::ISceneNode*)boid);
				this->flock->removeBoid(boid);

#ifdef _SOUND
				if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
					demo->getSoundEngine()->play2D("media/sounds/button.wav");
#endif
				return true;
			}

			case irr::KEY_KEY_H: this->infoText->setVisible(!this->infoText->isVisible()); return true;

			default: ;
			}
		}

		keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	else if(irr::EET_MOUSE_INPUT_EVENT == event.EventType)
	{

		if (irr::EMIE_MOUSE_MOVED == event.MouseInput.Event)
		{
			this->mousePos.X = event.MouseInput.X;
			this->mousePos.Y = event.MouseInput.Y;

			return false; //if we return true here, our fps camera doesn't work anymore
		}

		//check right mouse button
		if (irr::EMIE_RMOUSE_PRESSED_DOWN == event.MouseInput.Event)
		{
			this->rightMouseButton = true;

			return true;
		}
		else if (irr::EMIE_RMOUSE_LEFT_UP == event.MouseInput.Event)
		{
			this->rightMouseButton = false;

			return true;
		}


		//check middle mouse button
		else if (irr::EMIE_MMOUSE_LEFT_UP == event.MouseInput.Event)
		{
			this->freezeTarget = !this->freezeTarget;

			return true;
		}

		//check mouse wheel
		else if (irr::EMIE_MOUSE_WHEEL == event.MouseInput.Event)
		{
			//zoom camera
			irr::scene::ICameraSceneNode* const camera = demo->getCamera();
			irr::f32 newFOV = camera->getFOV();

			if (event.MouseInput.Wheel < 0)
				newFOV = irr::core::min_(newFOV + irr::core::DEGTORAD, irr::core::PI*0.5f);
			else
				newFOV = irr::core::max_(newFOV - irr::core::DEGTORAD, irr::core::PI*0.0125f);

			camera->setFOV(newFOV);

			return true;
		}
	}

	return false;
}

void RunningState::onUpdate(Demo* const demo)
{
	//update flock target
	if (!this->freezeTarget)
	{
		irr::core::vector3df intersection;
		irr::core::triangle3df tri;
		irr::scene::ISceneCollisionManager* const collmgr = demo->getSceneManager()->getSceneCollisionManager();

		const irr::core::line3d<irr::f32>& line = collmgr->getRayFromScreenCoordinates(this->mousePos, demo->getCamera());

		if (collmgr->getCollisionPoint(line, demo->getSelector(), intersection, tri))
		{
			this->cursor->setPosition(intersection);
			this->flock->setTarget(intersection);
		}
	}

	//update flock
	if (!this->pauseFlock)
	{
		//update flock
		this->flock->update(demo->getElapsed(), this->rightMouseButton);
	}

	//check 'player' position and teleport him back if his Y position is -200.0
	if (demo->getCamera()->getPosition().Y < - 200.0f)
	{
		demo->getCamera()->setPosition(irr::core::vector3df(830.0f, 300.0f, 340.0f));
		demo->getCamera()->setTarget(irr::core::vector3df(1870.0f, 290.0f, 3300.0f));

		//in order to teleport a scene node with an attached collision response animator,
		//we need to update its target
		//this->anim->setTargetNode(demo->getCamera());
	}

	//update compass
	irr::core::vector3df vec(0.0f, 0.0f, 1.0f);
	demo->getCamera()->getAbsoluteTransformation().rotateVect(vec);
	this->pgCompass->SetCompassHeading(vec.getHorizontalAngle().Y);

#ifdef _SOUND
	if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
		demo->getSoundEngine()->setListenerPosition(
				demo->getCamera()->getPosition(), demo->getCamera()->getTarget());
#endif

	//update info text
	if (this->infoText->isVisible())
	{
		irr::core::stringw t(
				L"                                                  Help\n\n"
				" Camera:\n"
				"   WASD / Arrows - Move\n"
				"   SPACE - Jump\n"
				"   Wheel - Zoom\n\n"
				" Flock:\n"
				"   Flock rules - Edit config.xml\n"
				"   Point mouse - Set flock target\n"
				"   Middle mouse button - Freeze flock target\n"
				"   Right mouse button - Scatter flock\n"
				"   - / + - Number of boids: ");
		t.append(irr::core::stringw(this->flock->getBoids().size()));
		t.append(L"\n   P - ");	t.append(this->pauseFlock ? L"Unpause" : L"Pause");	t.append(L" flock\n");
		t.append(L"   Target: X:");
		t.append(irr::core::stringw(this->flock->getTarget().X));
		t.append(L" | Y:");
		t.append(irr::core::stringw(this->flock->getTarget().Y));
		t.append(L" | Z:");
		t.append(irr::core::stringw(this->flock->getTarget().Z));

		t.append(L"\n\n Demo:\n"
				"   F1 - Terrain wireframe : ");
		t.append(this->terrain->getMaterial(0).Wireframe ? L"(On)" : L"(Off)");
		t.append(L"\n   F2 - Toggle grass: ");
		t.append(irr::core::stringw(this->grassGeneratorNode->isVisible()? L"(On)" : L"(Off)"));
		t.append(L"\n   F3 / F4 - Number of trees: ");
		t.append(irr::core::stringw(this->forest->getNumberOfTrees()));
		t.append(L"\n   F5 - Toggle sounds: ");
#ifdef _SOUND
		t.append(demo->getConfiguration()->isSoundEnabled() ? L"(On)" : L"(Off)");
#else
		t.append(L"(Sounds disabled)");
#endif
		t.append(L"\n   F6 - Take screenshot\n");
		t.append(L"\n   H - Hide help\n"
				"   ESC - Exit\n\n"
				" Player:\n"
				"   Position: X:");
		t.append(irr::core::stringw(demo->getCamera()->getAbsolutePosition().X));
		t.append(L" | Y:");
		t.append(irr::core::stringw(demo->getCamera()->getAbsolutePosition().Y));
		t.append(L" | Z:");
		t.append(irr::core::stringw(demo->getCamera()->getAbsolutePosition().Z));

		this->infoText->setText(t.c_str());
	}
}

#endif /*RUNNINGSTATE_H_*/
