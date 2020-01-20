#if !defined(MAIN_HEADER)
#define MAIN_HEADER

#include <irrlicht.h>
#include <wchar.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// newton
#include <Newton.h>

#define IS_KIND_OF_FLY  100
#define IS_KIND_OF_LUMP 200
#define NewtonToIrr   32.0
#define GRAVITY -0.0
#define IrrToNewton  1/NewtonToIrr
#define MAX_FLYS 15
#define KEY_COUNT 14

struct KeyItem{
	KeyItem(){ state = false; code = KEY_ZOOM; }
	bool state;
	EKEY_CODE code;
};

#endif
