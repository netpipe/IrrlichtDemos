/*
The Irrlicht Engine License

Copyright � 2002-2005 Nikolaus Gebhardt

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

//Solar System Simulator
//written by Tobias Houfek

#include <string>
#include <iostream>
//#include <direct.h>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <irrlicht.h>
#include <ctime>
#include "solar.h"
#include <stdio.h>
#include "CRTTSkyBoxSceneNode.h"
#include "CBatchingMesh.h"
#include "bsc.h"
#include "Geopack-2008.h"


//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
#pragma comment(lib,"irrlicht.lib")
//#pragma comment(lib,"libIrrlicht.a")

using namespace irr;
//using namespace irrklang;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* flyCam;
scene::ICameraSceneNode* splitSCam;
scene::ICameraSceneNode* mayaCam;
scene::ICameraSceneNode* planetCam;
scene::ILightSceneNode* light;
gui::IGUIEnvironment* env;
gui::ICursorControl* cursor;
gui::IGUIFont* fnt;
gui::IGUIStaticText* info;
gui::IGUIStaticText* info2;

SolarSystem* mySolarSystem = new SolarSystem();
scene::ISceneNode* world;

//altitude for geo synchronous orbit with earth
//should be 35786 km <-Wikipedia
//debugger says altitude 35862.678092090246	f64
//precision and testing says ~ 36460.0f;
//numbers are in a really realistic range.... :)
u32 startplanet = 2;
f64 startAlt = 35786;
f64 getAway =  35786;

//Day += (1/86400)*timescale; <-- is to small for f64
const f64 incrementF = 1.1574074074074074074074074074074e-5;
const f64 Gconst = 6.67428e-11;
const f64 dayV = 24.0;
const f64 minSecV = 60.0;
const f64 scaleF = 100000.0;

s32 normalSpec;

bool view2 = false;
bool updateFieldLines = true;

bool up=false,down=false,left=false,right=false,keyQ=false,keyE=false,
leftB=false,rightB=false,mouseW=false,upA=false,
downA=false,rightA=false,leftA=false,
key1=false,key2=false,key3=false,key4=false,key5=false,key6=false,key7=false,key8=false,key9=false,
keyF1=false,keyF2=false,key_shift=false,key_ctrl=false;

bool tangentView = false;

core::position2d<f32> CursorPos;
core::vector2df deltaC;
int Syear=2012,Smonth=12,Sday=25,Shour=12,Sminute=0,Ssecond=0;
u32 introseconds = 0;
u32 introcount = 0;

f64 lastWheelMovement;
f64 Day = 0;
f64 timescale = 10000;
f64 delta;
f64 fpsincrement;
f64 Width;
f64 Height;

f64 sunDistanceKM;
f64 gforce;
f64 speed;
f64 oSpeed;

f64 CspeedUD = 0.15;
f64 CspeedLR = 0.15;
f64 CspeedFB = 0.25;
f64 camSpeedr = 0;

f64 currentOrbitDistance;
f32 camOrbitDistance;

scene::ISceneNode* closestBody;
scene::ISceneNode* oldBody;
u32 closestIndex;
f64 closestDistance;

f32 lati = 90;
f32 longi = 0;

scene::IBillboardSceneNode* flightmarker;
scene::IBillboardSceneNode* flightmarker2;

scene::ISceneNode* allstars;
scene::ISceneNode* FieldLineNode;
scene::ISceneNode* FieldLineNodeGSW;

core::array<scene::ISceneNode*> planets;
core::array<scene::ISceneNode*> planetsR;
core::array<scene::ISceneNode*> moons;
core::array<scene::ISceneNode*> moonsR;

core::array<scene::IBillboardSceneNode*> trail;
core::array<scene::ISceneNode*> FieldLineCubes;

scene::IBillboardSceneNode* trail2[900];
u32 trailcount = 0;
u32 traillast = 0;

core::array<f64> planetSize;
core::array<f64> pMass;
core::array<f64> pRot;
core::array<f64> mDist;
core::array<f64> mSize;
core::array<f64> mFOrb;
core::array<f64> mMass;
core::array<f64> orbitT;

core::array<core::stringw> randomFacts;
core::array<core::stringw> intro;

core::array<core::vector3df> updatePos;

//ouch
core::array<core::array<core::vector3df> > FieldLines;

core::vector3df camPos = core::vector3df(0,0,0);
core::vector3df camVector = core::vector3df(0,0,0);
core::vector3df pull = core::vector3df(0,0,0);
core::vector3df startPos = core::vector3df(0,0,0);
core::vector3df flyAt = core::vector3df(0,0,0);
core::vector3df lastPos = core::vector3df(0,0,0);
core::vector3df relaPos = core::vector3df(0,0,0);

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		// set inverted world matrix
		// if we are using highlevel shaders (the user can select this when
		// starting the program), we must set the constants by name.

		core::matrix4 invWorld = driver->getTransform(video::ETS_WORLD);
		invWorld.makeInverse();

		services->setVertexShaderConstant("mInvWorld", invWorld.pointer(), 16);

		// set clip matrix

		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		core::vector3df posLight2 = light->getAbsolutePosition();
		services->setVertexShaderConstant("mLight2Pos", reinterpret_cast<f32*>(&posLight2), 3);

		// set camera position

		core::vector3df pos = device->getSceneManager()->getActiveCamera()->getAbsolutePosition();
		services->setVertexShaderConstant("mLightPos", reinterpret_cast<f32*>(&pos), 3);


		// set light color

		video::SColorf col(0.0f,1.0f,1.0f,0.0f);
		services->setVertexShaderConstant("mLightColor", reinterpret_cast<f32*>(&col), 4);
		video::SColorf fvAmbient(0.368f,0.368f,0.368f,1.0f);
		services->setVertexShaderConstant("fvAmbient", reinterpret_cast<f32*>(&fvAmbient), 4);
		video::SColorf fvSpecular(0.790f,0.488f,0.488f,1.0f);
		services->setVertexShaderConstant("fvSpecular", reinterpret_cast<f32*>(&fvSpecular), 4);
		video::SColorf fvDiffuse(0.886f,0.885f,0.885f,1.0f);
		services->setVertexShaderConstant("fvDiffuse", reinterpret_cast<f32*>(&fvDiffuse), 4);
		f32 fSpecularPower = 50.0f;
		services->setVertexShaderConstant("fSpecularPower", reinterpret_cast<f32*>(&fSpecularPower), 1);

		// set transposed world matrix

		core::matrix4 world = driver->getTransform(video::ETS_WORLD);
		world = world.getTransposed();

		services->setVertexShaderConstant("mTransWorld", world.pointer(), 16);

		//irr::core::vector3df camPos = camera->getAbsolutePosition();
		//services->setVertexShaderConstant("camPos", reinterpret_cast<f32*>(&camPos), 3);
	}
};
class MyeventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
		if(event.EventType == EET_KEY_INPUT_EVENT) {

			if(event.KeyInput.PressedDown == true) {
				if(event.KeyInput.Key==KEY_KEY_A)
					left=true;
				if(event.KeyInput.Key==KEY_KEY_D)
					right=true;
				if(event.KeyInput.Key==KEY_KEY_W)
					up=true;
				if(event.KeyInput.Key==KEY_KEY_S)
					down=true;
				if(event.KeyInput.Key==KEY_KEY_Q)
					keyQ=true;
				if(event.KeyInput.Key==KEY_KEY_E)
					keyE=true;
				if(event.KeyInput.Key==KEY_UP)
					upA=true;
				if(event.KeyInput.Key==KEY_DOWN)
					downA=true;
				if(event.KeyInput.Key==KEY_RIGHT)
					rightA=true;
				if(event.KeyInput.Key==KEY_LEFT)
					leftA=true;
				if(event.KeyInput.Key==KEY_KEY_1)
					key1=true;
				if(event.KeyInput.Key==KEY_KEY_2)
					key2=true;
				if(event.KeyInput.Key==KEY_KEY_3)
					key3=true;
				if(event.KeyInput.Key==KEY_KEY_4)
					key4=true;
				if(event.KeyInput.Key==KEY_KEY_5)
					key5=true;
				if(event.KeyInput.Key==KEY_KEY_6)
					key6=true;
				if(event.KeyInput.Key==KEY_KEY_7)
					key7=true;
				if(event.KeyInput.Key==KEY_KEY_8)
					key8=true;
				if(event.KeyInput.Key==KEY_KEY_9)
					key9=true;
				if(event.KeyInput.Key==KEY_F1)
					keyF1=true;
				if(event.KeyInput.Key==KEY_F2)
					keyF2=true;
				if(event.KeyInput.Key==KEY_SHIFT)
					key_shift=true;
				if(event.KeyInput.Key==KEY_KEY_Y) //irr::EKEY_CODE::KEY_RCONTROL
					key_ctrl=true;
			}

			if(event.KeyInput.PressedDown == false) {
				if(event.KeyInput.Key==KEY_KEY_A)
					left=false;
				if(event.KeyInput.Key==KEY_KEY_D)
					right=false;
				if(event.KeyInput.Key==KEY_KEY_W)
					up=false;
				if(event.KeyInput.Key==KEY_KEY_S)
					down=false;
				if(event.KeyInput.Key==KEY_KEY_Q)
					keyQ=false;
				if(event.KeyInput.Key==KEY_KEY_E)
					keyE=false;
				if(event.KeyInput.Key==KEY_UP)
					upA=false;
				if(event.KeyInput.Key==KEY_DOWN)
					downA=false;
				if(event.KeyInput.Key==KEY_RIGHT)
					rightA=false;
				if(event.KeyInput.Key==KEY_LEFT)
					leftA=false;
				if(event.KeyInput.Key==KEY_KEY_1)
					key1=false;
				if(event.KeyInput.Key==KEY_KEY_2)
					key2=false;
				if(event.KeyInput.Key==KEY_KEY_3)
					key3=false;
				if(event.KeyInput.Key==KEY_KEY_4)
					key4=false;
				if(event.KeyInput.Key==KEY_KEY_5)
					key5=false;
				if(event.KeyInput.Key==KEY_KEY_6)
					key6=false;
				if(event.KeyInput.Key==KEY_KEY_7)
					key7=false;
				if(event.KeyInput.Key==KEY_KEY_8)
					key8=false;
				if(event.KeyInput.Key==KEY_KEY_9)
					key9=false;
				if(event.KeyInput.Key==KEY_F1)
					keyF1=false;
				if(event.KeyInput.Key==KEY_F2)
					keyF2=false;
				if(event.KeyInput.Key==KEY_SHIFT)
					key_shift=false;
				if(event.KeyInput.Key==KEY_KEY_Y) //irr::EKEY_CODE::KEY_RCONTROL
					key_ctrl=false;
			}
		}if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			// left mouse button state check
			if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				leftB = true;
			}
			if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
			{
				leftB = false;
			}
			if(event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
			{
				rightB = true;
			}
			if(event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
			{
				rightB = false;
			}
			if(event.MouseInput.Event == EMIE_MOUSE_WHEEL)
			{
				lastWheelMovement = event.MouseInput.Wheel;
				mouseW = true;
			}
			if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
			{
				CursorPos = cursor->getRelativePosition();
			}
		}
		return false;
	}

}my_event_receiver;
//make
void readconfig()
{
	io::IXMLReader* xml = device->getFileSystem()->createXMLReader("../config.xml");
	pMass.set_used(10);
	while(xml && xml->read())
	{
		switch(xml->getNodeType())
		{
		case io::EXN_ELEMENT:
			{
			if (core::stringw("Mercury") == xml->getNodeName())
				pMass[0] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Venus") == xml->getNodeName())
				pMass[1] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Earth") == xml->getNodeName())
				pMass[2] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Mars") == xml->getNodeName())
				pMass[3] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Jupiter") == xml->getNodeName())
				pMass[4] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Saturn") == xml->getNodeName())
				pMass[5] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Uranus") == xml->getNodeName())
				pMass[6] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Neptune") == xml->getNodeName())
				pMass[7] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Pluto") == xml->getNodeName())
				pMass[8] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("Sun") == xml->getNodeName())
				pMass[9] = xml->getAttributeValueAsFloat(L"mass");
			else
			if (core::stringw("CamSpeedUpDown") == xml->getNodeName())
				CspeedUD = (xml->getAttributeValueAsFloat(L"speed"));
			else
			if (core::stringw("CamSpeedLeftRight") == xml->getNodeName())
				CspeedLR = (xml->getAttributeValueAsFloat(L"speed"));
			else
			if (core::stringw("CamSpeedForwardBackward") == xml->getNodeName())
				CspeedFB = (xml->getAttributeValueAsFloat(L"speed"));
			else
			if (core::stringw("Timestep") == xml->getNodeName())
				timescale = xml->getAttributeValueAsFloat(L"step");
			else
			if (core::stringw("year") == xml->getNodeName())
				Syear = xml->getAttributeValueAsInt(L"is");
			else
			if (core::stringw("month") == xml->getNodeName())
				Smonth = xml->getAttributeValueAsInt(L"is");
			else
			if (core::stringw("day") == xml->getNodeName())
				Sday = xml->getAttributeValueAsInt(L"is");
			else
			if (core::stringw("hour") == xml->getNodeName())
				Shour = xml->getAttributeValueAsInt(L"is");
			else
			if (core::stringw("minute") == xml->getNodeName())
				Sminute = xml->getAttributeValueAsInt(L"is");
			else
			if (core::stringw("second") == xml->getNodeName())
				Ssecond = xml->getAttributeValueAsInt(L"is");
			}
			break;
			default:
			break;
		}
	}
	CspeedLR = (CspeedLR*timescale/scaleF)/1500;
	CspeedUD = (CspeedUD*timescale/scaleF)/1500;
	CspeedFB = (CspeedFB*timescale/scaleF)/1500;
	if(xml)
		xml->drop(); // don't forget to delete the xml reader
}
void makeGUI()
{
	//create text elements-----------------------------------------------------------------------------
	fnt = env->getFont("../data/orator.xml");
	fnt->setKerningHeight(6);

	gui::IGUISkin* skin = env->getSkin();
	skin->setFont(fnt);
	skin->setColor(gui::EGDC_BUTTON_TEXT,video::SColor(255,0,150,204));

	info = env->addStaticText(
		L"",core::rect<s32>(Width*0.76,Height*0.6,Width,Height*0.75), true, true, 0, -1, true);
	info->setDrawBackground(false);
	info->setDrawBorder(false);
	info->setTextAlignment(gui::EGUIA_SCALE,gui::EGUIA_SCALE);

	info2 = env->addStaticText(
		L"",core::rect<s32>(Width*0.5,Height*0,Width,Height*0.05), true, true, 0, -1, true);
	info2->setDrawBackground(false);
	info2->setDrawBorder(false);
	info2->setTextAlignment(gui::EGUIA_CENTER,gui::EGUIA_CENTER);

	//create flightmarker aid--------------------------------------------------------------------------
	video::IImage *flightmarkerAidIm = driver->createImageFromFile("../data/flightmarker2.png");
	video::IImage *flightmarkerAidIm_scaled = driver->createImage(video::ECF_A8R8G8B8,core::dimension2d<u32>(Width,Height));
	flightmarkerAidIm->copyToScaling(flightmarkerAidIm_scaled);
	video::ITexture *flightmarkerAid =  driver->addTexture("scaled",flightmarkerAidIm_scaled);
	flightmarkerAidIm->drop();
	flightmarkerAidIm_scaled->drop();

	gui::IGUIImage* flightaid = env->addImage(flightmarkerAid,core::vector2di(0,0));
	flightaid->setUseAlphaChannel(true);
	flightaid->setColor(video::SColor(50,255,255,255));

	//create flightmarker------------------------------------------------------------------------------
	flightmarker = smgr->addBillboardSceneNode(0,core::dimension2df(0.07,0.07));
	flightmarker->setMaterialFlag(video::EMF_LIGHTING, false);
	flightmarker->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	video::ITexture *tex21 =  driver->getTexture("../data/flightmarker.jpg");
	flightmarker->setMaterialTexture(0,tex21);
}
void initVars()
{

	randomFacts.push_back(core::stringw("Mercury has a radius of around 2440 km"));
	randomFacts.push_back(core::stringw("Venus has a radius of around 6052 km"));
	randomFacts.push_back(core::stringw("Earth has a radius of around 6378 km"));
	randomFacts.push_back(core::stringw("Mars has a radius of around 3397 km"));
	randomFacts.push_back(core::stringw("Jupiter has a radius of around 71492 km"));
	randomFacts.push_back(core::stringw("Saturn has a radius of around 60268 km"));
	randomFacts.push_back(core::stringw("Uranus has a radius of around 25559 km"));
	randomFacts.push_back(core::stringw("Neptune has a radius of around 2440 km"));
	randomFacts.push_back(core::stringw("Pluto has a radius of around 1150 km"));
	randomFacts.push_back(core::stringw("The sun has a radius of around 695000 km"));

	randomFacts.push_back(core::stringw("Mercury has a mass of around 33000e21 kg"));
	randomFacts.push_back(core::stringw("Venus has a mass of around 4869000e21 kg"));
	randomFacts.push_back(core::stringw("Earth has a mass of around 5972000e21 kg"));
	randomFacts.push_back(core::stringw("Mars has a mass of around 6421900e21 kg"));
	randomFacts.push_back(core::stringw("Jupiter has a mass of around 1900000000e21 kg"));
	randomFacts.push_back(core::stringw("Saturn has a mass of around 56800000e21 kg"));
	randomFacts.push_back(core::stringw("Uranus has a mass of around 86830000e21 kg"));
	randomFacts.push_back(core::stringw("Neptune has a mass of around 1024700000e21 kg"));
	randomFacts.push_back(core::stringw("Pluto has a mass of around 1270e21 kg"));
	randomFacts.push_back(core::stringw("The sun has a mass of around 1989000000000e21 kg"));

	randomFacts.push_back(core::stringw("One Mercury year is 87.97 earth days long"));
	randomFacts.push_back(core::stringw("One Venus year is 224.70 earth days long "));
	randomFacts.push_back(core::stringw("One Earth year is 365.261 earth days long "));
	randomFacts.push_back(core::stringw("One Mars year is 686.98 earth days long "));
	randomFacts.push_back(core::stringw("One Jupiter year is 4332.71 earth days long "));
	randomFacts.push_back(core::stringw("One Saturn year is 10759.50 earth days long "));
	randomFacts.push_back(core::stringw("One Uranus year is 30685 earth days long "));
	randomFacts.push_back(core::stringw("One Neptune year is 60190 earth days long "));
	randomFacts.push_back(core::stringw("One Pluto year is 90550 earth days long "));

	intro.push_back(core::stringw("W-A-S-D, Left-Mouse, Right-Mouse controls your craft..."));
	intro.push_back(core::stringw("W moves your ship up, S moves down"));
	intro.push_back(core::stringw("A moves your ship left, D moves right"));
	intro.push_back(core::stringw("Left Mouse Button moves forward,"));
	intro.push_back(core::stringw("Right Mouse Button moves backwards,"));
	intro.push_back(core::stringw("Think of them as 6 thrusters in all 3 axis,"));
	intro.push_back(core::stringw("up-down, left-right, forward-backwards..."));
	intro.push_back(core::stringw("The blue circle is your velocity vector or flightmarker."));
	intro.push_back(core::stringw("If you look at the flightmarker and press Left Mouse Button,"));
	intro.push_back(core::stringw("you will see we're accelerating !!!"));
	intro.push_back(core::stringw("If you press the Right-Button your reverse thruster ignites"));
	intro.push_back(core::stringw("So it's a truely 3-dimensional movement,"));
	intro.push_back(core::stringw("still unlike any aircraft you might have ever flown"));
	intro.push_back(core::stringw("It can get a little confusing, but you'll get used to it"));
	intro.push_back(core::stringw("Search for your flightmarker to understand your movement."));
	intro.push_back(core::stringw("It is important to understand that we're moving in zero G"));

	intro.push_back(core::stringw("Our Solarsystem is a vast place..."));
	intro.push_back(core::stringw("This model has proportional planets and orbits,"));
	intro.push_back(core::stringw("but their distance to the sun is scaled by 1/100000."));
	intro.push_back(core::stringw("In reality those planets would be 100000x farther away..."));
	intro.push_back(core::stringw("from here we wouldn't be able to see them, not even a pixel."));
	intro.push_back(core::stringw("Also the sun's radius is divided by 50."));
	intro.push_back(core::stringw("The planets masses, thus gravitational attractions are altered"));
	intro.push_back(core::stringw("Except for that, it's as real as it gets."));
	intro.push_back(core::stringw("The data down-right in the box is real and the planets"));
	intro.push_back(core::stringw("position is real, calculated by painfully hard maths,"));
	intro.push_back(core::stringw("it's supposedly accurate for many centuries ahead."));
	intro.push_back(core::stringw("The flyCam's gravitation simulation is a little simplified,"));
	intro.push_back(core::stringw("and the maths is easy but it subscribes to keplers and"));
	intro.push_back(core::stringw("newtons laws of motion..."));

	intro.push_back(core::stringw("Right now we are chaotically orbiting earth,"));
	intro.push_back(core::stringw("which caught us in its gravity field..."));
	intro.push_back(core::stringw("The easiest way to get into an orbit of a planet or the sun"));
	intro.push_back(core::stringw("is adjusting your distance and speed by checking the marker"));
	intro.push_back(core::stringw("little is more... try adjusting instead of controlling."));
	intro.push_back(core::stringw("Try doing nothing for some time, and ellipses will be round."));
	intro.push_back(core::stringw("Feel free to change the gravity by changing the planets mass"));
	intro.push_back(core::stringw("in main.cpp of the source code... pMass in createSolarSystem"));
	intro.push_back(core::stringw("Planets are ordered by distance to the sun, smallest up"));
	intro.push_back(core::stringw("Please lean back and watch Keplerian dynamics unfold..."));
	intro.push_back(core::stringw("enjoy..."));


	//Orbit Time in earth days
	//already divided for creating orbit rings in 360 steps
	orbitT.push_back(87.97/360);
	orbitT.push_back(224.70/360);
	orbitT.push_back(365.261/360);
	orbitT.push_back(686.98/360);
	orbitT.push_back(4332.71/360);
	orbitT.push_back(10759.50/360);
	orbitT.push_back(30685.00/360);
	orbitT.push_back(60190.00/360);
	orbitT.push_back(90550.00/360);

	/*
	//real values for mass of the planets in kg.
	//handled by config
	pMass.push_back(330e21);   //330e21 kg
	pMass.push_back(4869e21);   //4869e21 kg
	pMass.push_back(5972e21);   5972e21 kg
								73.477e22
	pMass.push_back(642.19e21);   //642.19e21 kg
	pMass.push_back(1900000e21); //1900000e21 kg
	pMass.push_back(568000e21); //568000e21 kg
	pMass.push_back(86830e21);   //86830e21 kg
	pMass.push_back(102470e21); //102470e21 kg
	pMass.push_back(12.7e21);       //12.7e21 kg
	pMass.push_back(1989000000e21);  //1989000000e21 kg


	pNames.push_back(c8("Mercury"));
	pNames.push_back(c8("Venus"));
	pNames.push_back(c8("Earth"));
	pNames.push_back(c8("Mars"));
	pNames.push_back(c8("Jupiter"));
	pNames.push_back(c8("Saturn"));
	pNames.push_back(c8("Uranus"));
	pNames.push_back(c8("Mercury"));
	pNames.push_back(c8("Neptune"));
	pNames.push_back(c8("Pluto"));
	pNames.push_back(c8("Sun"));
	*/

	//Planets radius in km
	planetSize.push_back(2440.0);
	planetSize.push_back(6051.8);
	planetSize.push_back(6371.0);
	planetSize.push_back(3392.0);
	planetSize.push_back(71492.0);
	planetSize.push_back(60268.0);
	planetSize.push_back(25559.0);
	planetSize.push_back(24764.0);
	planetSize.push_back(1159.0);
	planetSize.push_back(695000.0);

	//Planets sideral rotation period
	pRot.push_back( (360.0/(-58.646*dayV*minSecV*minSecV))*timescale);
	pRot.push_back( (360.0/(-243.0185*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	-0.00416666*timescale);
	pRot.push_back(	(360.0/(-1.025957*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	(360.0/(-9.925*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	(360.0/(-0.439*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	(360.0/(0.71833*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	(360.0/(6.387230*dayV*minSecV*minSecV))*timescale);
	pRot.push_back(	(360.0/(-0.6713*dayV*minSecV*minSecV))*timescale);

	//Earth
	//moon 0;
	mSize.push_back(1737);
	mDist.push_back(384400/scaleF);
	mMass.push_back(7.3477e22);
	mFOrb.push_back((360.0/(27.321582*dayV*minSecV*minSecV))*timescale);

	//Jupiter
	//Io
	mSize.push_back(1821.3f);
	mDist.push_back(421700/scaleF);
	mMass.push_back(8.9319e22);
	mFOrb.push_back((360.0/(8.9319e22*dayV*minSecV*minSecV))*timescale);
	//Europa
	mSize.push_back(1569);
	mDist.push_back(670900/scaleF);
	mMass.push_back(4.8e22);
	mFOrb.push_back((360.0/(3.551181*dayV*minSecV*minSecV))*timescale);
	//Ganymede
	mSize.push_back(2634.1);
	mDist.push_back(1070400/scaleF);
	mMass.push_back(1.4819e23);
	mFOrb.push_back((360.0/(7.15455296*dayV*minSecV*minSecV))*timescale);
	//Callisto
	mSize.push_back(2410.3);
	mDist.push_back(1882700/scaleF);
	mMass.push_back(1.075938e23);
	mFOrb.push_back((360.0/(16.6890184*dayV*minSecV*minSecV))*timescale);

	//Saturn
	//Mimas Enceladus Tethys Dione Rhea Titan Iapetus
	//Mimas
	mSize.push_back(400);
	mDist.push_back(185000/scaleF);
	mMass.push_back(0.4e20);
	mFOrb.push_back((360.0/(0.9*dayV*minSecV*minSecV))*timescale);
	//Enceladus
	mSize.push_back(500);
	mDist.push_back(238000/scaleF);
	mMass.push_back(1.1e20);
	mFOrb.push_back((360.0/(1.4*dayV*minSecV*minSecV))*timescale);
	//Tethys
	mSize.push_back(1060);
	mDist.push_back(295000/scaleF);
	mMass.push_back(6.2e20);
	mFOrb.push_back((360.0/(1.9*dayV*minSecV*minSecV))*timescale);
	//Dione
	mSize.push_back(1120);
	mDist.push_back(377000/scaleF);
	mMass.push_back(11e20);
	mFOrb.push_back((360.0/(2.7*dayV*minSecV*minSecV))*timescale);
	//Rhea
	mSize.push_back(1530);
	mDist.push_back(527000/scaleF);
	mMass.push_back(23e20);
	mFOrb.push_back((360.0/(4.5*dayV*minSecV*minSecV))*timescale);
	//Titan
	mSize.push_back(5150);
	mDist.push_back(1222000/scaleF);
	mMass.push_back(1350e20);
	mFOrb.push_back((360.0/(16.0*dayV*minSecV*minSecV))*timescale);
	//Iapetus
	mSize.push_back(1440);
	mDist.push_back(3560000/scaleF);
	mMass.push_back(20e20);
	mFOrb.push_back((360.0/(79.0*dayV*minSecV*minSecV))*timescale);

	//Uranus
	//Puck Miranda Ariel Umbriel Titania Oberon
	//Puck
	mSize.push_back(162);
	mDist.push_back(86004/scaleF);
	mMass.push_back(0.49e18);
	mFOrb.push_back((360.0/(0.761833*dayV*minSecV*minSecV))*timescale);
	//Miranda incl. 4.232
	mSize.push_back(471.6);
	mDist.push_back(129390/scaleF);
	mMass.push_back(66e18);
	mFOrb.push_back((360.0/(1.413479*dayV*minSecV*minSecV))*timescale);
	//Ariel
	mSize.push_back(1157.8);
	mDist.push_back(191020/scaleF);
	mMass.push_back(1350e18);
	mFOrb.push_back((360.0/(2.520379*dayV*minSecV*minSecV))*timescale);
	//Umbriel
	mSize.push_back(1169.4);
	mDist.push_back(266300/scaleF);
	mMass.push_back(1170e18);
	mFOrb.push_back((360.0/(4.144177*dayV*minSecV*minSecV))*timescale);
	//Titania
	mSize.push_back(1577.8);
	mDist.push_back(435910/scaleF);
	mMass.push_back(3530e18);
	mFOrb.push_back((360.0/(8.705872*dayV*minSecV*minSecV))*timescale);
	//Oberon
	mSize.push_back(1522.8);
	mDist.push_back(583520/scaleF);
	mMass.push_back(3010e18);
	mFOrb.push_back((360.0/(13.463239*dayV*minSecV*minSecV))*timescale);

}
void createSolarSystem()
{
	world = smgr->addEmptySceneNode();

	/*After quite a long excursion from this code I came back and decided it's time to comment
	a little more on what's going on. Start with the obsolete uncommented code below, which was
	a method of creating the planetary orbits using calculateAtDay to complete one planetary year
	in 360 steps and creating boxes for representaion. The new orbits are loaded as a model
	*/

	/*
	scene::ISceneNode* Orbits = smgr->addEmptySceneNode(world);
	//scene::IMesh* OnodeM = smgr->getMesh("../data/cube.obj");
	//OnodeM->setHardwareMappingHint(scene::EHM_STATIC);
	//scene::IBillboardSceneNode* Onode;
	u32 x;
	u32 j;
	for(x=0; x<9; x++){
		for(j=0; j<360; j++){
			Day += orbitT[x];
			core::array<core::vector3df> result = mySolarSystem->calculateAtDay((f64)Day,x);
			scene::IBillboardSceneNode* Onode = smgr->addBillboardSceneNode(Orbits,core::dimension2df(0.03f,0.03f),result[0]/scaleF);
			//scene::ISceneNode* Onode = smgr->addCubeSceneNode(x*0.4,0,-1,result[0]/scaleF);
			//Onode->setScale(core::vector3df(x*0.04,x*0.04,x*0.04));
			Onode->setMaterialFlag(video::EMF_LIGHTING, false);
			Onode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
			Onode->setMaterialFlag(video::EMF_WIREFRAME, true);
		}
	}
	*/
	//fill an array Ppos with Planet start positions
	//unsure what the purpose was, probably debug
	/*
	u32 x;
	u32 j;
	for(x=0; x<9; x++){
		core::array<core::vector3df> result = mySolarSystem->calculateAtDay((f64)Day,x);
		core::array<core::vector3df> Ppos;
		for(j=0; j<360; j++){
			Day += orbitT[x];
			core::array<core::vector3df> result = mySolarSystem->calculateAtDay((f64)Day,x);
			Ppos.push_back(result[0]);
		}
	}
	*/

	//Create a huge world axis for debug purposes
	/*
	scene::ISceneNode* axis = smgr->addEmptySceneNode();
	scene::IMesh* axisM = smgr->getMesh("../data/axis.b3d");
	axisM->setHardwareMappingHint(scene::EHM_STATIC);
	axis = smgr->addMeshSceneNode(axisM);
	axis->setMaterialFlag(video::EMF_LIGHTING, false);
	axis->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	axis->setMaterialFlag(video::EMF_WIREFRAME, true);
	axis->setScale(core::vector3df(5,5,5));
	*/

	//create Billboards for the star Sphere
	video::ITexture *star1 =  driver->getTexture("../data/starbill0.jpg");
	video::ITexture *star2 =  driver->getTexture("../data/starbill1.jpg");
	video::ITexture *star3 =  driver->getTexture("../data/starbill2.jpg");
	video::ITexture *star4 =  driver->getTexture("../data/starbill3.jpg");
	video::ITexture *star5 =  driver->getTexture("../data/starbill4.jpg");
	video::ITexture *star6 =  driver->getTexture("../data/starbill5.jpg");

	//36384 / 4 = 9096
	//create a sceneNode as parent for all billboards and create all stars
	//based on the Bright Star Catalouge 5 included in the header bsc.h
	allstars = smgr->addEmptySceneNode();
	for(s32 s = 0; s < 36384 ; s += 4){
		scene::IBillboardSceneNode* star = smgr->addBillboardSceneNode(allstars,
			core::dimension2df(bsc[s+3]/1024,bsc[s+3]/1024),
			core::vector3df(bsc[s]/160.0, bsc[s+1]/160.0, bsc[s+2]/160.0),-1);
		star->setMaterialFlag(video::EMF_LIGHTING, false);
		star->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
		star->setMaterialTexture(0,star6);

		u16 spectT = spect[s];

		switch(spectT)
		{
			case 0:
				star->setMaterialTexture(0,star6);
				break;
			case 1:
				star->setMaterialTexture(0,star5);
				break;
			case 2:
				star->setMaterialTexture(0,star4);
				break;
			case 3:
				star->setMaterialTexture(0,star3);
				break;
			case 4:
				star->setMaterialTexture(0,star2);
				break;
			case 5:
				star->setMaterialTexture(0,star1);
				break;
		}

	}
	allstars->setRotation(core::vector3df(-23.5,0,0));
	allstars->setRotation(allstars->getRotation() + core::vector3df(-90,90,0));
	allstars->setScale(core::vector3df(1,-1,1));

	//create stars and nebula with a skybox
	scene::ISceneNode* SkyBox = smgr->addSkyBoxSceneNode (
		driver->getTexture("../data/star_top.jpg"),
		driver->getTexture("../data/star_bot.jpg"),
		driver->getTexture("../data/star4.jpg"),
		driver->getTexture("../data/star2.jpg"),
		driver->getTexture("../data/star3.jpg"),
		driver->getTexture("../data/star1.jpg"));

	/*
	//create stars and nebula with a skybox
	scene::ISceneNode* SkyBox = smgr->addSkyBoxSceneNode (
		driver->getTexture("../data/gridFarbe.jpg"),
		driver->getTexture("../data/gridFarbe.jpg"),
		driver->getTexture("../data/gridFarbe.jpg"),
		driver->getTexture("../data/gridFarbe.jpg"),
		driver->getTexture("../data/gridFarbe.jpg"),
		driver->getTexture("../data/gridFarbe.jpg"));
	*/

	// make a 1024x1024 texture RTT skybox
	scene::CRTTSkyBoxSceneNode* sky = new scene::CRTTSkyBoxSceneNode(core::dimension2du(1024,1024), smgr->getRootSceneNode(),smgr,0);

	// render stars and nebula to new skybox with default blue bg
	sky->renderToSkyBox(core::vector3df(0,0,0));

	// hide billboard stars and old Skybox
	allstars->setVisible(false);
	SkyBox->setVisible(false);

	//The new orbits are loaded as a model
	scene::IMesh* orbitsM = smgr->getMesh("../data/orbits.obj");
	orbitsM->setHardwareMappingHint(scene::EHM_STATIC);
	scene::ISceneNode* orbits = smgr->addMeshSceneNode(orbitsM);
	orbits->setMaterialFlag(video::EMF_LIGHTING, false);
	orbits->setMaterialFlag(video::EMF_POINTCLOUD, true);
	orbits->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	orbits->setScale(core::vector3df((scaleF/100000.0f)*-1,scaleF/100000.0f,scaleF/100000.0f));
	orbits->setPosition(orbits->getPosition()+core::vector3df(0,-0.08,0));
	orbits->setParent(world);

	//load all the textures
	video::ITexture *tex1 =  driver->getTexture("../data/mercurymap.jpg");
	video::ITexture *tex2 =  driver->getTexture("../data/mercurybump.jpg");
	video::ITexture *tex3 =  driver->getTexture("../data/venusmap.jpg");
	video::ITexture *tex4 =  driver->getTexture("../data/venusbump.jpg");
	video::ITexture *tex5 =  driver->getTexture("../data/earthmap1k.jpg");
	video::ITexture *tex6 =  driver->getTexture("../data/earthbump1k.jpg");
	video::ITexture *tex7 =  driver->getTexture("../data/mars_1k_color.jpg");
	video::ITexture *tex8 =  driver->getTexture("../data/mars_1k_topo.jpg");
	video::ITexture *tex9 =  driver->getTexture("../data/jupitermap.jpg");
	video::ITexture *tex10 =  driver->getTexture("../data/nobump.jpg");
	video::ITexture *tex11 =  driver->getTexture("../data/saturnmap.jpg");
	video::ITexture *tex13 =  driver->getTexture("../data/uranusmap.jpg");
	video::ITexture *tex15 =  driver->getTexture("../data/neptunemap.jpg");
	video::ITexture *tex17 =  driver->getTexture("../data/plutomap1k.jpg");
	video::ITexture *tex18 =  driver->getTexture("../data/plutobump1k.jpg");
	video::ITexture *tex19 =  driver->getTexture("../data/earthcloudmap.tga");
	video::ITexture *tex20 =  driver->getTexture("../data/ebill.jpg");
	video::ITexture *tex21 =  driver->getTexture("../data/saturnringcolor.tga");
	video::ITexture *tex22 =  driver->getTexture("../data/moon.jpg");
	video::ITexture *tex23 =  driver->getTexture("../data/moon_height.jpg");
	video::ITexture *tex24 =  driver->getTexture("../data/io.jpg");
	video::ITexture *tex25 =  driver->getTexture("../data/io_height.jpg");
	video::ITexture *tex26 =  driver->getTexture("../data/europa.jpg");
	video::ITexture *tex27 =  driver->getTexture("../data/europa_height.jpg");
	video::ITexture *tex28 =  driver->getTexture("../data/ganymede.jpg");
	video::ITexture *tex29 =  driver->getTexture("../data/ganymede_height.jpg");
	video::ITexture *tex30 =  driver->getTexture("../data/callisto.jpg");
	video::ITexture *tex31 =  driver->getTexture("../data/europa_height.jpg");
	//video::ITexture *tex35 =  driver->getTexture("../data/earthcloudmaptrans.jpg");
	video::ITexture *sunbill =  driver->getTexture("../data/bill.jpg");
	video::ITexture *sun =  driver->getTexture("../data/diffuse.jpg");

	//make space to hold the meshes
	scene::IAnimatedMesh* ringMesh;
	scene::IAnimatedMesh* planetMesh;
	scene::IAnimatedMesh* moonMesh;
	scene::IMesh* tangentMesh;
	scene::IBillboardSceneNode* earthBill;
	scene::ISceneNodeAnimator* rota;

	scene::IMesh* PlanetNodeAxisM;
	scene::IMesh* PlanetAxisM;

	//create the SceneNodes for hierarchy as follows
	/*
	world
	^
	|
	planetNode		= is driven by solar.cpp and pushed into array<scene::ISceneNode*> planets
	|
	|--->planet		= is rotated according to sideral rotation period and pushed into array<scene::ISceneNode*> planetsR
	|	 |--->atmo
	|	 |--->ring
	|
	|---->moonNode	= is positioned according to moons orbital period and pushed into array<scene::ISceneNode*> moons
		 |--->moon  = is rotated according to moons sideral rotation period and pushed into array<scene::ISceneNode*> moonsR
	*/

	u32 i;
	for(i=0; i<9; i++){

		scene::ISceneNode* planetNode;
		scene::ISceneNode* planet;
		scene::ISceneNode* atmo;
		scene::ISceneNode* ring;
		scene::ISceneNode* moonNode;
		scene::ISceneNode* moon;

		scene::ISceneNode* PlanetNodeAxis;
		scene::ISceneNode* PlanetAxis;

		core::matrix4 mTmatr;

		switch(i){
		case 0:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Mercury",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex1);
			planet->setMaterialTexture(1,tex2);
			planet->getMaterial(0).SpecularColor.set(255,255,255,255);
			planet->getMaterial(0).Shininess = 3800;
			planet->setRotation(core::vector3df(0,0,-0.1));
			planet->setName("mercury");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();
			break;
		case 1:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Venus",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex3);
			planet->setMaterialTexture(1,tex4);
			planet->setRotation(core::vector3df(0,0,3));
			planet->setName("venus");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();
			break;
		case 2:
			planetNode = smgr->addEmptySceneNode(world);
			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon0",(mSize[0]/scaleF)*1,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moon->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			moonNode->setPosition(core::vector3df(0,0,mDist[0]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex22);
			moon->setMaterialTexture(1,tex23);

			moon->setName("Moon");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			//planetMesh = smgr->getMesh("../data/earth.obj");
			//smgr->getMeshManipulator()->recalculateNormals(planetMesh,true,true);
			planetMesh = smgr->addSphereMesh("Earth",planetSize[i]/scaleF,48,48);
			planetMesh->setHardwareMappingHint(scene::EHM_STATIC);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			if(driver->getDriverType() == video::EDT_OPENGL)
			{
				//planet->setMaterialType((video::E_MATERIAL_TYPE)normalSpec);
				//planet->getMaterial(0).MaterialTypeParam = 0.035f; // adjust height for parallax effect
				planet->setMaterialType(video::EMT_SOLID);

			}
			if(driver->getDriverType() == video::EDT_DIRECT3D9)
				planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex5);
			planet->setMaterialTexture(1,tex6);
			planet->setName("earth");
			tangentMesh->drop();

			atmo = smgr->addMeshSceneNode(planetMesh,planet);
			atmo->setScale(planet->getScale()*1.02);
			atmo->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			atmo->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			atmo->setMaterialTexture(0,tex19);
			atmo->setName("earth-atmo");
			atmo->getMaterial(0).SpecularColor.set(255,255,255,255);
			atmo->getMaterial(0).Shininess = 200;
			//rota = smgr->createRotationAnimator(core::vector3df(0.0042,0.015,0.0042));
			//atmo->addAnimator(rota);
			//rota->drop();


			earthBill = smgr->addBillboardSceneNode(planetNode,core::dimension2df((planetSize[i]/scaleF)*2.5f,(planetSize[i]/scaleF)*2.5f));
			earthBill->setMaterialFlag(video::EMF_LIGHTING, false);
			earthBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
			earthBill->setMaterialTexture(0,tex20);
			earthBill->setName("earth-bill");
			planet->setRotation(core::vector3df(0,0,23.5));
			planet->setParent(planetNode);

			/*
			PlanetNodeAxis = smgr->addEmptySceneNode();
			PlanetNodeAxisM = smgr->getMesh("../data/axis.b3d");
			PlanetNodeAxisM->setHardwareMappingHint(scene::EHM_STATIC);
			PlanetNodeAxis = smgr->addMeshSceneNode(PlanetNodeAxisM);
			PlanetNodeAxis->setPosition(planetNode->getPosition());
			PlanetNodeAxis->setParent(planetNode);
			PlanetNodeAxis->setScale(core::vector3df(0.01f,0.01f,0.01f));
			*/

			PlanetAxis = smgr->addEmptySceneNode();
			PlanetAxisM = smgr->getMesh("../data/axis.b3d");
			PlanetAxisM->setHardwareMappingHint(scene::EHM_STATIC);
			PlanetAxisM->setMaterialFlag(video::EMF_LIGHTING, false);
			PlanetAxisM->setMaterialFlag(video::EMF_WIREFRAME, true);
			PlanetAxisM->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
			PlanetAxis = smgr->addMeshSceneNode(PlanetAxisM);
			PlanetAxis->setPosition(planet->getPosition());
			PlanetAxis->setParent(planet);
			PlanetAxis->setScale(core::vector3df(0.0001f,0.0001f,0.0001f));

			planets.push_back(planetNode);
			planetsR.push_back(planet);
			break;
		case 3:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Mars",planetSize[i]/scaleF,48,48);
			smgr->getMeshManipulator()->recalculateNormals(planetMesh);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex7);
			planet->setMaterialTexture(1,tex8);
			planet->setRotation(core::vector3df(0,0,-25));
			planet->setName("mars");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();
			break;
		case 4:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Jupiter",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex9);
			planet->setMaterialTexture(1,tex10);
			planet->setRotation(core::vector3df(0,0,-3));
			planet->setName("jupiter");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon1",mSize[1]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[1]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex24);
			moon->setMaterialTexture(1,tex25);
			moon->setName("Io");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon2",mSize[2]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[2]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex26);
			moon->setMaterialTexture(1,tex27);
			moon->setName("Europa");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon3",mSize[3]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[3]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Ganymede");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon4",mSize[4]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[4]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex30);
			//moon->setMaterialTexture(1,tex27);
			moon->setName("Callisto");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();
			break;
		case 5:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Saturn",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex11);
			planet->setMaterialTexture(1,tex10);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon5",mSize[5]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[5]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Mimas");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon6",mSize[6]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[6]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Enceladus");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon7",mSize[7]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[7]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Tethys");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon8",mSize[8]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[8]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Dione");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon9",mSize[9]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[9]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Rhea");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon10",mSize[10]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[10]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Titan");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon11",mSize[11]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[11]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Iapetus");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			ringMesh = smgr->getMesh("../data/ring.obj");
			ring = smgr->addMeshSceneNode(ringMesh,planetNode);
			ring->setParent(planet);
			ring->setScale(core::vector3df(1,0.1,1)*0.016);
			ring->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			ring->setMaterialTexture(0,tex21);
			ring->setMaterialFlag(video::EMF_LIGHTING,false);
			ring->setName("saturn-ring");

			planet->setRotation(core::vector3df(0,0,-26.44));
			planet->setName("saturn");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			break;
		case 6:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Uranus",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex13);
			planet->setMaterialTexture(1,tex10);
			planet->setRotation(core::vector3df(0,0,-98));
			planet->setName("uranus");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon12",mSize[12]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[12]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Puck");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon13",mSize[13]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[13]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Miranda");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon14",mSize[14]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[14]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Ariel");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon15",mSize[15]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[15]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Umbriel");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon16",mSize[16]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[16]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Titania");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();

			moonNode = smgr->addEmptySceneNode(planetNode);
			moonMesh = smgr->addSphereMesh("Moon17",mSize[17]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(moonMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);
			moon = smgr->addMeshSceneNode(tangentMesh,moonNode);
			moonNode->setPosition(core::vector3df(0,0,mDist[17]));
			moon->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			moon->setMaterialTexture(0,tex28);
			moon->setMaterialTexture(1,tex29);
			moon->setName("Oberon");
			moons.push_back(moonNode);
			moonsR.push_back(moon);
			tangentMesh->drop();
			break;
		case 7:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Neptune",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex15);
			planet->setMaterialTexture(1,tex10);
			planet->setRotation(core::vector3df(0,0,-30));
			planet->setName("neptune");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();
			break;
		case 8:
			planetNode = smgr->addEmptySceneNode(world);
			planetMesh = smgr->addSphereMesh("Pluto",planetSize[i]/scaleF,48,48);
			tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);
			tangentMesh->setHardwareMappingHint(scene::EHM_STATIC);

			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex17);
			planet->setMaterialTexture(1,tex18);
			planet->setRotation(core::vector3df(0,0,-120));
			planet->setName("pluto");
			planet->setParent(planetNode);
			planets.push_back(planetNode);
			planetsR.push_back(planet);
			tangentMesh->drop();
			break;
		}
	}
	scene::IBillboardSceneNode* sunBill = smgr->addBillboardSceneNode(0,core::dimension2df(planetSize[9]/scaleF,planetSize[9]/scaleF)*20);
	sunBill->setMaterialFlag(video::EMF_LIGHTING, false);
	sunBill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	sunBill->setMaterialTexture(0,sunbill);
	sunBill->setParent(world);

	scene::ISceneNode* Sun = smgr->addMeshSceneNode(planetMesh);
	Sun->setScale(core::vector3df(planetSize[9],planetSize[9],planetSize[9])/scaleF);
	Sun->setMaterialFlag(video::EMF_LIGHTING, false);
	Sun->setMaterialType(video::EMT_LIGHTMAP);
	Sun->setMaterialTexture(0,sun);
	Sun->setParent(world);
	planets.push_back(Sun);

	//const core::list<scene::ISceneNode*> list = world->getChildren();

}
SIrrlichtCreationParameters driverQuery(SIrrlichtCreationParameters param)
{

	printf("Please select the driver you want:\n"\
		" (a) Direct3D 9.0c\n (b) OpenGL 1.5\n");
	char i;
	std::cin >> i;
	switch(i)
	{
	case 'a': param.DriverType = video::EDT_DIRECT3D9;break;
	case 'b': param.DriverType =video::EDT_OPENGL;;break;

	}
	printf("Anti-Aliasing ?\n (a) yes\n (b) no\n\n");
	std::cin >> i;
	switch(i)
	{
	case 'a': param.AntiAlias = true;break;
	case 'b': param.AntiAlias = false;break;
	}
	printf("High-Precision FPU ?\n (a) yes\n (b) no\n\n");
	std::cin >> i;
	switch(i)
	{
	case 'a': param.HighPrecisionFPU = true;break;
	case 'b': param.HighPrecisionFPU = false;break;
	}
	printf("VSync ?\n (a) yes\n (b) no\n\n");
	std::cin >> i;
	switch(i)
	{
	case 'a': param.Vsync = true;break;
	case 'b': param.Vsync = false;break;
	}
	printf("Please select the resolution:\n"\
		" (a) 1920x1200\n (b) 1600x1200\n (c) 1280x1024\n"\
		" (d) 1024x768\n (e) 800x600\n"\
		" (f) 640x480\n (otherKey) exit\n\n");
	std::cin >> i;
	switch(i)
	{
	case 'f': param.WindowSize = core::dimension2di(640,480);break;
	case 'e': param.WindowSize = core::dimension2di(800,600);break;
	case 'd': param.WindowSize = core::dimension2di(1024,768);break;
	case 'c': param.WindowSize = core::dimension2di(1280,1024);break;
	case 'b': param.WindowSize = core::dimension2di(1600,1200);break;
	case 'a': param.WindowSize = core::dimension2di(1920,1200);break;
	}
	printf("Fullscreen ? In Windows alt-F4 quits !!!\n (a) yes\n (b) no\n\n");
	std::cin >> i;
	switch(i)
	{
	case 'a': param.Fullscreen = true;break;
	case 'b': param.Fullscreen = false;break;
	}
	return param;
}
//run
core::vector3df calcEllipticalOrbVelocity(int type, int i, f64 Perigee, f64 Apogee)
{
	core::vector3df velocity;
	f64 distPerigee;
	f64 distApogee;
	f64 pMassS;

	if(type == 1){
		distPerigee = (mSize[i]+Perigee)*1000;
		distApogee = (mSize[i]+Apogee)*1000;
		pMassS = mMass[i];
	}
	if(type == 0){
		distPerigee = (planetSize[i]+Perigee)*1000;
		distApogee = (planetSize[i]+Apogee)*1000;
		pMassS = pMass[i];
	}

	f64 semiMajorAxisM = ((distPerigee+distApogee)/2);
	f64 pConst = Gconst*pMassS;

	//Calculate elliptical OrbitVelocity
	//Velocity at Perigee  	=      sqrt( GM * ( (2*distApogee) / ( distPerigee * (distPerigee+distApogee) ) ) )
	f64 ellipticOrbVelocity = sqrt(pConst* ( (2*distApogee) / ( distPerigee * (distPerigee+distApogee) ) ) );
	ellipticOrbVelocity /= 1000.0;
	ellipticOrbVelocity /= scaleF;

	//calculate planet movement
	f32 precisionF = core::round32(4000/timescale);
	core::array<core::vector3df> Pos1 = mySolarSystem->calculateAtDay(Day+fpsincrement,i);
	core::array<core::vector3df> Pos2 = mySolarSystem->calculateAtDay(Day+fpsincrement*precisionF,i);
	core::vector3df rPos1 = Pos1[0]/scaleF;
	core::vector3df rPos2 = Pos2[0]/scaleF;
	core::vector3df pmovement = rPos2 - rPos1;
	//f64 planetSpeed = f64(pmovement.getLength());
	pmovement /= timescale;
	pmovement /= precisionF-1.0;

	core::vector3df planRot = core::vector3df(planetsR[i]->getRotation().X,0,planetsR[i]->getRotation().Z);
	velocity = planRot.rotationToDirection(core::vector3df(1,0,0));
	velocity.setLength(ellipticOrbVelocity);
	velocity *= -1;
	velocity += pmovement;

	//determine startPos------------------------------------------------------------------------------
	startPos = rPos1+core::vector3df(0,0,(planetSize[i]+startAlt)/scaleF);
	//moons[0]->updateAbsolutePosition();

	//calculate orbital period in seconds, the formula for an ellipse is
	//p = 2PI*sqrt(semi-major axis^3/GM) where p is in seconds and semi-major axis in meters
	f64 oPeriod = (2*core::PI*sqrt(pow(semiMajorAxisM,3)/pConst))/60.0;
	printf("Period in minutes: %f\n",oPeriod);
	f64 temp = pConst*pow(1440.0*60.0/(2.0*core::PI),2);
	f64 altitude = (pow(temp,f64(1.0/3.0))/1000.0)-planetSize[i];
	return velocity;
}

