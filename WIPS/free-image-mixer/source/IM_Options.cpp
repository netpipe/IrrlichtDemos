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
#include "IM_Exception.h"
#include "IM_Options.h"
#include "IM_Globals.h"
#include "IM_Bool_Pool.h"
#include "IM_Channel_Pool.h"
#include <irrlicht.h>


namespace mixer
{
    Options::Options()
    {
        if(!priv::G::GlobalDevice)
        {
            priv::G::GlobalDevice =
                irr::createDevice(irr::video::EDT_OPENGL,
                                  irr::core::dimension2du (priv::G::ScreenX, priv::G::ScreenY),
                                  32, priv::G::Fullscreen );

            if(!priv::G::GlobalDevice)
                throw Exception("Unable to create an irrlicht device.");

            Exception a("\nFreeImageMixer version 1.0.0 \nCopyright 2010-2012 by Dario Oliveri\n");
            a.print();
        }
        else
        {
            priv::G::GlobalDevice->grab();
        }
    }

    Options::~Options()
    {
        priv::ChannelPool::finalize();
        priv::BoolPool::finalize();
        priv::G::GlobalDevice->drop();

        priv::G::ChannelSize = 0; //reset channel size. Now multiple Image mixer scripts are allowed if they are scoped.
    }

    void Options::setSize(unsigned int X, unsigned int Y) const
    {
        if(priv::G::ChannelSize!=0)
            throw Exception("Unable to change image size after inizialitazion!");

        priv::G::ChannelSize = X*Y;
        priv::G::WorkAreaX = X;
        priv::G::WorkAreaY = Y;

        priv::G::ScreenX = 320;
        priv::G::ScreenY = 240;
    }

    void Options::setAutoClamping(bool enabled)
    {
        priv::G::AutoClamp = enabled;
    }

    void Options::setBGColor(unsigned int R, unsigned int G, unsigned int B)
    {
        priv::G::BGColor = irr::video::SColor (0,R,G,B);
    }

    void Options::setBGColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A)
    {
        priv::G::BGColor = irr::video::SColor (A,R,G,B);
    }

    void Options::setBGColor(float R, float G, float B)
    {
        priv::G::BGColor = irr::video::SColor (0,R*255,G*255,B*255);
    }

    void Options::setBGColor(float R, float G, float B, float A)
    {
        priv::G::BGColor = irr::video::SColor (A*255,R*255,G*255,B*255);
    }

    void Options::setTolerance (float tolerance)
    {
        priv::G::CustomTolerance = tolerance;
    }
}
