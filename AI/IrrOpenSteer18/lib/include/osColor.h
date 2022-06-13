// ----------------------------------------------------------------------------<br>
// Irrlicht implementation by A.Buschhüter<br> (http://abusoft.g0dsoft.com<br>)
// ----------------------------------------------------------------------------<br>
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------<br>
//
//
// Color class and predefined colors.
//
// May 05, 2005 bk:  created
//
//
// ----------------------------------------------------------------------------<br>
#include "../../../../config.h"
#ifdef OPENSTEER
#ifndef OPENSTEER_COLOR_H
#define OPENSTEER_COLOR_H

#include <irrlicht.h>

namespace OpenSteer{
    // Forward declaration. Full declaration in Vec3.h
class Vec3;

class Color{
  public:
    Color();
    Color(irr::video::SColor col);
    explicit Color(float greyValue);
    Color(float rValue, float gValue, float bValue, float aValue = 1.0f);
    explicit Color(Vec3 const& vector);

    float r() const;
    float g() const;
    float b() const;
    float a() const;
    int intR() const;
    int intG() const;
    int intB() const;
    int intA() const;
    irr::video::SColor SColor() const;

    void setR(float value);
    void setG(float value);
    void setB(float value);
    void setA(float value);

    void setR(int value);
    void setG(int value);
    void setB(int value);
    void setA(int value);

    void set(irr::video::SColor col);
    void set(float rValue, float gValue, float bValue, float aValue = 1.0f);

    Vec3 convertToVec3() const;

		// this is necessary so that graphics API's such as DirectX
		// requiring a pointer to colors can do their conversion
		// without a lot of copying.
		float const*const colorFloatArray() const { return &r_; }

    Color& operator+=( Color const& other );
    /**
     * @todo What happens if color components become negative?
     */
    Color& operator-=( Color const& other );
    /**
     * @todo What happens if color components become negative?
     */
    Color& operator*=( float factor );
    /**
     * @todo What happens if color components become negative?
     */
    Color& operator/=( float factor );

  private:
    irr::video::SColor colIrr;
    float r_;
    float g_;
    float b_;
    float a_;	// provided for API's which require four components
    }; // class Color

    Color operator+( Color const& lhs, Color const& rhs );

    /**
     * @todo What happens if color components become negative?
     */
    Color operator-( Color const& lhs, Color const& rhs );

    /**
     * @todo What happens if color components become negative?
     */
    Color operator*( Color const& lhs, float rhs );

    /**
     * @todo What happens if color components become negative?
     */
    Color operator*( float lhs, Color const& rhs );

    /**
     * @todo What happens if color components become negative?
     */
    Color operator/( Color const& lhs, float rhs );


    Color grayColor( float value );

    extern Color const gBlack;
    extern Color const gWhite;
    extern Color const gRed;
    extern Color const gGreen;
    extern Color const gBlue;
    extern Color const gYellow;
    extern Color const gCyan;
    extern Color const gMagenta;
    extern Color const gOrange;
    extern Color const gDarkRed;
    extern Color const gDarkGreen;
    extern Color const gDarkBlue;
    extern Color const gDarkYellow;
    extern Color const gDarkCyan;
    extern Color const gDarkMagenta;
    extern Color const gDarkOrange;

    extern Color const gGray10;
    extern Color const gGray20;
    extern Color const gGray30;
    extern Color const gGray40;
    extern Color const gGray50;
    extern Color const gGray60;
    extern Color const gGray70;
    extern Color const gGray80;
    extern Color const gGray90;

} // namespace OpenSteer


#endif // OPENSTEER_COLOR_H
#endif