void resetWorld()
{
	world->updateAbsolutePosition();
	closestBody->updateAbsolutePosition();

	core::vector3df currentOrigin = world->getAbsolutePosition();
	core::vector3df newOrigin = closestBody->getPosition();
	core::vector3df offset = currentOrigin - newOrigin;

	flyCam->setPosition(flyCam->getPosition()+offset);
	planetCam->setPosition(planetCam->getPosition()+offset);
	mayaCam->setPosition(mayaCam->getPosition()+offset);
	splitSCam->setPosition(splitSCam->getPosition()+offset);

	printf("%f,%f,%f\n",planets[9]->getPosition().X,planets[9]->getPosition().Y,planets[9]->getPosition().Z);

	world->setPosition(world->getPosition()+offset);

	for(u32 i=0; i<planets.size(); i++)
	{
		planets[i]->updateAbsolutePosition();
	}

	world->updateAbsolutePosition();
	camPos = flyCam->getAbsolutePosition();
	printf("%f,%f,%f\n",planets[9]->getPosition().X,planets[9]->getPosition().Y,planets[9]->getPosition().Z);

}
void debug(scene::ISceneNode* node, f64 radius)
{
	//debug:
	scene::ISceneNodeAnimator* circle = smgr->createFlyCircleAnimator(node->getPosition(),radius,0.0005);
	flyCam->setPosition(node->getPosition()+core::vector3df(0,0,radius));
	flyCam->addAnimator(circle);
	scene::IMesh* grid = smgr->getMesh("../data/grid.x");
	scene::IMeshSceneNode* gridn = smgr->addMeshSceneNode(grid);
	gridn->setScale(core::vector3df(10,10,10));
	grid->setMaterialFlag(video::EMF_WIREFRAME,true);
	grid->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
	gridn->setPosition(node->getPosition()+core::vector3df(0,-5000,0));
}


