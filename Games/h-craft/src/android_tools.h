#ifndef __IRR_ANDROID_TOOLS_H__
#define __IRR_ANDROID_TOOLS_H__

#include "compile_config.h"

#ifdef _IRR_ANDROID_PLATFORM_

#include <irrlicht.h>
#include <android_native_app_glue.h>
#include <string>

namespace hc1
{
namespace android
{
struct SDisplayMetrics
{
	irr::s32 widthPixels;
	irr::s32 heightPixels;
	irr::f32 density;
	irr::s32 densityDpi;
	irr::f32 scaledDensity;
	irr::f32 xdpi;
	irr::f32 ydpi;
};

// Access SDisplayMetrics
extern bool getDisplayMetrics(android_app* app, SDisplayMetrics & metrics);

// Get the application path. That's also the path into which applications are allowed to write.
extern std::string getFilesDir(android_app* app);

// Show/hide the virtual keyboard
extern void setSoftInputVisibility(android_app* app, bool visible);

// vibrate device (when the device supports it)
// AndroidManifest.xml needs: <uses-permission android:name="android.permission.VIBRATE" />
extern void vibrate(android_app* app, int ms);

}
}

#endif // _IRR_ANDROID_PLATFORM_
#endif // __IRR_ANDROID_TOOLS_H__
