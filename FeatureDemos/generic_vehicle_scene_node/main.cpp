#include <irrlicht.h>
#include <iostream>
#include "util.h"
#include "vehicle_scene_node.h"

#ifndef MAIN_CPP_PROPERTIES
#define MAIN_CPP_PROPERTIES
#define IRRLICHT_DIR "/home/smso/downloads/irrlicht/irrlicht-svn/"
#endif


using namespace irr;


IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;
scene::ISceneNode* fairy = 0;
VehicleSceneNode* vehicle = 0;

// for controlling pitch of body:
bool gasPedalPressed = false;
bool brakePedalPressed = false;

bool showHelpScreen = true;

void drawHelpText
(
	IrrlichtDevice* device,
	const core::stringw& text,
	s32 sx,
	s32 sy
)
{
	gui::IGUIFont* font = device->getGUIEnvironment()->getFont("media/lucida.xml");
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

	video::SColor blue(0xFF0000FF);
	font->draw(text, rect, blue, false, false);
}

enum
{
	GUI_ID_DIALOG_ROOT_WINDOW  = 0x10000,

	GUI_ID_BODY_MESH_FILE,
	GUI_ID_TIRE_MESH_FILE,

	GUI_ID_BODY_MESH_ROT_X,
	GUI_ID_BODY_MESH_ROT_Y,
	GUI_ID_BODY_MESH_ROT_Z,

	GUI_ID_TIRE_SCALE,

	GUI_ID_FR_TIRE_ROT_X,
	GUI_ID_FR_TIRE_ROT_Y,
	GUI_ID_FR_TIRE_ROT_Z,

	GUI_ID_FL_TIRE_ROT_X,
	GUI_ID_FL_TIRE_ROT_Y,
	GUI_ID_FL_TIRE_ROT_Z,

	GUI_ID_RR_TIRE_ROT_X,
	GUI_ID_RR_TIRE_ROT_Y,
	GUI_ID_RR_TIRE_ROT_Z,

	GUI_ID_RL_TIRE_ROT_X,
	GUI_ID_RL_TIRE_ROT_Y,
	GUI_ID_RL_TIRE_ROT_Z,

	GUI_ID_TIRE_OFFSET_DX,
	GUI_ID_TIRE_OFFSET_DY,
	GUI_ID_TIRE_OFFSET_DZ1,
	GUI_ID_TIRE_OFFSET_DZ2,


	GUI_ID_SET_BODY_MESH_FILE,
	GUI_ID_SET_TIRE_MESH_FILE,
	GUI_ID_SET_BODY_MESH_ROT,
	GUI_ID_SET_TIRE_OFFSETS,
	GUI_ID_SET_TIRE_SCALE,

	GUI_ID_SET_FR_TIRE_ROT,
	GUI_ID_SET_FL_TIRE_ROT,
	GUI_ID_SET_RR_TIRE_ROT,
	GUI_ID_SET_RL_TIRE_ROT,

	GUI_ID_SELECT_BODY_MESH_FILE,
	GUI_ID_SELECT_TIRE_MESH_FILE,

	GUI_ID_LOAD_PARAMS,

	GUI_ID_BODY_MESH_FILE_DIALOG,
	GUI_ID_TIRE_MESH_FILE_DIALOG,
	GUI_ID_LOAD_PARAMS_DIALOG,

	GUI_ID_SAVE_DIR,
	GUI_ID_SELECT_DIR,
	GUI_ID_SELECT_DIR_DIALOG,
	GUI_ID_SAVE_FILENAME,
	GUI_ID_SAVE_PARAMS,

	GUI_ID_QUIT
};