void rotateAroundCentre(core::vector3df &point, core::vector3df center, core::vector3df rotation)
{
   point -= center;
   core::matrix4 m;
   m.setRotationDegrees(rotation);
   m.rotateVect(point);
   point += center;
}
void rotateVectorAroundAxis(core::vector3df & vector, const core::vector3df & axis, f64 radians)
{
	core::quaternion MrQuaternion;
	core::matrix4 MrMatrix;
	(void)MrQuaternion.fromAngleAxis(radians, axis);
	MrQuaternion.getMatrix(MrMatrix,core::vector3df(0,0,0));
	MrMatrix.rotateVect(vector);
}
void handleZeroGCam(scene::ICameraSceneNode* cam)
{

	f64 camSpeed = 0;
	f64 camSpeed2 = 0;
	f64 camSpeedh = 0;
	f64 camSpeedv = 0;
	core::vector2df deltaC = CursorPos - core::vector2df(0.5,0.5);
	//cursor->setPosition(core::vector2df(0.5,0.5));
	deltaC /= 20;
	f64 deadzone = 0.002;
	f64 cX = 0;
	f64 cY = 0;
	if(deltaC.X > deadzone){
		cX = deadzone - deltaC.X;
	}if(deltaC.X < -deadzone){
		cX = -deadzone - deltaC.X;
	}if(deltaC.Y > deadzone){
		cY = deadzone - deltaC.Y;
	}if(deltaC.Y < -deadzone){
		cY = -deadzone - deltaC.Y;
	}if(deltaC.X < deadzone && deltaC.X > -deadzone){
		cX = 0.0;
	}if(deltaC.Y < deadzone && deltaC.Y > -deadzone){
		cY = 0.0;
	}
	deltaC = core::vector2df(cX*-1,cY*-1);
	//printf("%f , %f\n",cX,cY);
	core::vector3df forwardD = (flyCam->getTarget() - flyCam->getAbsolutePosition()).normalize();
	core::vector3df camRot = flyCam->getRotation();
	core::vector3df upD = flyCam->getUpVector();
	core::vector3df rightD = forwardD.crossProduct(upD);

	if(leftB && !key_shift){
		camSpeed += CspeedFB;
		camVector += forwardD*camSpeed;
		//printf("%f\n",camSpeed);
	}if(rightB && !key_shift){
		camSpeed -= CspeedFB;
		camVector += forwardD*camSpeed;
		//printf("%f\n",camSpeed);
	}if(leftB && key_shift){
		camSpeed2 += CspeedFB;
		camVector += flyAt*camSpeed2;
		//printf("%f\n",camSpeed);
	}if(rightB && key_shift){
		camSpeed2 -= CspeedFB;
		camVector += flyAt*camSpeed2;
		//printf("%f\n",camSpeed);
	}if(left){
		camSpeedh += CspeedLR;
		camVector += rightD*camSpeedh;
		//printf("%f\n",camSpeedh);
	}if(right){
		camSpeedh -= CspeedLR;
		camVector += rightD*camSpeedh;
		//printf("%f\n",camSpeedh);
	}if(up){
		camSpeedv += CspeedUD;
		camVector += upD*camSpeedv;
		//printf("%f\n",camSpeedh);
	}if(down){
		camSpeedv -= CspeedUD;
		camVector += upD*camSpeedv;
		//printf("%f\n",camSpeedh);
	}if(keyQ){
		camSpeedr += 0.0001;
	}if(keyE){
		camSpeedr -= 0.0001;
	}
	rotateVectorAroundAxis(upD, forwardD, camSpeedr);
	rotateVectorAroundAxis(upD, rightD, deltaC.Y*-1);
	rotateVectorAroundAxis(forwardD, upD, deltaC.X);
	rotateVectorAroundAxis(forwardD, rightD, deltaC.Y*-1);
	if(!keyQ && !keyE){
		if(camSpeedr > 0){
			camSpeedr -= 0.00005;
		}if(camSpeedr < 0){
			camSpeedr += 0.00005;
		}
	}
	// And re-orient the flyCam to face along the foward and up axes.
	core::vector3df targetD = flyCam->getAbsolutePosition() + forwardD*10000;
	//flyCam->setTarget(planets[2]->getAbsolutePosition());
	flyCam->setTarget(targetD);
	flyCam->setUpVector(upD);
}

