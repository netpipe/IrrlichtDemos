#include <irrlicht.h>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <iostream>
#include "irFinder.h"
#include "IrrUtils.h"
#include <math.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;
using namespace cv;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif