#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <IrrlichtDevice.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif
#include "Configuration.h"

class Application
{

private:

	Configuration configuration;

	irr::IrrlichtDevice* device;
#ifdef _SOUND
	irrklang::ISoundEngine* soundEngine;
#endif

public:

	Application();

	const bool init();

	const void run();
};

#endif /*APPLICATION_H_*/
