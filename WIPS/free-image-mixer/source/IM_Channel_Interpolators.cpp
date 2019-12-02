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
#include "IM_Channel_Implementation.h"

namespace mixer
{
    void Channel::mix(const Channel& other, float t)
    {
        const float ct = 1.f - t;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = Impl->Data[i]*ct + t*other.Impl->Data[i];
        }
    }

    void Channel::mix(const Channel& other, const Channel& t)
    {
        const float * p = t.Impl->Data;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = Impl->Data[i]*(1.f - p[i]) + p[i]*other.Impl->Data[i];
        }
    }

    void Channel::mix(float value, float t)
    {
        const float ct = 1.f - t;
        const float v  = value*t;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = Impl->Data[i]*ct + v;
        }
    }

    void Channel::mix(float value, const Channel& t)
    {
        const float * p = t.Impl->Data;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = Impl->Data[i]*(1.f - p[i]) + p[i]*value;
        }
    }

} // end of namespace
