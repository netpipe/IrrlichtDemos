/*

Flag - cpp file

*/

#include "Flag.h"


// constructor
cFlag::cFlag() {
	i=0;
	speed=5;
}

// destructor
cFlag::~cFlag() {
}

// Initialization
bool cFlag::Initialize(IrrlichtDevice *device, core::vector3df position, core::vector3df rotation, core::vector3df scale) {
	
	scene::ISceneManager *smgr = device->getSceneManager();
	video::IVideoDriver *driver = device->getVideoDriver();
	
	for (irr::s32 i=0; i<SKT_BONE_COUNT; i++)
		bonesOsci.push_back(core::vector3df(0,0,0));
	
			// save bone names in boneName array
	for (int n=0; n < SKT_BONE_COUNT; n++) {

		switch (n) {
		// main items
		case SKT_BONE01:
			boneName.push_back((irr::c8*) "bone01");
			break;
		case SKT_BONE02:
			boneName.push_back((irr::c8*) "bone02");
			break;
		case SKT_BONE03:
			boneName.push_back((irr::c8*) "bone03");
			break;
		case SKT_BONE04:
			boneName.push_back((irr::c8*) "bone04");
			break;
		case SKT_BONE05:
			boneName.push_back((irr::c8*) "bone05");
			break;
		case SKT_BONE06:
			boneName.push_back((irr::c8*) "bone06");
			break;
		case SKT_BONE07:
			boneName.push_back((irr::c8*) "bone07");
			break;
		}
		
	}
	
	scene::IAnimatedMesh *mesh = smgr->getMesh("media/flag/flag.x");
	flagNode = smgr->addAnimatedMeshSceneNode(mesh);

	if (flagNode) {
		flagNode->setScale(scale);
		flagNode->setPosition(position);
		flagNode->setRotation(rotation);
		
		flagNode->setMaterialTexture(0, driver->getTexture("media/flag/flag.png"));
		flagNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		flagNode->setMaterialFlag(video::EMF_LIGHTING, false);		

		flagNode->setFrameLoop(0,0);
		flagNode->setJointMode(irr::scene::EJUOR_CONTROL);
		
		// get all bone scene nodes
		for (int n=0; n < SKT_BONE_COUNT; n++) {
			bones.push_back(flagNode->getJointNode(boneName[n]));
		}		
	} else
		return false;
	
	bonesRot.clear();
	for (irr::u32 n=0; n<SKT_BONE_COUNT; n++)
		bonesRot.push_back( bones[n]->getRotation() );

	return true;
}

// draw flag
void cFlag::drawFlag() {
	
	u32 a = i%360;
	u32 b = (i-50)%360;
	u32 c = (i-100)%360;
	u32 d = (i-150)%360;
	u32 e = (i-200)%360;
	u32 f = (i-250)%360;
	u32 g = (i-300)%360;

	f32 rad = 3.14159/180;
	f32 multi = 2.f;

	f32 const a1 = a * rad;
	f32 const a2 = b * rad;
	f32 const a3 = c * rad;
	f32 const a4 = d * rad;
	f32 const a5 = e * rad;
	f32 const a6 = f * rad;
	f32 const a7 = g * rad;

	// oscillation functions
	f32 const sinA = sin(a*rad);
	f32 const sinB = sin(b*rad);
	f32 const sinC = sin(c*rad);
	f32 const sinD = sin(d*rad);
	f32 const sinE = sin(e*rad);
	f32 const sinF = sin(f*rad);
	f32 const sinG = sin(g*rad);
	
	// oscillation functions
	f32 const sinATwo = sin(2*a1);
	f32 const sinBTwo = sin(2*a2);
	f32 const sinCTwo = sin(2*a3);
	f32 const sinDTwo = sin(2*a4);
	f32 const sinETwo = sin(2*a5);
	f32 const sinFTwo = sin(2*a6);
	f32 const sinGTwo = sin(2*a7);

	// set new bone rotations
	bonesOsci[SKT_BONE01] = core::vector3df(0,0, 5.f * sinA);
	bonesOsci[SKT_BONE02] = core::vector3df(multi*1.f*sinATwo,0, 9.f * sinB);
	bonesOsci[SKT_BONE03] = core::vector3df(multi*2.f*sinBTwo,0, 13.f * sinC);
	bonesOsci[SKT_BONE04] = core::vector3df(multi*4.f*sinCTwo,0, 17.f * sinD);
	bonesOsci[SKT_BONE05] = core::vector3df(multi*8.f*sinDTwo,0, 25.f * sinE);
	bonesOsci[SKT_BONE06] = core::vector3df(multi*12.f*sinETwo,0, 30.f * sinF);
	bonesOsci[SKT_BONE07] = core::vector3df(multi*14.f*sinFTwo,0, 15.f * sinG);
	
	// apply all rotations to bones
	for (irr::u32 n=0; n<SKT_BONE_COUNT; n++)
		bones[n]->setRotation(matrixRotation(bonesRot[n], bonesOsci[n]));

	// check if one periode is passed and set a new speed variable
	// to simulate more or less wind
	if (a<10) {
		speed = rand() % 10;
		if (speed < 4)
			speed+=4;
	}
		
	i+=speed;
}

