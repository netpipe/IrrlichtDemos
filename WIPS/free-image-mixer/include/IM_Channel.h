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

#ifndef IMAGE_MIXER_CHANNEL_H_INCLUDED
#define IMAGE_MIXER_CHANNEL_H_INCLUDED

/// windows only. dll setup. (MACROS are defined in the project settings.
#ifndef MIXER_API
    #ifdef DLL_IMPORT
        #define MIXER_API __declspec(dllimport)
    #elif defined (DLL_EXPORT)
        #define MIXER_API __declspec(dllexport)
    #else
        #define MIXER_API
    #endif
#endif


namespace mixer
{
    namespace priv
    {
        struct ChannelImpl;
    } // end of namespace
    class  Mask;

    class Channel
    {
        priv::ChannelImpl * Impl;

    public:

        /**
                ALGEBRAIC OPERATORS:
        */

        Channel();
        Channel(float value); //used for giving the chance to use numbers instead of channels
        Channel(Channel& other); //this is the ctor for a shallow copy. For a deep copy use "operator =".
        ~Channel();

        Channel& operator = (const Channel& other);
        Channel& operator +=(const Channel& other);
        Channel& operator -=(const Channel& other);
        Channel& operator *=(const Channel& other);
        Channel& operator /=(const Channel& other);
        Channel& operator %=(const Channel& other);

        Channel& operator = (float value);
        Channel& operator +=(float value);
        Channel& operator -=(float value);
        Channel& operator *=(float value);
        Channel& operator /=(float value);
        Channel& operator %=(float value);

        Channel operator+(const Channel& other)const;
        Channel operator-(const Channel& other)const;
        Channel operator*(const Channel& other)const;
        Channel operator/(const Channel& other)const;
        Channel operator%(const Channel& other)const;

        Channel operator+(float value)const;
        Channel operator-(float value)const;
        Channel operator*(float value)const;
        Channel operator/(float value)const;
        Channel operator%(float value)const;


        Channel operator -() const;

                            /// ALGEBRAIC OPERATORS, WITH MASK FILTERING

        Channel& mod(float value, const Mask& mask) ;   // modulus
        Channel& mult(float value, const Mask& mask) ;  // multiplication
        Channel& add(float value, const Mask& mask) ;   // addiction
        Channel& sub(float value, const Mask& mask) ;   // subtraction
        Channel& div(float value, const Mask& mask) ;   // division

        Channel& mod(const Channel& other, const Mask& mask) ;   // modulus
        Channel& mult(const Channel& other, const Mask& mask) ;  // multiplication
        Channel& add(const Channel& other, const Mask& mask) ;   // addiction
        Channel& sub(const Channel& other, const Mask& mask) ;   // subtraction
        Channel& div(const Channel& other, const Mask& mask) ;   // division

        /**
                LOGIC OPERATORS:
        */

        Mask operator < (const Channel& other) const;
        Mask operator > (const Channel& other) const;
        Mask operator <=(const Channel& other) const;
        Mask operator >=(const Channel& other) const;
        Mask operator ==(const Channel& other) const; //equals
        Mask operator !=(const Channel& other) const; //differs

        Mask operator < (float value) const;
        Mask operator > (float value) const;
        Mask operator <=(float value) const;
        Mask operator >=(float value) const;
        Mask operator ==(float value) const; //equals
        Mask operator !=(float value) const; //differs

        Mask equals(const Channel& other) const;
        Mask differs(const Channel& other) const;

        Mask equals(float value) const;
        Mask differs(float value) const;

        Mask equals(const Channel& other, const Channel& tolerance) const;
        Mask differs(const Channel& other, const Channel& tolerance) const;

        Mask equals(float value, const Channel& tolerance) const;
        Mask differs(float value, const Channel& tolerance) const;

        Mask isBetween(float min, float max) const;
        Mask isBetween(const Channel& min, float max) const;
        Mask isBetween(float min, const Channel& max) const;
        Mask isBetween(const Channel& min, const Channel& max) const;

        /// OTHER UTILITIES:
        float * getData();
        const float* getConstData() const;
        unsigned int getSize() const;


        /**
                OTHER OPERATIONS:
        */

        void set(float value);
        void set(const Channel& other);
        void clamp(float min=0.0, float max=1.0);
        void clamp(float min, const Channel& max);
        void clamp(const Channel& min, float max);
        void clamp(const Channel& min, const Channel& max);
        void swap(const Channel& other);

                            /// OTHER OPERATORS, MASK FILTERING

        void set(float value, const Mask & mask);
        void set(const Channel& other, const Mask & mask);
        void clamp(float min, float max, const Mask & mask);
        void clamp(float min, const Channel& max, const Mask & mask);
        void clamp(const Channel& min, float max, const Mask & mask);
        void clamp(const Channel& min, const Channel& max, const Mask & mask);
        void swap(const Channel& other, const Mask & mask);


        ///
        ///         INTERPOLATORS: (very nice and usefull)
        ///

        // Linear interpolation.
        void mix(const Channel& other, float t);
        void mix(const Channel& other, const Channel& t);
        void mix(float value, float t);
        void mix(float value, const Channel& t);


                            /// INTERPOLATORS, MASK FILTERING

        // Linear interpolation.
        void mix(const Channel& other, float t, const Mask & mask);
        void mix(const Channel& other, const Channel& t, const Mask & mask);
        void mix(float value, float t, const Mask & mask);
        void mix(float value, const Channel& t, const Mask & mask);

    }; //end of class

} // end of namespace




#endif //IMAGE_MIXER_CHANNEL_H_INCLUDED
