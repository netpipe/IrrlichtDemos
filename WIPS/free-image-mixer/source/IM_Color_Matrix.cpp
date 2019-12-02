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
#include "IM_Image.h"
#include "IM_Mask.h"
#include "IM_Channel.h"
#include "IM_ColorMatrix.h"
#include <cstring>
#include <irrMath.h>

namespace mixer
{

    ColorMatrix::ColorMatrix()
    {
        memset(M, 0, 25*sizeof(float)); // identity.
        M[0] = M[6] = M[12] = M[18] = M[24] = 1;
    }

    ColorMatrix::ColorMatrix(const ColorMatrix& other)
    {
        memcpy(M,other.M, 25*sizeof(float));
    }

    ColorMatrix::~ColorMatrix()
    {

    }

    bool ColorMatrix::load(const char* filename)
    {
        return false;
    }

    bool ColorMatrix::save(const char* filename) const
    {
        return false;
    }

    void ColorMatrix::applyTo(Channel & R, Channel & G, Channel& B) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];

            r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15] + M[20];
            g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16] + M[21];
            b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17] + M[22];
        }

    }

    void ColorMatrix::applyTo(Channel & R, Channel & G, Channel& B, Channel& A)const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        float * a = A.getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];
            const float xa = a[i];

            r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15]*xa + M[20];
            g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16]*xa + M[21];
            b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17]*xa + M[22];
            a[i] = M[3]*xr + M[8]*xg + M[13]*xb + M[18]*xa + M[23];
        }
    }

    void ColorMatrix::applyTo(Image& image)const
    {
        float * r = image.getRed().getData();
        float * g = image.getGreen().getData();
        float * b = image.getBlue().getData();
        float * a = image.getAlpha().getData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float xr = r[i];
            const float xg = g[i];
            const float xb = b[i];
            const float xa = a[i];

            r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15]*xa + M[20];
            g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16]*xa + M[21];
            b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17]*xa + M[22];
            a[i] = M[3]*xr + M[8]*xg + M[13]*xb + M[18]*xa + M[23];
        }
    }

    void ColorMatrix::applyTo(Channel & R, Channel & G, Channel& B, const Mask & mask) const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];

                r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15] + M[20];
                g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16] + M[21];
                b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17] + M[22];
            }
        }

    }

    void ColorMatrix::applyTo(Channel & R, Channel & G, Channel& B, Channel& A, const Mask & mask)const
    {
        float * r = R.getData();
        float * g = G.getData();
        float * b = B.getData();
        float * a = A.getData();
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];
                const float xa = a[i];

                r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15]*xa + M[20];
                g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16]*xa + M[21];
                b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17]*xa + M[22];
                a[i] = M[3]*xr + M[8]*xg + M[13]*xb + M[18]*xa + M[23];
            }
        }
    }

    void ColorMatrix::applyTo(Image& image, const Mask & mask)const
    {
        float * r = image.getRed().getData();
        float * g = image.getGreen().getData();
        float * b = image.getBlue().getData();
        float * a = image.getAlpha().getData();
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                const float xr = r[i];
                const float xg = g[i];
                const float xb = b[i];
                const float xa = a[i];

                r[i] = M[0]*xr + M[5]*xg + M[10]*xb + M[15]*xa + M[20];
                g[i] = M[1]*xr + M[6]*xg + M[11]*xb + M[16]*xa + M[21];
                b[i] = M[2]*xr + M[7]*xg + M[12]*xb + M[17]*xa + M[22];
                a[i] = M[3]*xr + M[8]*xg + M[13]*xb + M[18]*xa + M[23];
            }
        }
    }

    ColorMatrix& ColorMatrix::operator=(const ColorMatrix& other)
    {
        memcpy(M, other.M, 25*sizeof(float));
        return *this;
    }

    ColorMatrix ColorMatrix::operator*(const ColorMatrix& other) const
    {
        const float * m1 = M;
        const float * m2 = other.M;

        ColorMatrix temp;
        float * m3 = temp.M;

        m3[0]= m1[0]*m2[0] + m1[5]*m2[1] + m1[10]*m2[2] + m1[15]*m2[3] + m1[20]*m2[4];
        m3[1]= m1[1]*m2[0] + m1[6]*m2[1] + m1[11]*m2[2] + m1[16]*m2[3] + m1[21]*m2[4];
        m3[2]= m1[2]*m2[0] + m1[7]*m2[1] + m1[12]*m2[2] + m1[17]*m2[3] + m1[22]*m2[4];
        m3[3]= m1[3]*m2[0] + m1[8]*m2[1] + m1[13]*m2[2] + m1[18]*m2[3] + m1[23]*m2[4];
        m3[4]= m1[4]*m2[0] + m1[9]*m2[1] + m1[14]*m2[2] + m1[19]*m2[3] + m1[24]*m2[4];

        m3[5]= m1[0]*m2[5] + m1[5]*m2[6] + m1[10]*m2[7] + m1[15]*m2[8] + m1[20]*m2[9];
        m3[6]= m1[1]*m2[5] + m1[6]*m2[6] + m1[11]*m2[7] + m1[16]*m2[8] + m1[21]*m2[9];
        m3[7]= m1[2]*m2[5] + m1[7]*m2[6] + m1[12]*m2[7] + m1[17]*m2[8] + m1[22]*m2[9];
        m3[8]= m1[3]*m2[5] + m1[8]*m2[6] + m1[13]*m2[7] + m1[18]*m2[8] + m1[23]*m2[9];
        m3[9]= m1[4]*m2[5] + m1[9]*m2[6] + m1[14]*m2[7] + m1[19]*m2[8] + m1[24]*m2[9];

        m3[10]= m1[0]*m2[10] + m1[5]*m2[11] + m1[10]*m2[12] + m1[15]*m2[13] + m1[20]*m2[14];
        m3[11]= m1[1]*m2[10] + m1[6]*m2[11] + m1[11]*m2[12] + m1[16]*m2[13] + m1[21]*m2[14];
        m3[12]= m1[2]*m2[10] + m1[7]*m2[11] + m1[12]*m2[12] + m1[17]*m2[13] + m1[22]*m2[14];
        m3[13]= m1[3]*m2[10] + m1[8]*m2[11] + m1[13]*m2[12] + m1[18]*m2[13] + m1[23]*m2[14];
        m3[14]= m1[4]*m2[10] + m1[9]*m2[11] + m1[14]*m2[12] + m1[19]*m2[13] + m1[24]*m2[14];

        m3[15]= m1[0]*m2[15] + m1[5]*m2[16] + m1[10]*m2[17] + m1[15]*m2[18] + m1[20]*m2[19];
        m3[16]= m1[1]*m2[15] + m1[6]*m2[16] + m1[11]*m2[17] + m1[16]*m2[18] + m1[21]*m2[19];
        m3[17]= m1[2]*m2[15] + m1[7]*m2[16] + m1[12]*m2[17] + m1[17]*m2[18] + m1[22]*m2[19];
        m3[18]= m1[3]*m2[15] + m1[8]*m2[16] + m1[13]*m2[17] + m1[18]*m2[18] + m1[23]*m2[19];
        m3[19]= m1[4]*m2[15] + m1[9]*m2[16] + m1[14]*m2[17] + m1[19]*m2[18] + m1[24]*m2[19];

        m3[20]= m1[0]*m2[20] + m1[5]*m2[21] + m1[10]*m2[22] + m1[15]*m2[23] + m1[20]*m2[24];
        m3[21]= m1[1]*m2[20] + m1[6]*m2[21] + m1[11]*m2[22] + m1[16]*m2[23] + m1[21]*m2[24];
        m3[22]= m1[2]*m2[20] + m1[7]*m2[21] + m1[12]*m2[22] + m1[17]*m2[23] + m1[22]*m2[24];
        m3[23]= m1[3]*m2[20] + m1[8]*m2[21] + m1[13]*m2[22] + m1[18]*m2[23] + m1[23]*m2[24];
        m3[24]= m1[4]*m2[20] + m1[9]*m2[21] + m1[14]*m2[22] + m1[19]*m2[23] + m1[24]*m2[24];

        return temp;
    }

    ColorMatrix ColorMatrix::operator+(const ColorMatrix& otherm) const
    {
        const float * other = otherm.M;
        ColorMatrix temp;

        float * p = temp.M;

        p[0]  = M[0] +  other[0];
        p[1]  = M[1] +  other[1];
        p[2]  = M[2] +  other[2];
        p[3]  = M[3] +  other[3];
        p[4]  = M[4] +  other[4];
        p[5]  = M[5] +  other[5];
        p[6]  = M[6] +  other[6];
        p[7]  = M[7] +  other[7];
        p[8]  = M[8] +  other[8];
        p[9]  = M[9] +  other[9];
        p[10] = M[10] +  other[10];
        p[11] = M[11] +  other[11];
        p[12] = M[12] +  other[12];
        p[13] = M[13] +  other[13];
        p[14] = M[14] +  other[14];
        p[15] = M[15] +  other[15];
        p[16] = M[16] +  other[16];
        p[17] = M[17] +  other[17];
        p[18] = M[18] +  other[18];
        p[19] = M[19] +  other[19];
        p[20] = M[20] +  other[20];
        p[21] = M[21] +  other[21];
        p[22] = M[22] +  other[22];
        p[23] = M[23] +  other[23];
        p[24] = M[24] +  other[24];

        return temp;
    }

    void ColorMatrix::setCrossRotation(float X, float Y, float Z)
    {
        const double pi = irr::core::DEGTORAD;
        setCrossRotationRAD(X*pi,Y*pi,Z*pi);
    }

    void ColorMatrix::setCrossRotationRAD(float X, float Y, float Z)
    {
        const double Rs = sin( X );
        const double Rc = cos( X );
        const double Gs = sin( Y );
        const double Gc = cos( Y );
        const double Bs = sin( Z );
        const double Bc = cos( Z );

        M[0] = Gc*Bc - Bs;
        M[5] = Gc*Bs + Bc;
        M[10]=   -Gs;

        const double RsGs = Rs*Gs;

        M[1] = RsGs*Bc - Rc*Bs;
        M[6] = RsGs*Bs + Rc*Bc;
        M[11]= Gc*Rs;

        const double RcGs = Rc*Gs;

        M[2] = RcGs*Bc + Rs*Bs;
        M[7] = RcGs*Bs - Bc*Rs;
        M[12]= Rc*Gc;

    }

    void ColorMatrix::setEulerRotation(float R, float P, float Y)
    {
        const double pi = irr::core::DEGTORAD;
        setEulerRotationRAD(R*pi,P*pi,Y*pi);
    }

    void ColorMatrix::setEulerRotationRAD(float R, float P, float Y)
    {
        const double cr = cos( R ); //This function was taken directly from irrlicht.
		const double sr = sin( R ); //see irrlicht license by downloading irrlicht.
		const double cp = cos( P );
		const double sp = sin( P );
		const double cy = cos( Y );
		const double sy = sin( Y );

		M[0] =( cp*cy );
		M[1] =( cp*sy );
		M[2] =( -sp );

		const double srsp = sr*sp;
		const double crsp = cr*sp;

		M[5] =( srsp*cy-cr*sy );
		M[6] =( srsp*sy+cr*cy );
		M[7] =( sr*cp );

		M[10] = crsp*cy+sr*sy ;
		M[11] = crsp*sy-sr*cy ;
		M[12] = cr*cp ;
    }

    ColorMatrix& ColorMatrix::operator=(float value)
    {
        for(int i=0; i<25; i++)
        {
            M[i] = value;
        }
        return *this;
    }

    ColorMatrix ColorMatrix::operator*(float value) const
    {
        ColorMatrix temp;
        for(int i=0; i<25; i++)
        {
            temp.M[i] = M[i]*value;
        }
        return temp;

    }

    ColorMatrix ColorMatrix::operator+(float value) const
    {
        ColorMatrix temp;
        for(int i=0; i<25; i++)
        {
            temp.M[i] = M[i]+value;
        }
        return temp;
    }


    void ColorMatrix::setColorRotation(float Angle)
    {
        const double pi = irr::core::DEGTORAD;
        setColorRotationRAD(Angle*pi);
    }

    void ColorMatrix::setColorRotationRAD(float Angle)
    {
        const double sq = 1.0/sqrt(3.0);
        const double ssq = sq*sq;

        const double c = cos(Angle);
        const double s = sin(Angle);
        const double ct = 1 - c;
        const double p = ct*ssq;
        const double t = sq*s;
        const float a = p + c;
        const float k = p + t;
        const float i = p - t;

        M[0] = a;
        M[1] = i;
        M[2] = k;

        M[5] = k;
		M[6] = a;
		M[7] = i;

		M[10] = i;
		M[11] = k;
		M[12] = a;
    }

} // end of namespace

