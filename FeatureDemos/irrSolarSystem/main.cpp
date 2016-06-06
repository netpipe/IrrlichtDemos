/*
written by Tobias Houfek

The Irrlicht Engine License

Copyright © 2002-2005 Nikolaus Gebhardt

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include <string>
#include <iostream>
#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <irrlicht.h>
#include <ctime>
#include "solar.h"

#pragma comment(lib,"irrlicht.lib")
using namespace irr;

IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;
scene::ICameraSceneNode* camera2;
gui::IGUIEnvironment* env;

SolarSystem* mySolarSystem;

bool up=false,down=false,left=false,right=false,jump=false,crouch=false,leftB=false,rightB=false,mouseW=false;
f32 lastWheelMovement;
float Day = 0;
float timescale = 0.1;
float delta;
float scaleF = 100000.0f;

float sunDistanceKM;
float sunDistanceAU;
float gforce;
float speed;

core::vector3df camPos = core::vector3df(0,0,0);
core::vector3df camVector = core::vector3df(0,1.4,0);
core::vector3df pull = core::vector3df(0,0,0);

scene::IBillboardSceneNode* flightmarker;

core::array<scene::ISceneNode*> planets;
core::array<scene::IBillboardSceneNode*> trail;
core::array<float> pSize;
core::array<float> pMass;
core::array<float> orbitT;
core::array<core::stringw> randomFacts;
core::array<core::stringw> intro;
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
				if(event.KeyInput.Key==KEY_SPACE)
					jump=true;
				if(event.KeyInput.Key==KEY_KEY_C)
					crouch=true;
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
				if(event.KeyInput.Key==KEY_SPACE)
					jump=false;
				if(event.KeyInput.Key==KEY_KEY_C)
					crouch=false;
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
			}//if(event.EventType == EGET_SCROLL_BAR_CHANGED)
		}
		return false;
	}
}my_event_receiver;
void zeroGCam(scene::ICameraSceneNode* cam)
{
	float camSpeed = 0;
	float camSpeedh = 0;
	float camSpeedv = 0;
	core::vector3df currentDir = camPos - cam->getTarget();
	currentDir.normalize();

	if(leftB){
		camSpeed -= 0.25f;
		camVector += currentDir*camSpeed;
		//printf("%f\n",camSpeed);
	}if(rightB){
		camSpeed += 0.25f;
		camVector += currentDir*camSpeed;
		//printf("%f\n",camSpeed);
	}if(left){
		camSpeedh -= 0.15f;
		core::vector3df left = currentDir.crossProduct(cam->getUpVector());
		camVector += left*camSpeedh;
		//printf("%f\n",camSpeedh);
	}if(right){
		camSpeedh += 0.15f;
		core::vector3df right = currentDir.crossProduct(cam->getUpVector());
		camVector += right*camSpeedh;
		//printf("%f\n",camSpeedh);
	}if(up){
		camSpeedv += 0.15f;
		core::vector3df up = cam->getUpVector();
		camVector += up*camSpeedv;
		//printf("%f\n",camSpeedh);
	}if(down){
		camSpeedv -= 0.15f;
		core::vector3df down = cam->getUpVector();
		camVector += down*camSpeedv;
		//printf("%f\n",camSpeedh);
	}
}

void handleG()
{
	pull = core::vector3df(0,0,0);
	u32 i;
	f32 force = 0;
	for(i=0; i<planets.size(); i++)
	{
		core::vector3df planPos = planets[i]->getPosition();
		camPos = camera->getPosition();
		core::vector3df cam2plan = planPos-camPos;
		f32 distance = planPos.getDistanceFrom(camPos);
		force = (pMass[i]/7000)/pow(distance,2);
		cam2plan.setLength(force);
		pull += cam2plan;
		//printf("%f\n",force);
		//printf("%f ,%f ,%f ,\n",pull.X,pull.Y,pull.Z);
		if(i==9){
			sunDistanceKM = (distance*scaleF);
			sunDistanceAU = (distance*scaleF/149598000.0);
			gforce = (pull.getLength()*scaleF*delta);

		}
	}
	pull -= pull/1000;
}
void debug(scene::ISceneNode* node, f32 radius){

	//debug:
	scene::ISceneNodeAnimator* circle = smgr->createFlyCircleAnimator(node->getPosition(),radius,0.0005);
	camera->setPosition(node->getPosition()+core::vector3df(0,0,radius));
	camera->addAnimator(circle);
	scene::IMesh* grid = smgr->getMesh("./data/grid.x");
	scene::IMeshSceneNode* gridn = smgr->addMeshSceneNode(grid);
	gridn->setScale(core::vector3df(10,10,10));
//	grid->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_WIREFRAME,true);
//	grid->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS,true);
	gridn->setPosition(node->getPosition()+core::vector3df(0,-5000,0));
}

void handleTrail()
{
	//scene::IBillboardSceneNode* trailcube = smgr->addBillboardSceneNode(0,core::dimension2df(3,3),camera->getPosition());
//	trailcube->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_POINTCLOUD,true);
//	trailcube->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING,false);
	//trail.push_back(trailcube);
//	if(trail.size()>200){
	//	scene::ISceneNode* remove = trail[0];
	//	trail.erase(0);
	//	remove->remove();
	//}
}
void createSolarSystem()
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

	intro.push_back(core::stringw("Our Solarsystem is a vast place."));
	intro.push_back(core::stringw("This model has proportional planets and orbits,"));
	intro.push_back(core::stringw("but their distance to the sun is scaled by 1/100000."));
	intro.push_back(core::stringw("In reality those planets would be 100000x farther away..."));
	intro.push_back(core::stringw("from here you wouldn't be able to see them, not even a pixel."));
	intro.push_back(core::stringw("Also the sun's radius is divided by 50."));
	intro.push_back(core::stringw("The planets masses, thus gravitational attractions are altered"));
	intro.push_back(core::stringw("Except for that, it's as real as it gets."));
	intro.push_back(core::stringw("The planets position is calculated by painfully hard maths(I really dont get it)"));
	intro.push_back(core::stringw("and supposedly accurate for many centuries ahead"));
	intro.push_back(core::stringw("The gravitation simulation is somewhat simplified"));
	intro.push_back(core::stringw("The maths is painfully easy, even for me, but it obeys newtons laws"));
	intro.push_back(core::stringw("The distance you can read down-right is real, so is the speed"));
	intro.push_back(core::stringw("W-A-S-D controls your craft..."));
	intro.push_back(core::stringw("W moves your ship straight up, S moves down"));
	intro.push_back(core::stringw("A moves your ship left, D moves right"));
	intro.push_back(core::stringw("think of them as thrusters in 2 axis, up-down, left-right"));
	intro.push_back(core::stringw("It is important to understand that we're moving in zero G"));
	intro.push_back(core::stringw("That is nearly zero gravity, because right now..."));
	intro.push_back(core::stringw("we are chaotically orbiting earth"));
	intro.push_back(core::stringw("The blue circle you can sometimes see is your flightmarker"));
	intro.push_back(core::stringw("if you look at it and press the left mouse button you will see"));
	intro.push_back(core::stringw("we're accelerating !!!"));
	intro.push_back(core::stringw("if you press the right-button your reverse thruster ignites"));
	intro.push_back(core::stringw("so it's a truely 3-dimensional movement,"));
	intro.push_back(core::stringw("still unlike any aircraft you might have ever flown"));
	intro.push_back(core::stringw("it can get a little confusing, but you'll get used to it"));
	intro.push_back(core::stringw("search for your flightmarker to understand your movement"));
	intro.push_back(core::stringw("The easiest way to get into an orbit with a planet or the sun"));
	intro.push_back(core::stringw("is keep looking at it and adjusting your ellipse and distance"));
	intro.push_back(core::stringw("or just sit back and watch Keplerian dynamics unfold..."));
	intro.push_back(core::stringw("enjoy..."));

	//km
	pSize.push_back(2440.0f);
	pSize.push_back(6052.0f);
	pSize.push_back(6378.0f);
	pSize.push_back(3397.0f);
	pSize.push_back(71492.0f);
	pSize.push_back(60268.0f);
	pSize.push_back(25559.0f);
	pSize.push_back(24766.0f);
	pSize.push_back(1150.0f);
	pSize.push_back(695000.0f);

	//+e21 kg
	pMass.push_back(4300000); //33000
	pMass.push_back(4869000); //4869000
	pMass.push_back(5972000);
	pMass.push_back(6421900);
	pMass.push_back(2900000000); //1900000000
	pMass.push_back(3680000000);//56800000
	pMass.push_back(86830000);
	pMass.push_back(1024700000);
	pMass.push_back(1270);
	pMass.push_back(198900000); //Sun 1989000000000

	//earth days
	orbitT.push_back(87.97/500);
	orbitT.push_back(224.70/500);
	orbitT.push_back(365.261/500);
	orbitT.push_back(686.98/500);
	orbitT.push_back(4332.71/500);
	orbitT.push_back(10759.50/500);
	orbitT.push_back(30685.00/500);
	orbitT.push_back(60190.00/500);
	orbitT.push_back(90550.00/500);

	core::array<core::vector3df> startDayPos = mySolarSystem->calculateAtDay(Day,-1);

	u32 i;
	u32 j;
	for(i=0; i<9; i++){
		for(j=0; j<500; j++){
			Day += orbitT[i];
			core::array<core::vector3df> result = mySolarSystem->calculateAtDay(Day,i);
			scene::ISceneNode* Onode = smgr->addCubeSceneNode(i+1,0,-1,result[0]/scaleF);
//			Onode->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
//				Onode->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_WIREFRAME, true);
		}
	}

	scene::ISceneNode* SkyBox = smgr->addSkyBoxSceneNode (
		driver->getTexture("./data/star_top.jpg"),
		driver->getTexture("./data/star_bot.jpg"),
		driver->getTexture("./data/star1.jpg"),
		driver->getTexture("./data/star3.jpg"),
		driver->getTexture("./data/star2.jpg"),
		driver->getTexture("./data/star4.jpg"));

	video::ITexture *tex1 =  driver->getTexture("./data/mercurymap.jpg");
	video::ITexture *tex2 =  driver->getTexture("./data/mercurybump.jpg");
	video::ITexture *tex3 =  driver->getTexture("./data/venusmap.jpg");
	video::ITexture *tex4 =  driver->getTexture("./data/venusbump.jpg");
	video::ITexture *tex5 =  driver->getTexture("./data/earthmap1k.jpg");
	video::ITexture *tex6 =  driver->getTexture("./data/earthbump1k.jpg");
	video::ITexture *tex7 =  driver->getTexture("./data/mars_1k_color.jpg");
	video::ITexture *tex8 =  driver->getTexture("./data/mars_1k_topo.jpg");
	video::ITexture *tex9 =  driver->getTexture("./data/jupitermap.jpg");
	video::ITexture *tex10 =  driver->getTexture("./data/mercurybump.jpg");
	video::ITexture *tex11 =  driver->getTexture("./data/saturnmap.jpg");
	video::ITexture *tex12 =  driver->getTexture("./data/mercurybump.jpg");
	video::ITexture *tex13 =  driver->getTexture("./data/uranusmap.jpg");
	video::ITexture *tex14 =  driver->getTexture("./data/mercurybump.jpg");
	video::ITexture *tex15 =  driver->getTexture("./data/neptunemap.jpg");
	video::ITexture *tex16 =  driver->getTexture("./data/mercurybump.jpg");
	video::ITexture *tex17 =  driver->getTexture("./data/plutomap1k.jpg");
	video::ITexture *tex18 =  driver->getTexture("./data/plutobump1k.jpg");
	video::ITexture *tex19 =  driver->getTexture("./data/earthcloudmap.tga");
	video::ITexture *tex20 =  driver->getTexture("./data/ebill.jpg");
	video::ITexture *tex21 =  driver->getTexture("./data/saturnringcolor.tga");


	video::ITexture *sunbill =  driver->getTexture("./data/bill.jpg");
	video::ITexture *sun =  driver->getTexture("./data/diffuse.jpg");

	scene::IBillboardSceneNode* earthBill = smgr->addBillboardSceneNode(0,core::dimension2df(80,80));
//	earthBill->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
//	earthBill->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ADD_COLOR);
	earthBill->setMaterialTexture(0,tex20);

	Day = mySolarSystem->day_number(2012,12,25,12,00);
	startDayPos = mySolarSystem->calculateAtDay(Day,-1);

	for(i=0; i<startDayPos.size(); i++){
		scene::ISceneNode* planet;
		scene::ISceneNode* atmo;
		scene::ISceneNode* ring;
		scene::IAnimatedMesh* planetMesh = smgr->getMesh("./data/planet.obj");
		scene::IAnimatedMesh* ringMesh = smgr->getMesh("./data/ring.obj");
		scene::IMesh* tangentMesh = smgr->getMeshManipulator()->createMeshWithTangents(planetMesh);

		switch(i){
		case 0:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex1);
			planet->setMaterialTexture(1,tex2);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 1:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex3);
			planet->setMaterialTexture(1,tex4);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 2:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex5);
			planet->setMaterialTexture(1,tex6);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			atmo = smgr->addMeshSceneNode(tangentMesh);
			 //NO IDEA WHY THE OFFSET!!! TRIAL & ERROR
			atmo->setPosition((startDayPos[i]/scaleF)+core::vector3df(-1469,-1,88));
			atmo->setParent(planet);
			atmo->setScale(planet->getScale()*1.64);
//			atmo->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
			atmo->setMaterialTexture(0,tex19);
//			atmo->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			earthBill->setPosition((startDayPos[i]/scaleF)+core::vector3df(-1469,-1,88));
			earthBill->setParent(planet);
			break;
		case 3:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex7);
			planet->setMaterialTexture(1,tex8);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 4:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex9);
			planet->setMaterialTexture(1,tex10);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 5:
			planet = smgr->addMeshSceneNode(tangentMesh);
			ring = smgr->addMeshSceneNode(ringMesh);
			ring->setParent(planet);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex11);
			planet->setMaterialTexture(1,tex12);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
			//NO IDEA WHY THE OFFSET!!! TRIAL & ERROR
			ring->setPosition((startDayPos[i]/scaleF)+core::vector3df(8190,-625,12119));
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
//			ring->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ALPHA_CHANNEL);
			ring->setMaterialTexture(0,tex21);
//			ring->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
//			ring->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING,false);
			planet->setRotation(core::vector3df(0,0,-26.44));
			planets.push_back(planet);
			break;
		case 6:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex13);
			planet->setMaterialTexture(1,tex14);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 7:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex15);
			planet->setMaterialTexture(1,tex16);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		case 8:
			planet = smgr->addMeshSceneNode(tangentMesh);
			planet->setMaterialType(video::EMT_NORMAL_MAP_SOLID);
			planet->setMaterialTexture(0,tex17);
			planet->setMaterialTexture(1,tex18);
			planet->setScale(core::vector3df(pSize[i]/(scaleF/10),pSize[i]/(scaleF/10),pSize[i]/(scaleF/10)));
			planet->setPosition(startDayPos[i]/scaleF);
//			planet->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
			planets.push_back(planet);
			break;
		}


	}

	scene::IBillboardSceneNode* sunBill = smgr->addBillboardSceneNode(0,core::dimension2df(600,600));
//	sunBill->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
//	sunBill->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ADD_COLOR);
	sunBill->setMaterialTexture(0,sunbill);
	scene::ISceneNode* Sun = smgr->addSphereSceneNode(pSize[9]/scaleF,512);
	Sun->setMaterialFlag(video::EMF_LIGHTING, false);
//	Sun->setMaterialType(video::E_MATERIAL_TYPE::EMT_LIGHTMAP);
	Sun->setMaterialTexture(0,sun);
	planets.push_back(Sun);
}
int main()
{
	irr::SIrrlichtCreationParameters param;
	param.AntiAlias = true;
	param.HighPrecisionFPU= true;
	param.Vsync= true;
	param.DriverType = video::EDT_OPENGL;
	param.WindowSize = core::dimension2di(1024,768);
	param.Bits = 32;
	param.Stencilbuffer = true;
	param.Fullscreen = 0;

	device=createDeviceEx(param);
	device->setEventReceiver(&my_event_receiver);
	device->getCursorControl()->setVisible(false);
	driver=device->getVideoDriver();
	smgr=device->getSceneManager();
	device->getTimer()->setTime( 0 );
	env = device->getGUIEnvironment();

	f32 Width = driver->getScreenSize().Width;
	f32 Height = driver->getScreenSize().Height;

	gui::IGUIFont* fnt = env->getFont("./data/Impact_11px.xml");
	gui::IGUIFont* fnt2 = env->getFont("./data/SegoeUI_10px_bold.xml");

	env->getSkin()->setFont(fnt);
	gui::IGUIStaticText* info = env->addStaticText(
		L"",core::rect<s32>(Width*0.8,Height*0.65,Width,Height*0.79), true, true, 0, -1, true);
	gui::IGUIStaticText* info2 = env->addStaticText(
		L"",core::rect<s32>(Width*0.05,0,Width*0.95,Height*0.05), true, true, 0, -1, true);

	core::matrix4 mat4;
	video::SMaterial mat;
	mat.Lighting = false;
	driver->setMaterial(mat);
	driver->setTransform(video::ETS_WORLD,mat4);

	createSolarSystem();

	smgr->addLightSceneNode(0, core::vector3df(0,0,0),video::SColorf(1.0f,1.0f,1.0f),8000.0f);

	camera=smgr->addCameraSceneNodeFPS(0,100,0);
	camera->setFarValue(10000);
	camera->setPosition(planets[2]->getPosition()+core::vector3df(0,0,140));
	camera->setTarget(core::vector3df(0,0,0));

	camera2=smgr->addCameraSceneNode();
	camera2->setFarValue(10000);
	camera2->isOrthogonal();
	//smgr->setActiveCamera(camera2);

	core::vector3df camPos = camera->getPosition();
	core::vector3df camDir = camPos - camera->getTarget();
	flightmarker = smgr->addBillboardSceneNode(0,core::dimension2df(0.6,0.6),camDir);
	flightmarker->setMaterialFlag(video::EMF_LIGHTING, false);
//	flightmarker->setMaterialFlag(video::E_MATERIAL_FLAG::EMF_LIGHTING, false);
//	flightmarker->setMaterialType(video::E_MATERIAL_TYPE::EMT_TRANSPARENT_ADD_COLOR);
	video::ITexture *tex21 =  driver->getTexture("./data/flightmarker.jpg");
	flightmarker->setMaterialTexture(0,tex21);


	f32 limitseconds = 0;
	f32 introseconds = 0;
	u32 introcount = 0;
	f32 oldseconds = 0;

	//debug(planets[5],pSize[5]+10000);
	//loop
	while(device->run()) {
		//begin

		driver->setViewPort(core::rect<s32>(0,0,Width,Width));
		driver->beginScene(true,true,video::SColor(0,0,0,0));
		//determine time
		u32 mseconds = (device->getTimer()->getTime());
		delta = mseconds-oldseconds;
		oldseconds = mseconds;
		limitseconds += delta;
		introseconds += delta;
		delta /= 1000;

		//handle camera
		camVector += pull;
		camPos = camera->getPosition();
		camera->setPosition(camPos+camVector);
		speed = core::round32(camVector.getLength()*scaleF*delta);

		camera2->setPosition(core::vector3df(camPos.X,camPos.Y,camPos.Z)+core::vector3df(500,1700,500));//planets[5]->getPosition()+
		camera2->setTarget(camera->getPosition());
		//handle trail
		//if(limitseconds >= 30){
		//	limitseconds = 0;
		//	handleTrail();
		//}
		if(introcount < intro.size()){
			if(introseconds >= 6500){
				introseconds = 0;
				env->getSkin()->setFont(fnt2);
				info2->setText(intro[introcount].c_str());
				introcount++;
			}
		}else if(introcount >= intro.size()){
			if(introseconds >= 1000){
				introseconds = 0;
				srand(time(NULL));
				//generates the random number by using the system clock time
				int Number = (rand() % (29));
				env->getSkin()->setFont(fnt2);
				info2->setText(randomFacts[Number].c_str());
				//printf("%d \n",Number);
			}
		}
		flightmarker->setPosition(camPos+camVector*2);
		zeroGCam(camera);

		//handle G-effects
		handleG();

		//set time
		Day += delta*timescale;

		//update Position
		core::array<core::vector3df> updatePos = mySolarSystem->calculateAtDay(Day,-1);

		//set Position
		u32 i;
		for(i=0; i<updatePos.size(); i++)
		{
			planets[i]->setPosition(updatePos[i]/scaleF);
		}
		//draw & end "Distance to Sun: XX AU\n\nCurrent G-force: XXXXX m/s\n\nCurrent Speed: XXXXXXX km/s"
		core::stringw str = L"Distance to Sun: ";
        str += (int)sunDistanceKM;
		str += " KM\n = ";
		str += sunDistanceAU;
		str +=	" AU\nCurrent G-force:";
        str += (int)gforce;
		str += " KM/s\nCurrent Speed:";
        str += (int)speed;
		str += " KM/s";
		env->getSkin()->setFont(fnt);
		info->setText(str.c_str());
		smgr->drawAll();
		env->drawAll();
		//Set viewpoint to the first quarter (left top)
		smgr->setActiveCamera(camera2);
		driver->setViewPort(core::rect<s32>(Width*0.8,Height*0.8,Width,Height));
		smgr->drawAll();
		smgr->setActiveCamera(camera);
		/*
		u32 j;
		core::matrix4 mat4;
		video::SMaterial mat;
		mat.Lighting = false;
		driver->setMaterial(mat);
		driver->setTransform(video::ETS_WORLD,mat4);
		for(j=0; j<planetsO.size(); j++){
			driver->drawVertexPrimitiveList(
			&planetsO[j],
			100,
			&planetOi[j],
			201,
			video::E_VERTEX_TYPE::EVT_STANDARD,
			scene::E_PRIMITIVE_TYPE::EPT_LINES,
			video::E_INDEX_TYPE::EIT_16BIT);
		}
		*/
		device->sleep(15,0);
		driver->endScene();
	}

	device->drop();
	return 0;


}
