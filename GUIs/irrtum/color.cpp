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

#include "color.h"
#include <cassert>

// Loosely based on the color parsing in Minetest
//   https://github.com/minetest/minetest/blob/0.4.13/src/util/string.cpp#L463
// Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>
//
// Original is licensed under LGPL 2.1 or later, here relicensed to GPL 2 or later.

static struct {
    const char *name;
    u32 color;
} named_colors[] = {
    {"aliceblue",            0xfff8f0},
    {"antiquewhite",         0xd7ebfa},
    {"aqua",                 0xffff00},
    {"aquamarine",           0xd4ff7f},
    {"azure",                0xfffff0},
    {"beige",                0xdcf5f5},
    {"bisque",               0xc4e4ff},
    {"black",                0x000000},
    {"blanchedalmond",       0xcdebff},
    {"blue",                 0xff0000},
    {"blueviolet",           0xe22b8a},
    {"brown",                0x2a2aa5},
    {"burlywood",            0x87b8de},
    {"cadetblue",            0xa09e5f},
    {"chartreuse",           0x00ff7f},
    {"chocolate",            0x1e69d2},
    {"coral",                0x507fff},
    {"cornflowerblue",       0xed9564},
    {"cornsilk",             0xdcf8ff},
    {"crimson",              0x3c14dc},
    {"cyan",                 0xffff00},
    {"darkblue",             0x8b0000},
    {"darkcyan",             0x8b8b00},
    {"darkgoldenrod",        0x0b86b8},
    {"darkgray",             0xa9a9a9},
    {"darkgreen",            0x006400},
    {"darkkhaki",            0x6bb7bd},
    {"darkmagenta",          0x8b008b},
    {"darkolivegreen",       0x2f6b55},
    {"darkorange",           0x008cff},
    {"darkorchid",           0xcc3299},
    {"darkred",              0x00008b},
    {"darksalmon",           0x7a96e9},
    {"darkseagreen",         0x8fbc8f},
    {"darkslateblue",        0x8b3d48},
    {"darkslategray",        0x4f4f2f},
    {"darkturquoise",        0xd1ce00},
    {"darkviolet",           0xd30094},
    {"deeppink",             0x9314ff},
    {"deepskyblue",          0xffbf00},
    {"dimgray",              0x696969},
    {"dodgerblue",           0xff901e},
    {"firebrick",            0x2222b2},
    {"floralwhite",          0xf0faff},
    {"forestgreen",          0x228b22},
    {"fuchsia",              0xff00ff},
    {"gainsboro",            0xdcdcdc},
    {"ghostwhite",           0xfff8f8},
    {"gold",                 0x00d7ff},
    {"goldenrod",            0x20a5da},
    {"gray",                 0x808080},
    {"green",                0x008000},
    {"greenyellow",          0x2fffad},
    {"honeydew",             0xf0fff0},
    {"hotpink",              0xb469ff},
    {"indianred",            0x5c5ccd},
    {"indigo",               0x82004b},
    {"ivory",                0xf0ffff},
    {"khaki",                0x8ce6f0},
    {"lavender",             0xfae6e6},
    {"lavenderblush",        0xf5f0ff},
    {"lawngreen",            0x00fc7c},
    {"lemonchiffon",         0xcdfaff},
    {"lightblue",            0xe6d8ad},
    {"lightcoral",           0x8080f0},
    {"lightcyan",            0xffffe0},
    {"lightgoldenrodyellow", 0xd2fafa},
    {"lightgray",            0xd3d3d3},
    {"lightgreen",           0x90ee90},
    {"lightpink",            0xc1b6ff},
    {"lightsalmon",          0x7aa0ff},
    {"lightseagreen",        0xaab220},
    {"lightskyblue",         0xface87},
    {"lightslategray",       0x998877},
    {"lightsteelblue",       0xdec4b0},
    {"lightyellow",          0xe0ffff},
    {"lime",                 0x00ff00},
    {"limegreen",            0x32cd32},
    {"linen",                0xe6f0fa},
    {"magenta",              0xff00ff},
    {"maroon",               0x000080},
    {"mediumaquamarine",     0xaacd66},
    {"mediumblue",           0xcd0000},
    {"mediumorchid",         0xd355ba},
    {"mediumpurple",         0xdb7093},
    {"mediumseagreen",       0x71b33c},
    {"mediumslateblue",      0xee687b},
    {"mediumspringgreen",    0x9afa00},
    {"mediumturquoise",      0xccd148},
    {"mediumvioletred",      0x8515c7},
    {"midnightblue",         0x701919},
    {"mintcream",            0xfafff5},
    {"mistyrose",            0xe1e4ff},
    {"moccasin",             0xb5e4ff},
    {"navajowhite",          0xaddeff},
    {"navy",                 0x800000},
    {"oldlace",              0xe6f5fd},
    {"olive",                0x008080},
    {"olivedrab",            0x238e6b},
    {"orange",               0x00a5ff},
    {"orangered",            0x0045ff},
    {"orchid",               0xd670da},
    {"palegoldenrod",        0xaae8ee},
    {"palegreen",            0x98fb98},
    {"paleturquoise",        0xeeeeaf},
    {"palevioletred",        0x9370db},
    {"papayawhip",           0xd5efff},
    {"peachpuff",            0xb9daff},
    {"peru",                 0x3f85cd},
    {"pink",                 0xcbc0ff},
    {"plum",                 0xdda0dd},
    {"powderblue",           0xe6e0b0},
    {"purple",               0x800080},
    {"red",                  0x0000ff},
    {"rosybrown",            0x8f8fbc},
    {"royalblue",            0xe16941},
    {"saddlebrown",          0x13458b},
    {"salmon",               0x7280fa},
    {"sandybrown",           0x60a4f4},
    {"seagreen",             0x578b2e},
    {"seashell",             0xeef5ff},
    {"sienna",               0x2d52a0},
    {"silver",               0xc0c0c0},
    {"skyblue",              0xebce87},
    {"slateblue",            0xcd5a6a},
    {"slategray",            0x908070},
    {"snow",                 0xfafaff},
    {"springgreen",          0x7fff00},
    {"steelblue",            0xb48246},
    {"tan",                  0x8cb4d2},
    {"teal",                 0x808000},
    {"thistle",              0xd8bfd8},
    {"tomato",               0x4763ff},
    {"turquoise",            0xd0e040},
    {"violet",               0xee82ee},
    {"wheat",                0xb3def5},
    {"white",                0xffffff},
    {"whitesmoke",           0xf5f5f5},
    {"yellow",               0x00ffff},
    {"yellowgreen",          0x32cd9a},
};

