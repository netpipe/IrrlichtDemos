/*******************************************************************************
* Copyright (C) 2011-2013 Dario Oliveri
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#ifndef IMAGE_MIXER_UTILS_INCLUDED
#define IMAGE_MIXER_UTILS_INCLUDED

#include <vector3d.h>
#include <SColor.h>
#include <irrMath.h>

namespace irr
{
namespace color
{

    //! Function for converting a SColor wich is assumed to have only RGB components to a vector3d<f32>
    /** \return a vector3df wich XYZ correspond to RGB values in a range between 0 and 1
    */
    inline core::vector3df RGBColorToVectorf3df(video::SColor & RGBColor)
    {
        const f32 rec = 1.0f/255.0f;
        return core::vector3df(rec*RGBColor.getRed(),rec*RGBColor.getGreen(), rec*RGBColor.getBlue() );
    }

    //! Function for converting a SColor wich is assumed to have only RGB components to a vector3d<f32>
    /** \return a vector3df wich XYZ correspond to RGB values in a range between 0 and 1
    */
    inline core::vector3df RGBColorToVectorf3df(video::SColorf & RGBColor)
    {
        return core::vector3df(RGBColor.r,RGBColor.g,RGBColor.b);
    }

} // end of namespace color

//! Give the index for the images
/** Where columns is usually represented by X and row by Y or Z*/
inline u32 getIndex(const u32 col, const u32 row, const u32 numcolumns)
{
    return row*numcolumns + col;
}


} // end of namespace irr



#endif // IMAGE_MIXER_UTILS_INCLUDED
