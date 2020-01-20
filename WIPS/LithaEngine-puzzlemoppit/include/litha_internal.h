
#ifndef LITHA_INTERNAL_H
#define LITHA_INTERNAL_H

// This file includes Irrlicht and some utility methods and macros.

#include <irrlicht.h>

using namespace irr;

// Utility function libaries
#include "utils/maths.h"
#include "utils/log.h"
#include "utils/str.h"
#include "utils/file.h"
#include "utils/os.h"
#include "utils/os/path.h"

// Utility classes
#include "utils/Set.h"
#include "utils/VariantMap.h"
#include "utils/Variant.h"

using namespace utils;

#define SET_REF_COUNTED_POINTER(oldPointer, newPointer) \
		if (newPointer != oldPointer) \
		{ \
			if (newPointer) \
				newPointer->grab(); \
			if (oldPointer) \
				oldPointer->drop(); \
			oldPointer = newPointer; \
		} \

enum E_AXIS
{
	AXIS_X = (1<<0),
	AXIS_Y = (1<<1),
	AXIS_Z = (1<<2)
};

#define ALL_AXES	(AXIS_X|AXIS_Y|AXIS_Z)

template<class T> const T &min(const T &a, const T &b)
{
	return (a < b) ? a : b;
}

template<class T> const T &max(const T &a, const T &b)
{
	return (a > b) ? a : b;
}

#endif