static bool parseNamedColorString(const std::string &str, u32 *color)
{
    for (int i = 0; i < sizeof(named_colors) / sizeof(*named_colors); ++i) {
        if (str == named_colors[i].name) {
            *color = named_colors[i].color;
            return true;
        }
    }
    return false;
}

static bool decodeHexDigit(char hexdigit, u8 *value)
{
    if (hexdigit >= '0' && hexdigit <= '9')
        *value = hexdigit - '0';
    else if (hexdigit >= 'A' && hexdigit <= 'F')
        *value = hexdigit - 'A' + 10;
    else if (hexdigit >= 'a' && hexdigit <= 'f')
        *value = hexdigit - 'a' + 10;
    else
        return false;
    return true;
}

static bool parseHexColorString(const std::string &str, u32 *color)
{
    size_t pos = 0;
    size_t len = str.size();

    // Skip optional octothorpe
    if (len != 0 && str[pos] == '#')
        ++pos;

    bool short_form;

    if (len - pos == 6) // RRGGBB
        short_form = false;
    else if (len - pos == 3) // RGB
        short_form = true;
    else
        return false;

    bool success = true;
    unsigned char components[] = { 0x00, 0x00, 0x00 }; // R, G, B

    for (int i = 0; i < 3; ++i) {
        if (short_form) {
            unsigned char d;
            if (!decodeHexDigit(str[pos], &d)) {
                success = false;
                break;
            }
            components[i] = (d & 0xf) << 4 | (d & 0xf);
            ++pos;
        } else {
            unsigned char d1, d2;
            if (!decodeHexDigit(str[pos], &d1) ||
                    !decodeHexDigit(str[pos + 1], &d2)) {
                success = false;
                break;
            }
            components[i] = (d1 & 0xf) << 4 | (d2 & 0xf);
            pos += 2;
        }
    }

    if (success) {
        *color = components[0] | (components[1] << 8) | (components[2] << 16);
    }

    return success;
}

bool parseColorString(const std::string &str, u32 *color)
{
    return parseNamedColorString(str, color)
        || parseHexColorString(str, color);
}
