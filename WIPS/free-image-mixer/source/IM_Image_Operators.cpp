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
#include "IM_Image.h"
#include "IM_Channel.h"
#include "IM_Image_Implementation.h"
#include "IM_Exception.h"
#include <irrlicht.h>


namespace mixer
{

    Image::Image() //TODO move device init into implementation of the image.
    {
        if(!priv::G::GlobalDevice)
                throw Exception("First create an Options object");

        priv::G::GlobalDevice->grab();
        Impl = new priv::ImageImpl();
    }

    Image::Image(float value)
    {
        if(!priv::G::GlobalDevice)
                throw Exception("First create an Options object");

        priv::G::GlobalDevice->grab();
        Impl = new priv::ImageImpl();
        Impl->R = value;
        Impl->G = value;
        Impl->B = value;
    }

    Image::Image(const Channel & chan)
    {
        if(!priv::G::GlobalDevice)
                throw Exception("First create an Options object");

        priv::G::GlobalDevice->grab();
        Impl = new priv::ImageImpl();
        Impl-> R = Impl -> G = Impl -> B = chan;
    }

    Image::~Image()
    {
        delete Impl;
        priv::G::GlobalDevice->drop();
    }

    Image& Image::operator=( const Image& other)
    {
        Impl->R = other.Impl->R;
        Impl->G = other.Impl->G;
        Impl->B = other.Impl->B;

        return *this;
    }

    Image& Image::operator*=( const Image& other)
    {
        Impl->R *= other.Impl->R;
        Impl->G *= other.Impl->G;
        Impl->B *= other.Impl->B;

        return *this;
    }

    Image& Image::operator+=( const Image& other)
    {
        Impl->R += other.Impl->R;
        Impl->G += other.Impl->G;
        Impl->B += other.Impl->B;

        return *this;
    }

    Channel& Image::R()
    {
        return Impl->R;
    }

    Channel& Image::G()
    {
        return Impl->G;
    }

    Channel& Image::B()
    {
        return Impl->B;
    }

    Channel& Image::A()
    {
        return Impl->A;
    }

    bool Image::load(const char* filename)
    {
        irr::video::IVideoDriver *driver= priv::G::GlobalDevice->getVideoDriver();
        irr::video::IImage * image = driver->createImageFromFile(filename);

        if(!image)
        {
            Exception ex("Unable to find and load an image.");
            ex.print(); //dont' throw! I need to warn the user, bu throwing will cause memory leach.
            return false;
        }

        const unsigned int X = priv::G::WorkAreaX;
        const unsigned int Y = priv::G::WorkAreaY;

        irr::video::IImage * target =
            driver->createImage( irr::video::ECF_A8R8G8B8 ,
                                 irr::core::dimension2d<u32>( X, Y));

        image->copyToScaling(target->lock(), X, Y, video::ECF_A8R8G8B8);
        target->unlock();

        Impl->channelize(target);

        image->drop();
        target->drop();

        return true;
    }

    bool Image::load(const char* filename, int ax, int ay, int bx, int by)
    {
        irr::video::IVideoDriver *driver= priv::G::GlobalDevice->getVideoDriver();
        irr::video::IImage * image = driver->createImageFromFile(filename);

        if(bx<ax) //keep coordinates order right
        {
            int temp = ax;
            ax = bx;
            bx = temp;
        }

        if(by<ay)
        {
            int temp = by;
            ay = by;
            by = temp;
        }

        if((by == ay) || (bx == ax))
        {
            Exception ex("Unable to resize a Image to size 0 on one or both sides.");
            ex.print(); //dont' throw! I need to warn the user, bu throwing will cause memory leach.
            return false;
        }

        if(!image)
        {
            Exception ex("Unable to find and load an image.");
            ex.print(); //dont' throw! I need to warn the user, bu throwing will cause memory leach.
            return false;
        }

        const unsigned int X = priv::G::WorkAreaX;
        const unsigned int Y = priv::G::WorkAreaY;
        const unsigned int dx = bx - ax;
        const unsigned int dy = by - ay;

        irr::video::IImage * target =
            driver->createImage( irr::video::ECF_A8R8G8B8 ,
                                 irr::core::dimension2d<u32>( dx, dy));

        image->copyToScaling(target->lock(), dx, dy, video::ECF_A8R8G8B8);
        target->unlock();

        irr::video::IImage * target2 =
            driver->createImage( irr::video::ECF_A8R8G8B8 ,
                                 irr::core::dimension2d<u32>( X, Y));

        target2->fill(priv::G::BGColor);
        target->copyTo(target2, irr::core::position2di(ax,Y-by) );

        Impl->channelize(target2);

        image->drop();
        target2->drop();
        target->drop();

        return true;
    }

