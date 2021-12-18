// Jorge Rosa (1999 - 2018 Portugal)
// Email: jorge.bigarte@gmail.com
// Portfolio: https://sites.google.com/site/jorgerosaportfolio
// Facebook: https://www.facebook.com/jorge.bigarte
// Youtube: https://www.youtube.com/user/jorgeerosa/videos

/// ################################################################################################################################################################################
///
/// Compiling in my system: (May be different in yours)
///
/// Windows 7 (64 Bits)
/// Code::Blocks 16.01 (32 Bits) - Installed at: "C:\Program Files (x86)\CodeBlocks"
/// The selected compiler: GNU GCC Compiler - Installed at: "C:\MinGW"
/// The "Toolchain executables" are: (set by this order) "mingw32-gcc.exe","mingw32-g++.exe","mingw32-g++.exe","ar.exe","GDB/CDB Debugger: Default","windres.exe","mingw32-make.exe"
///
/// ################################################################################################################################################################################
/// STATIC LIBRARIES in Code::Blocks: add '-static' in settings --> compiler --> Linker settings --> Other linker options

/// System
// #include <windows.h>
// #include <shellapi.h>
#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdio.h>
#include <dirent.h>
#include <functional>
#include <algorithm>
#include <ctype.h>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <cmath>
#include <stdint.h>
#include <iterator>
#include <numeric>
using namespace std;

/// Icons (Resources at "imp-resources.rc" file) - Not used...
// #define IDI_ICON 1

/// Engine (Irrlicht)
// #define _IRR_STATIC_LIB_ // Define for Static Irrlicht
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace io;
using namespace gui;

/// Events (For Irrlicht)
#include "libraries/irrlicht-extras/myEvents.h"

#include "libraries/irrlicht-extras/Water/RealisticWater.h"

/// Effects (For Irrlicht)
// EFFECTS STUFF - Include the headers:
// #include "libraries/irrlicht-extras/PostProcess/CRendererPostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CEffectPostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CTransitionPostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CSplitPostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CLensFlarePostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CWaterPostProc.h"

// MUST include "XEffects.h", and add all the XEffects ".cpp" files to your project/makefile:
// #include "XEffects.h"

// #include "libraries/irrlicht-extras/CReflectedWater.h"




