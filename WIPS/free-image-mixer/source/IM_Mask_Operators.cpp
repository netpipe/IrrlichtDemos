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
#include "IM_Exception.h"
#include "IM_Mask.h"
#include "IM_Mask_Implementation.h"
#include <cstring>


namespace mixer
{
    Mask::Mask()
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::MaskImpl();
        Impl->Data = priv::BoolPool::get();
    }

    Mask::Mask(bool value)
    {
        if(priv::G::ChannelSize==0)
            throw Exception("Unspecified size: please initialize your working area first.");

        Impl = new priv::MaskImpl();
        Impl->Data = priv::BoolPool::get();

        this->operator=(value);
    }

    Mask::Mask(const Mask& other)
    {
        Impl = new priv::MaskImpl();
        Impl->Data = other.Impl->Data;
        Impl->ShallowCopy = true;
    }

    Mask::~Mask()
    {
        delete Impl;
    }

    Mask& Mask::operator = (const Mask& other)
    {
        memcpy( Impl->Data, other.Impl->Data, priv::G::ChannelSize*sizeof(bool) );
        return *this;
    }

    Mask& Mask::operator = (bool value)
    {
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            Impl->Data[i] = value;
        }

        return *this;
    }

    Mask Mask::operator ==(const Mask& other) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] == other.Impl->Data[i];
        }

        return temp;
    }

    Mask Mask::operator !=(const Mask& other) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] != other.Impl->Data[i];
        }

        return temp;
    }

    Mask Mask::operator &&(const Mask& other) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] && other.Impl->Data[i];
        }

        return temp;
    }

    Mask Mask::operator ||(const Mask& other) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] || other.Impl->Data[i];
        }

        return temp;
    }

    Mask Mask::operator ^ (const Mask& other) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] ^ other.Impl->Data[i];
        }

        return temp;
    }

    Mask Mask::operator ==(bool value) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] == value;
        }

        return temp;
    }

    Mask Mask::operator !=(bool value) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] != value;
        }

        return temp;
    }

    Mask Mask::operator &&(bool value) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] && value;
        }

        return temp;
    }

    Mask Mask::operator ||(bool value) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] || value;
        }

        return temp;
    }

    Mask Mask::operator ^ (bool value) const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = Impl->Data[i] ^ value;
        }

        return temp;
    }

    Mask Mask::operator ! () const
    {
        Mask temp;
        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            temp.Impl->Data[i] = !Impl->Data[i];
        }

        return temp;
    }

    bool * Mask::getData()
    {
        return Impl->Data;
    }

    const bool* Mask::getConstData() const
    {
        return Impl->Data;
    }

    unsigned int Mask::getSize() const
    {
        return priv::G::ChannelSize;
    }

} // end of namespace
