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
#include "irrtum.h"
#include "intervallist.h"
#include "color.h"
#include <popt.h>

#define PROJECT_NAME "irrtum"
#define VERSION_STRING "1.0.2"
#define BUILD_TYPE "Release"

static int version(Irrtum& irrtum)
{
    cout << "irrtum " << VERSION_STRING << endl;
    bool good = true;

    if (irrtum.initLibpng())
    {
        cout << "libpng version: " << irrtum.getLibpngVersion() << endl;
    }
    else
    {
        cerr << "Unable to initialize libpng: " << irrtum.getLastError() << endl;
        good = false;
    }

    if (irrtum.initFreetype())
    {
        cout << "freetype version: " << irrtum.getFreetypeVersion() << endl;
    }
    else
    {
        cerr << "Unable to initialize freetype: " << irrtum.getLastError() << endl;
        good = false;
    }

    return (good ? 0 : 1);
}

int main(int argc, char *argv[])
{
    Irrtum irrtum;

    float        opt_size = 16;
    u32          opt_color = 0xffffffUL;
    float        opt_dpi = 72;
    int          opt_outwidth = 0;
    int          opt_outheight = 0;
    IntervalList opt_ranges;

    struct poptOption poptopts[] = {
        {"size", 's', POPT_ARG_FLOAT, &opt_size, 0, "Set font size in points", "POINTS"},
        {"color", 'c', POPT_ARG_STRING, 0, 'c', "Set output color", "NAME|RRGGBB"},
        {"dpi", 'd', POPT_ARG_FLOAT, &opt_dpi, 0, "Set DPI value", "DPI"},
        {"outwidth", 'w', POPT_ARG_INT, &opt_outwidth, 0, "Set width of output image", "PIXELS"},
        {"outheight", 'h', POPT_ARG_INT, &opt_outheight, 0, "Set height of output image. Ignored if --outwidth is not set.", "PIXELS"},
        {"range", 'r', POPT_ARG_STRING, 0, 'r', "Add character range", "START-END"},
        {"version", 'V', 0, 0, 'V', "Display version number and exit", 0},
        POPT_AUTOHELP
        POPT_TABLEEND
    };

    poptContext poptcon = poptGetContext("irrtum",
            argc, const_cast<const char**>(argv), poptopts, 0);
    poptSetOtherOptionHelp(poptcon, "[OPTION...] FILE...");
    int rc;
    while ((rc = poptGetNextOpt(poptcon)) > 0) {
        if (rc == 'c') {
            const char *color_str = poptGetOptArg(poptcon);
            if (!parseColorString(color_str, &opt_color)) {
                cerr << color_str << ": invalid color (must be a color name or RRGGBB)" << endl;
                return 1;
            }
        } else if (rc == 'r') {
            const char *range_str = poptGetOptArg(poptcon);
            s32 from, to;
            if (opt_ranges.parseInterval(range_str, from, to)) {
                opt_ranges.addInterval(from, to);
            } else {
                cerr << range_str << ": invalid range (must be FROM-TO)" << endl;
                return 1;
            }
        } else if (rc == 'V') {
            return version(irrtum);
        }
    }
    if (rc < -1) {
        /* an error occurred during option processing */
        cerr << poptBadOption(poptcon, POPT_BADOPTION_NOALIAS) << ": "
            << poptStrerror(rc) << endl;
        return 1;
    }

    // Clean character range
    if (opt_ranges.isEmpty())
    {
        opt_ranges.addInterval(32, 255);
    }
    if (opt_ranges.getMin() < IRRTUM_CHAR_MIN)
    {
        cerr << "warning: character ranges below " << IRRTUM_CHAR_MIN << " are ignored" << endl;
        opt_ranges.removeBelow(IRRTUM_CHAR_MIN);
    }
    if (opt_ranges.getMax() > IRRTUM_CHAR_MAX)
    {
        cerr << "warning: character ranges above " << IRRTUM_CHAR_MAX << " are ignored" << endl;
        opt_ranges.removeAbove(IRRTUM_CHAR_MAX);
    }
    if (opt_ranges.isEmpty())
    {
        cerr << "error: all specified character ranges are ignored, exiting" << endl;
        return 1;
    }

    if (!irrtum.initLibpng())
    {
        cerr << "Unable to initialize libpng: " << irrtum.getLastError() << endl;
        return 1;
    }

    if (!irrtum.initFreetype())
    {
        cerr << "Unable to initialize freetype: " << irrtum.getLastError() << endl;
        return 1;
    }

    irrtum.setColor(opt_color);
    irrtum.setCharacterRanges(opt_ranges);

    const char* filename = poptGetArg(poptcon);
    if (!filename)
    {
        cerr << "irrtum: no input files" << endl;
        return 1;
    }
    while (filename)
    {
        // stage 1: loading the font face
        cerr << "Loading font face: " << filename << endl;
        if (!irrtum.loadFace(filename, opt_size, opt_dpi))
        {
            cerr << filename << ": Unable to load font: " << irrtum.getLastError() << endl;
            return 1;
        }

        // stage 2: building the layout
        if (!irrtum.layout(opt_outwidth, opt_outheight))
        {
            cerr << filename << ": Unable to create layout: " << irrtum.getLastError() << endl;
            return 1;
        }

        // stage 3: drawing an intermediate grayscale bitmap
        if (!irrtum.drawGrayscaleBitmap())
        {
            cerr << filename << ": Unable to draw bitmap font: " << irrtum.getLastError() << endl;
            return 1;
        }

        // stage 4: converting to ARGB and writing the PNG file
        std::string outputFilename = irrtum.getOutputFilename(filename);
        cout << "Writing " << irrtum.getLayoutWidth() << "x" << irrtum.getLayoutHeight() << " PNG image: " << outputFilename << endl;
        if (!irrtum.outputPNG(outputFilename))
        {
            cerr << outputFilename << ": Unable to write PNG: " << irrtum.getLastError() << endl;
            return 1;
        }

        filename = poptGetArg(poptcon);
    }

    poptFreeContext(poptcon);
    return 0;
}
