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

#ifndef IMAGE_MIXER_IMAGE_H_INCLUDED
#define IMAGE_MIXER_IMAGE_H_INCLUDED

/// windows only. dll setup. (MACROS are defined in the project settings)
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
        struct ImageImpl;
    } // end of namespace
    class Channel;
    class Mask;
    class ColorMatrix;

    class Image
    {
    private:

        priv::ImageImpl * Impl;

    public:

        //TODO: operators Channel To Image (example. Image*Red)

        Image();
        Image(float value);
        Image(const Channel& chan); //deep copy.
        ~Image();

        Image& operator= ( const Image& other);
        Image& operator*=(const Image& other); /// TODO: missing!
        Image& operator+=(const Image& other); /// TODO: missing!

        bool load(const char* filename);
        bool load(const char* filename, int ax, int ay, int bx, int by);
        bool save(const char* filename);
        void clamp();

        void setGreen(const Channel& green);
        void setBlue(const Channel& blue);
        void setRed(const Channel& red);
        void setAlpha(const Channel& alpha);

        Channel getGreen() const;
        Channel getBlue() const;
        Channel getRed() const;
        Channel getAlpha() const;
        Channel getLightness() const;
        Channel getLuminance() const;
        Channel getAverage() const;
        //Channel getColor(float r, float g, float b) const;  BETA:

        //utility. Allow direct write to channel components.
        Channel& R();  ///TODO: Implement this!
        Channel& G();  ///TODO: Implement this!
        Channel& B();  ///TODO: Implement this!
        Channel& A();  ///TODO: Implement this!

        void setRGB(const Channel& red, const Channel& green, const Channel& blue);
        void setRGBA(const Channel& red, const Channel& green, const Channel& blue, const Channel& alpha);
        void getRGB(Channel& red, Channel& green, Channel& blue) const;
        void getRGBA(Channel& red, Channel& green, Channel& blue, Channel& alpha) const;

        ///
        ///     INTERPOLATORS: (very nice and usefull).
        ///

        void mix(const Image& image, const Channel& t);
        void mixCosine(const Image& image, const Channel& t);
        void mixQuadratic(const Image& B, const Image& C, const Channel& t);
        void mixCubic(const Image& B, const Image& C, const Image& D, const Channel& t);

            /// INTERPOLATORS WITH MASK FILTERING

        void mix(const Image& image, const Channel& t, const Mask& mask);
        void mixCosine(const Image& image, const Channel& t, const Mask& mask);
        void mixQuadratic(const Image& B, const Image& C, const Channel& t, const Mask& mask);
        void mixCubic(const Image& B, const Image& C, const Image& D, const Channel& t, const Mask& mask);


    }; // end of class

} // end of namespace


#endif // IMAGE_MIXER_IMAGE_H_INCLUDED

