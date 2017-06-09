#include "CController.h"

#include "CCore.h"
#include "ThirdPersonCamera.h"
#include "Randomizer.h"

//#include <math>

namespace island
{
	CController::CController(CCore* core,irr::scene::ISceneNode* islandNode)
	{
		this->core = core;
		this->island = islandNode;

		//get the current camera
		playerCamera = core->getSceneManager()->getActiveCamera();
		playerCamera->setPosition(irr::core::vector3df(750,150,0));
		playerCamera->setTarget(irr::core::vector3df(0,0,0));
		playerCamera->setFarValue(85000.0f);

		//check if the island is given and reset rotation and start the eruption
		if (island)
		{
			island->setRotation(irr::core::vector3df(0,0,0));
			erupt();
		}

		bar1_points  = core->getGUIEnvironment()->addStaticText(L"0",irr::core::rect<irr::s32>(62,22,126,34),false,false,0,121,false);
		bar2_ballons = core->getGUIEnvironment()->addStaticText(L"0",irr::core::rect<irr::s32>(62,42,126,54),false,false,0,122,false);
		bar3_wagons  = core->getGUIEnvironment()->addStaticText(L"0",irr::core::rect<irr::s32>(62,62,126,74),false,false,0,123,false);

		//setup the camera for the user to use
		core->cam = new ThirdPersonCamera(island,core);

		//get the island selector and add it to the scene selector
		islandSelector = core->getSceneManager()->createMetaTriangleSelector();
		islandSelector->addTriangleSelector(island->getTriangleSelector());

		//setup play vars
		nextLevelUpTime[0] = nextLevelUpTime[1] = startTime = lastAnimationTime = core->getRealTime();
		level = 0;
		fireEmitter = 0;

		momentumForceX = momentumForceZ = 0;

		ctrlPressed = false;
		gamelost = false;
	}

	CController::~CController()
	{
		//remove un needed gui elements
		irr::gui::IGUIElement* element = core->getGUIEnvironment()->getRootGUIElement()->getElementFromId(10);
		element->remove();

		bar1_points->remove();
		bar2_ballons->remove();
		bar3_wagons->remove();

		//remove any animator from the island (for example the erupt)
		island->removeAnimators();

		irr::core::map<irr::scene::ISceneNode*,irr::scene::ISceneNode*>::Iterator i = this->ballons.getIterator();
		//remove all the scene nodes before we delete the map node
		while(this->ballons.size() > 0)
		{
			//get the first iterator
			irr::core::map<irr::scene::ISceneNode*,irr::scene::ISceneNode*>::Iterator i = this->ballons.getIterator();
			//get the key and value nodes
			irr::scene::ISceneNode* ballonNode = (irr::scene::ISceneNode*)(*i).getKey();
			irr::scene::ISceneNode* dummyNode = (irr::scene::ISceneNode*)(*i).getValue();
			//remove him from the list
			this->ballons.remove(ballonNode);
			//and delete the nodes
			ballonNode->remove();
			dummyNode->remove();
		}
		//somewhat unnecesarry
		this->ballons.clear();

		//remove all the scene nodes before we delete the list
		while(this->trainParts.getSize() > 0)
		{
			//get first it
			irr::core::list<irr::scene::ISceneNode*>::Iterator d = this->trainParts.begin();
			//fill the node
			irr::scene::ISceneNode* node = (*d);
			//remove him,
			node->remove();
			//and delete the node from the list
			this->trainParts.erase(d);
		}
		//somewhat unnecesarry
		this->trainParts.clear();
	}

