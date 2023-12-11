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
#include "nvsg/Drawable.h"
#include "nvsg/PrimitiveSet.h"

namespace nvsg
{
  /*! \brief Abstract base class for shaped geometries like Boxes, Spheres, etc.
   *  \par Namespace: nvsg
   *  \remarks This class provides the common interface for shaped geometries, holding zero or
   *  more PrimitiveSet objects.
   *  \sa Drawable, PrimitiveSet */
  class Shape : public Drawable
  {
    public:
      /*! \brief Destructs a Shape. */
      NVSG_API virtual ~Shape();

      /*! \brief Get the number of PrimitiveSet objects held by this Shape.
       *  \return The number of PrimitiveSet objects.
       *  \sa getPrimitiveSet */
      NVSG_API size_t getNumberOfPrimitiveSets() const;

      /*! \brief Returns the PrimitiveSet at the specified zero-based index.
       *  \param index Zero-based index of the PrimitiveSet to get.
       *  \return A pointer to a PrimitiveSetHandle specifying PrimitiveSet at index \a index.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  PrimitiveSet objects in this Shape.
       *  \sa getNumberOfPrimitiveSets */
      NVSG_API PrimitiveSetHandle * getPrimitiveSet( size_t index ) const;

      /*! \brief Test for equivalence with another Shape.
       *  \param p A reference to the constant Shape to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the Shape \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Drawable, they are equivalent if their
       *  PrimitiveSet objects are pair-wise equivalent. If \a deepCompare is \c false, the
       *  PrimitiveSet objects are considered to be equivalent if they are pairwise the same
       *  pointers; otherwise, a full equivalence test on each pair is initiated.
       *  \sa Drawable, PrimitiveSet */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Protected default constructor to prevent explicit creation.
       *  \remarks This default constructor is accessible only by derived classes. By default, a
       *  Shape holds no PrimitiveSet objects. */
      NVSG_API Shape();

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant Shape to copy from
       *  \remarks This copy constructor is accessible only by derived classes. The new Shape
       *  holds a copy of the PrimitiveSet objects of \a rhs. */
      NVSG_API Shape( const Shape& rhs );

      /*! \brief Adds a PrimitiveSet to this Shape.
       *  \param p A pointer to a PrimitiveSetHandle specifying the PrimitiveSet to add.
       *  \remarks This function is protected to be accessible only by derived classes. Only those
       *  are allowed to modify the PrimitiveSet objects of a Shape.
       *  \sa getNumberOfPrimitives, getPrimitiveSet, removePrimitiveSet */
      NVSG_API void addPrimitiveSet( PrimitiveSetHandle * p );

      /*! \brief Removes the PrimitiveSet at the specified zero-based index from this Shape.
       *  \param index Zero-based index of the PrimitiveSet to remove from this Shape.
       *  \remarks This function is protected to be accessible only by derived classes. 
       *  Only those are allowed to modify the PrimitiveSet objects of a Shape.
       *  \note The behavior is undefined if \a index is larger than or equal to the number of
       *  PrimitiveSet objects in this Shape.
       *  \sa addPrimitiveSet, getNumberOfPrimitiveSets, getPrimitiveSet */
      NVSG_API void removePrimitiveSet( size_t index );

      /*! \brief Calculate the bounding sphere of this Shape.
       *  \return The bounding sphere of this Shape.
       *  \remarks This function is called by the framework to determine the sphere to be used as
       *  the bounding sphere. It combines the bounding spheres of the PrimitiveSet objects of
       *  this Shape. If there are none, an invalid sphere is returned.
       *  \sa nvmath::Sphere3f, PrimitiveSet */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere() const;

      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of all its
       *  PrimitiveSet objects is requested to continue validation down the tree hierarchy.
       *  \sa nvutil::Incarnation, PrimitiveSet */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Shape to copy from.
       *  \return A reference to the assigned Shape.
       *  \remarks This assignment operator is accessible only by derived classes. It calls the
       *  assignment operator of Drawable, decreases the reference count of all its PrimitiveSet
       *  objects, and copies the PrimitiveSet objects from \a rhs. */
      NVSG_API Shape & operator=(const Shape & rhs);

    private:
      typedef std::vector<PrimitiveSetHandle *>::const_iterator ConstPrimitiveSetIterator; 
      typedef std::vector<PrimitiveSetHandle *>::iterator PrimitiveSetIterator; 
      std::vector<PrimitiveSetHandle *> m_primitiveSets;
  };

  inline size_t Shape::getNumberOfPrimitiveSets() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_primitiveSets.size() );
  }

  inline PrimitiveSetHandle * Shape::getPrimitiveSet( size_t i ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    NVSG_ASSERT( i < m_primitiveSets.size() );
    return( m_primitiveSets[i] );
  }

} //  namespace nvsg
