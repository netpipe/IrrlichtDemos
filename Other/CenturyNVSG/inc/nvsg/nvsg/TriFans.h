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
#include "nvsg/nvsg.h"
#include "nvsg/StrippedPrimitiveSet.h"

namespace nvsg
{
  /*! \brief StrippedPrimitiveSet for quad strips.
   *  \par Namespace: nvsg
   *  \remarks A TriFans is a StrippedPrimitiveSet that represents the vertex attributes of the
   *  StrippedPrimitiveSet as triangle fans.\n
   *  The number of vertices per IndexSet must be at least 3 for anything to be drawn.
   *  \sa LineStrips, StrippedPrimitiveSet, TriStrips */
  class TriFans : public StrippedPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a TriFans. */
      NVSG_API TriFans();

      /*! \brief Constructs a TriFans from a StrippedPrimitiveSet. */
      NVSG_API explicit TriFans( const StrippedPrimitiveSet &rhs );

      /*! \brief Constructs a TriFans as a copy from another TriFans. */
      NVSG_API explicit TriFans( const TriFans &rhs );

      /*! \brief Destructs a TriFans. */
      NVSG_API virtual ~TriFans();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant TriFans to copy from.
       *  \return A reference to the assigned TriFans.
       *  \remarks The assignment operator calls the assignment operator of StrippedPrimitiveSet. */
      NVSG_API TriFans & operator=(const TriFans & rhs);
  };

  /*! \relates TriFans
   *  \brief Generate tangents and binormals for a TriFans.
   *  \param tf A pointer to the TriFans to create tangents and binormals for.
   *  \param tu The optional texture unit that holds texture coordinates to use; default is 0.
   *  \param tg The optional texture unit that is going to hold the tangents; default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals; default is 2.
   *  \remarks This routine calculates tangents and binormals for every vertex in the TriFans \a
   *  tf. The tangent points along the second component of the texture parameterization. The
   *  binormal is calculated by the cross product of the normal and the tangent at that vertex. */
  NVSG_API void calculateTangentSpace( TriFans * tf
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2 );

  /*! \relates TriFans
   *  \brief Generate normals for a TriFans.
   *  \param q A pointer to the TriFans to create normals for.
   *  \sa calculateTangentSpace */
  NVSG_API void generateNormals( TriFans * q );

}
