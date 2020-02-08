// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef CONFIG_ENUMS_H
#define CONFIG_ENUMS_H

enum EParticleMode
{
    EPM_ON,
    EPM_WEATHER_OFF,
    EPM_ALL_OFF,
};

enum ELightMode
{
	ELM_STATIC,
	ELM_DYNAMIC
};

enum ETouchInput
{
	ETI_NO_TOUCH,
	ETI_TOUCH_SIMULATION,	// use mouse-clicks to simulate touch-input
	ETI_TOUCH_HARDWARE		// so far this means Android devices
};

#endif // CONFIG_ENUMS_H
