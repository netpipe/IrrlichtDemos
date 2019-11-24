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
#include "nvsg/Face.h"
#include "nvsg/IndependentPrimitiveSet.h"

namespace nvsg
{
  /*! \brief IndependentPrimitiveSet for quads.
   *  \par Namespace: nvsg
   *  \remarks A Quads is an IndependentPrimitiveSet that holds an array of index-quadruples. Each
   *  of these quadruples describe a quad with the indices pointing in the array of vertices. All
   *  vertex attributes (such as normal, texture,...) are indexed with the same index.
   *  \sa IndependentPrimitiveSet, Points, Lines, Triangles */
  class Quads : public IndependentPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a Quads. */
      NVSG_API Quads();

      /*! \brief Constructs a Quads from a PrimitiveSet. */
      NVSG_API Quads( const PrimitiveSet &rhs );

      /*! \brief Constructs a Quads from an IndependentPrimitiveSet. */
      NVSG_API Quads( const IndependentPrimitiveSet &rhs );

      /*! \brief Constructs a Quads as a copy from another Quads. */
      NVSG_API Quads( const Quads &rhs );

      /*! \brief Destructs a Quads. */
      NVSG_API virtual ~Quads();

      /*! \brief Query if this Quads has faces.
       *  \return \c true, if this Quads has at least four indices, otherwise \c false.
       *  \remarks Each quadruple of successive indices represent one quadrangular face.
       *  \sa getNumberOfFaces, getFaces, setFaces */
      NVSG_API bool hasFaces() const;

      /*! \brief Get the number of quadrangular faces.
       *  \return The number of indices of this Quads divided by four.
       *  \remarks Each quadruple of successive indices represent one quadrangular face.
       *  \sa hasFaces, getFaces, setFaces */
      NVSG_API size_t getNumberOfFaces() const;

      /*! \brief Get a pointer to the faces.
       *  \return A pointer to the constant indices of this IndependentPrimitiveSet.
       *  \remarks Each quadruple of successive indices represent one quadrangular face.
       *  \sa hasFaces, getNumberOfFaces, setFaces */
      NVSG_API const Face4 * getFaces() const;

      /*! \brief Set the faces of this Quads.
       *  \param faces A pointer to the constant faces to use.
       *  \param numFaces The number of faces, with four indices per face.
       *  \remarks Copies four times \a numFaces indices from \a faces into this Quads by just
       *  calling the appropriate setIndices.
       *  \note The behavior is undefined if there are less than \a numFaces values of type
       *  nvsg::Face4 located at \a faces.
       *  \sa hasFaces, getNumberOfFaces, getFaces */
      NVSG_API void setFaces( const Face4 * faces
                            , size_t numFaces );

      /*! \brief Set a subset of faces of this Quads.
       *  \param pos The start position inside the range of pre-existing faces.
       *  \param faces A pointer to the constant faces to use.
       *  \param numFaces The number of faces.
       *  \remarks Copies four times \a numFaces indices from \a faces into this Quads, starting
       *  at position four times \a pos inside the range of pre-existing indices.\n
       *  Pre-existing indices in the range [4*\a pos, 4*(\a pos + \a numFaces)) will be replaced.
       *  Pre-existing indices outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of faces currently
       *  stored, the faces pointed to by \a faces will be appended to the pre-existing faces.\n
       *  \note The behavior is undefined if \a pos is neither a valid position inside the range
       *  of pre-existing faces, nor the number of faces currently stored, nor (-1).
       *  \note The behavior is undefined if there are less than \a numFaces values of type
       *  Face4 located at \a segments.
       *  \sa hasFaces, getNumberOfFaces, getFaces */
      NVSG_API void setFaces( size_t pos
                            , const Face4 * faces
                            , size_t numFaces );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Quads to copy from.
       *  \return A reference to the assigned Quads.
       *  \remarks The assignment operator calls the assignment operator of
       *  IndependentPrimitiveSet. */
      NVSG_API Quads & operator=( const Quads & rhs );
  };

  /*! \relates Quads
   *  \brief Generate tangents and binormals for a Quads.
   *  \param q A pointer to the constant Quads to create tangents and binormals for.
   *  \param tu The optional texture unit that holds texture coordinates to use; default is 0.
   *  \param tg The optional texture unit that is going to hold the tangents; default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals; default is 2.
   *  \remarks This routine calculates tangents and binormals for every vertex in the Quads \a q.
   *  The tangent points along the second component of the texture parameterization. The binormal
   *  is calculated by the cross product of the normal and the tangent at that vertex. */
  NVSG_API void calculateTangentSpace( Quads * q
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2 );

  /*! \relates Quads
   *  \brief Generate normals for a Quads.
   *  \param q A pointer to the constant Quads to create normals for.
   *  \sa calculateTangentSpace */
  NVSG_API void generateNormals( Quads * q );

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline bool Quads::hasFaces() const
  { 
    NVSG_TRACE();
    return( 4 <= getNumberOfIndices() );
  }

  inline size_t Quads::getNumberOfFaces() const
  { 
    NVSG_TRACE();
    return( getNumberOfIndices() / 4 );
  }

  inline const Face4 * Quads::getFaces() const 
  { 
    NVSG_TRACE();
    return( (const Face4 *) getIndices() );
  }

  inline void Quads::setFaces( const Face4 * pFaces, size_t numFaces )
  {
    NVSG_TRACE();
    setIndices( (const unsigned int *) pFaces, 4 * numFaces );
  }

  inline void Quads::setFaces( size_t pos, const Face4* pFaces, size_t numFaces )
  {
    NVSG_TRACE();
    if ( (size_t)-1 != pos )
    {
      pos *= 4;
    }
    setIndices( pos, (const unsigned int *) pFaces, 4 * numFaces );
  }
}
