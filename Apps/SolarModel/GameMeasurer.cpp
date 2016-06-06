#include "GameGlobals.h"

void GameMeasurer::Init()
{
	imgMain = 0;
	butClose = 0;

	ruler = 0;
	rulerInfo = 0;
	rulerNet = 0;
	rulerLine = 0;
	curMouseDistance = 0;

	msLBPos = core::position2d<s32>();
	msLB = false;

	userPos = core::position2df(0.0f, 0.0f);
	userZoom = 512.0f; // 512.0f == Sun 1.0x zoom (512x512 pixels)
	zoomMul = 0.0f;
}

void GameMeasurer::Open()
{
	// Setup main image

	imgMain = Game.GUI->addImage(core::rect<s32>(0,0,Game.Width,Game.Height));

	if (imgMain)
	{
		// Switching off all 3D objects, otherwise we will
		// render all 3D objects that do not visible.
		// Note: we will switch on them back at GameMeasurer::Close().
		Game.Scene->getRootSceneNode()->setVisible(false);
	}
	else
	{
		return;
	}

	imgMain->setScaleImage(true);
	imgMain->setImage(Game.Video->getTexture("media_measurer/space.png"));

	// Setup all space bodies

	bodies.clear();
	BodyAdd(L"Sun", "media_measurer/sun.jpg", 1392.0f, 0.0f);
	BodyAdd(L"Mercury", "media_measurer/mercury.jpg", 4.879f, 57930.0f);
	BodyAdd(L"Venus", "media_measurer/venus.jpg", 12.104f, 108200.0f);
	BodyAdd(L"Earth", "media_measurer/earth.jpg", 12.756f, 149600.0f);
	BodyAdd(L"Mars", "media_measurer/mars.jpg", 6.788f, 225000.0f);
	BodyAdd(L"Jupiter", "media_measurer/jupiter.jpg", 142.984f, 778330.0f);
	BodyAdd(L"Saturn", "media_measurer/saturn.jpg", 120.536f*2.3f, 1426980.0f); // *2.3 because it has rings (and 1024x1024)
	BodyAdd(L"Uranus", "media_measurer/uranus.jpg", 51.118f*2.0f, 2871000.0f); // *2.0 because it has rings (and 1024x1024)
	BodyAdd(L"Neptune", "media_measurer/neptune.jpg", 49.520f, 4497000.0f);

	// Setup GUI

	Game.GUI->addStaticText(
		L"[i] Hold left mouse button and drag to navigate\n"
		L"[i] Use mouse wheel to zoom in/out",
		core::rect<s32>(12, Game.Height-48, Game.Width-12, Game.Height-8),
		false, true, imgMain);

	ruler = Game.GUI->addImage(
		Game.Video->getTexture("media_gui/gui_measurer_ruler.png"),
		core::position2d<s32>((Game.Width-1024)/2,Game.Height-112), true, imgMain);

	rulerNet = Game.GUI->addImage(
		Game.Video->getTexture("media_gui/gui_measurer_net.png"),
		core::position2d<s32>((Game.Width-1024)/2,Game.Height-160), true, imgMain);
	rulerNet->setScaleImage(true);
	rulerNet->setRelativePosition(core::rect<s32>((Game.Width-1024)/2,64,(Game.Width+1024)/2,Game.Height-92));

	rulerLine = Game.GUI->addImage(
		Game.Video->getTexture("media_gui/gui_measurer_line.png"),
		core::position2d<s32>((Game.Width-1024)/2,Game.Height/2), true, imgMain);

	rulerInfo = Game.GUI->addStaticText(L"",
		core::rect<s32>(16, Game.Height-72, Game.Width-16, Game.Height-56),
		false, true, imgMain);
	rulerInfo->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);

	Game.GUI->addStaticText(L"Mouse Pointing At",
		core::rect<s32>(16, Game.Height-48, Game.Width-12, Game.Height-28),
		false, false, imgMain)->
			setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_CENTER);

	curMouseDistance = Game.GUI->addStaticText(L"0",
		core::rect<s32>(16, Game.Height-32, Game.Width-12, Game.Height),
		false, false, imgMain);
	curMouseDistance->setTextAlignment(gui::EGUIA_LOWERRIGHT, gui::EGUIA_CENTER);

	butClose = Game.GUI->addButton(
		Game.GUI->getRootGUIElement()->getElementFromId(780, false)->getAbsolutePosition(),
		0, -1, L"Close", L"Exit Measurer");

	// Force recalculation process

	BodyUserZoomChanged();
}

void GameMeasurer::Close()
{
	butClose->remove();
	butClose = 0;

	imgMain->remove();
	imgMain = 0;

	Game.Scene->getRootSceneNode()->setVisible(true);
}

