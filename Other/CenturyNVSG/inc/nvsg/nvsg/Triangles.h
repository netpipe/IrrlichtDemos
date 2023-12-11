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
  /*! \brief IndependentPrimitiveSet for triangles.
   *  \par Namespace: nvsg
   *  \remarks A Triangles is an IndependentPrimitiveSet that holds an array of index-triples.
   *  Each of these triples describe a triangle with the indices pointing in the array of
   *  vertices. All vertex attributes (like normal, texture,...) are indexed with the same index.
   *  \sa IndependentPrimitiveSet, Points, Lines, Quads */
  class Triangles : public IndependentPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a Triangles. */
      NVSG_API Triangles();

      /*! \brief Constructs a Triangles from an IndependentPrimitiveSet. */
      NVSG_API Triangles( const IndependentPrimitiveSet &rhs );

      /*! \brief Constructs a Triangles from a PrimitiveSet. */
      NVSG_API Triangles( const PrimitiveSet &rhs );

      /*! \brief Constructs a Triangles as a copy from another Triangles. */
      NVSG_API Triangles( const Triangles &rhs );

      /*! \brief Destructs a Triangles. */
      NVSG_API virtual ~Triangles();

      /*! \brief Query if this Triangles has faces.
       *  \return \c true, if this Triangles has at least three indices, otherwise \c false.
       *  \remarks Each triple of successive indices represent one triangular face.
       *  \sa getNumberOfFaces, getFaces, setFaces */
      NVSG_API bool hasFaces() const;

      /*! \brief Get the number of triangular faces.
       *  \return The number of indices of this Triangles divided by three.
       *  \remarks Each triple of successive indices represent one triangular face.
       *  \sa hasFaces, getFaces, setFaces */
      NVSG_API size_t getNumberOfFaces() const;

      /*! \brief Get a pointer to the faces.
       *  \return A pointer to the constant indices of this IndependentPrimitiveSet.
       *  \remarks Each triple of successive indices represent one triangular face.
       *  \sa hasFaces, getNumberOfFaces, setFaces */
      NVSG_API const Face3 * getFaces() const;

      /*! \brief Set the faces of this Triangles.
       *  \param faces A pointer to the constant faces to use.
       *  \param numFaces The number of faces, with three indices per face.
       *  \remarks Copies three times \a numFaces indices from \a faces into this Triangles by
       *  just calling the appropriate setIndices.
       *  \note The behavior is undefined if there are less than \a numFaces values of type
       *  nvsg::Face3 located at \a faces.
       *  \sa hasFaces, getNumberOfFaces, getFaces */
      NVSG_API void setFaces( const Face3 * faces
                            , size_t numFaces );

      /*! \brief Set a subset of faces of this Triangles.
       *  \param pos The start position inside the range of pre-existing faces.
       *  \param faces A pointer to the constant faces to use.
       *  \param numFaces The number of faces.
       *  \remarks Copies three times \a numFaces indices from \a faces into this Triangles,
       *  starting at position three times \a pos inside the range of pre-existing indices.\n
       *  Pre-existing indices in the range [3*\a pos, 3*(\a pos + \a numFaces)) will be replaced.
       *  Pre-existing indices outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of faces currently
       *  stored, the faces pointed to by \a faces will be appended to the pre-existing faces.\n
       *  \note The behavior is undefined if \a pos is neither a valid position inside the range
       *  of pre-existing faces, nor the number of faces currently stored, nor (-1).
       *  \note The behaviour is undefined if there are less than \a numFaces values of type
       *  Face3 located at \a segments.
       *  \sa hasFaces, getNumberOfFaces, getFaces */
      NVSG_API void setFaces( size_t pos
                            , const Face3 * faces
                            , size_t numFaces );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Triangles to copy from.
       *  \return A reference to the assigned Triangles.
       *  \remarks The assignment operator calls the assignment operator of 
       *  IndependentPrimitiveSet. */
      NVSG_API Triangles & operator=(const Triangles & rhs);

  };

  /*! \relates Triangles
   *  \brief Generate tangents and binormals for a Triangles.
   *  \param t A pointer to the constant Triangles to create tangents and binormals for.
   *  \param tu The optional texture unit that holds texture coordinates to use; default is 0.
   *  \param tg The optional texture unit that is going to hold the tangents; default is 1.
   *  \param bn The optional texture unit that is going to hold the binormals; default is 2.
   *  \remarks This routine calculates tangents and binormals for every vertex in the Triangles \a
   *  t. The tangent points along the second component of the texture parameterization. The
   *  binormal is calculated by the cross product of the normal and the tangent at that vertex. */
  NVSG_API void calculateTangentSpace( Triangles * t
                                     , size_t tu = 0
                                     , size_t tg = 1
                                     , size_t bn = 2 );

  /*! \relates Triangles
   *  \brief Generate normals for a Triangles.
   *  \param q A pointer to the constant Triangles to create normals for.
   *  \sa calculateTangentSpace */
  NVSG_API void generateNormals( Triangles * t );

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline bool Triangles::hasFaces()  const 
  { 
    NVSG_TRACE();
    return( 3 <= getNumberOfIndices() );
  }

  inline size_t Triangles::getNumberOfFaces()  const
  { 
    NVSG_TRACE();
    return( getNumberOfIndices() / 3 );
  }

  inline const Face3 * Triangles::getFaces() const 
  { 
    NVSG_TRACE();
    return( (const Face3 *) getIndices() );
  }

  inline void Triangles::setFaces( const Face3* pFaces, size_t numFaces )
  {
    NVSG_TRACE();
    setIndices( (const unsigned int *) pFaces, 3 * numFaces );
  }

  inline void Triangles::setFaces( size_t pos, const Face3* pFaces, size_t numFaces )
  {
    NVSG_TRACE();
    if ( (size_t)-1 != pos )
    {
      pos *= 3;
    }
    setIndices( pos, (const unsigned int *) pFaces, 3 * numFaces );
  }
}
