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
  /*! \brief StrippedPrimitiveSet for quad strips.
   *  \par Namespace: nvsg
   *  \remarks A QuadStrips is a StrippedPrimitiveSet that represents the vertex attributes of the
   *  StrippedPrimitiveSet as quad strips.\n
   *  \par Example
   *  If you have an array of eight vertices in the StrippedPrimitiveSet, one possible method for making
   *  them a cube is to define three IndexSet elements as follows:
   *  \code
   *    set[0] = { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };
   *    set[1] = { 1, 7, 3, 5 };
   *    set[2] = { 6, 0, 4, 2 };
   *  \endcode
   *  The ordering ensures that the quads are all drawn with the same orientation so that the quad
   *  strip correctly forms part of a surface. Preserving the orientation is important for some
   *  operations, such as culling. The number of vertices per IndexSet must be at least 4 for
   *  anything to be drawn.
   *  \sa LineStrips, StrippedPrimitiveSet, TriangleStrips */
  class QuadStrips : public StrippedPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a QuadStrips. */
      NVSG_API QuadStrips();

      /*! \brief Constructs a QuadStrips from a StrippedPrimitiveSet. */
      NVSG_API explicit QuadStrips( const StrippedPrimitiveSet &rhs );

      /*! \brief Constructs a QuadStrips as a copy from another QuadStrips. */
      NVSG_API explicit QuadStrips( const QuadStrips &rhs );

      /*! \brief Destructs a QuadStrips. */
      NVSG_API virtual ~QuadStrips();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant QuadStrips to copy from.
       *  \return A reference to the assigned QuadStrips.
       *  \remarks The assignment operator calls the assignment operator of StrippedPrimitiveSet. */
      NVSG_API QuadStrips & operator=(const QuadStrips & rhs);
  };

  /*! \relates QuadStrips
   *  \brief Generate tangents and binormals for a QuadStrips.
   *  \param qs A pointer to the QuadStrips to create tangents and binormals for.
   *  \param tg The optional texture unit that is going to hold the tangents; default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals; default is 2.
   *  \remarks This routine calculates tangents and binormals for every vertex in the QuadStrips
   *  \a qs. The tangent points along the second component of the texture parameterization. The
   *  binormal is calculated by the cross product of the normal and the tangent at that vertex. */
  NVSG_API void calculateTangentSpace( QuadStrips * qs
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2 );

  /*! \relates QuadStrips
   *  \brief Generate normals for a QuadStrips.
   *  \param q A pointer to the QuadStrips to create normals for.
   *  \sa calculateTangentSpace */
  NVSG_API void generateNormals( QuadStrips * q );

}