bool GameMeasurer::Event(SEvent event)
{
	if (imgMain == 0) return false;

	if (event.EventType == EET_GUI_EVENT)
	{
		if (event.GUIEvent.Caller == butClose)
		{
			if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
			{
				Close();
				return true;
			}
		}
	}

	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
		{
			msLB = true;
			msLBPos.X = event.MouseInput.X;
			msLBPos.Y = event.MouseInput.Y;
			msLBuserPos = userPos;
			return false; // not true
		}

		if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
		{
			msLB = false;
			return false; // not true
		}

		if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
		{
			if (msLB)
			{
				float dx = float(event.MouseInput.X-msLBPos.X);

				userPos = msLBuserPos - core::position2df(dx/(2*zoomMul), 0);
				BodyUserPosChanged();

				return false; // not true
			}


			float d = GetDistanceFromScreenXCoord(event.MouseInput.X);
			core::stringw s = GetDistanceAsString(fabs(d)*1000);
			curMouseDistance->setText(s.c_str());
		}

		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
		{
			userPos.X += float(event.MouseInput.X)/(2*zoomMul); // {
			userZoom += userZoom*float(event.MouseInput.Wheel)/16;
			BodyUserZoomChanged();
			userPos.X -= float(event.MouseInput.X)/(2*zoomMul); // }
			BodyUserPosChanged();

			return false; // not true
		}

		//GameLog("Mouse: X=%u, Y=%u, Wheel=%f", event.MouseInput.X, event.MouseInput.Y, event.MouseInput.Wheel);
	}

	return false;
}

void GameMeasurer::Done()
{
	// nothing
}

bool GameMeasurer::IsVisible()
{
	return imgMain != 0;
}

void GameMeasurer::BodyAdd(wchar_t *name, char *tex, float size, float dist)
{
	Body b;
	b.name = name;
	b.dist = dist;
	b.size = size;
	
	b.guiImage = Game.GUI->addImage(Game.Video->getTexture(tex), core::position2d<s32>(), true, imgMain);
	b.guiImage->setScaleImage(true);

	b.guiText = Game.GUI->addStaticText(name, core::rect<s32>(), false, false, imgMain);
	b.guiText->setOverrideColor(video::SColor(128,255,255,255));
	b.guiText->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);

	bodies.push_back(b);
}

int GameMeasurer::GetScreenXCoordFromDistance(float distance)
{
	return int((distance-userPos.X) * zoomMul * 2);
}

float GameMeasurer::GetDistanceFromScreenXCoord(int screenXCoord)
{
	return (screenXCoord/(zoomMul*2)) + userPos.X;
}

void GameMeasurer::BodyUserPosChanged()
{
	gui::IGUIFont *font = Game.Skin->getFont();

	// Defining places at the left and right borders of the screen

	int fontHeight = font->getDimension(L"Qq").Height;
	core::array<int> lPlaces, rPlaces;
	for (u32 i=0; i<bodies.size(); i++)
	{
		lPlaces.push_back(32 + i*fontHeight);
		rPlaces.push_back(32 + i*fontHeight);
	}

	// Defining position of bodies

	for (u32 i=0; i<bodies.size(); i++)
	{
		Body &b = bodies[i];

		// for body.guiImage

		float x = (float)GetScreenXCoordFromDistance(b.dist);
		float y = Game.Height/2.0f;
		float d = b.size*zoomMul;

		b.guiImage->setRelativePosition( // minimum body.image size 1x1
			core::rect<s32>(int(x-d), int(y-d), int(x+d+1), int(y+d+1)));

		// for body.guiText
		// (next lines of code scaries... yes i know that :)

		core::stringw t = b.name;
		bool canGotNewT = true;
		goto skipFalse;
gotNewT:
		canGotNewT = false;
skipFalse:
		core::dimension2d<s32> dim = font->getDimension(t.c_str());
		int tX = int(x-dim.Width/2);
		if (tX < 0) { if (canGotNewT) {t=core::stringw("<-")+t; goto gotNewT;} else tX=0; }
		if (tX+dim.Width > Game.Width) { if (canGotNewT) {t+="->"; goto gotNewT;} else tX=Game.Width-dim.Width; }

		int tY = 32;
		if (t[0] == L'<')
		{
			for (u32 i=0; i<lPlaces.size(); i++)
			{
				if (lPlaces[i]>0) { tY=lPlaces[i]; lPlaces[i]=0; break; }
			}
		}
		else if (t[t.size()-1] == L'>')
		{
			for (u32 i=0; i<rPlaces.size(); i++)
			{
				if (rPlaces[i]>0) { tY=rPlaces[i]; rPlaces[i]=0; break; }
			}
		}

		b.guiText->setRelativePosition(
			core::rect<s32>(tX, tY, tX+dim.Width, tY+dim.Height));

		b.guiText->setText(t.c_str());
	}
}

void GameMeasurer::BodyUserZoomChanged()
{
	zoomMul = (userZoom/(2*bodies[0].size));

	float kkm = (bodies[0].size/(8*userZoom))*5120000.0f;
	core::stringw s = GetDistanceAsString(kkm);
	rulerInfo->setText(s.c_str());

	BodyUserPosChanged();
}

core::stringw GameMeasurer::GetDistanceAsString(float distanceKM)
{
	wchar_t s[1024];

	if (distanceKM < 100)
	{
	//	_snwprintf(s, sizeof(s), L"%.0lf m", distanceKM*1000.0f);
	}
	else
	{
		// 1 au == 149,597,871 km
		wchar_t au[512];
	//	_snwprintf(au, sizeof(au), L" (%.3lf au)",
	//		distanceKM/149597871.0f);

		// 1 ly == 9,460,730,472,581 km
		wchar_t ly[512];
	//	_snwprintf(ly, sizeof(ly), L" (%.3lf ly)",
	//		distanceKM/9460730472581.0f);

	//	_snwprintf(s, sizeof(s), L"%.0lf km%s%s",
	//		distanceKM, distanceKM>149598?au:L"", distanceKM>9460730473?ly:L"");
	}

	return core::stringw(s);
}
