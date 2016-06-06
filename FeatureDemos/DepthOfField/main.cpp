#include "shadermaterial.h"
#include "shadergroup.h"
#include <irrlicht.h>
#include <iostream>
#include <string>

using namespace irr;
#pragma comment(lib, "Irrlicht.lib")

IrrlichtDevice *device = 0;
ShaderGroup* hdr = 0;

bool g_bQuit = false;

// Flagged "true" when MessageBox is closed
bool g_bMessageBoxClosed = false;
// ID of message box
#define MESSAGEBOX_ID 1

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event)
	{
		// Test for GUI events
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			// Test for messagebox
			if (id == MESSAGEBOX_ID) {
				if ((event.GUIEvent.EventType == gui::EGET_MESSAGEBOX_OK) ||
					(event.GUIEvent.EventType == gui::EGET_MESSAGEBOX_CANCEL))
				{
					// MesageBox OK button was clicked or Cancel button clicked
					g_bMessageBoxClosed = true;
				}
			}
		}

		if (event.EventType == irr::EET_KEY_INPUT_EVENT )
		{
			switch(event.KeyInput.Key)
			{
			case KEY_ESCAPE:
				g_bQuit = true;
				return true;
			case KEY_F1:
				hdr->range += .1f;
				return true;
			case KEY_F2:
				hdr->range -= .1f;
				return true;
			case KEY_F3:
				hdr->focus += .02f;
				return true;
			case KEY_F4:
				hdr->focus -= .02f;
				return true;
			case KEY_F5:
				hdr->sampleDist0 += .0001f;
				hdr->sampleDist1 += .0001f;
				return true;
			case KEY_F6:
				hdr->sampleDist0 -= .0001f;
				hdr->sampleDist1 -= .0001f;
				return true;
			case KEY_F7:
				hdr->distanceScale += .00001f;
				return true;
			case KEY_F8:
				hdr->distanceScale -= .00001f;
				return true;
			case KEY_SPACE:
				video::IImage* i = device->getVideoDriver()->createScreenShot();
				bool x = device->getVideoDriver()->writeImageToFile(i,"./depth.png");
				return x;
			}
		}
		return false;
	}
};


int main()
{

	MyEventReceiver receiver;

	core::dimension2di windowSize(800,600);

	device = createDevice(video::EDT_OPENGL, windowSize, 32, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	scene::ISceneManager *smgr = device->getSceneManager();
	video::IVideoDriver *driver = device->getVideoDriver();
	gui::IGUIEnvironment* env = device->getGUIEnvironment();

	// Set a different font
	gui::IGUISkin* skin = env->getSkin();
	gui::IGUIFont* font = env->getFont("fonts/fontlucida.png");
	if (font)
		skin->setFont(font);

	// Create a camera
	SKeyMap keyMap[8] = {
		// WASD navigation
		{EKA_MOVE_FORWARD,	KEY_KEY_W},
		{EKA_MOVE_BACKWARD,	KEY_KEY_S},
		{EKA_STRAFE_LEFT,	KEY_KEY_A},
		{EKA_STRAFE_RIGHT,	KEY_KEY_D},
		// Cursor-key navigation
		{EKA_MOVE_FORWARD,	KEY_UP},
		{EKA_MOVE_BACKWARD,	KEY_DOWN},
		{EKA_STRAFE_LEFT,	KEY_LEFT},
		{EKA_STRAFE_RIGHT,	KEY_RIGHT},
	};
	//buggy wasd movement
	//scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 200.0f, -1, keyMap, 8);
	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();

	cam->setPosition(core::vector3df(-280.0f,140.0f,540.0f));
	cam->setTarget(core::vector3df(0,100,400));
	cam->setFarValue(8000.0f);

	// Enable mouse pointer so user can click OK button on MessageBox
	device->getCursorControl()->setVisible(true);

	scene::ISceneNode* sky = smgr->addAnimatedMeshSceneNode(smgr->getMesh("./data/skydome.my3d"));
	sky->setMaterialFlag(video::EMF_LIGHTING, false);
	sky->setScale( irr::core::vector3df(0.25,0.25,0.25) );
	sky->setMaterialType(video::EMT_SOLID);

	scene::ISceneNode* test_scene = smgr->addOctTreeSceneNode(smgr->getMesh("./data/temple.my3d"));
	test_scene->setPosition( irr::core::vector3df(0,0,0) );
	test_scene->setScale( irr::core::vector3df(0.25,0.25,0.25) );
	test_scene->setRotation( irr::core::vector3df(0,0,0) );
	test_scene->setMaterialFlag(video::EMF_LIGHTING, false);

	hdr = new ShaderGroup(device, smgr);
	hdr->add(test_scene);

	// Create a message window and disable main camera until user clicks OK button.
	wchar_t *caption = L"Irrlicht 3D Engine - Depth Of Field Demo";
	wchar_t *text =
		L"This is a demo of Depth Of Field where the viewing camera focuses on a portion of the scene. "
		L"In this demo camera focus is manually controlled by the user (see Keys).\n"
		L"\nKeys:\n"
		L"    F1/F2            - alter camera Range\n"
		L"    F3/F4            - alter camera Focus\n"
		L"    F5/F6            - alter camera Blur0 and Blur1 parameters\n"
		L"    F7/F8            - alter camera Distance\n"
		L"    WASD/ Disabled use Cursor keys - Move camera\n"
		L"    Mouse            - Orientate camera\n"
		L"    Escape           - Exit demo\n";
	gui::IGUIWindow *messageBox = env->addMessageBox(caption, text, true, gui::EMBF_OK, 0, MESSAGEBOX_ID);
	cam->setInputReceiverEnabled(false);

	while(!g_bQuit && device->run()) {
		if (device->isWindowActive()) {

			if (g_bMessageBoxClosed) {
				// MessageBox closed - reenable main camera
				cam->setInputReceiverEnabled(true);
				device->getCursorControl()->setVisible(false);
			}

			hdr->render();

			int fps = driver->getFPS();
			core::stringw str = L" FPS:";
			str += fps;
			str += "   range=";
			str += hdr->range;
			str += "   focus=";
			str += hdr->focus;
			str += "   blur0=";
			str += hdr->sampleDist0;
			str += "   blur11=";
			str += hdr->sampleDist1;
			str += "   dist=";
			str += hdr->distanceScale;
			device->setWindowCaption(str.c_str());
		}
		device->sleep(10,0);
	}

	device->drop();

	return 0;

}


