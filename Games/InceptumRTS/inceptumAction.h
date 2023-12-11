#pragma once
#include <irrlicht.h>
#include <vector>
#include "inceptumUnit.h"


using namespace irr;

class MyEventReceiver;

// ===========================================================================
class Action
{
protected:
	// ========================================================
	IrrlichtDevice *device;
	scene::ISceneManager* smgr;
	video::IVideoDriver* driver;
	MyEventReceiver* receiver;
	f32* deltaTime;

	// cursor =================================================
	gui::ICursorControl *cursor;
	video::ITexture* cursorImage;
	core::position2d<s32> cursorPosCurrent, cursorPosSaved, screenCenter, screenEdges;
	s32 deltaX, deltaY;
	bool MouseDown[2];

	// mouse terrain ray cast =================================
	scene::ITerrainSceneNode* terrain;
	scene::ISceneCollisionManager* collisionManager;
	core::line3d<f32> ray;
	scene::ITriangleSelector* selector;
	core::vector3df point;
	core::triangle3df triangle;
	scene::ISceneNode* node;
	bool commandAnticipated, selectionAnticipated, commandModeOn;
	unsigned unitIDX = 0;

	// camera =================================================
	scene::ICameraSceneNode* camera;
	f32 camSpeed, camHeight, camRadius, recipsqrt2, tarHeight;
	core::vector3df camPos, camTar1, camTar2; // camera position and camera target
	core::vector2d<f32> direction; // of camera facing (camTar - camPos).Norm (normalized)
	f32 Xup, Yup; // X and Y updates for translation
	f32 currentHight;
	int n; // translation state

	// camera rotation optimization variables =================
	bool rotationMode;
	f32 cosA, sinA;
	f32 rotSpeed, currentAngle, tempAngle, aSinA, bSinA;
	f32 cosASq, aSinASq, bSinASq, bSinATarTemp2X, cosATarTemp2Y, aSinATarTemp2Z;
	void YTranslation();
	void XZTranslation();
	void MouseActions();

	// units // ==============================================
	int xvc;
	std::vector<UnitClass> unit;
public:
	Action(MyEventReceiver* const receiver, IrrlichtDevice* const device,
		scene::ITerrainSceneNode* const terrain, f32* const deltaTime);
	void update();
	void DrawCursor();
};

// ===========================================================================
class MyEventReceiver : public IEventReceiver
{
protected:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MouseDown[2];
	bool wheel[2];
public:
	MyEventReceiver();
	bool OnEvent(const SEvent& event);
	bool IsKeyDown(EKEY_CODE keyCode) const;
	bool isMouseLDown() const;
	bool isMouseRDown() const;
	bool isWheelUp() const;
	bool getWheelState() const;
	void setWheelState(bool wheel1);
};
