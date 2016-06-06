#include <irrlicht.h>
#include <irrXML.h>
#include <iostream>
#include "GameWorld.h"
using namespace irr;
#pragma comment(lib,"Irrlicht.lib")
//uncomment to disable console
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
irr::io::IrrXMLReader *xml;
int main()
{
	u32 bit = 32;
	//video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;

	//std::string a = "../res/xml/config.xml" ;
	//xml = irr::io::createIrrXMLReader(a.c_str());

	IrrlichtDevice* device = createDevice(driverType, core::dimension2d<s32>(800, 600),bit,false);
	GameWorld *gWorld = new GameWorld(device);

	gWorld->run();

	if(gWorld->device != 0)
		gWorld->device->drop();

	//uncomment so that cmd don't close at the end of program
	//system("PAUSE");

	return 0;
}
