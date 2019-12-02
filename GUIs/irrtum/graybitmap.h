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

#ifndef GRAYBITMAP_HEADER
#define GRAYBITMAP_HEADER

#include "common.h"
#include "rect.h"

class GrayBitmap
{
public:
    GrayBitmap(s32 width, s32 height);
    GrayBitmap(FT_Bitmap* bitmap);
    explicit GrayBitmap(const GrayBitmap& other);
    GrayBitmap& operator=(const GrayBitmap& other);
    ~GrayBitmap();

    s32 getWidth() const;
    s32 getHeight() const;
    u8* getScanline(s32 y);
    const u8* getScanline(s32 y) const;
    u8 getPixel(s32 x, s32 y) const;
    void setPixel(s32 x, s32 y, u8 value);
    void clear(u8 value);

    // Set the clipping rectangle used when this bitmap is specified
    // as a blitTo() destination.
    void setClipRect(Rect rect);

    // Clear the clipping rectangle set by setClipRect().
    void clearClipRect();

    // Retrieve the clipping rectangle set by setClipRect().
    Rect getClipRect() const;

    // Copy the contents of this bitmap to a part of another one.
    // Rsspects the clipping rectangle of the destination bitmap.
    void blitTo(GrayBitmap& dest, s32 destX, s32 destY) const;

    void debug() const;

private:
    s32 m_width;
    s32 m_height;
    u8* m_data;
    Rect m_clip;
};

#endif

