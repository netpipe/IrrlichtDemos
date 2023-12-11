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
#include "nvsg/IndependentPrimitiveSet.h" // base class definition

namespace nvsg
{  
  /*! \brief IndependentPrimitiveSet for points.
   *  \par Namespace: nvsg
   *  \remarks A Points is an IndependentPrimitiveSet that holds an array of indices. Each of
   *  these indices describes a point with the index pointing in the array of vertices. All vertex
   *  attributes (like normal, texture,...) are indexed with the same index.
   *  \sa IndependentPrimitiveSet, Lines, Quads, Triangles */
  class Points : public IndependentPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a Points. */
      NVSG_API Points();

      /*! \brief Constructs a Points from an IndependentPrimitiveSet. */
      NVSG_API explicit Points( const IndependentPrimitiveSet &rhs );

      /*! \brief Constructs a Points from a PrimitiveSet. */
      NVSG_API explicit Points( const PrimitiveSet &rhs );

      /*! \brief Constructs a Points as a copy from another Points. */
      NVSG_API Points( const Points &rhs );

      /*! \brief Destructs a Points. */
      NVSG_API virtual ~Points();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Points to copy from.
       *  \return A reference to the assigned Points.
       *  \remarks The assignment operator calls the assignment operator of
       *  IndependentPrimitiveSet. */
      NVSG_API Points & operator=(const Points & rhs);

  };

}
