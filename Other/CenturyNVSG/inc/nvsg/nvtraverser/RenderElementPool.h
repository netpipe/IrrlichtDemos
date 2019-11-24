// Copyright NVIDIA Corporation 2002-2006
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once

namespace nvtraverser
{

class RenderElementPool
{
public:
  RenderElementPool();
  ~RenderElementPool();  

  void * alloc(size_t size); // allocates size bytes of memory and returns the address to the allocated memory
  void reset(); // resets the running pointer for each allocated block

private:
  struct Block
  {
    Block();    
    ~Block();

    enum { BLOCKSIZE = 0x400000 };
    unsigned char * m_start;   // marks the beginning of the raw memory block
    unsigned char * m_end;     // points past the end of the memory block
    unsigned char * m_current; // points to next available free space
    Block * m_next;
  };
  Block * m_head; // head of block list
  Block * m_tail; // always points to the current block
};

template <typename REType>
inline REType * constructRenderElement(RenderElementPool * pool)
{
  return new(pool->alloc(sizeof(REType))) REType;
}

template <typename REType, typename Arg>
inline REType * constructRenderElement(RenderElementPool * pool, Arg arg)
{
  return new(pool->alloc(sizeof(REType))) REType(arg);
}

template <typename REType, typename Arg0, typename Arg1>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1);
}

template <typename REType, typename Arg0, typename Arg1, typename Arg2>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1, Arg2 arg2)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1, arg2);
}

template <typename REType, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1, arg2, arg3);
}

template <typename REType, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1, arg2, arg3, arg4);
}

template <typename REType, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1, arg2, arg3, arg4, arg5);
}

template <typename REType, typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
inline REType * constructRenderElement(RenderElementPool * pool, Arg0 arg0, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6)
{
  return new(pool->alloc(sizeof(REType))) REType(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
}


}