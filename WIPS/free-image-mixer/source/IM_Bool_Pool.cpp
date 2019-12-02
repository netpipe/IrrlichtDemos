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
#include "IM_Bool_Pool.h"
#include <IM_Globals.h>
#include <list>


namespace mixer
{
namespace priv
{

    std::list<bool *> maskPool;
    unsigned int BoolPool::PoolMaxSize = priv::G::PoolMaxSize; //should be enough for most applications.

    bool * BoolPool::get()
    {
        if(maskPool.empty())
        {
            for(unsigned int i = 0; i <PoolMaxSize; ++i)
                maskPool.push_back(new bool[G::ChannelSize]);
        }

        bool * p = maskPool.front();
        maskPool.pop_front();

        return p;
    }

    void BoolPool::free(bool * buffer)
    {
        maskPool.push_back(buffer);

        if(maskPool.size()>(PoolMaxSize*2))
            while(maskPool.size()>PoolMaxSize)
            {
                bool * p = maskPool.front();
                maskPool.pop_front();
                delete [] p;
            }
    }

    void BoolPool::finalize()
    {
        if(maskPool.empty())
        {
            return;
        }

        while(!maskPool.empty())
        {
            bool * p = maskPool.front();
            maskPool.pop_front();
            delete [] p;
        }
    }

} // end of namespace priv
} // end of namespace mixer