void handleMap()
{
	f32 moveSpeed = delta;
	// Work out the 3 axes for the flyCam.
	core::vector3df forwardDm = (splitSCam->getTarget() - splitSCam->getAbsolutePosition()).normalize();
	core::vector3df upDm = splitSCam->getUpVector();
	core::vector3df rightDm = forwardDm.crossProduct(upDm);

	// yaw around the up axis
	if(leftA && !key_shift)
		rotateVectorAroundAxis(forwardDm, upDm, -moveSpeed);
	else if(rightA && !key_shift)
		rotateVectorAroundAxis(forwardDm, upDm, +moveSpeed);

	// roll around the forward axis
	if(leftA && key_shift)
		rotateVectorAroundAxis(upDm, forwardDm, +moveSpeed);
	else if(rightA && key_shift)
		rotateVectorAroundAxis(upDm, forwardDm, -moveSpeed);

	// pitch around the right axis (we need to change both forward AND up)
	if(upA && !key_shift)
	{
		rotateVectorAroundAxis(forwardDm, rightDm, -moveSpeed);
		rotateVectorAroundAxis(upDm, rightDm, -moveSpeed);
	}
	else if(downA && !key_shift)
	{
		rotateVectorAroundAxis(forwardDm, rightDm, +moveSpeed);
		rotateVectorAroundAxis(upDm, rightDm, +moveSpeed);
	}
	//Zoom in and out
	if(upA && key_shift)
		camOrbitDistance = core::max_(0.1f, camOrbitDistance - (camOrbitDistance * moveSpeed));
	else if(downA && key_shift)
		camOrbitDistance = core::min_(20000.f, camOrbitDistance + (camOrbitDistance * moveSpeed));

	// Move BACK up the forward axis of the flyCam to place it in its orbit.
	splitSCam->setPosition(camPos - (forwardDm * camOrbitDistance));

	// Point the flyCam at the target node, and align its up axis correctly.
	splitSCam->setTarget(camPos);
	splitSCam->setUpVector(upDm);

	if(keyF1)
		view2 = false;
	if(keyF2)
		view2 = true;
	if(key3)
		smgr->setActiveCamera(mayaCam);

}
void handleG()
{
	pull = core::vector3df(0,0,0);
	u32 i;
	f64 force = 0;

	for(i=0; i<planets.size(); i++)
	{
		planets[i]->updateAbsolutePosition();
		//current Planet psition
		core::vector3df planPos = planets[i]->getAbsolutePosition();
		//flyCam position
		flyCam->updateAbsolutePosition();
		camPos = flyCam->getPosition();
		//direction from flyCam to planet
		core::vector3df cam2plan = planPos-camPos;
		f64 pdistance = planPos.getDistanceFrom(camPos);
		if(i==0){
			closestDistance = pdistance;
			closestIndex = i;
		}
		if(pdistance < closestDistance){
			closestDistance = pdistance;
			closestIndex = i;
		}
		//distance in meters
		f64 distanceM = pdistance*scaleF*1000.0;
		//force in m/s^2
		force = (Gconst*pMass[i]/pow(distanceM,2));
		if(force >= 0)
		{
			//force in scale factor of sim
			force = (force/scaleF)/1000.0;
			//scale vector according to pull
			cam2plan.setLength(force);
			//add pull to total pull
			pull += cam2plan;
		}
		//if sun
		if(i==9){
			sunDistanceKM = (pdistance*scaleF);
		}
	}
	for(i=0; i<moons.size(); i++)
	{
		//current Planet psition
		moons[i]->updateAbsolutePosition();
		core::vector3df planPos = moons[i]->getAbsolutePosition();
		//direction from flyCam to planet
		core::vector3df cam2plan = planPos-camPos;
		f64 pdistance = planPos.getDistanceFrom(camPos);

		if(pdistance < closestDistance){
			closestDistance = pdistance;
			closestIndex = i+planets.size();
		}
		//distance in meters
		f64 distanceM = pdistance*scaleF*1000.0;
		//force in m/s^2
		force = (Gconst*mMass[i]/pow(distanceM,2));
		//force in scale factor of sim
		force = (force/scaleF)/1000.0;
		//scale vector according to pull
		cam2plan.setLength(force);
		//add pull to total pull
		pull += cam2plan;
	}

	currentOrbitDistance = closestDistance;
	gforce = pull.getLength()*scaleF*1000.0;

	if(closestIndex < planets.size())
	{
		closestBody = planets[closestIndex];
		if(closestBody != oldBody)
		{
			//resetWorld();
		}
		oldBody = closestBody;
	}
	if(closestIndex > planets.size() - 1){
		closestBody = moons[closestIndex-planets.size()];
	}

	//printf("%s\n",closestBody->getName());
}
void handleTrail()
{
	core::vector3df pos;
	closestBody->updateAbsolutePosition();
	pos = camPos - closestBody->getAbsolutePosition();

	if(trailcount < 900)
	{
		traillast = trailcount;
		trail2[traillast] = smgr->addBillboardSceneNode(closestBody,core::dimension2df(0.003,0.003),pos);

		trailcount++;
		trail2[traillast]->setMaterialFlag(video::EMF_POINTCLOUD,true);
		trail2[traillast]->setMaterialFlag(video::EMF_LIGHTING,false);
	}
	else
	{
		trail2[traillast]->setParent(closestBody);
		trail2[traillast]->setPosition(pos);

		traillast++;
		if(traillast == 900)
		{
			traillast = 0;
		}
	}
}

