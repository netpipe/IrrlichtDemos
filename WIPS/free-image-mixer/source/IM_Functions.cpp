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
#include "IM_Channel.h"
#include "IM_Functions.h"
#include <cmath>

namespace mixer
{
    Channel sin(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::sin(a[i]);
        }

        return temp;
    }

    Channel cos(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::cos(a[i]);
        }

        return temp;
    }

    Channel sqrt(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::sqrt(a[i]);
        }

        return temp;
    }

    Channel tan(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::tan(a[i]);
        }

        return temp;
    }

    Channel asin(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::asin(a[i]);
        }

        return temp;
    }

    Channel acos(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::acos(a[i]);
        }

        return temp;
    }

    Channel atan(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::atan(a[i]);
        }

        return temp;
    }

    Channel exp(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::exp(a[i]);
        }

        return temp;
    }

    Channel pow(const Channel& x, const Channel& y)
    {
        Channel temp;
        const float * a = x.getConstData();
        const float * b = y.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::pow(a[i],b[i]);
        }

        return temp;
    }

    Channel log(const Channel& x)
    {
        Channel temp;
        const float * a = x.getConstData();
        float * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = std::log(a[i]);
        }

        return temp;
    }

}
