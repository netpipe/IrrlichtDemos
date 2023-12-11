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
#include "IM_Channel.h"
#include "IM_Exception.h"
#include "IM_Image.h"
#include "IM_Mask.h"
#include "IM_ColorMatrix.h"
#include "IM_MatrixChannel.h"
#include "IM_MatrixImplementation.h"
#include <cstring>
#include <irrMath.h>

namespace mixer
{

    ColorMatrixChannel::ColorMatrixChannel()
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::MatrixChanImpl();
    }

    ColorMatrixChannel::ColorMatrixChannel(const ColorMatrixChannel& other)
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::MatrixChanImpl();

        for(int i=0; i<25; i++)
        {
            Impl->M[i] = other.Impl->M[i];
        }
    }

    ColorMatrixChannel::ColorMatrixChannel(const ColorMatrix& other)
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::MatrixChanImpl();

        for(int i=0; i<25; i++)
        {
            Impl->M[i] = other.M[i];
        }
    }

    ColorMatrixChannel::~ColorMatrixChannel()
    {
        delete Impl;
    }

    bool ColorMatrixChannel::load(const char* filename)
    {
        return false;
    }

    bool ColorMatrixChannel::save(const char* filename)const
    {
        return false;
    }

    void ColorMatrixChannel::applyTo(Channel & R, Channel & G, Channel& B) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        priv::MatrixChanImpl * M = Impl;

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];

            r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i) + M->P(20,i);
            g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i) + M->P(21,i);
            b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i) + M->P(22,i);
        }
    }

    void ColorMatrixChannel::applyTo(Channel & R, Channel & G, Channel& B, Channel& A) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        float * a = A.getData();
        priv::MatrixChanImpl * M = Impl;

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];
            const float xa = a[i];

            r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i)*xa + M->P(20,i);
            g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i)*xa + M->P(21,i);
            b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i)*xa + M->P(22,i);
            a[i] = M->P(3,i)*xr + M->P(8,i)*xg + M->P(13,i)*xb + M->P(18,i)*xa + M->P(23,i);
        }
    }

    void ColorMatrixChannel::applyTo(Image& image) const
    {
        float * r = image.getRed().getData();
        float * g = image.getGreen().getData();
        float * b = image.getBlue().getData();
        float * a = image.getAlpha().getData();
        priv::MatrixChanImpl * M = Impl;

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];
            const float xa = a[i];

            r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i)*xa + M->P(20,i);
            g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i)*xa + M->P(21,i);
            b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i)*xa + M->P(22,i);
            a[i] = M->P(3,i)*xr + M->P(8,i)*xg + M->P(13,i)*xb + M->P(18,i)*xa + M->P(23,i);
        }
    }

    void ColorMatrixChannel::applyTo(Channel & R, Channel & G, Channel& B, const Mask & mask) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        priv::MatrixChanImpl * M = Impl;
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];

                r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i) + M->P(20,i);
                g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i) + M->P(21,i);
                b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i) + M->P(22,i);
            }
        }
    }

    void ColorMatrixChannel::applyTo(Channel & R, Channel & G, Channel& B, Channel& A, const Mask & mask) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        float * a = A.getData();
        priv::MatrixChanImpl * M = Impl;
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];
                const float xa = a[i];

                r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i)*xa + M->P(20,i);
                g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i)*xa + M->P(21,i);
                b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i)*xa + M->P(22,i);
                a[i] = M->P(3,i)*xr + M->P(8,i)*xg + M->P(13,i)*xb + M->P(18,i)*xa + M->P(23,i);
            }
        }
    }

    void ColorMatrixChannel::applyTo(Image& image, const Mask & mask) const
    {
        float * r = image.getRed().getData();
        float * g = image.getGreen().getData();
        float * b = image.getBlue().getData();
        float * a = image.getAlpha().getData();
        priv::MatrixChanImpl * M = Impl;
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];
                const float xa = a[i];

                r[i] = M->P(0,i)*xr + M->P(5,i)*xg + M->P(10,i)*xb + M->P(15,i)*xa + M->P(20,i);
                g[i] = M->P(1,i)*xr + M->P(6,i)*xg + M->P(11,i)*xb + M->P(16,i)*xa + M->P(21,i);
                b[i] = M->P(2,i)*xr + M->P(7,i)*xg + M->P(12,i)*xb + M->P(17,i)*xa + M->P(22,i);
                a[i] = M->P(3,i)*xr + M->P(8,i)*xg + M->P(13,i)*xb + M->P(18,i)*xa + M->P(23,i);
            }
        }
    }

    void ColorMatrixChannel::setCrossRotation(const Channel& X, const Channel& Y, const Channel& Z)
    {
        const double pi = irr::core::DEGTORAD;
        setCrossRotationRAD(X*pi,Y*pi,Z*pi);
    }

    void ColorMatrixChannel::setCrossRotationRAD(const Channel& X, const Channel& Y, const Channel& Z)
    {
        const float * x = X.getConstData();
        const float * y = Y.getConstData();
        const float * z = Z.getConstData();
        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const double Rs = sin( x[i] );
            const double Rc = cos( x[i]);
            const double Gs = sin( y[i] );
            const double Gc = cos( y[i] );
            const double Bs = sin( z[i] );
            const double Bc = cos( z[i] );

            p1[i] = Gc*Bc - Bs;
            p4[i] = Gc*Bs + Bc;
            p7[i] =   -Gs;

            const double RsGs = Rs*Gs;

            p2[i] = RsGs*Bc - Rc*Bs;
            p5[i] = RsGs*Bs + Rc*Bc;
            p8[i] = Gc*Rs;

            const double RcGs = Rc*Gs;

            p3[i] = RcGs*Bc + Rs*Bs;
            p6[i] = RcGs*Bs - Bc*Rs;
            p9[i] = Rc*Gc;
        }
    }

    void ColorMatrixChannel::setEulerRotation(const Channel& R, const Channel& P, const Channel& Y)
    {
        const double pi = irr::core::DEGTORAD;
        setEulerRotationRAD(R*pi,P*pi,Y*pi);
    }

    void ColorMatrixChannel::setEulerRotationRAD(const Channel& R, const Channel& P, const Channel& Y)
    {
        const float * r = R.getConstData();
        const float * p = P.getConstData();
        const float * y = Y.getConstData();
        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const double cr = cos( r[i] ); //This function was taken directly from irrlicht.
            const double sr = sin( r[i] ); //see irrlicht license by downloading irrlicht.
            const double cp = cos( p[i] );
            const double sp = sin( p[i] );
            const double cy = cos( y[i] );
            const double sy = sin( y[i] );

            p1[i] =( cp*cy );
            p2[i] =( cp*sy );
            p3[i] =( -sp );

            const double srsp = sr*sp;
            const double crsp = cr*sp;

            p4[i] =( srsp*cy-cr*sy );
            p5[i] =( srsp*sy+cr*cy );
            p6[i] =( sr*cp );

            p7[i] = crsp*cy+sr*sy ;
            p8[i] = crsp*sy-sr*cy ;
            p9[i] = cr*cp ;
        }
    }

    void ColorMatrixChannel::setColorRotation(const Channel& Angle)
    {
        const double pi = irr::core::DEGTORAD;
        setColorRotationRAD(Angle*pi);
    }

    void ColorMatrixChannel::setColorRotationRAD(const Channel& Angle)
    {
        double asq = 1.0/sqrt(3.0);
        double assq = asq*asq;

        const float * r = Angle.getConstData();

        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const double c = cos(r[i]);
            const double s = sin(r[i]);

            const double ct = 1 - c;
            const double p = ct*assq;
            const double t = asq*s;

            const float a = p + c;
            const float k = p + t;
            const float j = p - t;

            p1[i] = a ;
            p2[i] = j ;
            p3[i] = k ;

            p4[i] = k ;
            p5[i] = a ;
            p6[i] = j ;

            p7[i] = j ;
            p8[i] = k ;
            p9[i] = a ;
        }
    }

    void ColorMatrixChannel::setCrossRotation(const Channel& X, const Channel& Y, const Channel& Z, const Mask & mask)
    {
        const double pi = irr::core::DEGTORAD;
        setCrossRotationRAD(X*pi,Y*pi,Z*pi,mask);
    }

    void ColorMatrixChannel::setCrossRotationRAD(const Channel& X, const Channel& Y, const Channel& Z, const Mask & mask)
    {
        const float * x = X.getConstData();
        const float * y = Y.getConstData();
        const float * z = Z.getConstData();
        const bool * l = mask.getConstData();
        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(l[i])
            {
                const double Rs = sin( x[i] );
                const double Rc = cos( x[i]);
                const double Gs = sin( y[i] );
                const double Gc = cos( y[i] );
                const double Bs = sin( z[i] );
                const double Bc = cos( z[i] );

                p1[i] = Gc*Bc - Bs;
                p4[i] = Gc*Bs + Bc;
                p7[i] =   -Gs;

                const double RsGs = Rs*Gs;

                p2[i] = RsGs*Bc - Rc*Bs;
                p5[i] = RsGs*Bs + Rc*Bc;
                p8[i] = Gc*Rs;

                const double RcGs = Rc*Gs;

                p3[i] = RcGs*Bc + Rs*Bs;
                p6[i] = RcGs*Bs - Bc*Rs;
                p9[i] = Rc*Gc;
            }
        }
    }

    void ColorMatrixChannel::setEulerRotation(const Channel& R, const Channel& P, const Channel& Y, const Mask & mask)
    {
        const double pi = irr::core::DEGTORAD;
        setEulerRotationRAD(R*pi,P*pi,Y*pi,mask);
    }

    void ColorMatrixChannel::setEulerRotationRAD(const Channel& R, const Channel& P, const Channel& Y, const Mask & mask)
    {
        const float * r = R.getConstData();
        const float * p = P.getConstData();
        const float * y = Y.getConstData();
        const bool * l = mask.getConstData();
        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(l[i])
            {
                const double cr = cos( r[i] ); //This function was taken directly from irrlicht.
                const double sr = sin( r[i] ); //see irrlicht license by downloading irrlicht.
                const double cp = cos( p[i] );
                const double sp = sin( p[i] );
                const double cy = cos( y[i] );
                const double sy = sin( y[i] );

                p1[i] =( cp*cy );
                p2[i] =( cp*sy );
                p3[i] =( -sp );

                const double srsp = sr*sp;
                const double crsp = cr*sp;

                p4[i] =( srsp*cy-cr*sy );
                p5[i] =( srsp*sy+cr*cy );
                p6[i] =( sr*cp );

                p7[i] = crsp*cy+sr*sy ;
                p8[i] = crsp*sy-sr*cy ;
                p9[i] = cr*cp ;
            }
        }
    }

    void ColorMatrixChannel::setColorRotation(const Channel& Angle, const Mask & mask)
    {
        const double pi = irr::core::DEGTORAD;
        setColorRotationRAD(Angle*pi,mask);
    }

    void ColorMatrixChannel::setColorRotationRAD(const Channel& Angle, const Mask & mask)
    {
        double asq = 1.0/sqrt(3.0);
        double assq = asq*asq;

        const float * r = Angle.getConstData();
        const bool * l = mask.getConstData();
        float * p1 = Impl->T[0];
        float * p2 = Impl->T[1];
        float * p3 = Impl->T[2];
        float * p4 = Impl->T[5];
        float * p5 = Impl->T[6];
        float * p6 = Impl->T[7];
        float * p7 = Impl->T[10];
        float * p8 = Impl->T[11];
        float * p9 = Impl->T[12];

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(l[i])
            {
                double c = cos(r[i]);
                double s = sin(r[i]);

                double ct = 1 - c;
                double p = ct*assq;
                double t = asq*s;

                float a = p + c;
                float k = p + t;
                float j = p - t;

                p1[i] = a;
                p2[i] = j;
                p3[i] = k;

                p4[i] = k;
                p5[i] = a;
                p6[i] = j;

                p7[i] = j;
                p8[i] = k;
                p9[i] = a;
            }
        }
    }

    ColorMatrixChannel& ColorMatrixChannel::operator=(const ColorMatrixChannel& other)
    {
        for(unsigned int i=0; i<25;i++)
        {
            Impl->M[i] = other.Impl->M[i];
        }
        //Impl->synchronize();  ///   !!!!   <----
        return *this;
    }

    ColorMatrixChannel ColorMatrixChannel::operator*(const ColorMatrixChannel& other) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i]*other.Impl->M[i];
        }
        return temp;

    }

    ColorMatrixChannel ColorMatrixChannel::operator+(const ColorMatrixChannel& other) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i] + other.Impl->M[i];
        }
        return temp;
    }


    ColorMatrixChannel& ColorMatrixChannel::operator=(const ColorMatrix& other)
    {
        for(unsigned int i=0; i<25;i++)
        {
            Impl->M[i] = other.M[i];
        }
        //Impl->synchronize();
        return *this;
    }


    ColorMatrixChannel ColorMatrixChannel::operator*(const ColorMatrix& other) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i]*other.M[i];
        }
        return temp;
    }

    ColorMatrixChannel ColorMatrixChannel::operator+(const ColorMatrix& other) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i] + other.M[i];
        }
        return temp;
    }

    ColorMatrixChannel& ColorMatrixChannel::operator=(float value)
    {
        for(unsigned int i=0; i<25;i++)
        {
            Impl->M[i] = value;
        }
        return *this;
    }

    ColorMatrixChannel ColorMatrixChannel::operator*(float value) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i] * value;
        }
        return temp;
    }

    ColorMatrixChannel ColorMatrixChannel::operator+(float value) const
    {
        ColorMatrixChannel temp;
        for(unsigned int i=0; i<25;i++)
        {
            temp.Impl->M[i] = Impl->M[i] + value;
        }
        return temp;
    }


} // end of namespace