void createToolBox()
{
	// remove tool box if already there
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	gui::IGUIElement* root = guienv->getRootGUIElement();
	gui::IGUIElement* e = root->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (e)
		e->remove();

	// create the toolbox window
	gui::IGUIWindow* win = guienv->addWindow
	(
		core::rect<s32>(300,45,1000,750),
		false,
		L"Toolset",
		0,
		GUI_ID_DIALOG_ROOT_WINDOW
	);
	win->getCloseButton()->setEnabled(false);
	win->getMinimizeButton()->setVisible(true);
	win->getMaximizeButton()->setVisible(true);

	gui::IGUITabControl* tab = guienv->addTabControl
	(core::rect<s32>(2,20,1000,750-7), win, true, true);

	gui::IGUITab* t1 = tab->addTab(L"Config");

	// body mesh file:
	guienv->addStaticText(L"Body Mesh File:", core::rect<s32>(10,20,100,40), false, false, t1);
	guienv->addEditBox(L"", core::rect<s32>(105,20,560,40), true, t1, GUI_ID_BODY_MESH_FILE);
	guienv->addButton(core::rect<s32>(565,20,620,40), t1, GUI_ID_SELECT_BODY_MESH_FILE, L"Browse...");
	guienv->addButton(core::rect<s32>(625,20,675,40), t1, GUI_ID_SET_BODY_MESH_FILE, L"Set");

	// body mesh rot:
	guienv->addStaticText(L"X", core::rect<s32>(110,48,160,68), false, false, t1);
	guienv->addStaticText(L"Y", core::rect<s32>(165,48,215,68), false, false, t1);
	guienv->addStaticText(L"Z", core::rect<s32>(220,48,270,68), false, false, t1);

	guienv->addStaticText(L"Body Mesh Rot:", core::rect<s32>(5,70,105,90), false, false, t1);
	guienv->addEditBox(L"0.0", core::rect<s32>(110,70,160,90), true, t1, GUI_ID_BODY_MESH_ROT_X);
	guienv->addEditBox(L"0.0", core::rect<s32>(165,70,215,90), true, t1, GUI_ID_BODY_MESH_ROT_Y);
	guienv->addEditBox(L"0.0", core::rect<s32>(220,70,270,90), true, t1, GUI_ID_BODY_MESH_ROT_Z);
	guienv->addButton(core::rect<s32>(275,70,325,90), t1, GUI_ID_SET_BODY_MESH_ROT, L"Set");

	// tire mesh file:
	guienv->addStaticText(L"Tire Mesh File:", core::rect<s32>(10,145,100,165), false, false, t1);
	guienv->addEditBox(L"", core::rect<s32>(105,145,560,165), true, t1, GUI_ID_TIRE_MESH_FILE);
	guienv->addButton(core::rect<s32>(565,145,620,165), t1, GUI_ID_SELECT_TIRE_MESH_FILE, L"Browse...");
	guienv->addButton(core::rect<s32>(625,145,675,165), t1, GUI_ID_SET_TIRE_MESH_FILE, L"Set");

	// tire scale:
	guienv->addStaticText(L"Tire Scale:", core::rect<s32>(5,185,105,205), false, false, t1);
	gui::IGUISpinBox* spinBox = guienv->addSpinBox(L"1.0", core::rect<s32>(110,185,170,205), true, t1, GUI_ID_TIRE_SCALE);
	spinBox->setDecimalPlaces(2);
	spinBox->setRange(0.0f, 10.0f);
	spinBox->setStepSize(0.05f);
	guienv->addButton(core::rect<s32>(175,185,205,205), t1, GUI_ID_SET_TIRE_SCALE, L"Set");

	// set tire offsets: dx, dy, dz1, dz2
	guienv->addStaticText(L"DX", core::rect<s32>(110,218,190,238), false, false, t1);
	guienv->addStaticText(L"DY", core::rect<s32>(195,218,275,238), false, false, t1);
	guienv->addStaticText(L"DZ1", core::rect<s32>(280,218,360,238), false, false, t1);
	guienv->addStaticText(L"DZ2", core::rect<s32>(365,218,445,238), false, false, t1);

	guienv->addStaticText(L"Tire Offset:", core::rect<s32>(5,240,105,260), false, false, t1);
	gui::IGUISpinBox* spinBoxDx = guienv->addSpinBox(L"1.0", core::rect<s32>(110,240,190,260), true, t1, GUI_ID_TIRE_OFFSET_DX);
	gui::IGUISpinBox* spinBoxDy = guienv->addSpinBox(L"0.0", core::rect<s32>(195,240,275,260), true, t1, GUI_ID_TIRE_OFFSET_DY);
	gui::IGUISpinBox* spinBoxDz1 = guienv->addSpinBox(L"1.0", core::rect<s32>(280,240,360,260), true, t1, GUI_ID_TIRE_OFFSET_DZ1);
	gui::IGUISpinBox* spinBoxDz2 = guienv->addSpinBox(L"-1.0", core::rect<s32>(365,240,445,260), true, t1, GUI_ID_TIRE_OFFSET_DZ2);

	spinBoxDx->setDecimalPlaces(2); spinBoxDx->setRange(-10.0f, 10.0f); spinBoxDx->setStepSize(0.01f);
	spinBoxDy->setDecimalPlaces(2); spinBoxDy->setRange(-10.0f, 10.0f); spinBoxDy->setStepSize(0.01f);
	spinBoxDz1->setDecimalPlaces(2); spinBoxDz1->setRange(-10.0f, 10.0f); spinBoxDz1->setStepSize(0.01f);
	spinBoxDz2->setDecimalPlaces(2); spinBoxDz2->setRange(-10.0f, 10.0f); spinBoxDz2->setStepSize(0.01f);


	guienv->addButton(core::rect<s32>(450,240,500,260), t1, GUI_ID_SET_TIRE_OFFSETS, L"Set");

	// tire rots:
	s32 x0 = 10; s32 x1 = 110;
	s32 x2 = 120; s32 x3 = 170;
	s32 x4 = 180; s32 x5 = 230;
	s32 x6 = 240; s32 x7 = 290;
	s32 x8 = 300; s32 x9 = 350;

	s32 y0 = 273; s32 y1 = 293;
	s32 y2 = 295; s32 y3 = 315;
	s32 y4 = 320; s32 y5 = 340;
	s32 y6 = 345; s32 y7 = 365;
	s32 y8 = 370; s32 y9 = 390;
	s32 y10 = 395; s32 y11 = 415;


	guienv->addStaticText(L"X", core::rect<s32>(x2,y0,x3,y1), false, false, t1);
	guienv->addStaticText(L"Y", core::rect<s32>(x4,y0,x5,y1), false, false, t1);
	guienv->addStaticText(L"Z", core::rect<s32>(x6,y0,x7,y1), false, false, t1);


	guienv->addStaticText(L"FR Tire Rot:", core::rect<s32>(x0,y2,x1,y3), false, false, t1);
	guienv->addEditBox(L"0.0", core::rect<s32>(x2,y2,x3,y3), true, t1, GUI_ID_FR_TIRE_ROT_X);
	guienv->addEditBox(L"0.0", core::rect<s32>(x4,y2,x5,y3), true, t1, GUI_ID_FR_TIRE_ROT_Y);
	guienv->addEditBox(L"0.0", core::rect<s32>(x6,y2,x7,y3), true, t1, GUI_ID_FR_TIRE_ROT_Z);
	guienv->addButton(core::rect<s32>(x8,y2,x9,y3), t1, GUI_ID_SET_FR_TIRE_ROT, L"Set");

	guienv->addStaticText(L"FL Tire Rot:", core::rect<s32>(x0,y4,x1,y5), false, false, t1);
	guienv->addEditBox(L"0.0", core::rect<s32>(x2,y4,x3,y5), true, t1, GUI_ID_FL_TIRE_ROT_X);
	guienv->addEditBox(L"0.0", core::rect<s32>(x4,y4,x5,y5), true, t1, GUI_ID_FL_TIRE_ROT_Y);
	guienv->addEditBox(L"0.0", core::rect<s32>(x6,y4,x7,y5), true, t1, GUI_ID_FL_TIRE_ROT_Z);
	guienv->addButton(core::rect<s32>(x8,y4,x9,y5), t1, GUI_ID_SET_FL_TIRE_ROT, L"Set");

	guienv->addStaticText(L"RR Tire Rot:", core::rect<s32>(x0,y6,x1,y7), false, false, t1);
	guienv->addEditBox(L"0.0", core::rect<s32>(x2,y6,x3,y7), true, t1, GUI_ID_RR_TIRE_ROT_X);
	guienv->addEditBox(L"0.0", core::rect<s32>(x4,y6,x5,y7), true, t1, GUI_ID_RR_TIRE_ROT_Y);
	guienv->addEditBox(L"0.0", core::rect<s32>(x6,y6,x7,y7), true, t1, GUI_ID_RR_TIRE_ROT_Z);
	guienv->addButton(core::rect<s32>(x8,y6,x9,y7), t1, GUI_ID_SET_RR_TIRE_ROT, L"Set");

	guienv->addStaticText(L"RL Tire Rot:", core::rect<s32>(x0,y8,x1,y9), false, false, t1);
	guienv->addEditBox(L"0.0", core::rect<s32>(x2,y8,x3,y9), true, t1, GUI_ID_RL_TIRE_ROT_X);
	guienv->addEditBox(L"0.0", core::rect<s32>(x4,y8,x5,y9), true, t1, GUI_ID_RL_TIRE_ROT_Y);
	guienv->addEditBox(L"0.0", core::rect<s32>(x6,y8,x7,y9), true, t1, GUI_ID_RL_TIRE_ROT_Z);
	guienv->addButton(core::rect<s32>(x8,y8,x9,y9), t1, GUI_ID_SET_RL_TIRE_ROT, L"Set");

	// save vehicle params
	core::stringw dir = core::stringw(device->getFileSystem()->getWorkingDirectory());
	guienv->addStaticText(L"Save In Dir:", core::rect<s32>(10,450,100,470), false, false, t1);
	guienv->addEditBox(dir.c_str(), core::rect<s32>(105,450,560,470), true, t1, GUI_ID_SAVE_DIR);
	guienv->addButton(core::rect<s32>(565,450,620,470), t1, GUI_ID_SELECT_DIR, L"Browse...");

	guienv->addStaticText(L"Filename:", core::rect<s32>(10,475,100,495), false, false, t1);
	guienv->addEditBox(L"untitled.xml", core::rect<s32>(105,475,215,495), true, t1, GUI_ID_SAVE_FILENAME);
	guienv->addButton(core::rect<s32>(10,500,160,520), t1, GUI_ID_SAVE_PARAMS, L"Save Vehicle Params");

}

