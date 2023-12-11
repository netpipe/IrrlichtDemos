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
#include "IM_Channel_Pool.h"
#include "IM_Exception.h"
#include <cstring>
#include <cmath>


namespace mixer
{
    Channel::Channel()
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::ChannelImpl();
        Impl->Data = priv::ChannelPool::get();
    }

    Channel::Channel(float value)
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::ChannelImpl();
        Impl->Data = priv::ChannelPool::get();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = value;
        }
    }

    Channel::Channel(Channel& other)
    {
        Impl = new priv::ChannelImpl();
        Impl->Data = other.Impl->Data;
        Impl->ShallowCopy = true;
    }

    Channel::~Channel()
    {
        delete Impl;
    }

    ///
    /// Assignment by Channels Operators.
    ///

    Channel& Channel::operator = (const Channel& other)
    {
        memcpy( Impl->Data, other.Impl->Data, priv::G::ChannelSize*sizeof(float) );
        return *this;
    }

    Channel& Channel::operator += (const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] += other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::operator -= (const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] -= other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::operator *= (const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] *= other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::operator /= (const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] /= other.Impl->Data[i];
        }

        return *this;
    }

    Channel& Channel::operator %= (const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = fmod(Impl->Data[i], other.Impl->Data[i]);
        }

        return *this;
    }

    ///
    /// Assignment by Value Operators.
    ///

    Channel& Channel::operator = (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = value;
        }

        return *this;

    }

    Channel& Channel::operator += (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] += value;
        }

        return *this;
    }

    Channel& Channel::operator -= (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] -= value;
        }

        return *this;
    }

    Channel& Channel::operator *= (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] *= value;
        }

        return *this;
    }

    Channel& Channel::operator /= (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] /= value;
        }

        return *this;
    }

    Channel& Channel::operator %= (float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = fmod(Impl->Data[i], value);
        }

        return *this;
    }

    ///
    /// Operators Channel by Channel
    ///

    Channel Channel::operator + (const Channel& other) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] + other.Impl->Data[i];
        }

        return temp;
    }

    Channel Channel::operator - (const Channel& other) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] - other.Impl->Data[i];
        }

        return temp;
    }

    Channel Channel::operator * (const Channel& other) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] * other.Impl->Data[i];
        }

        return temp;
    }

    Channel Channel::operator / (const Channel& other) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] / other.Impl->Data[i];
        }

        return temp;
    }

    Channel Channel::operator % (const Channel& other) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = fmod(Impl->Data[i] , other.Impl->Data[i]) ;
        }

        return temp;
    }

    ///
    /// Operators Channel by Value
    ///

    Channel Channel::operator + (float value) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] + value;
        }

        return temp;
    }

    Channel Channel::operator - (float value) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] - value;
        }

        return temp;
    }

    Channel Channel::operator * (float value) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] * value;
        }

        return temp;
    }

    Channel Channel::operator / (float value) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] / value;
        }

        return temp;
    }

    Channel Channel::operator % (float value) const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = fmod(Impl->Data[i] ,value) ;
        }

        return temp;
    }

    ///
    /// Other Operators.
    ///

    Channel Channel::operator -() const
    {
        Channel temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = - Impl->Data[i] ;
        }

        return temp;

    }

    void Channel::set(float value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = value;
        }
    }

    void Channel::set(const Channel& other)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = other.Impl->Data[i];
        }
    }

    void Channel::clamp(float min, float max)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = (Impl->Data[i] < min)? min:
                            (Impl->Data[i] > max)? max:
                                Impl->Data[i];
        }
    }

    void Channel::clamp(float min, const Channel& max)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = (Impl->Data[i] < min)? min:
                            (Impl->Data[i] > max.Impl->Data[i])? max.Impl->Data[i]:
                                Impl->Data[i];
        }
    }

    void Channel::clamp(const Channel& min, float max)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = (Impl->Data[i] < min.Impl->Data[i])? min.Impl->Data[i]:
                            (Impl->Data[i] > max)? max:
                                Impl->Data[i];
        }
    }

    void Channel::clamp(const Channel& min, const Channel& max)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = (Impl->Data[i] < min.Impl->Data[i])? min.Impl->Data[i]:
                            (Impl->Data[i] > max.Impl->Data[i])? max.Impl->Data[i]:
                                Impl->Data[i];
        }
    }

    void Channel::swap(const Channel& other)
    {
        if(! Impl->ShallowCopy && ! other.Impl->ShallowCopy)
        {
            float * p = Impl->Data;
            Impl->Data = other.Impl->Data;
            other.Impl->Data = p;
        }
        else
        {
            for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
            {
                float temp = Impl->Data[i];
                Impl->Data[i] = other.Impl->Data[i];
                other.Impl->Data[i] = temp;
            }
        }
    }

    float * Channel::getData()
    {
        return Impl->Data;
    }

    const float* Channel::getConstData() const
    {
        return Impl->Data;
    }

    unsigned int Channel::getSize() const
    {
        return priv::G::ChannelSize;
    }
}
