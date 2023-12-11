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

#ifndef IMAGE_MIXER_COLOR_MATRIX_CHANNEL_H_INCLUDED
#define IMAGE_MIXER_COLOR_MATRIX_CHANNEL_H_INCLUDED

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
        struct MatrixChanImpl;
    }
    class Channel;
    class Image;
    class Mask;

    class ColorMatrixChannel
    {
        priv::MatrixChanImpl * Impl;

    public:

        ColorMatrixChannel();
        ColorMatrixChannel(const ColorMatrixChannel& other);
        ColorMatrixChannel(const ColorMatrix& other);
        ~ColorMatrixChannel();

        bool load(const char* filename);
        bool save(const char* filename)const;

        /** Apply the color matrix to 3 channels, faster than similiar functions. */
        void applyTo(Channel & R, Channel & G, Channel& B) const;

        /** Apply the color matrix to 4 channels. */
        void applyTo(Channel & R, Channel & G, Channel& B, Channel& A) const;

        /** Apply the color matrix to a Image. */
        void applyTo(Image& image) const;


                /// MASKED COLOR MATRIX APPLICATION

        void applyTo(Channel & R, Channel & G, Channel& B, const Mask & mask) const;
        void applyTo(Channel & R, Channel & G, Channel& B, Channel& A, const Mask & mask) const;
        void applyTo(Image& image, const Mask & mask) const;



        /** SetRotation. By X, Y, Z. This is not a standard rotation. */
        void setCrossRotation(const Channel& X, const Channel& Y, const Channel& Z);
        void setCrossRotationRAD(const Channel& X, const Channel& Y, const Channel& Z);

        /** Using EulerRotation does not make a real sense. But some effects are nice! :)*/
        void setEulerRotation(const Channel& R, const Channel& P, const Channel& Y);
        void setEulerRotationRAD(const Channel& R, const Channel& P, const Channel& Y);

        /** Standard Color Rotation (as can you see in many graphics applications). */
        void setColorRotation(const Channel& Angle);
        void setColorRotationRAD(const Channel& Angle);


                /// MASKED MATRICES CREATORS.

        void setCrossRotation(const Channel& X, const Channel& Y, const Channel& Z, const Mask & mask);
        void setCrossRotationRAD(const Channel& X, const Channel& Y, const Channel& Z, const Mask & mask);
        void setEulerRotation(const Channel& R, const Channel& P, const Channel& Y, const Mask & mask);
        void setEulerRotationRAD(const Channel& R, const Channel& P, const Channel& Y, const Mask & mask);
        void setColorRotation(const Channel& Angle, const Mask & mask);
        void setColorRotationRAD(const Channel& Angle, const Mask & mask);


        /** Assignment operator. */
        ColorMatrixChannel& operator=(const ColorMatrixChannel& other);

        /** Matrix concatenation. */
        ColorMatrixChannel operator*(const ColorMatrixChannel& other) const;

        /** Matrix sum element wise. */
        ColorMatrixChannel operator+(const ColorMatrixChannel& other) const;



        /** Assignment operator. */
        ColorMatrixChannel& operator=(const ColorMatrix& other);

        /** Matrix concatenation. */
        ColorMatrixChannel operator*(const ColorMatrix& other) const;

        /** Matrix sum element wise. */
        ColorMatrixChannel operator+(const ColorMatrix& other) const;


        /** Assignment operator. */
        ColorMatrixChannel& operator=(float value);

        /** Matrix multiplication. */
        ColorMatrixChannel operator*(float value) const;

        /** Matrix sum element wise. */
        ColorMatrixChannel operator+(float value) const;

    };

} // end of namespace


#endif // IMAGE_MIXER_COLOR_MATRIX_CHANNEL_H_INCLUDED
