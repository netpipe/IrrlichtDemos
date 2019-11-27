/*

Init class - header

*/

#ifndef __Init_h
#define __Init_h

#include <irrlicht.h>

using namespace irr;
using namespace io;

#include <EventReceiver.h>
//#include <IIniFileReader.h>
#include <CXMLReader.h>

class cInit {
public:

	// constructor
	cInit();

	// destructor
	~cInit();

	IrrlichtDevice *getDevice();

	cEventReceiver *getEventReceiver();

	// returns the camera
	scene::ICameraSceneNode *getCamera();

	// return device parameters
	SIrrlichtCreationParameters *getDeviceParameters();


private:
	IrrlichtDevice *device;
	SIrrlichtCreationParameters parameters;
	cEventReceiver eventReceiver;
	scene::ISceneManager *smgr;
//	ISoundEngine *audio;
	video::IVideoDriver *driver;
	scene::ICameraSceneNode *camera;

	// pointer to ini file reader
//	io::IIniFileReader *ini;

	irr::c8 *videoIniSection;
	irr::c8	*keyboardIniSection;
	irr::c8 *audioIniSection;

};

#endif