	bool CController::OnEvent(const irr::SEvent & event)
	{
		//check which event is called
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			//when the left mouse is pressed the user want to do something with the ballons
			//if shift is pressed he wants to remove otherwise he wants to add a ballon
			if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				if (!ctrlPressed)
				{
					placeBallon();
				}
				else
				{
					removeBallon();
				}
			}
			else
			{
				core->cam->onMouse(event);
			}
		}
		else if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			//if the game is lossed every button restarts the game
			if (gamelost)
			{
				if (event.KeyInput.PressedDown)
				{
					core->restart();
					return true;
				}
			}
			//if the game is still busy we want to detect keys
			else
			{
				//check if ctrl is pressed
				if (event.KeyInput.PressedDown == true && event.KeyInput.Key == irr::KEY_SHIFT)
				{
					ctrlPressed = true;
				}
				else if (event.KeyInput.PressedDown == false && event.KeyInput.Key == irr::KEY_SHIFT)
				{
					ctrlPressed = false;
				}
				else if (event.KeyInput.PressedDown == false && event.KeyInput.Key == irr::KEY_UP)
				{
					irr::SEvent sevent;
					sevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
					sevent.MouseInput.Wheel = 1;
					sevent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
					core->cam->onMouse(sevent);
				}
				else if (event.KeyInput.PressedDown == false && event.KeyInput.Key == irr::KEY_DOWN)
				{
					irr::SEvent sevent;
					sevent.EventType = irr::EET_MOUSE_INPUT_EVENT;
					sevent.MouseInput.Wheel = -1;
					sevent.MouseInput.Event = irr::EMIE_MOUSE_WHEEL;
					core->cam->onMouse(sevent);
				}
				else if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					core->stop();
				}
			}
		}
		return false;
	}

	//calculates the new angles
	void CController::inclinedPlane()
	{
		//get rotation of the island
		irr::core::vector3df rot = island->getRotation();

		if (gamelost)
		{
			return;
		}

		//calculate score
		irr::s32 points = (core->getRealTime() - startTime) / 100;
		bar1_points->setText(((irr::core::stringw)points).c_str());

		//Check if the player lost already!
		if (rot.X > 50 || rot.Z > 50 || rot.X < -50 || rot.Z < -50)
		{
			gamelost = true;

			irr::video::ITexture* lose = core->getVideoDriver()->getTexture("textures/youlose.png");
			irr::s32 height = core->getVideoDriver()->getScreenSize().Height;
			irr::s32 width = core->getVideoDriver()->getScreenSize().Width;
			irr::gui::IGUIImage* image = core->getGUIEnvironment()->addImage(lose,irr::core::position2d<irr::s32>(width/2-256,height/2-128),true,0,10);

			//remove animators. when lost the scene stops
			for(irr::core::list<irr::scene::ISceneNode*>::Iterator s = this->trainParts.begin(); s != this->trainParts.end(); s++)
			{
				(*s)->removeAnimators();
			}

			return;
		}

		irr::core::map<irr::scene::ISceneNode*,irr::scene::ISceneNode*>::Iterator i = this->ballons.getIterator();

		irr::core::vector3df posIsland = island->getPosition();

		irr::s32 timeDif = core->getRealTime() - lastAnimationTime;
		lastAnimationTime = core->getRealTime();

		irr::f32 totalmomentumX, totalmomentumZ;
		totalmomentumX = totalmomentumZ = 0;

		//get powers from the X and Z ax
		for(irr::s32 s = 0; s < this->ballons.size(); s++)
		{

			irr::scene::ISceneNode* node = (irr::scene::ISceneNode*)(i->getValue());
			irr::core::vector3df pos = node->getPosition();

			//posIsland would always be zero... but the rule was opensource, so making the code better here
			irr::f32 differenceX = pos.X - posIsland.X;
			irr::f32 differenceZ = pos.Z - posIsland.Z;

			//formula: M = Fr
			//r = length of the arm   (irr::s32 difference)
			//F = the strength in Newton   (100)
			//M = momentum
			totalmomentumX += (differenceX/2) * 10;
			totalmomentumZ += (differenceZ/2) * 10;

			i++;
		}

		irr::f32 trainmomentumZ, trainmomentumX;
		trainmomentumZ = trainmomentumX = 0;
		//get powers from the trains!
		for(irr::core::list<irr::scene::ISceneNode*>::Iterator s = this->trainParts.begin(); s != this->trainParts.end(); s++)
		{
			irr::core::vector3df pos = (*s)->getPosition();

			//posIsland would always be zero... but the rule was opensource, so making the code better here
			irr::f32 differenceX = pos.X - posIsland.X;
			irr::f32 differenceZ = pos.Z - posIsland.Z;

			//formula: M = Fr
			//r = length of the arm   (irr::s32 difference)
			//F = the strength in Newton   (100)
			//M = momentum
			//Power goes down instead up
			trainmomentumX = (differenceX/2) * 80;
			trainmomentumZ = (differenceZ/2) * 80;

			faceTarget(*s);
		}

		momentumForceX += (totalmomentumX / 500000);
		momentumForceZ += (totalmomentumZ / 500000);

		trainmomentumX /= 50000;
		trainmomentumZ /= 50000;

		rot.Z += ((momentumForceX - trainmomentumX) / 500) * timeDif;
		rot.X -= ((momentumForceZ - trainmomentumZ) / 500) * timeDif;
		island->setRotation(rot);

		//set new ballon positions!
		i = this->ballons.getIterator();

		//get powers from the X and Z ax
		for(irr::s32 s = 0; s < this->ballons.size(); s++)
		{
			irr::scene::ISceneNode* dummy = (irr::scene::ISceneNode*)(i->getValue());
			irr::scene::ISceneNode* ballon = (irr::scene::ISceneNode*)(i->getKey());
			irr::core::vector3df pos = dummy->getAbsolutePosition();
			ballon->setPosition(pos);

			i++;
		}
	}

	void CController::placeBallon()
	{
		if (gamelost)
			return;

		irr::core::position2d<irr::s32> mouseCursor = core->getCursor()->getPosition();

		//will be filled after the collision detection with the ray
		irr::core::vector3df outCollisionPoint;
		irr::core::triangle3df outTriangle;
		irr::scene::ISceneNode* outNode;
		//get the ray from screen coords to-unknown pos
		irr::core::line3d<irr::f32> ray = core->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseCursor,playerCamera);

		//execute collision detection and checks if it has a hit
		if (core->getSceneManager()->getSceneCollisionManager()->getCollisionPoint(ray,islandSelector,outCollisionPoint,outTriangle,outNode))
		{
			irr::core::matrix4 mat;// = island->getRelativeTransformation();
			//mat.setTranslation(outCollisionPoint);
			mat.setRotationDegrees(irr::core::vector3df(360-island->getRotation().X,0,360-island->getRotation().Z));
			mat.rotateVect(outCollisionPoint);
			irr::core::vector3df pos = outCollisionPoint - island->getPosition();

			if (pos.Y < 0)
				return;

			//create the dummy on the surface,
			irr::scene::ISceneNode* dummy = core->getSceneManager()->addEmptySceneNode();
			dummy->setParent(island);
			dummy->setPosition(pos);

			//get random wagon model
			irr::s32 randomNum = irr::core::Randomizer::rand();

			irr::core::stringc file = "models/balloon";
			file += (randomNum % 4) + 1;
			file += ".x";

			//creates the ballon
			irr::scene::IAnimatedMesh* ballonMesh = core->getSceneManager()->getMesh(file.c_str());
			irr::scene::ISceneNode* ballon = core->getSceneManager()->addAnimatedMeshSceneNode(ballonMesh);
			ballon->setPosition(outCollisionPoint);
			ballon->setName("Balloon");

			//add them to the list (map)
			this->ballons.insert(ballon,dummy);
		}

		bar2_ballons->setText(((irr::core::stringw)(this->ballons.size())).c_str());
	}

	void CController::removeBallon()
	{
		//get the ray from screen coords to-unknown pos
		irr::core::position2d<irr::s32> mouseCursor = core->getCursor()->getPosition();

		irr::core::line3d<irr::f32> ray = core->getSceneManager()->getSceneCollisionManager()->getRayFromScreenCoordinates(mouseCursor,playerCamera);

		//get the scenenode from a line, starting from camera to somewhere
		irr::scene::ISceneNode* node = core->getSceneManager()->getSceneCollisionManager()->getSceneNodeFromRayBB(ray);
		if (node)
		{
			//get the key-node from the ballon list and remove them from screen
			irr::core::map<irr::scene::ISceneNode*,irr::scene::ISceneNode*>::Node* mapnode =  this->ballons.find(node);
			if (mapnode)
			{
				irr::scene::ISceneNode* dummy = (irr::scene::ISceneNode*) mapnode->getValue();
				this->ballons.delink(node);
				dummy->remove();
				node->remove();
			}
		}

		bar2_ballons->setText(((irr::core::stringw)(this->ballons.size())).c_str());
	}

	void CController::difficulty()
	{
		//60 means 29 wagons behind a loc. This would be a nice max
		if (level > 60 || gamelost)
			return;

		//get timer values and level
		//setup the train beneath the mountain
		if (nextLevelUpTime[0] < core->getRealTime())
		{
			LevelUpTrain(1);
		}

		//setup the train on the mountain
		if (nextLevelUpTime[1] < core->getRealTime())
		{
			LevelUpTrain(2);
		}
	}

	//Level up train
	void CController::LevelUpTrain(irr::s32 trainNumber)
	{
		//check if we gave an trainnumber
		if (trainNumber < 1)
			return;

		level++;

		//update wagons - locs
		if (level > 2)
			bar3_wagons->setText(((irr::core::stringw)(level - 2)).c_str());

		//setup rail  track parameters
		irr::f32 height = 143.0f;
		irr::f32 radius = 425.0f;
		irr::f32 speed = 0.0004f;
		irr::s32 createWagonPenalty = 230;
		//change them if its an other train
		if (trainNumber == 2)
		{
			height = 268.0f;
			radius = 246.5f;
			speed = 0.0005f;
			createWagonPenalty = 285;
		}

		//fix calculation with
		irr::f32 newTimeToWait = (2 * irr::core::PI) * (1.0f / speed) + createWagonPenalty;
		nextLevelUpTime[trainNumber-1] += newTimeToWait;

		irr::scene::ISceneNodeAnimator* cirleAnimatorTrain = core->getSceneManager()->createFlyCircleAnimator(irr::core::vector3df(0,height,0),radius,speed);

		irr::scene::ISceneNode* trainPart = 0;
		irr::scene::IAnimatedMesh* trainMesh = 0;

		//get train mesh if level < 2 else we need some wagons
		if (level <= 2)
		{
			trainMesh = core->getSceneManager()->getMesh("models/train.X");
		}
		else
		{
			//get random wagon model
			irr::s32 randomNum = irr::core::Randomizer::rand();

			irr::c8* file = "models/wagon.X";
			if (randomNum % 2 == 0)
			{
				file = "models/wagon2.X";
			}

			trainMesh = core->getSceneManager()->getMesh(file);
		}

		if (trainMesh)
		{
			trainPart = core->getSceneManager()->addAnimatedMeshSceneNode(trainMesh,island);
			//trainPart->addAnimator(rotatorAnimatorTrain);
			trainPart->setScale(irr::core::vector3df(0.5f,0.5f,0.5f));
		}
		else
		{
			trainPart = core->getSceneManager()->addCubeSceneNode(10,island);
		}

		trainPart->addAnimator(cirleAnimatorTrain);
		cirleAnimatorTrain->drop();
		trainParts.push_back(trainPart);
	}

	void CController::faceTarget(irr::scene::ISceneNode* node)
	{
		//face the target to the center
		irr::core::vector3df center(0,0,0);
		irr::core::vector3df nodePos = center - node->getPosition();
		irr::core::vector3df myRotation(0,0,0);
		myRotation.Y = atan(nodePos.Z/nodePos.X) * (180.0f / irr::core::PI);

		//fix rotation of the default model rotation
		if((center.X - node->getPosition().X) > 0)
		{
			myRotation.Y = 90 - myRotation.Y;
		} else if((center.X - node->getPosition().X) < 0) {
			myRotation.Y = -90 - myRotation.Y;
		}
		myRotation.Y -= 90;

		//set rotation to the node
		node->setRotation(myRotation);

	}

	void CController::erupt()
	{
		if (!fireEmitter)
			return;

		//create smoke particles
		fireEmitter = core->getSceneManager()->addParticleSystemSceneNode(false,island,-1,irr::core::vector3df(0,150,0));

		irr::scene::IParticleEmitter* em = fireEmitter->createBoxEmitter(irr::core::aabbox3d<irr::f32>(-22,0,-22,22,1,22), // emitter size
				irr::core::vector3df(0.0f,0.06f,0.0f),	// initial direction
				40,60,									// emit rate
				irr::video::SColor(0,128,128,128),		// darkest color
				irr::video::SColor(0,128,128,128),		// brightest color
				3200,7000,20,							// min and max age, angle
				irr::core::dimension2df(40.f,40.f),		// min size
				irr::core::dimension2df(70.f,70.f));	// max size

		fireEmitter->setEmitter(em);
		em->drop();

		irr::scene::IParticleAffector* paf = fireEmitter->createFadeOutParticleAffector();
		fireEmitter->addAffector(paf);
		paf->drop();

		//setup smoke particle material
		fireEmitter->setScale(irr::core::vector3df(2,2,2));
		fireEmitter->setMaterialFlag(irr::video::EMF_LIGHTING, false);
		fireEmitter->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);
		fireEmitter->setMaterialTexture(0, core->getVideoDriver()->getTexture("textures/smoke.bmp"));
		fireEmitter->getMaterial(0).DiffuseColor.set(255,128,128,128);
		fireEmitter->getMaterial(0).AmbientColor.set(255,255,255,255);
		fireEmitter->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
	}
}
