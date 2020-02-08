// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef COMPILE_CONFIG_H
#define COMPILE_CONFIG_H

#define HC1_MAJOR_REV 1
#define HC1_MINOR_REV 3

// #define HOVER_RELEASE // define if this version will go to customers (enabled by default in release targets). TODO: Make this HOVER_RETAIL and add another target besides debug and release - retail and release are not the same.
// #define HOVER_DEMO 	// define if this should only be a demo-version of the game
#define HOVER_NO_LOCKS 	// define if you don't want any keymanagement for restrictions at all (should by now always be set)

//	#define NEURAL_AI   // I can't yet get myself to completely remove it, but don't expect it to do anything

#define HC1_HAS_FREETYPE

// IRR_SVN: Remember the svn versions used as base for the Irrlicht version against which we compile.
// 			My Irrlicht is maybe modified - but helps having that info around.
// IRR_REV: An internal counter for when Irrlicht is updated in the project in a way where code using it will no longer be backward compatible.
#ifdef _IRR_ANDROID_PLATFORM_
	#define IRR_SVN 5063
	#define IRR_REV 4
	#define HC1_ENABLE_SOUND
	#define HC1_OGLES
	#define HOVER_ADS	// Allow advertising (can be disabled again by runtime settings)
	#ifdef HC1_ENABLE_SOUND
		#define HC1_ENABLE_OPENAL
		#define HC1_ENABLE_OGG
	#endif
#else
	#define IRR_SVN 4973
	#define IRR_REV 4

	#define HC1_ENABLE_SOUND
	#ifdef HC1_ENABLE_SOUND
		#define HC1_ENABLE_OPENAL
		#define HC1_ENABLE_OGG
	#endif
	#define HC1_HAS_SDL  	// is SDL available? Needed for joystick

	#define HOVER_ADS	// Allow advertising (can be disabled again by runtime settings)

	// Enable editor.
	#define HC1_ENABLE_EDITOR
#endif

#if (IRR_REV >= 3)
	 #define HC1_ENABLE_PROFILE	// comment out to remove the profiling code
#endif

// Make sure debug stuff is all disabled in release
#if defined(HOVER_RELEASE)
	#undef HC1_ENABLE_PROFILE
	#undef HC1_ENABLE_EDITOR
#endif

#endif // COMPILE_CONFIG_H
