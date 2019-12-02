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

#include "common.h"
#include "graybitmap.h"
#include "rect.h"

GrayBitmap::GrayBitmap(s32 width, s32 height):
    m_width(width),
    m_height(height),
    m_data(0),
    m_clip(0, 0, width, height)
{
    assert(width >= 0);
    assert(height >= 0);
    m_data = new u8[width * height];
    memset(static_cast<void*>(m_data), 0, width * height);
}

GrayBitmap::GrayBitmap(FT_Bitmap* bitmap):
    m_width(bitmap->width),
    m_height(bitmap->rows),
    m_data(0),
    m_clip(0, 0, bitmap->width, bitmap->rows)
{
    assert(m_width >= 0);
    assert(m_height >= 0);
    m_data = new u8[m_width * m_height];
    for (s32 y = 0; y < m_height; ++y)
    {
        u8* destdata = m_data + y * m_width;
        u8* srcdata = bitmap->buffer + y * bitmap->pitch;
        memcpy(static_cast<void*>(destdata), static_cast<void*>(srcdata), m_width);
    }
}

GrayBitmap::GrayBitmap(const GrayBitmap& other):
    m_width(other.m_width),
    m_height(other.m_height),
    m_data(0),
    m_clip(other.m_clip)
{
    m_data = new u8[m_width * m_height];
    memcpy(static_cast<void*>(m_data), static_cast<void*>(other.m_data), m_width * m_height);
}

GrayBitmap& GrayBitmap::operator=(const GrayBitmap& other)
{
    if (this == &other)  // self assignment
        return *this;
    m_width = other.m_width;
    m_height = other.m_height;
    m_clip = other.m_clip;
    delete[] m_data;
    m_data = new u8[m_width * m_height];
    memcpy(static_cast<void*>(m_data), static_cast<void*>(other.m_data), m_width * m_height);
}

GrayBitmap::~GrayBitmap()
{
    delete[] m_data;
}

s32 GrayBitmap::getWidth() const
{
    return m_width;
}

s32 GrayBitmap::getHeight() const
{
    return m_height;
}

u8* GrayBitmap::getScanline(s32 y)
{
    assert(y >= 0 && y < m_height);
    return m_data + (y * m_width);
}

const u8* GrayBitmap::getScanline(s32 y) const
{
    assert(y >= 0 && y < m_height);
    return m_data + (y * m_width);
}

u8 GrayBitmap::getPixel(s32 x, s32 y) const
{
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    return m_data[x + y * m_width];
}

void GrayBitmap::setPixel(s32 x, s32 y, u8 value)
{
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    m_data[x + y * m_width] = value;
}

void GrayBitmap::clear(u8 value)
{
    memset(m_data, value, m_width * m_height);
}

void GrayBitmap::setClipRect(Rect rect)
{
    m_clip.left = my_max(rect.left, 0);
    m_clip.top = my_max(rect.top, 0);
    m_clip.right = my_min(rect.right, m_width);
    m_clip.bottom = my_min(rect.bottom, m_height);
}

void GrayBitmap::clearClipRect()
{
    m_clip.left = 0;
    m_clip.top = 0;
    m_clip.right = m_width;
    m_clip.bottom = m_height;
}

Rect GrayBitmap::getClipRect() const
{
    return m_clip;
}

void GrayBitmap::blitTo(GrayBitmap& dest, s32 destX, s32 destY) const
{
    s32 xstart = my_max(0, dest.m_clip.left - destX);
    s32 xend = my_min(m_width, dest.m_clip.right - destX);
    s32 ystart = my_max(0, dest.m_clip.top - destY);
    s32 yend = my_min(m_height, dest.m_clip.bottom - destY);

    if (xend <= xstart || yend <= ystart)
        return;

    for (s32 y = ystart; y < yend; ++y)
    {
        u8* destdata = dest.getScanline(y + destY) + (xstart + destX);
        const u8* srcdata = getScanline(y) + xstart;
        s32 len = xend - xstart;
        memcpy(static_cast<void*>(destdata), static_cast<const void*>(srcdata), len);
    }
}

void GrayBitmap::debug() const
{
    s32 width = getWidth();
    s32 height = getHeight();
    cerr << "Bitmap (" << width << "x" << height << "):" << endl;
    for (s32 y = 0; y < height; ++y)
    {
        const u8* scanline = getScanline(y);
        for (int x = 0; x < width; ++x)
        {
            u8 value = scanline[x];
            if (value > 255*3/4)
                cerr << "#";
            else if (value > 255*1/2)
                cerr << "+";
            else if (value > 255*1/4)
                cerr << ".";
            else
                cerr << " ";
        }
        cout << endl;
    }
}
