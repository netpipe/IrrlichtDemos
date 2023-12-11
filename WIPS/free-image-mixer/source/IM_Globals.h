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
#ifndef IMAGE_MIXER_GLOBALS_H_INCLUDED
#define IMAGE_MIXER_GLOBALS_H_INCLUDED

//#undef new
#include <SColor.h>


namespace irr
{
    class IrrlichtDevice;
}


namespace mixer
{
namespace priv
{

    struct G
    {
        static unsigned int ChannelSize;
        static float        DefaultTolerance;
        static float        CustomTolerance;
        static irr::IrrlichtDevice * GlobalDevice;
        static unsigned int        WorkAreaX;
        static unsigned int        WorkAreaY; //ChannelSize should be WorkAreaY*WorkAreaX
        static unsigned int        ScreenX;
        static unsigned int        ScreenY;
        static unsigned int        PoolMaxSize;
        static bool                Fullscreen;
        static bool                AutoClamp;
        static irr::video::SColor  BGColor;
    };

} //end of namespace priv
} //end of namespace mixer


#endif // IMAGE_MIXER_GLOBALS_H_INCLUDED
