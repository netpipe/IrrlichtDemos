/*
  DecalSystem Example

  Copyright (C) 2011 Thijs Ferket (RdR)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <irrlicht.h>
#include "driverChoice.h"
#include "DecalManager.h"

using namespace irr;

class MyEventReceiver: public IEventReceiver {
public:

	MyEventReceiver(DecalManager* decalManager, scene::ITerrainSceneNode* terrain, irr::scene::ISceneNode* sphere) {
		this->decalManager = decalManager;
		this->terrain = terrain;
		this->sphere = sphere;
		smgr = terrain->getSceneManager();
		size = 4;
		lifetime = 0;
		distance = 0;
		rotation = 0;
		showWireFrame = false;
		useCameraNormal = false;
		useParent = false;
	}

	bool OnEvent(const SEvent &event) {
		if (event.EventType == EET_MOUSE_INPUT_EVENT) {
			// Create decal
			if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN) {
				scene::ISceneNode* outNode; // Change to 'const scene::ISceneNode* outNode;' for Irrlicht 1.7.2
				core::line3d<f32> line;
				core::vector3df collisionPoint;
				core::triangle3df triangle;
				line.start = smgr->getActiveCamera()->getAbsolutePosition();
				line.end = smgr->getActiveCamera()->getTarget();
				scene::ITriangleSelector* selector = decalManager->getMetaTriangleSelector();

				if (smgr->getSceneCollisionManager()->getCollisionPoint(line, selector, collisionPoint, triangle, outNode)) {
					core::vector3df normal = triangle.getNormal();
					if (useCameraNormal) {
						normal = line.start - line.end;
					}
					irr::scene::ISceneNode* parent = 0;
					if(useParent) {
						parent = sphere;
					}

					decalManager->addDecal("media/decal.png", collisionPoint, irr::core::vector3df(size, size, size), normal, rotation, parent, lifetime, distance);
				}
				return true;
			}
			// Rotate
			if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN) {
				rotation = (rand() % 3600) / 10.0f;
				return true;
			}

		} else if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {

			// switch terrain wireframe
			case irr::KEY_KEY_T: {
				terrain->getMaterial(0).Wireframe = !terrain->getMaterial(0).Wireframe;
				sphere->getMaterial(0).Wireframe = !sphere->getMaterial(0).Wireframe;
				return true;
			}
				// Toggle decal wireframe
			case irr::KEY_KEY_W: {
				irr::core::array<irr::scene::ISceneNode*> nodes;
				smgr->getSceneNodesFromType((irr::scene::ESCENE_NODE_TYPE) irr::scene::DECAL_SCENE_NODE_ID, nodes, smgr->getRootSceneNode());

				showWireFrame = !showWireFrame;
				for (irr::u32 i = 0; i < nodes.size(); i++) {
					nodes[i]->getMaterial(0).Wireframe = showWireFrame;
				}

				return true;
			}

				// Random rotation
			case irr::KEY_KEY_R: {
				rotation = (rand() % 3600) / 10.0f;
				return true;
			}

				// Combine permanent decals into 1 decal
			case irr::KEY_KEY_C: {
				decalManager->combineDecals(decalManager->getPermenantDecals());
				return true;
			}

				// Increase size of decal
			case irr::KEY_KEY_S: {
				size++;
				if (size > 20) {
					size = 20;
				}
				return true;
			}
				// Decrease size of decal
			case irr::KEY_KEY_D: {
				size--;
				if (size < 1) {
					size = 1;
				}
				return true;
			}

				// Increase lifetime of decal
			case irr::KEY_KEY_O: {
				lifetime += 5;
				return true;
			}
				// Decrease lifetime of decal
			case irr::KEY_KEY_L: {
				lifetime -= 5;
				if (lifetime < 0) {
					lifetime = 0;
				}
				return true;
			}

				// Increase distance of decal
			case irr::KEY_KEY_I: {
				distance += 50;
				return true;
			}
				// Decrease distance of decal
			case irr::KEY_KEY_K: {
				distance -= 50;
				if (distance < 0) {
					distance = 0;
				}
				return true;
			}
			// Toggle use camera direction as normal
			case irr::KEY_KEY_N: {
				useCameraNormal = !useCameraNormal;
				return true;
			}

			// Toggle use spehre as parent
			case irr::KEY_KEY_P: {
				useParent = !useParent;
				return true;
			}

			default:
				break;
			}
		}

		return false;
	}

public:
	DecalManager* decalManager;
	scene::ITerrainSceneNode* terrain;
	irr::scene::ISceneManager* smgr;
	irr::scene::ISceneNode* sphere;
	irr::f32 size;
	irr::f32 lifetime;
	irr::f32 rotation;
	irr::f32 distance;
	bool showWireFrame;
	bool useCameraNormal;
	bool useParent;
};

int main() {
	// ask user for driver
	video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	if (driverType == video::EDT_COUNT)
		return 1;

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = core::dimension2d<u32>(1024, 768);
	IrrlichtDevice* device = createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	env->getSkin()->setFont(env->getFont("media/fontlucida.png"));

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	// Add camera
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	camera->setPosition(core::vector3df(820, 100, 850));
	camera->setTarget(core::vector3df(1530, 708, 1296));
	camera->setFarValue(42000.0f);

	//	camera->setPosition(core::vector3df(50,50,-60));
	//	camera->setTarget(core::vector3df(-70,30,-60));

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	// Add terrain scene node
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("media/terrain-heightmap.bmp", 0, // parent node
			-1, // node id
			core::vector3df(0.f, 0.f, 0.f), // position
			core::vector3df(0.f, 0.f, 0.f), // rotation
			core::vector3df(7.f, 1.f, 7.f), // scale
			video::SColor(255, 255, 255, 255), // vertexColor
			5, // maxLOD
			scene::ETPS_17, // patchSize
			4 // smoothFactor
			);

	terrain->setMaterialFlag(video::EMF_LIGHTING, false);

	terrain->setMaterialTexture(0, driver->getTexture("media/terrain-texture.jpg"));
	terrain->setMaterialTexture(1, driver->getTexture("media/detailmap3.jpg"));

	terrain->setMaterialType(video::EMT_DETAIL_MAP);

	terrain->scaleTexture(1.0f, 100.0f);

	// create triangle selector for the terrain
	scene::ITriangleSelector* selector = smgr->createTerrainTriangleSelector(terrain, 0);
	terrain->setTriangleSelector(selector);

	// create collision response animator and attach it to the camera
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(1, 1, 1), core::vector3df(0, 0, 0));
	selector->drop();
	camera->addAnimator(anim);
	anim->drop();

	// sphere
	scene::IMeshSceneNode* sphere = smgr->addSphereSceneNode(25.f, 100);
	sphere->setPosition(core::vector3df(800, 100, 800));
	sphere->setScale(core::vector3df(2, 2, 2));

	// Create decal manager
	DecalManager* decalManager = new DecalManager(smgr);
	decalManager->setTerrain(terrain);
	decalManager->addMesh(sphere);

	MyEventReceiver* receiver = new MyEventReceiver(decalManager, terrain, sphere);
	device->setEventReceiver(receiver);

	irr::video::SColor backgroundColor(10, 255, 255, 255);
	irr::video::SColor textColor(255, 255, 255, 255);

	int lastFPS = -1;

	irr::core::vector3df position = irr::core::vector3df(800, 300, 800);
irr::core::vector3df dimension = irr::core::vector3df(1, 1, 1);
irr::core::vector3df normal = irr::core::vector3df(1, 1, 0);
irr::f32 textureRotation = 0;
irr::scene::ISceneNode* parent = 0;
irr::f32 lifeTime = 110;
irr::f32 distance = 110;

				//	decalManager->addDecal("media/decal.png", collisionPoint, irr::core::vector3df(size, size, size), normal, rotation, parent, lifetime, distance);
decalManager->addDecal("media/decal.png", position, dimension, normal, textureRotation, parent, lifeTime, distance);
	while (device->run()) {
		if (device->isWindowActive()) {
			driver->beginScene(true, true, 0);

			smgr->drawAll();

			env->getVideoDriver()->draw2DRectangle(backgroundColor, irr::core::rect<s32>(0, 0, 280, 220));

			irr::core::stringw str = "SIZE: ";
			str += (irr::u32) receiver->size;
			str += "   [ S & D ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 10, 200, 20), textColor, false, true);

			str = "LIFETIME: ";
			str += (irr::u32) receiver->lifetime;
			str += "   [ O & L ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 30, 200, 50), textColor, false, true);

			str = "DISTANCE: ";
			str += (irr::u32) receiver->distance;
			str += "   [ I & K ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 60, 200, 80), textColor, false, true);

			str = "ROTATION: ";
			str += (irr::u32) receiver->rotation;
			str += "   [ R or R Mouse Button ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 90, 200, 110), textColor, false, true);

			str = "COMBINE [C]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 120, 200, 140), textColor, false, true);

			str = "DECAL NORMAL: ";
			if (receiver->useCameraNormal) {
				str += "CAMERA DIRECTION";
			} else {
				str += "HIT TRIANGLE";
			}
			str += "   [ N ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 150, 200, 170), textColor, false, true);

			str = "SPHERE AS PARENT: ";
			if (receiver->useParent) {
				str += "YES";
			} else {
				str += "NO";
			}
			str += "   [ P ]";
			env->getSkin()->getFont()->draw(str.c_str(), irr::core::rect<s32>(10, 180, 200, 200), textColor, false, true);

			irr::video::SColor color = irr::video::SColor(255, 255, 255, 255);
			u32 size = 15;
			irr::u32 cenW = (irr::u32) (driver->getScreenSize().Width * 0.5);
			irr::u32 cenH = (irr::u32) (driver->getScreenSize().Height * 0.5);

			//Draw crosshair
			driver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH - size, cenW + 1, cenH - 4)); //above
			driver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW + 4, cenH - 1, cenW + size, cenH + 1)); //right
			driver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH + 4, cenW + 1, cenH + size)); //down
			driver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - size, cenH - 1, cenW - 4, cenH + 1)); //left
			driver->draw2DRectangle(color, irr::core::rect<irr::s32>(cenW - 1, cenH - 1, cenW + 1, cenH + 1)); //center of screen

			env->drawAll();

			driver->endScene();

			// display frames per second in window title
			int fps = driver->getFPS();
			if (lastFPS != fps) {
				core::stringw str = L"TANK@WAR DecalManager [";
				str += driver->getName();
				str += "] FPS:";
				str += fps;
				str += " Nodes: ";
				str += smgr->getRootSceneNode()->getChildren().size();
				str += " Primitives: ";
				str += driver->getPrimitiveCountDrawn();

				device->setWindowCaption(str.c_str());
				lastFPS = fps;
			}
		}
	}

	device->drop();

	return 0;
}
