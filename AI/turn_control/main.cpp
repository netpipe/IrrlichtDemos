#include <irrlicht.h>
#include <iostream>
#include "turn_control.h"

#ifndef MAIN_CPP_PROPERTIES
#define MAIN_CPP_PROPERTIES
#define IRRLICHT_DIR "/home/smso/downloads/irrlicht/irrlicht-svn/"
#endif


using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;
scene::ISceneNode* fairy = 0;
scene::ISceneNode* cube = 0;

TurnControl* turnControl = 0;

void drawLine(scene::ISceneNode* n1, scene::ISceneNode* n2)
{
	core::line3d<f32> ray;
	ray.start = n1->getAbsolutePosition();
	ray.end = n2->getAbsolutePosition();

	// set mat:
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 2.0f;
	lineMaterial.FrontfaceCulling = false;
	lineMaterial.BackfaceCulling = false;
	lineMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	video::IVideoDriver* driver = device->getVideoDriver();
	driver->setMaterial(lineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	driver->draw3DLine(ray.start, ray.end, 0x8000FF00);
}

void drawForwardDir(scene::ISceneNode* node)
{
	core::vector3df dir = TurnControl::getHeading(node);
	dir.normalize();

	f32 COLLISION_DETECTION_RANGE = 500.0f;
	core::line3d<f32> ray;
	ray.start = node->getAbsolutePosition();
	ray.end = (dir * COLLISION_DETECTION_RANGE) + ray.start;

	// set mat:
	video::SMaterial lineMaterial;
	lineMaterial.Lighting = false;
	lineMaterial.Thickness = 2.0f;
	lineMaterial.FrontfaceCulling = false;
	lineMaterial.BackfaceCulling = false;
	lineMaterial.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;

	video::IVideoDriver* driver = device->getVideoDriver();
	driver->setMaterial(lineMaterial);
	driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);
	driver->draw3DLine(ray.start, ray.end, 0x800000FF);

}

void print(const core::vector3df& vec)
{
	printf("(%.2f,%.2f,%.2f)\n", vec.X, vec.Y, vec.Z);
}


void circulate
(
	scene::ISceneNode* node,
	scene::ISceneNode* center,
	f32 degs
)
{
	node->updateAbsolutePosition();
	center->updateAbsolutePosition();

	core::vector3df vect = node->getAbsolutePosition() - center->getAbsolutePosition();

	core::vector3df yaxis(0.0f, 1.0f, 0.0f);

	core::quaternion quat;
	quat.fromAngleAxis(degs * core::DEGTORAD, yaxis);
	core::matrix4 mat = quat.getMatrix();
	mat.rotateVect(vect);
	node->setPosition(vect + center->getAbsolutePosition());
}

////////////////////////////////////////////////////////////////////////////////
class EventReceiver: public IEventReceiver
{

public:
bool OnEvent(const SEvent& event)
{
	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == true)
	)
	{
		if (event.KeyInput.Key == irr::KEY_ESCAPE)
		{
			device->closeDevice();
			device->run();
			return true;
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_W)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_S)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_A)
		{
			circulate(cube, fairy, -15.0f);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_D)
		{
			circulate(cube, fairy, 15.0f);
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_Q)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_E)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_Z)
		{
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
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_X)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_KEY_G)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_B)
		{
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
			print(camera->getPosition());
		}
		else if (event.KeyInput.Key == irr::KEY_F2)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F3)
		{
			bool enabled = camera->isInputReceiverEnabled();
			camera->setInputReceiverEnabled(!enabled);
			device->getCursorControl()->setVisible(enabled);
		}
		else if (event.KeyInput.Key == irr::KEY_F4)
		{

		}
		else if (event.KeyInput.Key == irr::KEY_F5)
		{
			turnControl->turnTo(cube);
		}
		else if (event.KeyInput.Key == irr::KEY_F6)
		{
		}

		else if (event.KeyInput.Key == irr::KEY_F7)
		{

		}
		else if (event.KeyInput.Key == irr::KEY_F8)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F9)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F10)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_F11)
		{

		}
		else if (event.KeyInput.Key == irr::KEY_F12)
		{

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
	else
	{

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
	io::IFileSystem* fs = device->getFileSystem();

	////////////////////////////////////////////////////////////////////////////
	// create skybox
	//fs->addFileArchive(IRRLICHT_DIR);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

	smgr->addSkyBoxSceneNode
	(
		driver->getTexture("../../media/irrlicht2_up.jpg"),
		driver->getTexture("../../media/irrlicht2_dn.jpg"),
		driver->getTexture("../../media/irrlicht2_lf.jpg"),
		driver->getTexture("../../media/irrlicht2_rt.jpg"),
		driver->getTexture("../../media/irrlicht2_ft.jpg"),
		driver->getTexture("../../media/irrlicht2_bk.jpg")
	);

	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);



	////////////////////////////////////////////////////////////////////////////
	// setup fairy:
	fairy = smgr->addEmptySceneNode();

	scene::IAnimatedMesh* amesh = smgr->getMesh("../../media/faerie.md2");
	scene::IAnimatedMeshSceneNode* fairyAnimatedMeshSceneNode = smgr->addAnimatedMeshSceneNode(amesh, fairy);

	if (!fairyAnimatedMeshSceneNode)
	{
		printf("Failed creating fairy scene node!\n");
		return -1;
	}

	fairyAnimatedMeshSceneNode->setLoopMode(true);
	fairyAnimatedMeshSceneNode->setJointMode(scene::EJUOR_READ);
	fairyAnimatedMeshSceneNode->setScale(core::vector3df(2.5f));
	fairyAnimatedMeshSceneNode->setRotation(core::vector3df(0.0f, -90.0f, 0.0f));

	fairyAnimatedMeshSceneNode->setMaterialTexture(0, driver->getTexture("../../media/faerie2.bmp"));
	fairyAnimatedMeshSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
	if (fairyAnimatedMeshSceneNode)
		fairyAnimatedMeshSceneNode->setMD2Animation(scene::EMAT_STAND);


	////////////////////////////////////////////////////////////////////////////
	// setup cube:
	cube = smgr->addCubeSceneNode();
	cube->setScale(core::vector3df(2.0f));
	cube->setPosition(core::vector3df(250.0f, 0.0f, 250.0f));
	cube->setMaterialTexture(0, driver->getTexture("../../media/wall.bmp"));
	cube->setMaterialFlag(video::EMF_LIGHTING, false);

	////////////////////////////////////////////////////////////////////////////
	// setup turnControl:
	turnControl = new TurnControl(device, fairy);

	////////////////////////////////////////////////////////////////////////////
	// create camera

	f32 camRotateSpeed = 100.0f;
	f32 camMoveSpeed = 0.05f;
	f32 camJumpSpeed = 3.0f;

	camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);
	camera->setPosition(core::vector3df(57.0f, 270.0f, -34.0f));

	core::vector3df target = (cube->getPosition() + fairy->getPosition()) * 0.5f;
	camera->setTarget(target);

	////////////////////////////////////////////////////////////////////////////
	device->getCursorControl()->setVisible(false);

	// ----------------------------------------------------

	while (device->run())
	if (device->isWindowActive())
	{
		turnControl->update();
		driver->beginScene(ECBF_COLOR | ECBF_DEPTH, SColor(255,100,101,140));
		//driver->beginScene(true, true, 0);
		smgr->drawAll();

		drawForwardDir(fairy);
		drawLine(fairy, cube);

		driver->endScene();
	}
	else { device->yield(); }

	if (turnControl) { delete turnControl; turnControl = 0; }
	if (device) { device->drop(); }

	return 0;
}



