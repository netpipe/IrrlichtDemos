/*!
	PhoenixCore Config File.
*/
#ifndef __PHOENIXCORE_CONFIG_H__
#define __PHOENIXCORE_CONFIG_H__

// Version Name/Number/Release.
#define PHOENIXCORE_VERSION "PhoenixCore 0.3 Raven"
#define PHEONIXCORE_VERSION_NUMBER "0.3"
#define PHEONIXCORE_VERSION_NAME "Raven"

// OpenGL Include
#include "GL/glew.h"
#include <GL/glfw.h>

//! Define this to use the default GLFW Window Manager, or set to 0 to not compile the GLFW window manager.
/*!
	\note If this is 0, the system will not have a window manager, your custom window manager must be Instance()'d before calling RenderSystem(); 
*/
#define PH_USE_GLFW 1

#if defined(__GNUC__) && (defined(__linux__) || defined(__linux) || defined(WIN32) || defined(__WIN32__) || defined(__WIN32))
// Define this to ensure correct linkage with boost threads in MinGW 4.7+.
#ifndef BOOST_THREAD_USE_LIB
#define BOOST_THREAD_USE_LIB 1
#endif
#endif

#endif //__PHOENIXCORE_CONFIG_H__
