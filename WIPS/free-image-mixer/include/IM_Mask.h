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

#ifndef IMAGE_MIXER_MASK_H_INCLUDED
#define IMAGE_MIXER_MASK_H_INCLUDED

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
        struct MaskImpl;
    } // end of namespace

    class Mask
    {
        priv::MaskImpl * Impl;

    public:

        Mask();
        Mask(bool value);
        Mask(const Mask& other); //this is the ctor for a shallow copy. For a deep copy use "operator =".
        ~Mask();

        Mask& operator = (const Mask& other);

        Mask& operator = (bool value);

        Mask operator ==(const Mask& other) const;
        Mask operator !=(const Mask& other) const;
        Mask operator &&(const Mask& other) const;
        Mask operator ||(const Mask& other) const;
        Mask operator ^ (const Mask& other) const;

        Mask operator ==(bool value) const;
        Mask operator !=(bool value) const;
        Mask operator &&(bool value) const;
        Mask operator ||(bool value) const;
        Mask operator ^ (bool value) const;

        Mask operator ! () const;

        bool * getData();
        const bool* getConstData() const;
        unsigned int getSize() const;

    }; // end of class

} // end of namespace


#endif // IMAGE_MIXER_MASK_H_INCLUDED

