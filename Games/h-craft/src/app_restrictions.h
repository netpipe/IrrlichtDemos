// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef RELEASE_MODE_H
#define RELEASE_MODE_H

#include "compile_config.h"

class AppRestrictions
{
public:
	static bool IsKeyUnlocked();
	static bool DoDisplayAds();


private:
	// Update internal info used in DoDisplayAds
	// return value < 0 for errors, 0 for OK, > 0 for no answer, but might get one with further polling
	static int PollBillingServer();
};

#if defined(HOVER_DEMO)
    #define IS_DEMO_VERSION 1
#elif defined(HOVER_NO_LOCKS)
    #define IS_DEMO_VERSION 0
#else
	// Don't use this anymore. Doesn't make sense once open-sourced.
    // Just keeping it in for fun for now - will kick some day.
    // Will need libcrypto++ to compile/link which is no longer part
    // of the project.
    #define IS_DEMO_VERSION (AppRestrictions::IsKeyUnlocked()?0:1)
#endif

#if defined(HOVER_ADS)
	#define DO_DISPLAY_ADS (AppRestrictions::DoDisplayAds()?1:0)
#else
	#define DO_DISPLAY_ADS 0
#endif

#endif // RELEASE_MODE_H
