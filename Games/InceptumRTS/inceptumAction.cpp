#include "inceptumAction.h"
#include <vector>
#include "inceptumUnit.h"
#include "inceptumEnum.h"

using namespace irr;

// Action // =====================================================================
Action::Action(MyEventReceiver* const receiver, IrrlichtDevice* const device,
	scene::ITerrainSceneNode* const terrain, f32* const deltaTime)
{
	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->receiver = receiver;
	this->deltaTime = deltaTime;
	this->terrain = terrain;
	this->cursor = device->getCursorControl();
	this->rotationMode = false;

	// set parameters ============================================================
	this->camHeight = 150;
	this->tarHeight = 50;
	this->camSpeed = 0.3f;
	this->rotSpeed = 1.0f / 550.0f;

	// mouse terrain ray cast ====================================================
	this->selector = smgr->createTerrainTriangleSelector(terrain);
	this->terrain->setTriangleSelector(selector);
	selector->drop();
	this->collisionManager = smgr->getSceneCollisionManager();

	//============================================================================
	core::vector3df camPos = core::vector3df(360, 128, 360);
	core::vector3df camTar = core::vector3df(511, 0, 512);
	f32 terrainHeightTemp = terrain->getHeight(camPos.X, camPos.Z);
	camPos.Y = camHeight + terrainHeightTemp;
	camTar.Y = tarHeight + terrainHeightTemp;
	camRadius = sqrt(camPos.X*camPos.X + camTar.Z*camTar.Z);
	this->camera = smgr->addCameraSceneNode(0, camPos, camTar);

	direction.X = camera->getTarget().X - camera->getPosition().X;
	direction.Y = camera->getTarget().Z - camera->getPosition().Z;
	direction.normalize();
	recipsqrt2 = core::reciprocal_squareroot((f32)2);

	// ==========================================================================
	this->cursorImage = driver->getTexture("./media/cursor.png");

	// ==========================================================================
	core::dimension2du tempVar = driver->getScreenSize();
	screenCenter.X = tempVar.Width / 2; screenCenter.Y = tempVar.Height / 2;
	screenEdges.X = tempVar.Width - 2; screenEdges.Y = tempVar.Height - 2;

	// ==========================================================================
	UnitClass::setPointers(device, terrain, deltaTime);

	unit.push_back(UnitClass());
	unit[0].Initialize(core::vector2d<f32>(312, 412), 0, false);
	unit.push_back(UnitClass());
	unit[1].Initialize(core::vector2d<f32>(212, 212), 0, true);
}

void Action::update()
{
	YTranslation();

	XZTranslation();

	MouseActions();

	camera->setPosition(camPos);
	camera->setTarget(camTar1);
}

void Action::DrawCursor()
{
	if (!rotationMode)
	{
		cursorPosCurrent = cursor->getPosition();
		driver->draw2DImage(cursorImage, core::rect<s32>(
			cursorPosCurrent.X, cursorPosCurrent.Y,
			cursorPosCurrent.X + 40, cursorPosCurrent.Y + 40),
			core::rect<s32>(0, 0, 64, 64), 0, 0, true);
	}
}

void Action::YTranslation()
{
	if (receiver->getWheelState())
	{
		receiver->setWheelState(false);
		if (receiver->isWheelUp())
		{
			if (camHeight > 50)
			{
				camHeight -= 20.0f;
				tarHeight -= 20.0f;
				camSpeed -= 0.02f;
			}
		}
		else
		{
			if (camHeight < 210)
			{
				camHeight += 20.0f;
				tarHeight += 20.0f;
				camSpeed += 0.02f;
			}
		}
	}
}

