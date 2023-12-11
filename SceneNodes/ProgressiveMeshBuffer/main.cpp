/*
 * Progressive Mesh Buffer Example

 Copyright (C) 2011-2012 Thijs Ferket (RdR)

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
#include "ProgressiveMeshBuffer.h"
#include <time.h>

using namespace irr;

class MyEventReceiver: public IEventReceiver {
public:

	MyEventReceiver(scene::ISceneManager* smgr, scene::IMesh* mesh) {
		this->smgr = smgr;
		this->originalMesh = mesh;
		showWireframe = true;
		meshId = 0;

		pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), scene::ProgressiveMeshBuffer::MELAX);
		mb = pmb->getCurrentMeshBuffer();
		initialtriangleCount = mb->getIndexCount() / 3;
	}

	bool OnEvent(const SEvent &event) {
		if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) {
			switch (event.KeyInput.Key) {

			// Change algorithm
			case irr::KEY_KEY_A: {
				delete pmb;
				mb->drop();

				u32 timeBefore = (irr::u32) time(NULL);
				if (pmb->getAlgorithm() == scene::ProgressiveMeshBuffer::RANDOM) {
					pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), scene::ProgressiveMeshBuffer::SHORTEST);
				} else if (pmb->getAlgorithm() == scene::ProgressiveMeshBuffer::SHORTEST) {
					pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), scene::ProgressiveMeshBuffer::MELAX);
				} else if (pmb->getAlgorithm() == scene::ProgressiveMeshBuffer::MELAX) {
					pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), scene::ProgressiveMeshBuffer::QUADRIC);
				} else if (pmb->getAlgorithm() == scene::ProgressiveMeshBuffer::QUADRIC) {
					pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), scene::ProgressiveMeshBuffer::RANDOM);
				}

				u32 timeAfter = (irr::u32) time(NULL);
				printf("ProgressiveMeshBuffer creation: %d sec\n", timeAfter - timeBefore);

				// Set meshbuffer & triangle count
				mb = pmb->getCurrentMeshBuffer();
				initialtriangleCount = mb->getIndexCount() / 3;
				return true;
			}

				// Change model
			case irr::KEY_KEY_M: {
				scene::ProgressiveMeshBuffer::SimplificationAlgorithm algorithm = pmb->getAlgorithm();
				delete pmb;
				mb->drop();

				u32 timeBefore = (irr::u32) time(NULL);

				if (meshId == 0) {
					originalMesh = smgr->getMesh("media/car.b3d");
					meshId = 1;
				} else if (meshId == 1) {
					originalMesh = smgr->getMesh("media/bunny.b3d");
					meshId = 2;
				} else {
					originalMesh = smgr->getMesh("media/dwarf.x");
					meshId = 0;
				}

				pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), algorithm);
				// Set meshbuffer & triangle count
				mb = pmb->getCurrentMeshBuffer();
				initialtriangleCount = mb->getIndexCount() / 3;

				u32 timeAfter = (irr::u32) time(NULL);
				printf("ProgressiveMeshBuffer creation: %d sec\n", timeAfter - timeBefore);
				return true;
			}

				// Wireframe
			case irr::KEY_KEY_W: {
				showWireframe = !showWireframe;
				return true;
			}

				// Contract 1 edge
			case irr::KEY_KEY_Z: {
				pmb->contract(1);
				mb = pmb->getCurrentMeshBuffer();
				return true;
			}

				// Contract 5%
			case irr::KEY_KEY_X: {
				u32 timeBefore = (irr::u32) time(NULL);
				u32 triangleCount = (mb->getIndexCount() / 3) - (initialtriangleCount * 0.05);
				pmb->contractTillTriangleCount(triangleCount);
				mb = pmb->getCurrentMeshBuffer();
				u32 timeAfter = (irr::u32) time(NULL);
				printf("Duration: %d sec\n", timeAfter - timeBefore);

				return true;
			}

				// Reset
			case irr::KEY_KEY_R: {
				scene::ProgressiveMeshBuffer::SimplificationAlgorithm algorithm = pmb->getAlgorithm();
				delete pmb;
				mb->drop();

				pmb = new scene::ProgressiveMeshBuffer(originalMesh->getMeshBuffer(0), algorithm);
				mb = pmb->getCurrentMeshBuffer();
				initialtriangleCount = mb->getIndexCount() / 3;
				return true;
			}

			default:
				break;
			}
		}

		return false;
	}

public:
	scene::ISceneManager* smgr;
	scene::ProgressiveMeshBuffer* pmb;
	scene::IMeshBuffer* mb;
	scene::IMesh* originalMesh;
	bool showWireframe;
	u32 initialtriangleCount;
	u32 meshId;
};

int main() {
	// ask user for driver
	//video::E_DRIVER_TYPE driverType = driverChoiceConsole();
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	if (driverType == video::EDT_COUNT)
		return 1;

	// initialize random seed
	srand(time(NULL));

	// create device with full flexibility over creation parameters
	// you can add more parameters if desired, check irr::SIrrlichtCreationParameters
	irr::SIrrlichtCreationParameters params;
	params.DriverType = driverType;
	params.WindowSize = core::dimension2d<u32>(1024, 768);
	params.Bits = 32;
	IrrlichtDevice* device = createDeviceEx(params);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	//Driver Options
	driver->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);

	// Add camera
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.005f);
	camera->setPosition(core::vector3df(0, 0, -1.5f));
	camera->setTarget(core::vector3df(0, 0, 0));
	camera->setNearValue(0.001f);

	// disable mouse cursor
	device->getCursorControl()->setVisible(false);

	// set ambient light
	smgr->setAmbientLight(video::SColor(0, 255, 255, 255));

	scene::IMesh* mesh = smgr->getMesh("media/dwarf.x");
	video::SMaterial material = mesh->getMeshBuffer(0)->getMaterial();
	material.AmbientColor = video::SColor(255, 100, 100, 100);
	material.BackfaceCulling = true;

	video::SMaterial material2 = video::SMaterial();
	material2.Wireframe = true;
	material2.BackfaceCulling = true;
	material2.AmbientColor = video::SColor(200, 255, 255, 255);

	mesh = smgr->getMeshManipulator()->createMeshWelded(mesh);

	MyEventReceiver* receiver = new MyEventReceiver(smgr, mesh);
	device->setEventReceiver(receiver);

	irr::video::SColor backgroundColor(10, 255, 255, 255);
	irr::video::SColor textColor(255, 255, 255, 255);

	device->setWindowCaption(L"TANK@WAR ProgressiveMeshBuffer");

	gui::IGUIFont* font = env->getFont("media/fontlucida.png");

	while (device->run()) {
		if (device->isWindowActive()) {
			driver->beginScene(true, true, 0);

			smgr->drawAll();

			// Draw meshBuffer
			driver->setMaterial(material);
			driver->drawMeshBuffer(receiver->mb);

			if (receiver->showWireframe) {
				// Draw wireframe of meshBuffer
				driver->setMaterial(material2);
				driver->drawMeshBuffer(receiver->mb);
			}

			core::stringw str = L"Triangles: ";
			str += receiver->mb->getIndexCount() / 3;
			font->draw(str, core::recti(10, 10, 300, 30), video::SColor(255, 255, 255, 255));
			str = L"Vertices: ";
			str += receiver->mb->getVertexCount();
			font->draw(str, core::recti(10, 30, 300, 50), video::SColor(255, 255, 255, 255));
			str = L"Algorithm: ";
			str += receiver->pmb->getAlgorithmString().c_str();
			font->draw(str, core::recti(10, 50, 300, 70), video::SColor(255, 255, 255, 255));

			font->draw(L"[A] Change algorithm", core::recti(10, 110, 300, 130), video::SColor(200, 255, 255, 255));
			font->draw(L"[M] Change model", core::recti(10, 130, 300, 150), video::SColor(200, 255, 255, 255));
			font->draw(L"[W] Toggle wireframe", core::recti(10, 150, 300, 170), video::SColor(200, 255, 255, 255));
			font->draw(L"[Z] Contract 1 edge", core::recti(10, 170, 300, 190), video::SColor(200, 255, 255, 255));
			font->draw(L"[X] Contract 5% edges", core::recti(10, 190, 300, 210), video::SColor(200, 255, 255, 255));
			font->draw(L"[R] Reset", core::recti(10, 210, 300, 230), video::SColor(200, 255, 255, 255));

			driver->endScene();
		}
	}

	device->drop();

	return 0;
}
