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
#include "IM_Math.h"
#include "IM_Mask.h"
#include "IM_Channel.h"
#include "IM_Channel_Implementation.h"


namespace mixer
{
    Mask Channel::operator < (const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] < other.Impl->Data[i] ;
        }

        return temp;
    }

    Mask Channel::operator > (const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] > other.Impl->Data[i] ;
        }

        return temp;
    }

    Mask Channel::operator <=(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] <= other.Impl->Data[i] ;
        }

        return temp;
    }

    Mask Channel::operator >=(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] >= other.Impl->Data[i] ;
        }

        return temp;
    }

    Mask Channel::operator ==(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(other.Impl->Data[i],Impl->Data[i],priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::operator !=(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(other.Impl->Data[i],Impl->Data[i],priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::operator < (float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] < value ;
        }

        return temp;
    }

    Mask Channel::operator > (float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] > value ;
        }

        return temp;
    }

    Mask Channel::operator <=(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] <= value ;
        }

        return temp;
    }

    Mask Channel::operator >=(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = Impl->Data[i] >= value ;
        }

        return temp;
    }

    Mask Channel::operator ==(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(Impl->Data[i],value,priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::operator !=(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(Impl->Data[i],value,priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::equals(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(other.Impl->Data[i],Impl->Data[i],priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::differs(const Channel& other) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(other.Impl->Data[i],Impl->Data[i],priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::equals(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(Impl->Data[i],value,priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::differs(float value) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(Impl->Data[i],value,priv::G::CustomTolerance);
        }

        return temp;
    }

    Mask Channel::equals(const Channel& other, const Channel& tolerance) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(other.Impl->Data[i],Impl->Data[i],tolerance.Impl->Data[i]);
        }

        return temp;
    }

    Mask Channel::differs(const Channel& other, const Channel& tolerance) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(other.Impl->Data[i],Impl->Data[i],tolerance.Impl->Data[i]);
        }

        return temp;
    }

    Mask Channel::equals(float value, const Channel& tolerance) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::equals(value,Impl->Data[i],tolerance.Impl->Data[i]);
        }

        return temp;
    }

    Mask Channel::differs(float value, const Channel& tolerance) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = irr::core::differs(value,Impl->Data[i],tolerance.Impl->Data[i]);
        }

        return temp;
    }

    Mask Channel::isBetween(float min, float max) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float a = Impl->Data[i];
            p[i] = (a>=min) || (a<=max) ;
        }

        return temp;
    }

    Mask Channel::isBetween(const Channel& min, float max) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float a = Impl->Data[i];
            p[i] = (a>=min.Impl->Data[i]) || (a<=max) ;
        }

        return temp;
    }

    Mask Channel::isBetween(float min, const Channel& max) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float a = Impl->Data[i];
            p[i] = (a>=min) || (a<=max.Impl->Data[i]) ;
        }

        return temp;
    }

    Mask Channel::isBetween(const Channel& min, const Channel& max) const
    {
        Mask temp;
        bool * p = temp.getData();
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const float a = Impl->Data[i];
            p[i] = (a>=min.Impl->Data[i]) || (a<=max.Impl->Data[i]) ;
        }

        return temp;
    }

} // end of namespace mixer