    bool Image::save(const char* filename)
    {
        if(priv::G::AutoClamp)
            clamp();

        irr::video::IVideoDriver *driver= priv::G::GlobalDevice->getVideoDriver();

        const unsigned int X = priv::G::WorkAreaX;
        const unsigned int Y = priv::G::WorkAreaY;

        irr::video::IImage * image = driver->createImage( video::ECF_A8R8G8B8 , core::dimension2d<u32>( X, Y ) );

        if(!image)
        {
            return false;
        }

        Impl->rasterize(image);

        bool flag=driver->writeImageToFile(image,filename);
        if(!flag)
        {
            Exception ex("Unable to save the image.");
            ex.print(); //dont' throw! I need to warn the user, bu throwing will cause memory leach.
        }
        image->drop();
        return flag;
    }

    void Image::setGreen(const Channel& green)
    {
        Impl->G = green;
    }

    void Image::setBlue(const Channel& blue)
    {
        Impl->B = blue;
    }

    void Image::setRed(const Channel& red)
    {
        Impl->R = red;
    }

    void Image::setAlpha(const Channel& alpha)
    {
        Impl->A = alpha;
    }

    Channel Image::getGreen() const
    {
        return Impl->G;
    }

    Channel Image::getBlue() const
    {
        return Impl->B;
    }

    Channel Image::getRed() const
    {
        return Impl->R;
    }

    Channel Image::getAlpha() const
    {
        return Impl->A;
    }

    Channel Image::getLuminance() const
    {
        Channel temp;
        float * p = temp.getData();

        const float * r = Impl->R.getConstData();
        const float * g = Impl->G.getConstData();
        const float * b = Impl->B.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = r[i]*0.299 + g[i]*0.587 + b[i]*0.114;
        }

        return temp;
    }

    Channel Image::getAverage() const
    {
        Channel temp;
        float * p = temp.getData();

        const float * r = Impl->R.getConstData();
        const float * g = Impl->G.getConstData();
        const float * b = Impl->B.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            p[i] = (r[i] + g[i] + b[i])/3.0; //maximum precision. Multiply by (1/3) cause precision loss and don't optimize so much the code
        }

        return temp;
    }

    Channel Image::getLightness() const
    {
        Channel temp;
        float * p = temp.getData();

        const float * r = Impl->R.getConstData();
        const float * g = Impl->G.getConstData();
        const float * b = Impl->B.getConstData();

        for(unsigned int i=0; i<priv::G::ChannelSize; ++i)
        {
            const f32 max1 = r[i]>b[i]? r[i]: b[i];
            const f32 max2 = max1>g[i]? max1: g[i];
            const f32 min1 = r[i]>b[i]? b[i]: r[i];
            const f32 min2 = min1>g[i]? g[i]: min1;
            p[i] = (max2+min2)/2.0;
        }

        return temp;
    }


    void Image::setRGB(const Channel& red, const Channel& green, const Channel& blue)
    {
        Impl->R = red; Impl->G = green; Impl->B = blue;
    }

    void Image::setRGBA(const Channel& red, const Channel& green, const Channel& blue, const Channel& alpha)
    {
        Impl->R = red; Impl->G = green; Impl->B = blue; Impl->A = alpha;
    }

    void Image::getRGB(Channel& red, Channel& green, Channel& blue) const
    {
        red = Impl->R; green = Impl->G; blue = Impl->B;
    }

    void Image::getRGBA(Channel& red, Channel& green, Channel& blue, Channel& alpha) const
    {
        red = Impl->R; green = Impl->G; blue = Impl->B; alpha = Impl->A;
    }

    void Image::clamp()
    {
        Impl->R.clamp();
        Impl->G.clamp();
        Impl->B.clamp();
    }

} // end of namespace
