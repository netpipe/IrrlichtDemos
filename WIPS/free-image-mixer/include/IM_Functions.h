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

#ifndef IMAGE_MIXER_MATH_H_INCLUDED
#define IMAGE_MIXER_MATH_H_INCLUDED

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
    class Channel;

    Channel  sin(const Channel& x);
    Channel  cos(const Channel& x);
    Channel  sqrt(const Channel& x);
    Channel  tan(const Channel& x);
    Channel  asin(const Channel& x);
    Channel  acos(const Channel& x);
    Channel  atan(const Channel& x);
    Channel  exp(const Channel& x);
    Channel  pow(const Channel& x, const Channel& y);
    Channel  log(const Channel& x);

} // end of namespace

#endif // IMAGE_MIXER_MATH_H_INCLUDED
