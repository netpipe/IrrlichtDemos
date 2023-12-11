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

#ifndef IMAGE_MIXER_EXCEPTION_H_INCLUDED
#define IMAGE_MIXER_EXCEPTION_H_INCLUDED

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
    //Image mixer uses this class for internally handling exceptions. Every exception can be catched and printed
    class Exception
    {
    private:

        char my_char_string[256]; //255 characaters + string termination should be enough for all exceptions.

    public:

        Exception(const char*);

        void print() const; //print the content of the exception (a string with a description of the problem).

    }; // end of class

} // end of namespace


#endif // IMAGE_MIXER_EXCEPTION_H_INCLUDED
