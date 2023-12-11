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

#ifndef IMAGE_MIXER_OPTIONS_H_INCLUDED
#define IMAGE_MIXER_OPTIONS_H_INCLUDED

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
    class Options
    {
    public:

        Options();
        ~Options();

        /** Set the size of the working area. This canNOT be changed once setted*/
        void setSize(unsigned int X, unsigned int Y) const;

        /** Set the backgroundcolor for images. Used for example when loading with custom sizes*/
        void setBGColor(unsigned int R, unsigned int G, unsigned int B);
        void setBGColor(unsigned int R, unsigned int G, unsigned int B, unsigned int A);
        void setBGColor(float R, float G, float B);
        void setBGColor(float R, float G, float B, float A);

        /** Enable or disable autoclamping of imges when saving. */
        void setAutoClamping(bool enabled = true);

        /** Tolerance, used for example when comparing channel colors. This CAN be changed.
            (default value is 1.0/65536.0 = 0.00001526..) */
        void setTolerance (float tolerance);

    }; //end of class

} // end of namespace


#endif //IMAGE_MIXER_OPTIONS_H_INCLUDED