void createMenu()
{
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	gui::IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font = guienv->getFont("media/lucida.xml");
	if (font)
		skin->setFont(font);

	bool enabled = true;
	bool hasSubMenu = true;

	gui::IGUIContextMenu* contextMenu = guienv->addMenu();
	u32 fileMenuId = contextMenu->addItem(L"File", -1, enabled, hasSubMenu);

	hasSubMenu = false;
	contextMenu->getSubMenu(0)->addItem(L"Load vehicle params...", GUI_ID_LOAD_PARAMS, enabled, hasSubMenu);
	contextMenu->getSubMenu(0)->addItem(L"Quit", GUI_ID_QUIT, enabled, hasSubMenu);

}


void setUiData(const VehicleSceneNodeParams& params)
{
	gui::IGUIElement* win = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!win)
		return;

	win->getElementFromId(GUI_ID_BODY_MESH_FILE, true)->setText(params.BodyMeshFile.c_str());
	win->getElementFromId(GUI_ID_TIRE_MESH_FILE, true)->setText(params.TireMeshFile.c_str());

	f32 x, y, z;
	core::vector3df rot;

	rot = params.BodyMeshRot;
	x = rot.X; y = rot.Y; z = rot.Z;
	win->getElementFromId(GUI_ID_BODY_MESH_ROT_X, true)->setText(core::stringw(x).c_str());
	win->getElementFromId(GUI_ID_BODY_MESH_ROT_Y, true)->setText(core::stringw(y).c_str());
	win->getElementFromId(GUI_ID_BODY_MESH_ROT_Z, true)->setText(core::stringw(z).c_str());

	rot = params.FrontRightTireRot;
	x = rot.X; y = rot.Y; z = rot.Z;
	win->getElementFromId(GUI_ID_FR_TIRE_ROT_X, true)->setText(core::stringw(x).c_str());
	win->getElementFromId(GUI_ID_FR_TIRE_ROT_Y, true)->setText(core::stringw(y).c_str());
	win->getElementFromId(GUI_ID_FR_TIRE_ROT_Z, true)->setText(core::stringw(z).c_str());

	rot = params.FrontLeftTireRot;
	x = rot.X; y = rot.Y; z = rot.Z;
	win->getElementFromId(GUI_ID_FL_TIRE_ROT_X, true)->setText(core::stringw(x).c_str());
	win->getElementFromId(GUI_ID_FL_TIRE_ROT_Y, true)->setText(core::stringw(y).c_str());
	win->getElementFromId(GUI_ID_FL_TIRE_ROT_Z, true)->setText(core::stringw(z).c_str());

	rot = params.RearRightTireRot;
	x = rot.X; y = rot.Y; z = rot.Z;
	win->getElementFromId(GUI_ID_RR_TIRE_ROT_X, true)->setText(core::stringw(x).c_str());
	win->getElementFromId(GUI_ID_RR_TIRE_ROT_Y, true)->setText(core::stringw(y).c_str());
	win->getElementFromId(GUI_ID_RR_TIRE_ROT_Z, true)->setText(core::stringw(z).c_str());

	rot = params.RearLeftTireRot;
	x = rot.X; y = rot.Y; z = rot.Z;
	win->getElementFromId(GUI_ID_RL_TIRE_ROT_X, true)->setText(core::stringw(x).c_str());
	win->getElementFromId(GUI_ID_RL_TIRE_ROT_Y, true)->setText(core::stringw(y).c_str());
	win->getElementFromId(GUI_ID_RL_TIRE_ROT_Z, true)->setText(core::stringw(z).c_str());


	f32 dx = params.TireOffsetDx;
	f32 dy = params.TireOffsetDy;
	f32 dz1 = params.TireOffsetDz1;
	f32 dz2 = params.TireOffsetDz2;

	win->getElementFromId(GUI_ID_TIRE_OFFSET_DX, true)->setText(core::stringw(dx).c_str());
	win->getElementFromId(GUI_ID_TIRE_OFFSET_DY, true)->setText(core::stringw(dy).c_str());
	win->getElementFromId(GUI_ID_TIRE_OFFSET_DZ1, true)->setText(core::stringw(dz1).c_str());
	win->getElementFromId(GUI_ID_TIRE_OFFSET_DZ2, true)->setText(core::stringw(dz2).c_str());

	f32 s = params.TireScale;
	win->getElementFromId(GUI_ID_TIRE_SCALE, true)->setText(core::stringw(s).c_str());
}



