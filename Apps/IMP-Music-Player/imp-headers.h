// IMP - Irrlicht Music Player
// Jorge Rosa (1999 - 2018 Portugal)
// Email: jorge.bigarte@gmail.com
// Portfolio: https://sites.google.com/site/jorgerosaportfolio
// Facebook: https://www.facebook.com/jorge.bigarte
// Youtube: https://www.youtube.com/user/jorgeerosa/videos
// Project: https://sourceforge.net/projects/irrlichtmp

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

/// Audio (Audiere)
#include <audiere.h> // For Audio. ALSO must add "libaudiere.a" in the "Project"->"Build Options"->"Linker" !!!
using namespace audiere;

/// Audio (cAudio) - Not used...
// #include "cAudio.h" // For future Audio (Has 3D audio + reverb)

/// Audio Spectrum (Kiss FFT) a Fast Fourier Transform (TODO)
// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include "kiss_fft.h"
// #ifndef M_PI
// #define M_PI 3.14159265358979324
// #endif
// #define N 16
// Square for convenience
// #define SQ(a)   a*a
// #define FFT_LEN 1024
// We look for this bit in the output
// #define NUM 2
// #define WIN 512

// #include <unistd.h>
// unsigned int microseconds;
// #include "libraries/KissFFT-1.3.0/tools/kiss_fftr.h"
// #define M_PI 3.14159265358979324
// #define N 16
// #define FIXED_POINT 16
// #define NFFT 16
// Square for convenience:
// #define SQ(a) a*a
// #define FFT_LEN 1024
// We look for this bit in the output:
// #define NUM 2

/// Events (For Irrlicht)
#include "libraries/irrlicht-extras/myEvents.h"

/// Loading Bar (For Irrlicht)
#include "libraries/irrlicht-extras/CLoadingBar.h"

/// Clock (For Irrlicht)
#include "libraries/irrlicht-extras/Clock/CGUIClock.h"
#include "libraries/irrlicht-extras/Clock/CClockSceneNode.h"

/// Decal Manager: (For Irrlicht)
// #include "libraries/irrlicht-extras/DecalSystem/DecalManager.h"

/// Boost performance (For Irrlicht) - Not used...
// #include "libraries/MainLibs/CMeshCombiner.h"

/// Control for my animations embed at MD3 Nodes (For Irrlicht) - Not used...
// #include "libraries/MainLibs/SAnimation.h"
// #include "libraries/MainLibs/CAnimationController.h"
// #include "libraries/MainLibs/CAnimationController.cpp"
// #include "libraries/MainLibs/CAnimatedMeshSceneNode.cpp"

// http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=37296
// #include "libraries/MainLibs/irrUString.h"
// #include <CGUITTFont.h>

/// Effects (For Irrlicht)
// EFFECTS STUFF - Include the headers:
#include "libraries/irrlicht-extras/PostProcess/CRendererPostProc.h"
#include "libraries/irrlicht-extras/PostProcess/CEffectPostProc.h"
#include "libraries/irrlicht-extras/PostProcess/CTransitionPostProc.h"
#include "libraries/irrlicht-extras/PostProcess/CSplitPostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CLensFlarePostProc.h"
// #include "libraries/irrlicht-extras/PostProcess/CWaterPostProc.h"

// MUST include "XEffects.h", and add all the XEffects ".cpp" files to your project/makefile:
// #include "XEffects.h"

// #include "libraries/irrlicht-extras/CReflectedWater.h"




