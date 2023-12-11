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
#include "IM_Globals.h"

namespace mixer
{
namespace priv
{

    unsigned int G::ChannelSize = 0; //if zero we throw exception since that's mean that we have skipped initialization
    float        G::DefaultTolerance = 1.0/65536.0 ; //should be enough since usually precision is around 1/255.
    float        G::CustomTolerance  = 1.0/65536.0 ; //used instead of DefaultTolerance if user specify it.
    irr::IrrlichtDevice * G::GlobalDevice = 0; //null device. Created when needed.
    unsigned int G::ScreenX = 640;
    unsigned int G::ScreenY = 480;
    unsigned int G::WorkAreaX = 0;
    unsigned int G::WorkAreaY = 0;
    unsigned int G::PoolMaxSize = 8;
    bool         G::Fullscreen = false;
    bool         G::AutoClamp  = true;

    irr::video::SColor G::BGColor = irr::video::SColor(0,255,255,255);
} //end of namespace priv
} //end of namespace mixer
