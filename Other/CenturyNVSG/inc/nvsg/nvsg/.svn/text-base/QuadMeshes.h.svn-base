// Copyright NVIDIA Corporation 2002-2004
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
#include "nvsg/MeshedPrimitiveSet.h"

namespace nvsg
{  
  /*! \brief MeshedPrimitiveSet for quad meshes.
   *  \par Namespace: nvsg
   *  \remarks A QuadMeshes is a MeshedPrimitiveSet that interprets the vertex attributes and the
   *  vector of IndexMesh objects of a MeshedPrimitiveSet as a mesh of quads.
   *  \sa IndependentPrimitiveSet, StrippedPrimitiveSet */
  class QuadMeshes : public MeshedPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a QuadMeshes.
       *  \remarks After instantiation the QuadMeshes initially is empty. */
      NVSG_API QuadMeshes();

      /*! \brief Constructs a QuadMeshes from a MeshedPrimitiveSet.
       *  \param rhs Source MeshedPrimitiveSet.
       *  \remarks After instantiation the QuadMeshes initially is empty. */
      NVSG_API QuadMeshes( const MeshedPrimitiveSet &rhs );

      /*! \brief Copy-Constructs a QuadMeshes from another QuadMeshes.
       *  \param rhs Source QuadMeshes. */
      NVSG_API QuadMeshes( const QuadMeshes &rhs );

      /*! \brief Destructs a QuadMeshes. */
      NVSG_API virtual ~QuadMeshes();

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant QuadMeshes to copy from.
       *  \return A reference to the assigned QuadMeshes.
       *  \remarks The assignment operator calls the assignment operator of MeshedPrimitiveSet. */
      NVSG_API QuadMeshes & operator=( const QuadMeshes & rhs );
  };

  /*! \relates QuadMeshes
   *  \brief Generate tangents and binormals for a QuadMeshes.
   *  \param qm A pointer to the constant QuadMeshes to create tangents and binormals for.
   *  \param tu The optional texture unit that holds texture coordinates to use, default is 0.
   *  \param tg The optional texture unit that is going to hold the tangents, default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals, default is 2.
   *  \remarks This routine calculates tangents and binormals for every vertex in the QuadMeshes
   *  \a qm. The tangent points along the second component of the texture parameterization. The
   *  binormal is calculated by the cross product of the normal and the tangent at that vertex.
   *  \note The behavior is undefined if the QuadMeshes has no normals. */
  NVSG_API void calculateTangentSpace( QuadMeshes * qm
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2 );

  /*! \relates QuadMeshes
   *  \brief Generate normals for a QuadMeshes.
   *  \param qm A pointer to the constant QuadMeshes to create normals for.
   *  \sa calculateTangentSpace */
  NVSG_API void generateNormals( QuadMeshes * qm );

}
