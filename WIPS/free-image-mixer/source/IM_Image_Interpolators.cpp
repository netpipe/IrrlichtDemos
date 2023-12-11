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
#include "IM_Image.h"
#include "IM_Mask.h"
#include "IM_Channel.h"
#include "IM_Image_Implementation.h"
#include <cmath>

namespace mixer
{
    void Image::mix(const Image & image, const Channel & t)
    {
        const float * r = image.Impl->R.getConstData();
        const float * g = image.Impl->G.getConstData();
        const float * b = image.Impl->B.getConstData();
        const float * T = t.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float x = T[i];
            const float cx = 1.0-x;
            R [i] = R[i]*cx + r[i]*x;
            G [i] = G[i]*cx + g[i]*x;
            B [i] = B[i]*cx + b[i]*x;
        }
    }

    void Image::mixCosine(const Image& image, const Channel& t)
    {
        const float * r = image.Impl->R.getConstData();
        const float * g = image.Impl->G.getConstData();
        const float * b = image.Impl->B.getConstData();
        const float * T = t.getConstData();
        const float pix = 3.1415926535897932384626433832795;

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float x = (1.0-cos(T[i]*pix))/2.0;
            const float cx = 1.0-x;
            R [i] = R[i]*cx + r[i]*x;
            G [i] = G[i]*cx + g[i]*x;
            B [i] = B[i]*cx + b[i]*x;
        }
    }

    void Image::mixQuadratic(const Image& BB, const Image& C, const Channel& t)
    {
        const float * r = BB.Impl->R.getConstData();
        const float * g = BB.Impl->G.getConstData();
        const float * b = BB.Impl->B.getConstData();
        const float * T = t.getConstData();

        const float * r2 = C.Impl->R.getConstData();
        const float * g2 = C.Impl->G.getConstData();
        const float * b2 = C.Impl->B.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float x = T[i];
            const float cx = 1.0-x;
            const float m1 = cx*cx;
            const float m2 = 2.0*cx*x;
            const float m3 = x*x;

            R [i] = R[i]*m1 + r[i]*m2 + r2[i]*m3;
            G [i] = G[i]*m1 + g[i]*m2 + g2[i]*m3;
            B [i] = B[i]*m1 + b[i]*m2 + b2[i]*m3;
        }
    }

    void Image::mixCubic(const Image& BB, const Image& C, const Image& D, const Channel& t)
    {
        const float * r = BB.Impl->R.getConstData();
        const float * g = BB.Impl->G.getConstData();
        const float * b = BB.Impl->B.getConstData();
        const float * T = t.getConstData();

        const float * r2 = C.Impl->R.getConstData();
        const float * g2 = C.Impl->G.getConstData();
        const float * b2 = C.Impl->B.getConstData();

        const float * r3 = D.Impl->R.getConstData();
        const float * g3 = D.Impl->G.getConstData();
        const float * b3 = D.Impl->B.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float x = T[i];
            const float xx = x*x;

            float M1 = r3[i] - r2[i] + r[i] - R[i];
            float M2 = R[i] - r[i] - M1;
            float M3 = r2[i] - R[i];

            R[i] = x*(M1*xx + M2*x + M3) + r[i];

            M1 = g3[i] - g2[i] + g[i] - G[i];
            M2 = G[i] - g[i] - M1;
            M3 = g2[i] - G[i];

            G[i] = x*(M1*xx + M2*x + M3) + g[i];

            M1 = b3[i] - b2[i] + b[i] - B[i];
            M2 = B[i] - b[i] - M1;
            M3 = b2[i] - B[i];

            B[i] = x*(M1*xx + M2*x + M3) + b[i];
        }
    }

    void Image::mix(const Image & image, const Channel & t, const Mask& mask)
    {
        const float * r = image.Impl->R.getConstData();
        const float * g = image.Impl->G.getConstData();
        const float * b = image.Impl->B.getConstData();
        const float * T = t.getConstData();
        const bool * p = mask.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float x = T[i];
                const float cx = 1.0-x;
                R [i] = R[i]*cx + r[i]*x;
                G [i] = G[i]*cx + g[i]*x;
                B [i] = B[i]*cx + b[i]*x;
            }
        }
    }

    void Image::mixCosine(const Image& image, const Channel& t, const Mask& mask)
    {
        const float * r = image.Impl->R.getConstData();
        const float * g = image.Impl->G.getConstData();
        const float * b = image.Impl->B.getConstData();
        const float * T = t.getConstData();
        const float pix = 3.1415926535897932384626433832795;
        const bool * p = mask.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float x = (1.0-cos(T[i]*pix))/2.0;
                const float cx = 1.0-x;
                R [i] = R[i]*cx + r[i]*x;
                G [i] = G[i]*cx + g[i]*x;
                B [i] = B[i]*cx + b[i]*x;
            }
        }
    }

    void Image::mixQuadratic(const Image& BB, const Image& C, const Channel& t, const Mask& mask)
    {
        const float * r = BB.Impl->R.getConstData();
        const float * g = BB.Impl->G.getConstData();
        const float * b = BB.Impl->B.getConstData();
        const float * T = t.getConstData();
        const bool * p = mask.getConstData();

        const float * r2 = C.Impl->R.getConstData();
        const float * g2 = C.Impl->G.getConstData();
        const float * b2 = C.Impl->B.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float x = T[i];
                const float cx = 1.0-x;
                const float m1 = cx*cx;
                const float m2 = 2.0*cx*x;
                const float m3 = x*x;

                R [i] = R[i]*m1 + r[i]*m2 + r2[i]*m3;
                G [i] = G[i]*m1 + g[i]*m2 + g2[i]*m3;
                B [i] = B[i]*m1 + b[i]*m2 + b2[i]*m3;
            }
        }
    }

    void Image::mixCubic(const Image& BB, const Image& C, const Image& D, const Channel& t, const Mask& mask)
    {
        const float * r = BB.Impl->R.getConstData();
        const float * g = BB.Impl->G.getConstData();
        const float * b = BB.Impl->B.getConstData();
        const float * T = t.getConstData();
        const bool * p = mask.getConstData();

        const float * r2 = C.Impl->R.getConstData();
        const float * g2 = C.Impl->G.getConstData();
        const float * b2 = C.Impl->B.getConstData();

        const float * r3 = D.Impl->R.getConstData();
        const float * g3 = D.Impl->G.getConstData();
        const float * b3 = D.Impl->B.getConstData();

        float * R = Impl->R.getData();
        float * G = Impl->G.getData();
        float * B = Impl->B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float x = T[i];
                const float xx = x*x;

                float M1 = r3[i] - r2[i] + r[i] - R[i];
                float M2 = R[i] - r[i] - M1;
                float M3 = r2[i] - R[i];

                R[i] = x*(M1*xx + M2*x + M3) + r[i];

                M1 = g3[i] - g2[i] + g[i] - G[i];
                M2 = G[i] - g[i] - M1;
                M3 = g2[i] - G[i];

                G[i] = x*(M1*xx + M2*x + M3) + g[i];

                M1 = b3[i] - b2[i] + b[i] - B[i];
                M2 = B[i] - b[i] - M1;
                M3 = b2[i] - B[i];

                B[i] = x*(M1*xx + M2*x + M3) + b[i];
            }
        }
    }
}
