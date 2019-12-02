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

#include "common_c.h"
#include "pngwrite.h"
#include <stdio.h>
#include <setjmp.h>
#include <errno.h>

/*
 * Note: This file MUST be written in plain jane C so that setjmp doesn't
 * interfere with C++'s exception handling / RAII / etc.
 */

void pngwrite(
        u32 width,
        u32 height,
        u8* data,
        const char* filename,
        const char** error_msg,
        const char** error_extra)
{
    FILE* fp;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytepp row_pointers;
    u32 i;

    *error_msg = 0;
    *error_extra = 0;

    /* Allocate memory for row pointers */
    row_pointers = malloc(height * sizeof(png_bytep));
    if (row_pointers == 0)
    {
        *error_msg = "Failed to allocate memory";
        *error_extra = strerror(errno);
        return;
    }
    for (i = 0; i < height; ++i)
    {
        row_pointers[i] = (png_bytep) (data + i * width * 4);
    }

    /* Open file for writing */
    fp = fopen(filename, "wb");
    if (fp == 0)
    {
        *error_msg = "Could not open file for writing: ";
        *error_extra = strerror(errno);
        free(row_pointers);
        return;
    }

    /* Initialize PNG structures */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (png_ptr == 0)
    {
        *error_msg = "Could not allocate PNG write structure";
        fclose(fp);
        free(row_pointers);
        return;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == 0)
    {
        *error_msg = "Could not allocate PNG info structure";
        png_destroy_write_struct(&png_ptr, (png_infopp) 0);
        fclose(fp);
        free(row_pointers);
        return;
    }

    /* libpng exception handling ^_^ */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        *error_msg = "Could not write PNG stream";
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        free(row_pointers);
        return;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height, 8,
            PNG_COLOR_TYPE_RGB_ALPHA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);

    png_set_rows(png_ptr, info_ptr, row_pointers);

    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    fclose(fp);

    free(row_pointers);
}
