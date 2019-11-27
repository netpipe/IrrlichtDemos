/*

Hud - cpp file

*/

#include "Hud.h"

// constructor
cHud::cHud() {
	timeTemp=0;
	color=255;
	colorSpeed=17;
	playerImageFlip=false;
	pMode=HUD_PLAYER_STAND;
	pModeLast=HUD_PLAYER_STAND;
	radarMode=2;
}
	
// destructor
cHud::~cHud() {
	timeFlip=false;
	playerImageFlip=false;
	playerImage = core::rect<s32>(64,0,128,64);
}
	
// initialization
void cHud::Initialize(cInit *init) {
	
	IrrlichtDevice *device = init->getDevice();
	
	parameters = init->getDeviceParameters();
	
	video::IVideoDriver *driver = device->getVideoDriver();

	gui::IGUIEnvironment *environment = device->getGUIEnvironment();	

	images = driver->getTexture("media/hud/player_rocket.png");

	radarImage = driver->getTexture("media/hud/radar.png");
	
//	fontBig = environment->getFont("media/hud/bigfont.png");
	
}

// draw hud
void cHud::drawHud(IrrlichtDevice *device, scene::ICameraSceneNode *camera, cRocket *rocket, cPlayer *player, cPole *poles) {
	
	video::IVideoDriver *driver = device->getVideoDriver();
	irr::u32 time = device->getTimer()->getTime();
	scene::ISceneManager *smgr = device->getSceneManager();
	scene::ISceneCollisionManager *cmgr = smgr->getSceneCollisionManager();
	
	core::vector3df lookAt = camera->getTarget()-camera->getAbsolutePosition();
	lookAt.normalize();
	
	bool playerMode=player->getPlayerMode();
	
	switch (pMode) {
	case HUD_PLAYER_STAND:

		if (pModeLast!=pMode)
			playerImageFlip=true;
		
		if (playerImageFlip==false) {
			playerImage = core::rect<s32>(64,0,128,64);
		}
		
		break;
	case HUD_PLAYER_WALK:
		
		if (pModeLast!=pMode)
			playerImageFlip=true;
		
		if (playerImageFlip==false) {
			playerImage = core::rect<s32>(0,0,64,64);
		}
		
		break;
	case HUD_PLAYER_ROCKET:
		
		if (pModeLast!=pMode)
			playerImageFlip=true;
		if (playerImageFlip==false) {
			playerImage = core::rect<s32>(128,0,256,64);
		}

		break;	
	};
	
	if (playerImageFlip) {
		if (timeTemp!=time)
			color-=colorSpeed;
		if (color==0)
			playerImageFlip=false;
		timeTemp=time;		
	}
	
	if (playerImageFlip==false && color<255)
		color+=colorSpeed;


	driver->draw2DImage(images, core::position2d<s32>(0,0),
		playerImage, 0, 
		video::SColor(color,color,255,255), true);
		
/*	driver->draw2DImage(images, core::position2d<s32>(0, 600-70 ),
		core::rect<s32>(0,65,65,128), 0, 
		video::SColor(255,255,0,0), true);
		
	fontBig->draw(L"100%", core::rect<s32>(40,565,500,600), video::SColor(255,255,0,0));
		

	// score
	driver->draw2DImage(images, core::position2d<s32>(560, 0 ),
		core::rect<s32>(65,65,128,128), 0, 
		video::SColor(255, 255 , 255 , 255 ), true);
		
	fontBig->draw(L"Score: 0", core::rect<s32>(620, 85-70, 620+100, 85-70+40),
					video::SColor(255,217,216,183));
	
	// engine
	driver->draw2DImage(images, core::position2d<s32>(670,600-70),
		core::rect<s32>(128,64,192,128), 0, 
		video::SColor(255,179,162,150), true);

	fontBig->draw(L"100%", core::rect<s32>(715,565,800,600), video::SColor(255,179,162,150));
*/	
	
	pModeLast=pMode;
	
	core::position2d<s32> center;
	center = core::position2d<s32>(parameters->WindowSize.Width/2, parameters->WindowSize.Height/2 );
	
	core::array<core::vector3df> pos3d = poles->getPolePosition();
	
	// drawing radar
	if (radarMode != 0) {
		
		// draw ancient radar
		if (radarMode==1) {
			driver->draw2DImage(radarImage,
				core::position2d<s32>(center.X-105,2*center.Y-100),
				core::rect<s32>(0,70,211,170),0,
				video::SColor(255,255,255,255),true);
		}
		
		bool mode;
		(radarMode==1) ? (mode=true) : (mode=false);
		
		// draw radar pols
		for (irr::u32 i=0; i<pos3d.size(); i++) {
			core::vector3df vec = pos3d[i] + core::vector3df(0,1,0);
			core::position2d<s32> pos2d = cmgr->getScreenCoordinatesFrom3DPosition(vec,camera);
			pos2d = radar.calcRadarPosition(mode, lookAt, camera->getPosition()-vec, pos2d, center);			

			if (radarMode==1) {
				driver->draw2DRectangle(video::SColor(255,255,255,255),
										core::rect<s32>(pos2d.X,pos2d.Y,pos2d.X+3,pos2d.Y+3));
			} else {
				driver->draw2DImage(radarImage,
					pos2d - core::position2d<s32>(17,17),
					core::rect<s32>(0,0,35,35),0,
					video::SColor(255,255,255,255),true);
			}

		}
		
/*		core::position2d<s32> circlePos;
		for (s32 i=-100; i<100; i++) {
			circlePos.X = (s32) 300*sin(i);
			circlePos.Y = (s32) 150*cos(i);
			
			circlePos.X += center.X;
			circlePos.Y += center.Y;

			device->getVideoDriver()->draw2DPolygon(circlePos, 1.f, video::SColor(70,255,255,110),2);
		}
*/	
		if (!playerMode)
			return;
	
		pos3d.clear();
		pos3d.push_back(rocket->getRocketSceneNode()->getAbsolutePosition());
	
		// draw radar rockets
		for (irr::u32 i=0; i<pos3d.size(); i++) {
			core::vector3df vec = pos3d[i];
			core::position2d<s32> pos2d = cmgr->getScreenCoordinatesFrom3DPosition(vec,camera);
			pos2d = radar.calcRadarPosition(mode, lookAt, camera->getPosition()-vec, pos2d, center);

			if (radarMode==1) {
				driver->draw2DRectangle(video::SColor(255,255,100,100),
										core::rect<s32>(pos2d.X,pos2d.Y,pos2d.X+3,pos2d.Y+3));
			} else {
				driver->draw2DImage(radarImage,
					pos2d - core::position2d<s32>(17,17),
					core::rect<s32>(35,0,70,35),0,
					video::SColor(255,255,255,255),true);
			}
		}
	}
		
}

// setting player mode to show in hud
void cHud::setHudPlayerMode(HUD_PLAYER_MODE mode) {
	pMode=mode;
}

// get radar mode none=0/old=1/new=2
const irr::u32 cHud::getRadarMode() {
	return radarMode;
}
	
// set radar mode none=0/old=1/new=2
void cHud::setRadarMode(irr::u32 set) {
	if (radarMode<0 || radarMode>2)
		radarMode=0;
	else
		radarMode=set;
}

