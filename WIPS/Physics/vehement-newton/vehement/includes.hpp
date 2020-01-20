/* Vehement (C) Steven Page 2008 */

//For Depreciated function use
#define _CRT_SECURE_NO_DEPRECATE

//Define all third party libraries libraries
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "irrKlang.lib") 

//Include Physics Libraries
#pragma comment(lib, "newton.lib")
#pragma comment(lib, "IrrNewt.lib")

//Include the Lua library 
#pragma comment(lib, "lua5.1.lib")

//Standard Includes
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

//Irrlicht
#include <irrlicht.h>
#include <irrKlang.h>

//Physics Includes
#include <Newton.h>
#include <IrrNewt.hpp>		 /* Thanks to white tiger */

//Lua Includes
extern "C" 
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

//Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace irrklang;

//Standard 
using namespace std;




