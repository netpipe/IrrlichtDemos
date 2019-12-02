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
#include "IM_Mask.h"
#include "IM_Channel.h"
#include "IM_Channel_Implementation.h"
#include "IM_Channel_Pool.h"
#include "IM_Exception.h"
#include <cstring>
#include <cmath>


namespace mixer
{
    void Channel::set(float value, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = value;
        }
    }

    void Channel::set(const Channel& other, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = other.Impl->Data[i];
        }
    }

    void Channel::clamp(float min, float max, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = (Impl->Data[i] < min)? min:
                                (Impl->Data[i] > max)? max:
                                    Impl->Data[i];
        }
    }

    void Channel::clamp(float min, const Channel& max, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = (Impl->Data[i] < min)? min:
                                (Impl->Data[i] > max.Impl->Data[i])? max.Impl->Data[i]:
                                    Impl->Data[i];
        }
    }

    void Channel::clamp(const Channel& min, float max, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = (Impl->Data[i] < min.Impl->Data[i])? min.Impl->Data[i]:
                                (Impl->Data[i] > max)? max:
                                    Impl->Data[i];
        }
    }

    void Channel::clamp(const Channel& min, const Channel& max, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = (Impl->Data[i] < min.Impl->Data[i])? min.Impl->Data[i]:
                                (Impl->Data[i] > max.Impl->Data[i])? max.Impl->Data[i]:
                                    Impl->Data[i];
        }
    }

    void Channel::swap(const Channel& other, const Mask & mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
            {
                float temp = Impl->Data[i];
                Impl->Data[i] = other.Impl->Data[i];
                other.Impl->Data[i] = temp;
            }
        }
    }

    ///
    ///      MASKED ALGEBRAIC OPERATORS
    ///

    Channel& Channel::mod(float value, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = fmod(Impl->Data[i],value);
        }

        return *this;
    }

    Channel& Channel::mult(float value, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] * value;
        }

        return *this;
    }

    Channel& Channel::add(float value, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] + value;
        }

        return *this;
    }

    Channel& Channel::sub(float value, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] - value;
        }

        return *this;
    }

    Channel& Channel::div(float value, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] / value;
        }

        return *this;
    }


    Channel& Channel::mod(const Channel& other, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = fmod(Impl->Data[i], other.Impl->Data[i]);
        }

        return *this;
    }

    Channel& Channel::mult(const Channel& other, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] * other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::add(const Channel& other, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] + other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::sub(const Channel& other, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] - other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::div(const Channel& other, const Mask& mask)
    {
        const bool * p = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(p[i])
                Impl->Data[i] = Impl->Data[i] / other.Impl->Data[i];
        }

        return *this;
    }

    void Channel::mix(const Channel& other, float t, const Mask & mask)
    {
        const float ct = 1.f - t;
        const bool * m = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(m[i])
                Impl->Data[i] = Impl->Data[i]*ct + t*other.Impl->Data[i];
        }
    }

    void Channel::mix(const Channel& other, const Channel& t, const Mask & mask)
    {
        const float * p = t.Impl->Data;
        const bool * m = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(m[i])
                Impl->Data[i] = Impl->Data[i]*(1.f - p[i]) + p[i]*other.Impl->Data[i];
        }
    }

    void Channel::mix(float value, float t, const Mask & mask)
    {
        const float ct = 1.f - t;
        const float v  = value*t;
        const bool * m = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(m[i])
                Impl->Data[i] = Impl->Data[i]*ct + v;
        }
    }

    void Channel::mix(float value, const Channel& t, const Mask & mask)
    {
        const float * p = t.Impl->Data;
        const bool * m = mask.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            if(m[i])
                Impl->Data[i] = Impl->Data[i]*(1.f - p[i]) + p[i]*value;
        }
    }

} // end of namespace
