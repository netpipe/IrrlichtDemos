// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef KEYCCODE_STRINGS_H
#define KEYCCODE_STRINGS_H

#include "Keycodes.h"
#include <string>

extern std::string IrrKeyToString(irr::EKEY_CODE key);
extern irr::EKEY_CODE StringToIrrKey(const std::string& keyname);

#endif // KEYCCODE_STRINGS_H