void relativeMovement()
{
    /// BUGGY causes segfault 64 linux
    /*
	relaPos = trail2[traillast-1]->getPosition();
	if(trailcount < 900)
	{
		if(traillast > 10)
		{
			lastPos = trail2[traillast-11]->getPosition();
		}
	}
	else
	{
		if(traillast > 10)
		{
			lastPos = trail2[traillast-11]->getPosition();
		}
		else
		{
			lastPos = trail2[900-(11-traillast)]->getPosition();
		}
	}
	flyAt = relaPos - lastPos;
	oSpeed = ((flyAt.getLength()*scaleF)/10)/timescale;
	flyAt.normalize();
	*/
}

void handlePlanets()
{
	//update Planets--------------------------------------------------------------------------
	updatePos = mySolarSystem->calculateAtDay(Day,-1);
	//set their Positions
	u32 i;
	for(i=0; i<updatePos.size(); i++)
	{
		planets[i]->setPosition(updatePos[i]/scaleF);
	}

	//spin planets, spin moons around planets & spin moons itself
	//start with spinning planets as to say one planetary day...
	for(u32 i=0; i<9; i++)
	{
		//printf("%f\n",planetsR[i]->getRotation().Y);
		planetsR[i]->setRotation(planetsR[i]->getRotation() + core::vector3df(0,pRot[i],0));

	}

	//spin moons around planets & spin moons (moonsR) itself.
	for(u32 i=0; i<moons.size(); i++)
	{
		core::vector3df pos = moons[i]->getPosition();
		rotateAroundCentre(pos,core::vector3df(0,0,0),core::vector3df(0,mFOrb[i],0));
		moons[i]->setPosition(pos);
		moonsR[i]->setRotation(moonsR[i]->getRotation() + core::vector3df(0,mFOrb[i],0));
	}

}
void playIntro(u32 pintroseconds)
{
	//printf("%d\n",pintroseconds);
	//play Intro------------------------------------------------------------------------------
	if(introcount < intro.size()){
		if(pintroseconds >= 6500){
			introseconds = 0;
			info2->setText(intro[introcount].c_str());
			introcount++;
		}
	}else if(introcount >= intro.size()){
		if(pintroseconds >= 5000){
			introseconds = 0;
			//generates the random number by using the     system clock time
			srand(time(NULL));
			int Number = (rand() % (29));
			u32 o = 0;
			if(o < 10)
			{
				const wchar_t * text = info->getText();
				info2->setText(randomFacts[Number].c_str());
				//printf("%d \n",Number);
			}
		}
	}
}