void Action::XZTranslation()
{
	n = (receiver->IsKeyDown(KEY_KEY_W) << 0) | (receiver->IsKeyDown(KEY_KEY_A) << 1)
		| receiver->IsKeyDown(KEY_KEY_S) << 2 | receiver->IsKeyDown(KEY_KEY_D) << 3;

	cursorPosCurrent = cursor->getPosition();
	if (cursorPosCurrent.Y < 1)
		n |= 1 << 0;
	else if (cursorPosCurrent.Y > screenEdges.Y)
		n |= 1 << 2;
	if (cursorPosCurrent.X < 1)
		n |= 1 << 1;
	else if (cursorPosCurrent.X > screenEdges.X)
		n |= 1 << 3;

	// =========================================================================
	camPos = camera->getPosition();
	camTar1 = camera->getTarget();

	if (n != 0)
	{
		switch (n)
		{
			// up stands for update (delta)
		case 1:
			Xup = (f32)camSpeed * *deltaTime * direction.X;
			Yup = (f32)camSpeed * *deltaTime * direction.Y;
			break;
		case 2:
			Xup = (f32)-camSpeed * *deltaTime*direction.Y;
			Yup = (f32)camSpeed * *deltaTime*direction.X;
			break;
		case 8:
			Xup = (f32)camSpeed * *deltaTime*direction.Y;
			Yup = (f32)-camSpeed * *deltaTime*direction.X;
			break;
		case 4:
			Xup = (f32)-camSpeed * *deltaTime * direction.X;
			Yup = (f32)-camSpeed * *deltaTime * direction.Y;
			break;
		case 3:
			Xup = (f32)camSpeed * *deltaTime * (-direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
			break;
		case 9:
			Xup = (f32)camSpeed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (-direction.X + direction.Y) * recipsqrt2;
			break;
		case 6:
			Xup = (f32)-camSpeed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (direction.X - direction.Y) * recipsqrt2;
			break;
		case 12:
			Xup = camSpeed * *deltaTime * (direction.Y - direction.X) * recipsqrt2;
			Yup = -camSpeed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
			break;
		}

		// border collision + apply update
		if (camPos.X < 100)
		{
			if (Xup > 0)
			{
				camPos.X += Xup;
				camTar1.X += Xup;
			}
		}
		else if (camPos.X > 924)
		{
			if (Xup < 0)
			{
				camPos.X += Xup;
				camTar1.X += Xup;
			}
		}
		else
		{
			camPos.X += Xup;
			camTar1.X += Xup;
		}

		if (camPos.Z < 100)
		{
			if (Yup > 0)
			{
				camPos.Z += Yup;
				camTar1.Z += Yup;
			}
		}
		else if (camPos.Z > 924)
		{
			if (Yup < 0)
			{
				camPos.Z += Yup;
				camTar1.Z += Yup;
			}
		}
		else
		{
			camPos.Z += Yup;
			camTar1.Z += Yup;
		}
	}

	currentHight = terrain->getHeight(camPos.X, camPos.Z);
	camPos.Y = 0.85f*camPos.Y + 0.15f*(currentHight + camHeight);
	camTar1.Y = 0.85f*camTar1.Y + 0.15f*(currentHight + tarHeight);
}

void Action::MouseActions()
{
	// ==============================================================
	MouseDown[1] = receiver->isMouseRDown();
	MouseDown[2] = receiver->isMouseLDown();
	if (MouseDown[1] && MouseDown[2])
	{
		commandModeOn = false;
		commandAnticipated = false;
		selectionAnticipated = false;
		// get cursor data
		if (!rotationMode)
		{
			rotationMode = true;
			cursorPosSaved = cursor->getPosition();
			cursor->setPosition(screenCenter);
			deltaX = 0;
			deltaY = 0;
		}
		else
		{
			deltaX = screenCenter.X - cursorPosCurrent.X;
			deltaY = screenCenter.Y - cursorPosCurrent.Y;
		}
		// recet cursor position to center
		cursor->setPosition(screenCenter);

		// define deltas of cursor data
		deltaX = (deltaX < 130) ? deltaX : 130; deltaX = (deltaX > -130) ? deltaX : -130;
		deltaY = (deltaY < 130) ? deltaY : 130; deltaY = (deltaY > -130) ? deltaY : -130;

		// complex number rotation along azimuthal angle theta ===================
		tempAngle = deltaX*rotSpeed;
		cosA = cos(tempAngle);
		sinA = sin(tempAngle);

		camTar2 = camTar1 - camPos;

		camTar1.X = cosA*camTar2.X - sinA*camTar2.Z;
		camTar1.Y = camTar2.Y;
		camTar1.Z = cosA*camTar2.Z + sinA*camTar2.X;

		direction.X = camTar1.X;
		direction.Y = camTar1.Z;
		direction.normalize();

		// quaternion rotation along polar angle phi =============================

		tempAngle = deltaY*rotSpeed / 2;
		currentAngle = acos(camTar1.Y / camTar1.getLength());

		if (currentAngle > 2.84f)
			tempAngle = tempAngle > 0 ? tempAngle : 0;
		else if (currentAngle < 0.3f)
			tempAngle = tempAngle < 0 ? tempAngle : 0;

		cosA = cos(tempAngle);
		sinA = sin(tempAngle);

		aSinA = -sinA*direction.Y;
		bSinA = sinA*direction.X;

		cosASq = cosA*cosA;
		aSinASq = aSinA*aSinA;
		bSinASq = bSinA*bSinA;

		bSinATarTemp2X = bSinA*camTar1.X;
		cosATarTemp2Y = cosA*camTar1.Y;
		aSinATarTemp2Z = aSinA*camTar1.Z;

		camTar2.X = camTar1.X*(cosASq + aSinASq - bSinASq) + 2.0f * bSinA*(aSinATarTemp2Z - cosATarTemp2Y);
		camTar2.Y = camTar1.Y*(cosASq - aSinASq - bSinASq) + 2.0f * cosA*(bSinATarTemp2X - aSinATarTemp2Z);
		camTar2.Z = camTar1.Z*(cosASq - aSinASq + bSinASq) + 2.0f * aSinA*(bSinATarTemp2X + cosATarTemp2Y);

		tarHeight = camTar2.Y + camHeight;

		camTar1 = camTar2 + camPos;
	}
	else
	{
		if (rotationMode)
		{
			rotationMode = false;
			cursor->setPosition(cursorPosSaved);
		}
		if (commandModeOn)
		{
			if (MouseDown[1] && !MouseDown[2])
			{
				commandAnticipated = true;
				ray = collisionManager->getRayFromScreenCoordinates(cursorPosCurrent);
			}
			else if (!MouseDown[1] && MouseDown[2])
			{
				selectionAnticipated = true;
				ray = collisionManager->getRayFromScreenCoordinates(cursorPosCurrent);
			}

			if (commandAnticipated && !MouseDown[1])
			{
				commandAnticipated = false;
				if (collisionManager->getCollisionPoint(ray, selector, point, triangle, node))
				{
					unit[unitIDX].setPosition(point);
				}
			}
			else if (selectionAnticipated && !MouseDown[2])
			{
				selectionAnticipated = false;
				scene::ISceneNode* selectedSceneNode =
					collisionManager->getSceneNodeAndCollisionPointFromRay(
					ray,
					point, // This will be the position of the collision
					triangle,
					Pickable);
				if (selectedSceneNode)
				{
					point = selectedSceneNode->getPosition();
					point.X = point.X + 3;
					selectedSceneNode->setPosition(point);
				}
			}
		}
	}
	if (!MouseDown[1] && !MouseDown[2])
		commandModeOn = true;
}

// MyEventReceiver //=============================================================
MyEventReceiver::MyEventReceiver()
{
	wheel[1] = false;
	wheel[2] = false;
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if (event.MouseInput.isLeftPressed())
			MouseDown[1] = true;
		else
			MouseDown[1] = false;
		if (event.MouseInput.isRightPressed())
			MouseDown[2] = true;
		else
			MouseDown[2] = false;
	}

	if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
	{
		wheel[1] = true;
		wheel[2] = event.MouseInput.Wheel > 0;
	}

	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

bool MyEventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

bool MyEventReceiver::isMouseLDown() const
{
	return MouseDown[1];
}

bool MyEventReceiver::isMouseRDown() const
{
	return MouseDown[2];
}


bool MyEventReceiver::isWheelUp() const
{
	return wheel[2];
}

bool MyEventReceiver::getWheelState() const
{
	return wheel[1];
}

void MyEventReceiver::setWheelState(bool wheel1)
{
	wheel[1] = wheel1;
}
