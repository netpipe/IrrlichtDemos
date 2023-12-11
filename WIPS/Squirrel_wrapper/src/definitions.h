#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define ResX		640		// Screenresolution X
#define ResY		480		// Screenresolution Y

#define PRINTMSG(s) printf("%s\n",s);

// Irrlicht + general
#include <irrlicht.h>
#include <stdlib.h>
//#include <iostream>
#include <time.h>
#pragma comment(lib, "Irrlicht.lib")

// Squirrel+
#include <stdarg.h>
#include <squirrel.h>
#include <sqplus.h>
#include <sqstdio.h>
#include <sqstdaux.h>
#include <squtils.h>
using namespace SqPlus;
#ifdef _MSC_VER
	#pragma comment (lib ,"squirrelD.lib")
	#pragma comment (lib ,"sqstdlibD.lib")
	#pragma comment (lib ,"sqplusD.lib")
	//#pragma comment (lib ,"sqdbglibD.lib")
#endif
#define scvprintf vprintf

#endif	// DEFINITIONS_H
