// TexGen.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"

#include "Editor.h"

// Editor device Includes
#include "DevicePerlin.h"
#include "DeviceGradient.h"
#include "DeviceColourize.h"
#include "DeviceColourize2.h"

using namespace irr;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "freetype.lib")
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

/// The main function
int main()
{

	// The editor
	CEditor Editor;

	// Create it
	if (!Editor.Create())
		return 1;
	// Start the main loop going
	int ret = Editor.Main();

	return ret;
}

