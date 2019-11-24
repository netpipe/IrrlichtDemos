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
#include "nvsg/Face.h" // definition of Segment

namespace nvsg
{  
  /*! \brief IndependentPrimitiveSet for lines.
   *  \par Namespace: nvsg
   *  \remarks A Lines is an IndependentPrimitiveSet that holds an array of index-pairs. Each of
   *  these pairs describe a line with the indices referencing into the array of vertices. All
   *  vertex attributes (like normal, texture,...) are indexed with the same index.
   *  \sa IndependentPrimitiveSet, Points, Quads, Triangles */
  class Lines : public IndependentPrimitiveSet
  {
    public:
      /*! \brief Default-constructs a Lines. */
      NVSG_API Lines();

      /*! \brief Constructs a Lines from an IndependentPrimitiveSet. */
      NVSG_API explicit Lines( const IndependentPrimitiveSet &rhs );

      /*! \brief Constructs a Lines from a PrimitiveSet. */
      NVSG_API explicit Lines( const PrimitiveSet &rhs );

      /*! \brief Constructs a Lines as a copy from another Lines. */
      NVSG_API Lines( const Lines &rhs );

      /*! \brief Destructs a Lines. */
      NVSG_API virtual ~Lines();

      /*! \brief Query if this Lines has segments.
       *  \return \c true, if this Lines has at least two indices, otherwise \c false.
       *  \remarks Each pair of successive indices represent one line segment.
       *  \sa getNumberOfSegments, getSegments, hasIndices, setSegments */
      NVSG_API bool hasSegments() const;

      /*! \brief Get the number of line segments.
       *  \return The number of indices of this Lines divided by two.
       *  \remarks Each pair of successive indices represent one line segment.
       *  \sa hasSegments, getSegments, setSegments */
      NVSG_API size_t getNumberOfSegments() const;

      /*! \brief Get a pointer to the segments.
       *  \return A pointer to the constant indices of this IndependentPrimitiveSet.
       *  \remarks Each pair of successive indices represent one line segment.
       *  \sa hasSegments, getNumberOfSegments, setSegments */
      NVSG_API const Segment * getSegments() const;

      /*! \brief Set the segments of this Lines.
       *  \param segments A pointer to the constant segments to use.
       *  \param numSegments The number of segments, with two indices per segment.
       *  \remarks Copies 2x \a numSegments indices from \a segments into this
       *  Lines by just calling the appropriate setIndices.
       *  \note The behavior is undefined if there are less than \a numSegments values of type
       *  Segment located at \a segments.
       *  \sa hasIndices, getNumberOfIndices, getIndices */
      NVSG_API void setSegments( const Segment * segments
                               , size_t numSegments );

      /*! \brief Set a subset of segments of this Lines.
       *  \param pos The start position inside the range of pre-existing segments.
       *  \param segments A pointer to the constant segments to use.
       *  \param numSegments The number of segments.
       *  \remarks Copies 2x \a numSegments indices from \a segments into this Lines,
       *  starting at position 2x \a pos inside the range of pre-existing indices.\n
       *  Pre-existing indices in the range [2*\a pos, 2*(\a pos + \a numSegments)) will be
       *  replaced. Pre-existing indices outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of segments currently
       *  stored, the segments pointed to by \a segments will be appended to the pre-existing
       *  segments.\n
       *  \note The behavior is undefined if \a pos is neither a valid position inside the range
       *  of pre-existing segments, nor the number of segments currently stored, nor (-1).
       *  \note The behavior is undefined if there are less than \a numSegments values of type
       *  Segment located at \a segments.
       *  \sa hasSegments, getNumberOfSegments, getSegments */
      NVSG_API void setSegments( size_t pos
                               , const Segment * segments
                               , size_t numSegments );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Lines to copy from.
       *  \return A reference to the assigned Lines.
       *  \remarks The assignment operator calls the assignment operator of
       *  IndependentPrimitiveSet. */
      NVSG_API Lines & operator=(const Lines & rhs);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline bool Lines::hasSegments()  const 
  {
    NVSG_TRACE();
    return( 2 <= getNumberOfIndices() );
  }

  inline size_t Lines::getNumberOfSegments()  const
  { 
    NVSG_TRACE();
    return( getNumberOfIndices() / 2 );
  }

  inline const Segment * Lines::getSegments() const 
  {
    NVSG_TRACE();
    return( (const Segment *) getIndices() );
  }

  inline void Lines::setSegments( const Segment* pSegments, size_t numSegments )
  {
    NVSG_TRACE();
    setIndices( (const unsigned int *) pSegments, 2 * numSegments );
  }

  inline void Lines::setSegments( size_t pos, const Segment* pSegments, size_t numSegments )
  {
    NVSG_TRACE();
    if ( (size_t)-1 != pos )
    {
      pos *= 2;
    }
    setIndices( pos, (const unsigned int *) pSegments, 2 * numSegments );
  }

  inline Lines & Lines::operator=(const Lines & rhs)
  {
    NVSG_TRACE();
    IndependentPrimitiveSet::operator=(rhs);
    return( *this );
  }
}
