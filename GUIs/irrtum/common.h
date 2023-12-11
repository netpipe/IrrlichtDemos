/*
Irrtum
Copyright (C) 2011-2015 kahrl <kahrl@gmx.net>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef COMMON_HEADER
#define COMMON_HEADER

#include "common_c.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::cerr;
using std::endl;

using std::make_pair;
using std::pair;
using std::size_t;
using std::string;
using std::vector;

using std::stringstream;
using std::istringstream;
using std::ostringstream;

const s32 IRRTUM_CHAR_MIN = 0x20;
const s32 IRRTUM_CHAR_MAX = 0x10ffff;
const s32 IRRTUM_MAX_AUTOSIZE = 16384;

template<typename T>
inline T my_min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T>
inline T my_max(T a, T b)
{
    return a > b ? a : b;
}

#endif
