/*

Menu - cpp file

*/

#include "Menu.h"


// constructor
cMenu::cMenu() {
	mpos=0;
	exit=0;
	mousePosition.X=0;
	mousePosition.Y=0;
	mousePositionTemp=mousePosition=exMousePosition;
	menuEffectRightPosition=core::vector3df(22,31.5,40);
	menuEffectLeftPosition=menuEffectRightPosition+core::vector3df(17.5,0,-17.5);
}

// destructor
cMenu::~cMenu() {
}

void cMenu::Initialize(cInit *setInit) {

	init = setInit;
	device = init->getDevice();
	driver = device->getVideoDriver();
	eventReceiver = init->getEventReceiver();

	mouseCursor = driver->getTexture("media/cursor/pole_cursor.png");

	font = device->getGUIEnvironment()->getFont("media/font/curier.bmp");
	if (!font)
		font = device->getGUIEnvironment()->getBuiltInFont();
		
	menuItemName.push_back(L"singleplayer");
	menuItemName.push_back(L"multiplayer");
	menuItemName.push_back(L"options");
	menuItemName.push_back(L"eXit");

	// calculate position of menu items
	core::dimension2d<s32> windowSize = init->getDeviceParameters()->WindowSize;
	u32 centerPointX = (windowSize.Width-150)/2;
	u32 centerPointY = (windowSize.Height - menuItemName.size()*30)/2;
	u32 mult = (windowSize.Height/(menuItemName.size()*30));
	
	if (mult<1)
		mult=1;
	
	centerPointY -= (u32) (32.5f*mult - 32.5f);
	for (u32 i=0; i<menuItemName.size(); i++)
		menuItemPos.push_back(core::rect<s32>( centerPointX, i*30 + centerPointY , centerPointX+150 , i*30 + centerPointY+20 ));
		
	// creating menu effects
//	menuEffectRight = menuEffects.createMenuEffect(device, true);		
//	menuEffectLeft = menuEffects.createMenuEffect(device, false);
}

void cMenu::drawMenu() {
	for (u32 i=0; i<menuItemName.size(); i++)
		font->draw((wchar_t*) menuItemName[i],
			menuItemPos[i],
			video::SColor(255, (mpos==i)?(200):(100), (mpos==i)?(200):(100), (mpos==i)?(100):(100) ),true,true);
			
	device->getCursorControl()->setVisible(false);

	driver->draw2DImage(mouseCursor,
						mousePosition,
						core::rect<s32>(0,0,25,36),0,
						video::SColor(255,255,255,255),true);

	mousePositionTemp=mousePosition;
	
//	menuEffectRight->setPosition(menuEffectRightPosition -mpos*core::vector3df(0,5.2f,0));
//	menuEffectLeft->setPosition(menuEffectLeftPosition - mpos*core::vector3df(0,5.2f,0));
}

bool cMenu::checkInput() {
	
	mousePosition.X = eventReceiver->mouseX;
	mousePosition.Y = eventReceiver->mouseY;

	if (eventReceiver->checkKey(KEY_DOWN)) {
		(mpos==3) ? (mpos=0) : (mpos++);
		return true;
	}
		
	if (eventReceiver->checkKey(KEY_UP)) {
		(mpos==0) ? (mpos=3) : (mpos--);
		return true;
	}
		
	if (eventReceiver->checkKey(KEY_ESCAPE)) {
		exit=-1;
		return true;
	}
	
	for (u32 n=0; n<4; n++) {
		
		if (menuItemPos[n].isPointInside(core::position2d<s32>(eventReceiver->mouseX,eventReceiver->mouseY))) {
			mpos=n;

			if (eventReceiver->mouseInput==irr::EMIE_LMOUSE_PRESSED_DOWN) {
			
				switch (mpos) {
				case 0:
					exit=1;
					break;
				case 3:
					exit=-1;
					break;	
				}
				
				return true;
			}
		}
	}
	
	if (eventReceiver->checkKey(KEY_RETURN)) {
		
		switch (mpos) {
		case 0:
			exit=1;
			break;
		case 3:
			exit=-1;
			break;
		}
		
		return true;
	}
	
	return false;
}

s32 cMenu::getExit() {
	s32 x = exit;
	exit=0;
	return x;
}