bool getStringFromUi(s32 id, core::stringc& val)
{
	gui::IGUIElement* win = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!win)
		return false;

	gui::IGUIElement* element = win->getElementFromId(id, true);
	if (!element)
		return false;

	val = element->getText();
	return true;
}


bool getValueFromUi(s32 id, f32& val)
{
	gui::IGUIElement* win = device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
	if (!win)
		return false;

	gui::IGUIElement* element = win->getElementFromId(id, true);
	if (!element)
		return false;

	core::stringc s = element->getText();
	val = (f32)atof(s.c_str());
	return true;
}

bool getVector3dfFromUi(s32 id0, s32 id1, s32 id2, core::vector3df& vec)
{
	f32 x, y, z;
	if (!getValueFromUi(id0, x))
		return false;

	if (!getValueFromUi(id1, y))
		return false;

	if (!getValueFromUi(id2, z))
		return false;

	vec = core::vector3df(x, y, z);
	return true;
}


bool getParamsFromUi(VehicleSceneNodeParams& params)
{
	core::stringc filename = "";

	if (!getStringFromUi(GUI_ID_BODY_MESH_FILE, filename))
		return false;
	params.BodyMeshFile = core::stringw(filename.c_str());

	if (!getStringFromUi(GUI_ID_TIRE_MESH_FILE, filename))
		return false;
	params.TireMeshFile = core::stringw(filename.c_str());

	////////////////////////////////////////////////////////////////////////////
	core::vector3df rot(0.0f);

	if (!getVector3dfFromUi(GUI_ID_BODY_MESH_ROT_X, GUI_ID_BODY_MESH_ROT_Y, GUI_ID_BODY_MESH_ROT_Z, rot))
		return false;
	params.BodyMeshRot = rot;

	if (!getVector3dfFromUi(GUI_ID_FR_TIRE_ROT_X, GUI_ID_FR_TIRE_ROT_Y, GUI_ID_FR_TIRE_ROT_Z, rot))
		return false;
	params.FrontRightTireRot = rot;

	if (!getVector3dfFromUi(GUI_ID_FL_TIRE_ROT_X, GUI_ID_FL_TIRE_ROT_Y, GUI_ID_FL_TIRE_ROT_Z, rot))
		return false;
	params.FrontLeftTireRot = rot;

	if (!getVector3dfFromUi(GUI_ID_RR_TIRE_ROT_X, GUI_ID_RR_TIRE_ROT_Y, GUI_ID_RR_TIRE_ROT_Z, rot))
		return false;
	params.RearRightTireRot = rot;

	if (!getVector3dfFromUi(GUI_ID_RL_TIRE_ROT_X, GUI_ID_RL_TIRE_ROT_Y, GUI_ID_RL_TIRE_ROT_Z, rot))
		return false;
	params.RearLeftTireRot = rot;

	////////////////////////////////////////////////////////////////////////////
	f32 dx = 0.0f; f32 dy = 0.0f; f32 dz1 = 0.0f; f32 dz2 = 0.0f;
	if
	(
		(!getValueFromUi(GUI_ID_TIRE_OFFSET_DX, dx))
		|| (!getValueFromUi(GUI_ID_TIRE_OFFSET_DY, dy))
		|| (!getValueFromUi(GUI_ID_TIRE_OFFSET_DZ1, dz1))
		|| (!getValueFromUi(GUI_ID_TIRE_OFFSET_DZ2, dz2))
	)
		return false;
	params.TireOffsetDx = dx;
	params.TireOffsetDy = dy;
	params.TireOffsetDz1 = dz1;
	params.TireOffsetDz2 = dz2;

	f32 s = 1.0f;
	if (!getValueFromUi(GUI_ID_TIRE_SCALE, s))
		return false;
	params.TireScale = s;

	return true;
}

