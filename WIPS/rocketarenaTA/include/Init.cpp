/*

Init class - cpp

*/

#include "Init.h"

// constructor
cInit::cInit() {
	
	videoIniSection = (irr::c8*) ("video");
	keyboardIniSection = (irr::c8*) ("keyboard");
	audioIniSection = (irr::c8*) ("audio");
	
	// standard parameters
	parameters.AntiAlias = false;
	parameters.Bits = 32;
	parameters.DriverType = video::EDT_OPENGL;
	parameters.EventReceiver = &eventReceiver;
	parameters.Fullscreen = false;
	parameters.HighPrecisionFPU = false;
	parameters.Stencilbuffer = true;
	parameters.Vsync = false;
	parameters.WindowSize = core::dimension2d<s32>(800,600);
	
	// checking rocket.ini for different parameters
	IIniFileReader* ini = createIniFileReader("rocket.ini");
	
	// if ini file doesnt excists, create a new one with standard parameters
	if (!ini->load()) {
		
		ini->drop();
		ini = createIniFileReaderEmpty();
		ini->setSaveFileName("rocket.ini");
		
		// device section
		ini->addSection(videoIniSection);

		ini->addKeyToSection(videoIniSection, "AA", "0");
		ini->addKeyToSection(videoIniSection, "Bits", "32");
		ini->addKeyToSection(videoIniSection, "DriverType", "opengl");
		ini->addKeyToSection(videoIniSection, "Fullscreen", "0");
		ini->addKeyToSection(videoIniSection, "HPFPU", "0");
		ini->addKeyToSection(videoIniSection, "Stencilbuffer", "0");
		ini->addKeyToSection(videoIniSection, "VSync", "0");
		ini->addKeyToSection(videoIniSection, "DimensionH", "800");
		ini->addKeyToSection(videoIniSection, "DimensionV", "600");
		
		// keyboard section
/*		ini->addSection(keyboardIniSection);

		ini->addKeyToSection(keyboardIniSection, "forward", "w");
		ini->addKeyToSection(keyboardIniSection, "backward", "s");
		ini->addKeyToSection(keyboardIniSection, "turn_right", "right");
		ini->addKeyToSection(keyboardIniSection, "turn_left", "left");
		ini->addKeyToSection(keyboardIniSection, "jump", "space");
		ini->addKeyToSection(keyboardIniSection, "right_cable", "D");
		ini->addKeyToSection(keyboardIniSection, "left_cable", "A");
*/		
		ini->save();
		
		ini->drop();
		
	} else {
		
		// return parameters for device
		irr::c8 *value;
		video::E_DRIVER_TYPE driverType;
		core::dimension2d<s32> windowSize;
		parameters.AntiAlias = ini->getValueB(videoIniSection, "AA");
		parameters.Bits = ini->getValueI(videoIniSection, "Bits");

		value = ini->getValue(videoIniSection, "DriverType");
		if (value == "opengl")
			driverType = video::EDT_OPENGL;
		else if (value == "direct3d8")
			driverType = video::EDT_DIRECT3D8;
		else if (value == "direct3d9")
			driverType = video::EDT_DIRECT3D9;
		else if (value == "software")
			driverType = video::EDT_SOFTWARE;
		else if (value == "burningsvideo")
			driverType = video::EDT_BURNINGSVIDEO;
		else
			driverType = video::EDT_OPENGL;
			
		parameters.DriverType = driverType;
		
		parameters.Fullscreen = ini->getValueB(videoIniSection, "Fullscreen");
		parameters.HighPrecisionFPU = ini->getValueB(videoIniSection, "HPFPU");
		parameters.Stencilbuffer = ini->getValueB(videoIniSection, "Stencilbuffer");
		parameters.Vsync = false;
		
		windowSize.Width = ini->getValueI(videoIniSection, "DimensionH");
		windowSize.Height = ini->getValueI(videoIniSection, "DimensionV");
		parameters.WindowSize = windowSize;

	}
	
	device = createDeviceEx(parameters);
	
	device->getCursorControl()->setVisible(true);
	
	device->setEventReceiver(&eventReceiver);
	
	smgr = device->getSceneManager();
	
	driver = device->getVideoDriver();
	
	camera = smgr->addCameraSceneNode(0,core::vector3df(-710,-15,200), core::vector3df(0,0,0));

	camera->setInputReceiverEnabled(false);

	camera->setTarget(core::vector3df(0,10,0));

}

// destructor
cInit::~cInit() {
}

cEventReceiver *cInit::getEventReceiver() {
	return &eventReceiver;	
}

IrrlichtDevice* cInit::getDevice() {
	return device;
}

scene::ICameraSceneNode *cInit::getCamera() {
	return camera;
}

// return device parameters
SIrrlichtCreationParameters *cInit::getDeviceParameters() {
	return &parameters;
}
