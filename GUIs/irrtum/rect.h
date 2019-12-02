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

#ifndef RECT_HEADER
#define RECT_HEADER

#include "common.h"

class Rect
{
public:
    Rect(s32 l, s32 t, s32 r, s32 b):
        left(l), top(t), right(r), bottom(b)
    {
    }

    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

#endif

