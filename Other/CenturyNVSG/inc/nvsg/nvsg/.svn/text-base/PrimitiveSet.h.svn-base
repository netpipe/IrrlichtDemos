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
/** @file */

#include "nvsgcommon.h"
#include "nvsg/Drawable.h" // base class definition

namespace nvsg
{
  /*! \brief Base class for Drawable objects holding a VertexAttributeSet.
   *  \par Namespace: nvsg
   *  \remarks This class provides the common interface for Drawable object with a
   *  VertexAttributeSet, like IndependentPrimitiveSet or StrippedPrimitiveSet.
   *  \sa Drawable, IndependentPrimitiveSet, StrippedPrimitiveSet, VertexAttributeSet */
  class PrimitiveSet : public Drawable
  {
    public:
      /*! \brief Destructs a PrimitiveSet */
      NVSG_API virtual ~PrimitiveSet();

      /*! \brief Get a pointer to the constant VertexAttributeSet.
       *  \return A pointer to the constant VertexAttributeSet.
       *  \sa VertexAttributeSet */
      NVSG_API VertexAttributeSetHandle * getVertexAttributeSet() const;

      /*! \brief Set the VertexAttributeSet.
       *  \param set A pointer to the constant VertexAttributeSet.
       *  \remarks The reference count of \a set is incremented, while the reference count of any
       *  previous VertexAttributeSet is decremented.
       *  \sa VertexAttributeSet */
      NVSG_API void setVertexAttributeSet( VertexAttributeSetHandle * set );

      /*! \brief Test for compatibility with an other PrimitiveSet.
       *  \param p A reference to the constant PrimitiveSet to test for compatibility with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \return \c true if the PrimitiveSet \a p is compatible with \c this, otherwise \c false.
       *  \remarks \a p and \c this are compatible, if each of their vertex attributes have the
       *  same dimension, respectively.
       *  \note The behavior is undefined if \a p is not a PrimitiveSet nor derived from one.
       *  \sa Drawable, VertexAttributeSet */
      NVSG_API bool isCompatible( const PrimitiveSet *p
                                , bool ignoreNames = true ) const;

      /*! \brief Test for equivalence with an other PrimitiveSet.
       *  \param p A reference to the constant PrimitiveSet to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the PrimitiveSet \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Drawable, they are equivalent if the
       *  VertexAttributeSet objects are equivalent. If \a deepCompare is \c true, a full
       *  equivalence test is performed on the VertexAttributeSet objects, otherwise they are
       *  considered to be equivalent if the pointers are equal.
       *  \note The behavior is undefined if \a p is not a PrimitiveSet nor derived from one.
       *  \sa Drawable, VertexAttributeSet */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Protected default constructor to prevent explicit creation.
       *  \remarks This default constructor is accessible only by derived classes. By default, it
       *  holds no VertexAttributeSet. */
      NVSG_API PrimitiveSet();

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant PrimitiveSet to copy from
       *  \remarks This copy constructor is accessible only by derived classes. The new
       *  PrimitiveSet holds a copy of the VertexAttributeSet of \a rhs. */
      NVSG_API PrimitiveSet( const PrimitiveSet& rhs );

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of its
       *  VertexAttributeSet is requested to continue validation down the tree hierarchy.
       *  \sa nvutil::Incarnation, VertexAttributeSet */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

      /*! \brief Calculate the bounding sphere of this PrimitiveSet.
       *  \return The bounding sphere of this PrimitiveSet.
       *  \remarks This function is called by the framework to determine the sphere to be used as
       *  the bounding sphere. It calculates the sphere around the vertices of the
       *  VertexAttributeSet. If there are none, an invalid sphere is returned.
       *  \sa nvmath::Sphere3f */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant PrimitiveSet to copy from.
       *  \return A reference to the assigned PrimitiveSet.
       *  \remarks This assignment operator is accessible only by derived classes. It calls the
       *  assignment operator of PrimitiveSet. */
      NVSG_API PrimitiveSet & operator=( const PrimitiveSet & rhs );

    private:
      VertexAttributeSetHandle * m_vertexAttributeSet;
  };

  inline VertexAttributeSetHandle * PrimitiveSet::getVertexAttributeSet() const
  {
    NVSG_TRACE();
    return( m_vertexAttributeSet );
  }

} //  namespace nvsg