////////////////////////////////////////////////////////////////////////////////
class EventReceiver: public IEventReceiver
{

public:
bool OnEvent(const SEvent& event)
{
	// for controlling pitch of body:
	////////////////////////////////////////////////////////////////////////////
	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == true)
		&& (event.KeyInput.Key == irr::KEY_KEY_W)
		&& (!gasPedalPressed)
	)
		gasPedalPressed = true;

	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == true)
		&& (event.KeyInput.Key == irr::KEY_KEY_S)
		&& (!brakePedalPressed)
	)
		brakePedalPressed = true;

	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == false)
		&& (event.KeyInput.Key == irr::KEY_KEY_W)
		&& (gasPedalPressed)
	)
	{
		gasPedalPressed = false;
		vehicle->resetPitch();
	}

	if
	(
		(event.EventType == EET_KEY_INPUT_EVENT)
		&& (event.KeyInput.PressedDown == false)
		&& (event.KeyInput.Key == irr::KEY_KEY_S)
		&& (brakePedalPressed)
	)
	{
		brakePedalPressed = false;
		vehicle->resetPitch();
	}

	////////////////////////////////////////////////////////////////////////////


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
			vehicle->speedUp();
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_S)
		{
			vehicle->slowDown();
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_A)
		{
			vehicle->steerFrontWheels(5.0f);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_D)
		{
			vehicle->steerFrontWheels(-5.0f);
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_Q)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_E)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_Z)
		{
			vehicle->pitchBody(-0.1f);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_C)
		{
			vehicle->pitchBody(0.1f);
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_N)
		{
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_M)
		{
		}


		else if (event.KeyInput.Key == irr::KEY_KEY_R)
		{
			vehicle->resetPitch();
		}
		else if (event.KeyInput.Key == irr::KEY_KEY_X)
		{
			vehicle->stop();
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
			showHelpScreen = !showHelpScreen;

		}
		else if (event.KeyInput.Key == irr::KEY_F2)
		{
			if (vehicle) Util::rotateCameraTowardsNodeAroundYAxis(camera, vehicle, 5.0f);

		}
		else if (event.KeyInput.Key == irr::KEY_F3)
		{
			bool enabled = camera->isInputReceiverEnabled();
			camera->setInputReceiverEnabled(!enabled);
			device->getCursorControl()->setVisible(enabled);
		}
		else if (event.KeyInput.Key == irr::KEY_F4)
		{
			video::IImage* image = device->getVideoDriver()->createScreenShot(video::ECF_A8R8G8B8);
			if (!image)
				return false;
			device->getVideoDriver()->writeImageToFile(image, L"generic_vehicle_scene_node_screenshot.png");
			image->drop();
		}
		else if (event.KeyInput.Key == irr::KEY_F5)
		{
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
			gui::IGUIElement* win
			= device->getGUIEnvironment()->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
			if (!win)
			{
				printf("win is destroyed\n");
				return false;
			}
			if (!win->isVisible())
				win->setVisible(true);
			else
				win->setVisible(false);
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

	if (event.EventType == EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();
		gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
		switch (event.GUIEvent.EventType)
		{
			////////////////////////////////////////////////////////////////////
			case gui::EGET_BUTTON_CLICKED:
			if (id == GUI_ID_SET_BODY_MESH_FILE)
			{
				core::stringc filename = "";
				if (getStringFromUi(GUI_ID_BODY_MESH_FILE, filename))
				{
					vehicle->setBodyMeshFile(device, filename);
				}
			}
			else if (id == GUI_ID_SET_TIRE_MESH_FILE)
			{
				core::stringc filename = "";
				if (getStringFromUi(GUI_ID_TIRE_MESH_FILE, filename))
				{
					vehicle->setTireMeshFile(device, filename);
				}
			}
			else if (id == GUI_ID_SET_BODY_MESH_ROT)
			{
				core::vector3df rot(0.0f);
				if (getVector3dfFromUi(GUI_ID_BODY_MESH_ROT_X, GUI_ID_BODY_MESH_ROT_Y, GUI_ID_BODY_MESH_ROT_Z, rot))
				{
					Util::print(rot);
					vehicle->setBodyMeshRotation(rot);
				}
			}
			else if (id == GUI_ID_SET_TIRE_SCALE)
			{
				f32 scale = 1.0f;
				if (getValueFromUi(GUI_ID_TIRE_SCALE, scale))
				{
					vehicle->setTireScale(scale);
				}
			}
			else if (id == GUI_ID_SET_TIRE_OFFSETS)
			{
				f32 dx = 0.0f; f32 dy = 0.0f; f32 dz1 = 0.0f; f32 dz2 = 0.0f;
				if
				(
					(getValueFromUi(GUI_ID_TIRE_OFFSET_DX, dx))
					&& (getValueFromUi(GUI_ID_TIRE_OFFSET_DY, dy))
					&& (getValueFromUi(GUI_ID_TIRE_OFFSET_DZ1, dz1))
					&& (getValueFromUi(GUI_ID_TIRE_OFFSET_DZ2, dz2))
				)
				{
					printf("TireOffsets dx,dy,dz1,dz2=(%f,%f,%f,%f)\n", dx,dy,dz1,dz2);
					vehicle->setTireOffsets(dx,dy,dz1,dz2);
				}
			}
			else if (id == GUI_ID_SET_FR_TIRE_ROT)
			{
				core::vector3df rot(0.0f);
				if (getVector3dfFromUi(GUI_ID_FR_TIRE_ROT_X, GUI_ID_FR_TIRE_ROT_Y, GUI_ID_FR_TIRE_ROT_Z, rot))
				{
					Util::print(rot);
					vehicle->setFrontRightTireRotation(rot);
				}
			}
			else if (id == GUI_ID_SET_FL_TIRE_ROT)
			{
				core::vector3df rot(0.0f);
				if (getVector3dfFromUi(GUI_ID_FL_TIRE_ROT_X, GUI_ID_FL_TIRE_ROT_Y, GUI_ID_FL_TIRE_ROT_Z, rot))
				{
					Util::print(rot);
					vehicle->setFrontLeftTireRotation(rot);
				}
			}
			else if (id == GUI_ID_SET_RR_TIRE_ROT)
			{
				core::vector3df rot(0.0f);
				if (getVector3dfFromUi(GUI_ID_RR_TIRE_ROT_X, GUI_ID_RR_TIRE_ROT_Y, GUI_ID_RR_TIRE_ROT_Z, rot))
				{
					Util::print(rot);
					vehicle->setRearRightTireRotation(rot);
				}
			}
			else if (id == GUI_ID_SET_RL_TIRE_ROT)
			{
				core::vector3df rot(0.0f);
				if (getVector3dfFromUi(GUI_ID_RL_TIRE_ROT_X, GUI_ID_RL_TIRE_ROT_Y, GUI_ID_RL_TIRE_ROT_Z, rot))
				{
					Util::print(rot);
					vehicle->setRearLeftTireRotation(rot);
				}
			}
			else if (id == GUI_ID_SELECT_BODY_MESH_FILE)
			{
				gui::IGUIFileOpenDialog* dialog = guienv->addFileOpenDialog(L"Please select the body mesh file", false, 0, GUI_ID_BODY_MESH_FILE_DIALOG);
				dialog->setRelativePositionProportional(core::rect<f32>(0.1f, 0.1f, 0.9f, 0.9f));
			}
			else if (id == GUI_ID_SELECT_TIRE_MESH_FILE)
			{
				gui::IGUIFileOpenDialog* dialog = guienv->addFileOpenDialog(L"Please select the tire mesh file", false, 0, GUI_ID_TIRE_MESH_FILE_DIALOG);
				dialog->setRelativePositionProportional(core::rect<f32>(0.1f, 0.1f, 0.9f, 0.9f));
			}
			else if (id == GUI_ID_SELECT_DIR)
			{
				gui::IGUIFileOpenDialog* dialog = guienv->addFileOpenDialog(L"Please select the dir to save params", false, 0, GUI_ID_SELECT_DIR_DIALOG);
				dialog->setRelativePositionProportional(core::rect<f32>(0.1f, 0.1f, 0.9f, 0.9f));
			}
			else if (id == GUI_ID_SAVE_PARAMS)
			{
				gui::IGUIElement* win = guienv->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
				core::stringc dir = win->getElementFromId(GUI_ID_SAVE_DIR, true)->getText();
				core::stringc fn = win->getElementFromId(GUI_ID_SAVE_FILENAME, true)->getText();
				printf("Saving: %s\n", (dir+"/"+fn).c_str());
				VehicleSceneNodeParams params;
				if (getParamsFromUi(params))
					VehicleSceneNode::saveParamsToFile(device, dir+"/"+fn, params);
			}
			break;

			////////////////////////////////////////////////////////////////////
			case gui::EGET_FILE_SELECTED:
			{
				gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
				core::stringw filename = core::stringw(dialog->getFileName());
				core::stringc fn = core::stringc(dialog->getFileName());


				gui::IGUIElement* win = guienv->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
				if (!win)
					return false;

				if (id == GUI_ID_BODY_MESH_FILE_DIALOG)
				{
					win->getElementFromId(GUI_ID_BODY_MESH_FILE, true)->setText(filename.c_str());
					printf("id == GUI_ID_SELECT_BODY_MESH_FILE\n");
				}
				else if (id == GUI_ID_TIRE_MESH_FILE_DIALOG)
				{
					win->getElementFromId(GUI_ID_TIRE_MESH_FILE, true)->setText(filename.c_str());
					printf("id == GUI_ID_SELECT_TIRE_MESH_FILE\n");
				}
				else if (id == GUI_ID_LOAD_PARAMS_DIALOG)
				{
					printf("loading: fn=%s\n", fn.c_str());
					VehicleSceneNodeParams params;
					if (VehicleSceneNode::loadParamsFromFile(device, fn, params))
					{
						params.print();
						setUiData(params);
						vehicle->setup(device, params);
					}

				}

			}
			break;
			////////////////////////////////////////////////////////////////////
			case gui::EGET_DIRECTORY_SELECTED:
			{
				gui::IGUIFileOpenDialog* dialog = (gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
				core::stringw dirname = core::stringw(dialog->getDirectoryName());

				gui::IGUIElement* win = guienv->getRootGUIElement()->getElementFromId(GUI_ID_DIALOG_ROOT_WINDOW, true);
				if (!win)
					return false;

				if (id == GUI_ID_SELECT_DIR_DIALOG)
				{
					dialog->remove();
					win->getElementFromId(GUI_ID_SAVE_DIR, true)->setText(dirname.c_str());
				}
			}
			break;
			////////////////////////////////////////////////////////////////////

			default:
			break;
		}
	}

	if
	(
		(event.EventType == EET_GUI_EVENT)
		&& (event.GUIEvent.EventType == gui::EGET_MENU_ITEM_SELECTED)
	)
	{
		gui::IGUIContextMenu* menu = (gui::IGUIContextMenu*)event.GUIEvent.Caller;
		s32 id = menu->getItemCommandId(menu->getSelectedItem());

		if (id == GUI_ID_QUIT)
		{
			device->closeDevice();
			device->run();
			return true;
		}
		else if (id == GUI_ID_LOAD_PARAMS)
		{
			gui::IGUIFileOpenDialog* dialog
			= device->getGUIEnvironment()->addFileOpenDialog(L"Please select a file to load", false, 0, GUI_ID_LOAD_PARAMS_DIALOG);
			dialog->setRelativePositionProportional(core::rect<f32>(0.1f, 0.1f, 0.9f, 0.9f));
		}

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
	fs->addFileArchive(IRRLICHT_DIR);

	f32 camRotateSpeed = 100.0f;
	f32 camMoveSpeed = 0.01f;
	f32 camJumpSpeed = 3.0f;

	camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);
	camera->setFarValue(40000.0f);
	camera->setNearValue(0.1f);

	camera->setPosition(core::vector3df(5.5f,2.1f,-2.7f));
	camera->setTarget(core::vector3df(1.7f,1.9f,2.5f));

	////////////////////////////////////////////////////////////////////////////
	// create skybox
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
	vehicle = new VehicleSceneNode(0, smgr);

	////////////////////////////////////////////////////////////////////////////
	gui::IGUIFont* font = guienv->getFont("../../media/lucida.xml");
	if (font)
	{
		gui::IGUISkin* skin = guienv->getSkin();
		skin->setFont(font);
	}

	createToolBox();
	createMenu();


	////////////////////////////////////////////////////////////////////////////
	core::stringw helpText = L"Press:\n\tF1 to toggle help text on/off\n";
	helpText += L"\t'F2' to move camera near the vehicle\n";
	helpText += L"\t'F3' to show/hide cursor\n";
	helpText += L"\t     to disable/enable mouse input to fps camera\n";
	helpText += L"\t'F10' to hide/show toolset window\n";
	helpText += L"\t\n";
	helpText += L"\t'A' and 'D' to steer the front tires\n";
	helpText += L"\t'W' and 'S' to revolve/slow down the tires\n";
	helpText += L"\t'X' to stop tire revolution\n";
	helpText += L"\t\n";
	helpText += L"\t\n";
	helpText += L"\tESC to quit\n";

	////////////////////////////////////////////////////////////////////////////
	camera->setInputReceiverEnabled(false);
	device->getCursorControl()->setVisible(true);

	// ----------------------------------------------------
	u32 lastTime = device->getTimer()->getTime();
	u32 deltaTime = 0;

	while (device->run())
	if (device->isWindowActive())
	{
		deltaTime = device->getTimer()->getTime() - lastTime;
		vehicle->update(deltaTime);
		    driver->beginScene(true, true, video::SColor(0,0,110,110));

	//	driver->beginScene(true, true, 0);
		smgr->drawAll();

		Util::drawNodeLabel(device, vehicle->getFrontRightTireEmpty(), L"FR");
		Util::drawNodeLabel(device, vehicle->getFrontLeftTireEmpty(), L"FL");
		Util::drawNodeLabel(device, vehicle->getRearRightTireEmpty(), L"RR");
		Util::drawNodeLabel(device, vehicle->getRearLeftTireEmpty(), L"RL");

		Util::drawXYZAxes(device, vehicle->getRootEmpty()->getAbsolutePosition());
		if (showHelpScreen)
			drawHelpText(device, helpText, 5, 150);

		guienv->drawAll();
		driver->endScene();

		lastTime = device->getTimer()->getTime();
	}
	else { device->yield(); }

	if (vehicle) { delete vehicle; vehicle = 0; }
	//if (device) { device->drop(); }

	return 0;
}



