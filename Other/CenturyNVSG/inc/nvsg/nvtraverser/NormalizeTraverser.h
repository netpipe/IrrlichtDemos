// Copyright NVIDIA Corporation 2002-2005
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
/** \file */

#include <set>
#include "nvsgcommon.h"
#include "nvtraverser/Traverser.h"

namespace nvtraverser
{  

  //! Traverser that normalizes all normals.
  class NormalizeTraverser : public SharedTraverser
  {
    public:
      //! Constructor
      NVSG_API NormalizeTraverser(void);

      //! Get the index of the VertexAttributeSet that is to be normalized.
      /** The default index for normalizing is NORMAL. */
      NVSG_API size_t getVertexAttributeIndex() const;

      //! Set the index of the VertexAttributeSet that is to be normalized.
      /** The default index for normalizing is NORMAL. */
      NVSG_API void setVertexAttributeIndex( size_t attrib );

    protected:
      //! Protected destructor to prevent instantiation of a NormalizeTraverser on stack.
      NVSG_API virtual ~NormalizeTraverser(void);

      //! Initiate traversal of the scene.
      NVSG_API virtual void  doApply( nvsg::NodeHandle * root );

      //! Unify all VertexAttributeSets.
      void  handleVertexAttributeSet( nvsg::VertexAttributeSet * p );

    private:
      template<unsigned char dim>
        void  normalizeData( nvsg::VertexAttributeSet * p );

    private:
      size_t                                m_attrib;
      std::set<nvsg::VertexAttributeSet *>  m_multiOwnedVAS;
  };

  inline size_t NormalizeTraverser::getVertexAttributeIndex() const
  {
    return( m_attrib );
  }

  inline void NormalizeTraverser::setVertexAttributeIndex( size_t attrib )
  {
    m_attrib = attrib;
  }
}
