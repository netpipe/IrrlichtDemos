#pragma once

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace io;

class CIrrObject
{
public:
	CIrrObject();
	~CIrrObject();

	static IrrlichtDevice *device;
};
