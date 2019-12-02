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
#ifndef IMAGE_MIXER_MATH_INCLUDED
#define IMAGE_MIXER_MATH_INCLUDED

#include <irrMath.h> //note that cosf is faster only if you use a irrlicht version wich supports fast FPU (no irrlicht versions for now)
#include <S3DVertex.h>

namespace irr
{
namespace core
{
//! returns if a differs from b, taking possible rounding errors into account. Complementary to irr::core::equals
template<class T>
inline bool differs(const T a, const T b, const T tolerance = ROUNDING_ERROR_f32)
{
    return (a + tolerance < b) && (a - tolerance > b);
}

//! returns cosine interpolation of a and b with ratio t
//! \return: a if t==0, b if t==1, and the cosine interpolation else
template<class T>
inline T coerp(const T& a, const T& b, const f32 t)
{
    const f32 x = ( 1- cos(t*PI64)) / 2;
    return (T)(a*(1.f-x)) + (b*x);
}

//! (fast)returns cosine interpolation of a and b with ratio t
//! \return: a if t==0, b if t==1, and the cosine interpolation else
template<class T>
inline T coerpf(const T& a, const T& b, const f32 t)
{
    const f32 x = ( 1- cosf(t*PI)) / 2;
    return (T)(a*(1.f-x)) + (b*x);
}

//! returns cubic interpolation of a,b,c,d with ratio t
//! \return: b if t==0, c if t==1, and the cubic interpolation else
template<class T>
inline T cuerp(const T& a, const T& b, const T& c, const T& d, const f32 t)
{
    const T S = (a-b);
    const T P = (d-c) - S; //1 less algebric operation (subtraction)

    //return P*t*t*t + (S-P)*t*t + (c-a)*t + b;
    return t*( t*(P*t + S-P) + c-a) + b; //halved number of multyplications :)
}

//! returns bilinear interpolation of (a,b),(c,d) with ratio tx, ty
//! \return: lerp(a,c,y) if tx = 0 , lerp(b,d,y) if tx = 1, and the bilinear interpolation else
template<class T>
inline T bilerp(const T& a, const T& b, const T& c, const T& d, const f32 tx,const f32 ty)
{
    const T S = lerp(a,b,tx);
    const T P = lerp(c,d,tx);

    return lerp(S,P,ty);
}

//! returns bicosine interpolation of (a,b),(c,d) with ratio tx, ty
//! \return: coerp(a,c,y) if tx = 0 , coerp(b,d,y) if tx = 1, and the cosine interpolation else
template<class T>
inline T bicoerp(const T& a, const T& b, const T& c, const T& d, const f32 tx,const f32 ty)
{
    const T S = coerp(a,b,tx);
    const T P = coerp(c,d,tx);

    return coerp(S,P,ty);
}

//! (Fast)returns bi-cosine interpolation of (a,b),(c,d) with ratio tx, ty
//! \return: coerpf(a,c,ty) if tx = 0 , coerpf(b,d,ty) if tx = 1, and the cosine interpolation else
template<class T>
inline T bicoerpf(const T& a, const T& b, const T& c, const T& d, const f32 tx,const f32 ty)
{
    const T S = coerpf(a,b,tx);
    const T P = coerpf(c,d,tx);

    return coerpf(S,P,ty);
}

//! Bicubic/Spline interpolators are not provided. Irrlicht has its own spline interpolator (3D) and the bicubic
//  interpolator can be easy implemented by yourself, just look at references pages and to other implementations


}
}

#endif // IMAGE_MIXER_MATH_INCLUDED
