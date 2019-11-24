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

#include "nvsgcommon.h"
#include "nvsg/StrippedPrimitiveSet.h"
#include "nvsg/nvsg.h"

namespace nvsg
{
  /*! \brief StrippedPrimitiveSet for line strips.
   *  \par Namespace: nvsg
   *  \remarks A LineStrips is a StrippedPrimitiveSet that holds an array of arrays of indices.
   *  Each of the arrays of indices describe one series of connected line segments. All vertex
   *  attributes (like normal, texture,...) are indexed with the same index.
   *  \sa Lines, StrippedPrimitiveSet, TriangleStrips */
  class LineStrips : public StrippedPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a LineStrips. */
      NVSG_API LineStrips();

      /*! \brief Constructs a LineStrips from a StrippedPrimitiveSet. */
      NVSG_API explicit LineStrips( const StrippedPrimitiveSet &rhs );

      /*! \brief Constructs a LineStrips as a copy from another LineStrips. */
      NVSG_API explicit LineStrips( const LineStrips &rhs );

      /*! \brief Destructs a LineStrips. */
      NVSG_API virtual ~LineStrips();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant LineStrips to copy from.
       *  \return A reference to the assigned LineStrips.
       *  \remarks The assignment operator calls the assignment operator of StrippedPrimitiveSet.*/
      NVSG_API LineStrips & operator=( const LineStrips & rhs );
  };
}
