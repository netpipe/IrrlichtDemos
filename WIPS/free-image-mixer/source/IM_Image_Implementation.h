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
#ifndef IMAGE_IMPLEMENTATION_H_INCLUDED
#define IMAGE_IMPLEMENTATION_H_INCLUDED

#include "IM_Channel.h"
#include "IM_Channel_Implementation.h"
#include "IM_Globals.h"
#include <IImage.h>

using namespace irr; //for types

namespace mixer
{
namespace priv
{

    struct ImageImpl
    {
        Channel R;
        Channel G;
        Channel B;
        Channel A;

        ImageImpl()
        {

        }
        ~ImageImpl() {}

        void channelize( irr::video::IImage * image) // TODO: const pointer is not possible due to design of IImage. ask feature to irrlicht developers?
        {
            if(!image)
                return;

            image->grab(); //TODO: grab/drop is not needed since image is used inside this scope and we are not doing MultyThreading.

            u8* sB = (u8*)image->lock();
            const s32 sN = priv::G::WorkAreaX * priv::G::WorkAreaY;

            float* a = A.getData();
            float* r = R.getData();
            float* g = G.getData();
            float* b = B.getData();

            for (s32 x = 0; x < sN; ++x) //TODO: check later why division by 255 is slower.. -_- shouldn't be optimized by compiler?
            {
                a[x] = (float)sB[3]/255.;
                r[x] = (float)sB[2]/255.;
                g[x] = (float)sB[1]/255.;
                b[x] = (float)sB[0]/255.;

                sB += 4;
            }

            image->unlock();
            image->drop();
        }

        void rasterize(irr::video::IImage * image) const
        {
            if(!image)
                return;

            image->grab(); //TODO: scoped image. no grab/drop required.

            u8* dB = (u8*)image->lock();
            const s32 sN = priv::G::WorkAreaX * priv::G::WorkAreaY;

            const float* a = A.getConstData();
            const float* r = R.getConstData();
            const float* g = G.getConstData();
            const float* b = B.getConstData();

            for (s32 x = 0; x < sN; ++x)
            {
                dB[3] = (u8)(a[x]*255.);
                dB[2] = (u8)(r[x]*255.);
                dB[1] = (u8)(g[x]*255.);
                dB[0] = (u8)(b[x]*255.);

                dB += 4;
            }

            image->unlock();
            image->drop();
        }
    };

} // end of namespace priv
} // end of namespace mixer


#endif // IMAGE_IMPLEMENTATION_H_INCLUDED
