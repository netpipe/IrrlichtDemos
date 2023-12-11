#ifdef IRRLICHT
#ifndef IRRUTIL_H
#define IRRUTIL_H

#include "irrlicht.h"

struct SIrrlichtKey {
    irr::EKEY_CODE code;
    wchar_t ch;
};


SIrrlichtKey convertToIrrlichtKey(int key);

irr::SKeyMap* getCameraKeyMap();

#endif // IRRUTIL_H
#endif
