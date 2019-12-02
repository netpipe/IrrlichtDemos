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
#include "IM_Channel_Pool.h"
#include <IM_Globals.h>
#include <list>


namespace mixer
{
namespace priv
{

    std::list<float *> chanPool;
    unsigned int ChannelPool::PoolMaxSize = priv::G::PoolMaxSize; //should be enough for most applications.

    float * ChannelPool::get()
    {
        if(chanPool.empty())
        {
            for(unsigned int i = 0; i <PoolMaxSize; ++i)
                chanPool.push_back(new float[G::ChannelSize]);
        }

        float * p = chanPool.front();
        chanPool.pop_front();

        return p;
    }

    void ChannelPool::free(float * buffer)
    {
        chanPool.push_back(buffer);

        if(chanPool.size()>(PoolMaxSize*2))
            while(chanPool.size()>PoolMaxSize)
            {
                float * p = chanPool.front();
                chanPool.pop_front();
                delete [] p;
            }
    }

    void ChannelPool::finalize()
    {
        if(chanPool.empty())
        {
            return;
        }

        while(!chanPool.empty())
        {
            float * p = chanPool.front();
            chanPool.pop_front();
            delete [] p;
        }
    }

} // end of namespace priv
} // end of namespace mixer
