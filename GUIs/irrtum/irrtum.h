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

#ifndef IRRTUM_HEADER
#define IRRTUM_HEADER

#include "common.h"
#include "graybitmap.h"
#include "intervallist.h"
#include "rect.h"

class Irrtum
{
public:
    Irrtum();
    ~Irrtum();

    std::string getLastError() const;

    bool initLibpng();
    bool initFreetype();
    std::string getLibpngVersion() const;
    std::string getFreetypeVersion() const;
    void setColor(u32 color);
    void setCharacterRanges(const IntervalList& cranges);

    // stage 1: loading the font face
    bool loadFace(string filename, float size, float dpi);

    // stage 2: building the layout
    bool layout(s32 outwidth, s32 outheight);
    s32 getLayoutWidth() const;
    s32 getLayoutHeight() const;

    // stage 3: drawing a grayscale bitmap
    bool drawGrayscaleBitmap();

    // stage 4: converting to ARGB and writing the PNG file
    string getOutputFilename(string filename) const;
    bool outputPNG(string outputFilename);

private:
    Irrtum(const Irrtum&);
    Irrtum& operator=(const Irrtum&);

    bool getCharBitmapSize(s32 ch, s32& width, s32& height);
    bool getTotalBitmapSize(s32& area);
    bool tryLayout(s32 outwidth, s32 outheight, bool& tooLarge);
    void setRGBAPixel(s32 width, s32 height, u8* data, s32 x, s32 y, u32 color) const;

    void freetypeError(FT_Error error);

    // fields
    std::string m_error;
    FT_Library m_ftlibrary;
    bool m_ftinited;
    u8 m_color_red;
    u8 m_color_green;
    u8 m_color_blue;
    IntervalList m_cranges;

    // stage 1 fields: loading the font face
    FT_Face m_face;

    // stage 2 fields: building the layout
    s32 m_layoutwidth;
    s32 m_layoutheight;
    vector<Rect> m_layoutrects;

    // stage 3 fields: drawing a grayscale bitmap
    GrayBitmap* m_graybitmap;
};

#endif