void handlePlanetCam() {
	planetCam->setParent(planetsR[closestIndex]);
	f32 Yrot = lati-180;
	f32 Xrot = longi;
	planetCam->setRotation(planetsR[closestIndex]->getRotation()+core::vector3df(Yrot,Xrot,0));
	core::vector3df latlong;
	latlong.rotationToDirection(core::vector3df(Xrot,Yrot,0));
	planetCam->setPosition(latlong*(planetSize[closestIndex]/scaleF)*-3);
	planetCam->setUpVector(planetsR[closestIndex]->getRotation().rotationToDirection());
}
void handlePlanetJump()
{

	if(key1)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,0,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key2)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,1,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key3)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,2,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key4)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,3,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key5)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,4,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key6)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,5,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key7)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,6,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key8)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,7,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
	if(key9)
	{
		flyAt = core::vector3df(0,0,0);
		pull = core::vector3df(0,0,0);
		camVector = core::vector3df(0,0,0);
		camVector = calcEllipticalOrbVelocity(0,8,startAlt,getAway);
		flyCam->setPosition(startPos);
		flyCam->updateAbsolutePosition();
	}
}
int main()
{
	irr::SIrrlichtCreationParameters param;
	//param = driverQuery(param);

	param.WindowSize = core::dimension2di(1600,1200);
	param.Fullscreen = true;
	param.AntiAlias = true;
	param.DriverType = video::EDT_OPENGL;
	param.Vsync = true;
	param.HighPrecisionFPU = true;
	param.Doublebuffer = true;
	param.Bits = 32;

	//create pointers to device, scene manager, GUI env., sound engine, cursor--------------------------------
	device=createDeviceEx(param);
	device->setEventReceiver(&my_event_receiver);
	//device->getCursorControl()->setVisible(false);
	driver=device->getVideoDriver();
	smgr=device->getSceneManager();
	device->getTimer()->setTime( 0 );
	env = device->getGUIEnvironment();
	cursor = device->getCursorControl();
	//ISoundEngine* engine = createIrrKlangDevice();
	//ISound* music = engine->play2D("../data/01Vangelis-MainTitles.mp3", true, false, true);
	//if (music)
	//music->setMinDistance(5.0f);

	driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

	Width = driver->getScreenSize().Width;
	Height = driver->getScreenSize().Height;

	//GL Shaders
	if(driver->getDriverType() == video::EDT_OPENGL){

		core::stringw psFileName = "../shaders/rmonkey02.frag";
		core::stringw vsFileName = "../shaders/rmonkey02.vert";
		video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
		s32 newMaterialType1 = 0;
		s32 newMaterialType2 = 0;
		if (gpu)
		{
			MyShaderCallBack* mc = new MyShaderCallBack();
			normalSpec = gpu->addHighLevelShaderMaterialFromFiles(
				vsFileName, "main", video::EVST_VS_3_0,
				psFileName, "main", video::EPST_PS_3_0,
				mc, video::EMT_SOLID);
		}
	}
	//make GUI
	makeGUI();

	//Initialize variables-----------------------------------------------------------------------------
	readconfig();
	initVars();

	//create solar system------------------------------------------------------------------------------
	createSolarSystem();
	Day = mySolarSystem->gregToJ2000(Syear,Smonth,Sday,Shour,Sminute,Ssecond);

	//set inital planet position----------------------------------------------------
	core::array<core::vector3df> updatePosConf = mySolarSystem->calculateAtDay(Day,-1);
	//set Position
	u32 j;
	for(j=0; j<updatePosConf.size(); j++)
	{
		planets[j]->setPosition(updatePosConf[j]/scaleF);
		planets[j]->updateAbsolutePosition();
	}
	//add light----------------------------------------------------------------------------------------
	light = smgr->addLightSceneNode(0,
		core::vector3df(0,0,0),
		video::SColorf(1.0f,1.0f,1.0f),2000.0f);

	video::SLight ldata = light->getLightData();
	//ldata.Attenuation.set(0.0f,1.0f,0.0f);
	ldata.AmbientColor.set(0.076f,0.076f,0.076f);
	light->setLightData(ldata);

	//determine startPos------------------------------------------------------------------------------
	startPos = planets[startplanet]->getPosition()+core::vector3df(0,0,(planetSize[startplanet]+startAlt)/scaleF);
	//moons[0]->updateAbsolutePosition();
	//startPos = moons[0]->getAbsolutePosition()+core::vector3df(0,0,(mSize[0]+1000)/scaleF);

	//we need days (in the julian calender) as a unit for astronomical calculations... so...
	//how many fractional days pass in one frame (can be a very small number)----------------------
	//24*60*60 seconds = 86400 seconds = 1 day;
	//Day += (1/86400)*timescale; <-- is to small for f64
	fpsincrement = incrementF*timescale;

	//calculate inertial frame of reference------------------------------------------------------------
	camVector = calcEllipticalOrbVelocity(0,startplanet,startAlt,getAway);
	//camVector = calcEllipticalOrbVelocity(1,0,startAlt,getAway);
	closestIndex = startplanet;
	closestBody = planets[startplanet];
	oldBody = closestBody;

	//make force arrows--------------------------------------------------------------------------------
	scene::IAnimatedMesh* forceAm = smgr->addArrowMesh("forceAm",
		video::SColor(0,255,0,0),
		video::SColor(0,255,0,0),4,8,
		20,15, 0.5f,1);
	scene::IAnimatedMesh* velAm = smgr->addArrowMesh("velAm",
		video::SColor(0,0,255,0),
		video::SColor(0,0,255,0),4,8,
		20,15, 0.5f,1);
	scene::IAnimatedMesh* tanAm = smgr->addArrowMesh("tanAm",
		video::SColor(0,0,0,255),
		video::SColor(0,0,0,255),4,8,
		20,15, 0.5f,1);
	//arrow mesh points along Y axis originaly, we have to change that
	core::matrix4 matr;
	matr.setRotationDegrees(core::vector3df(90,0,0));
	smgr->getMeshManipulator()->transformMesh(velAm, matr);
	smgr->getMeshManipulator()->transformMesh(forceAm, matr);
	smgr->getMeshManipulator()->transformMesh(tanAm, matr);
	scene::ISceneNode * forceA = smgr->addMeshSceneNode(forceAm);
	scene::ISceneNode * velA = smgr->addMeshSceneNode(velAm);
	scene::ISceneNode * tanA = smgr->addMeshSceneNode(tanAm);
	forceA->setMaterialFlag(video::EMF_LIGHTING,false);
	velA->setMaterialFlag(video::EMF_LIGHTING,false);
	tanA->setMaterialFlag(video::EMF_LIGHTING,false);
	forceA->setVisible(false);
	velA->setVisible(false);
	tanA->setVisible(false);

	//make and position cameras------------------------------------------------------------------------
	//flyCam=smgr->addCameraSceneNodeFPS(0,100,0);
	flyCam=smgr->addCameraSceneNode();
	flyCam->setNearValue(0.01);
	flyCam->setFarValue(10000);
	flyCam->setPosition(startPos);
	flyCam->setFOV(1.1);
	flyCam->setTarget(planets[2]->getPosition());

	mayaCam=smgr->addCameraSceneNodeMaya(0,500,100,500);
	mayaCam->setNearValue(0.001);
	mayaCam->setFarValue(10000);
	mayaCam->setFOV(1.1);
	mayaCam->setPosition(planetsR[startplanet]->getAbsolutePosition()+core::vector3df(0,0,0.5));
	mayaCam->setTarget(planets[2]->getAbsolutePosition());

	planetCam=smgr->addCameraSceneNode();
	planetCam->bindTargetAndRotation(true);
	planetCam->setNearValue(0.01);
	planetCam->setFarValue(10000);
	planetCam->setFOV(1);

	splitSCam=smgr->addCameraSceneNode();
	splitSCam->setNearValue(0.1);
	splitSCam->setFarValue(10000);
	splitSCam->setTarget(startPos);
	splitSCam->setPosition(startPos + core::vector3df(0.2,0,0));
	camOrbitDistance = startPos.getDistanceFrom(splitSCam->getPosition());

	//I unchained the frame delta of irrlicht's timer from simulation time by purpose. Before I only
	//tied the planets motion to the timer, so when setting timescale 1 you were able to synchronize
	//real time and simulation time, that is you could see the simulation clock ticking real seconds.
	//However that was applied only for planets, not for the cameras motion so I could see how far I
	//travelled but not in which timeframe compared to the 'simulation-real-time'. Quite frankly I
	//also got confused with frame deltas, incremets, timescales, meters/s^2 and what not else. Anyway
	//when trying to apply the frame delta to the cameras motion aswell it got quite choppy at times.
	//Meaning because of varying frame deltas the flyCam had to jump varying distances to stay in tune.
	//So only one timesystem chained didn't work either because of velocity and orbital periods,
	//and now both are unchained from irrlichts timer but synchronized...
	//I also know that there is a whole load of unnecessary fractioning and multiplying from the scale
	//factor to meters and back again, but the formulas expect meters and sometimes AU's and like before,
	//I DO get confused a by lots and lots of divisors and multipliers :O The friggin devil is in those
	//details and brackets and +-*/'s ... Slowly I will find them... what a headache they caused...

	//initialize some more variables for loop------------------------------------------------------
	u32 oldseconds = 0;
	u32 trailseconds = 0;
	smgr->setActiveCamera(flyCam);

	//--------------------------------------------------------------------------------------------
	//loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop_loop___
	//--------------------------------------------------------------------------------------------
	while(device->run()) {

		//determine frame delta in seconds--------------------------------------------------------
		u32 mseconds = (device->getTimer()->getTime());
		delta = mseconds-oldseconds;
		oldseconds = mseconds;
		introseconds += delta;
		trailseconds += delta;
		delta /= 1000;
		//printf("%f\n",1/delta);

		//begin-----------------------------------------------------------------------------------
		driver->setViewPort(core::rect<s32>(0,0,Width,Height));
		driver->beginScene(true,true,video::SColor(0,0,0,0));

		//set time--------------------------------------------------------------------------------
		Day += fpsincrement;
		//determine date
		core::stringw date = mySolarSystem->J2000ToGreg(Day);

		//handlePlanets
		handlePlanets();

		if(updateFieldLines)
		{
			updateFieldLines = false;
			FieldLines = mySolarSystem->calculateFieldLines(mySolarSystem->year, mySolarSystem->dayM,
															mySolarSystem->hour, mySolarSystem->minute,
															mySolarSystem->second);

			FieldLineNode = smgr->addEmptySceneNode();
			FieldLineNodeGSW = smgr->addEmptySceneNode(FieldLineNode);
			f32 Re = planetSize[closestIndex];
			for(int l=0; l<FieldLines.size(); l++)
			{
				//core::array<core::vector3df> posA = FieldLines[l];
				for(int p=0; p<FieldLines[l].size(); p++)
				{
					core::vector3df pos = FieldLines[l][p];
					scene::IMesh* cube = smgr->getGeometryCreator()->createCubeMesh(core::vector3df(.001f,.001f,.001f));
					cube->setHardwareMappingHint(scene::EHM_STATIC);
					scene::ISceneNode* cubeNode = smgr->addMeshSceneNode(cube,FieldLineNodeGSW);
					cubeNode->setPosition(core::vector3df( ((pos.X*Re)/scaleF)*1.03, ((pos.Y*Re)/scaleF)*1.03, ((pos.Z*Re)/scaleF)*1.03 ));
					cubeNode->setMaterialFlag(video::EMF_LIGHTING, false);
					cubeNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
					cubeNode->setMaterialFlag(video::EMF_POINTCLOUD, true);
					FieldLineNodeGSW->setRotation(core::vector3df(90,-90,0));
				}
			}
		}


		FieldLineNode->setPosition(closestBody->getPosition());
		core::vector3df toSunV = planets[9]->getPosition() - closestBody->getPosition();
		core::vector3df toSunA = toSunV.getHorizontalAngle();
		FieldLineNode->setRotation(toSunA);
		handlePlanetJump();

		//handle G-effects------------------------------------------------------------------------
		handleG();

		//set cameras position--------------------------------------------------------------------
		camVector += pull*timescale;
		flyCam->setPosition(camPos+camVector*timescale);

		//determine speed-------------------------------------------------------------------------
		speed = camVector.getLength()*scaleF;

		//handle trail----------------------------------------------------------------------------
		handleTrail();
		relativeMovement();

		//play Intro------------------------------------------------------------------------------
		playIntro(introseconds);

		//set force arrows direction & length-----------------------------------------------------
		flightmarker->setPosition(camPos+flyAt);
		core::vector3df toPull = pull.getHorizontalAngle();
		core::vector3df toVel = camVector.getHorizontalAngle();
		core::vector3df toTan = flyAt.getHorizontalAngle();

		velA->setPosition(camPos);
		forceA->setPosition(camPos);
		tanA->setPosition(camPos);

		velA->setRotation(toVel);
		forceA->setRotation(toPull);
		tanA->setRotation(toTan);

		f64 VelScale = core::min_(0.01, (camVector.getLength()/100.0)*1000.0);
		velA->setScale(core::vector3df(1.0,1.0,1.0)*VelScale);
		forceA->setScale(core::vector3df(1.0,1.0,1.0)*pull.getLength()*100000.0);

		tanA->setScale(core::vector3df(1.0,1.0,1.0)*0.001);

		//handle flyCam---------------------------------------------------------------------------
		handleZeroGCam(flyCam);

		//handle map-----------------------------------------------------------------------------
		handleMap();

		//setLightRadius-------------------------------------------------------------------------
		light->setRadius((sunDistanceKM/scaleF)*1.1);

		//draw-----------------------------------------------------------------------------------
		//render the scene into the postprocessing texture
		//driver->setRenderTarget(PP_Test->getFirstMap(), true, true, video::SColor(0,0,0,0));
		smgr->drawAll();
		//driver->setRenderTarget(0, true, true, video::SColor(0,0,0,0));
		//render the effect chain
		//PP_Test->renderEffect();
		//draw GUI-------------------------------------------------------------------------------

		core::stringw speedS;
		core::stringw oSpeedS;
		core::stringw currentOrbitDistanceS;
		core::stringw gforceS;

		core::stringw str = L"Distance to Sun: ";
		str += (int)sunDistanceKM;
		str += " KM\nOrbit Distance: ";
		currentOrbitDistanceS = (core::stringw)(currentOrbitDistance*scaleF);
		str += currentOrbitDistanceS.subString(0,currentOrbitDistanceS.findFirst('.')+3);
		str += " KM\nCurrent G-force: ";
		gforceS = (core::stringw)gforce;
		str += gforceS.subString(0,gforceS.findFirst('.')+5);
		str += " m/s^2\nOrbital Velocity: ";
		oSpeedS = (core::stringw)oSpeed;
		str += oSpeedS.subString(0,oSpeedS.findFirst('.')+4);
		str += " KM/s\nTotal Velocity:  ";
		speedS = (core::stringw)speed;
		str += speedS.subString(0,speedS.findFirst('.')+4);
		str += " KM/s\n";
		str += date;
		env->getSkin()->setFont(fnt);
		info->setText(str.c_str());
		env->drawAll();

		//handle splitscreen---------------------------------------------------------------------
		if(view2)
		{
			smgr->setActiveCamera(flyCam);
			//allstars->setPosition(splitSCam->getAbsolutePosition());
		}
		if(!view2)
		{
			smgr->setActiveCamera(splitSCam);
			//allstars->setPosition(camPos);
		}
		driver->setViewPort(core::rect<s32>(Width*0.76,Height*0.76,Width,Height));

		if(view2)
		{
			forceA->setVisible(true);
			velA->setVisible(true);
			tanA->setVisible(true);
			smgr->drawAll();
			forceA->setVisible(true);
			velA->setVisible(true);
			tanA->setVisible(true);
			smgr->setActiveCamera(splitSCam);
		}
		if(!view2)
		{
			forceA->setVisible(true);
			velA->setVisible(true);
			tanA->setVisible(true);
			smgr->drawAll();
			forceA->setVisible(false);
			velA->setVisible(false);
			tanA->setVisible(false);
			smgr->setActiveCamera(flyCam);
		}

		//end-------------------------------------------------------------------------------------
		driver->endScene();

	}

	device->drop();
	//engine->drop();
	return 0;


}
