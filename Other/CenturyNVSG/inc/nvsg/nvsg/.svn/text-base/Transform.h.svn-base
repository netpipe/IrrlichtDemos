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

#include "nvmath/Trafo.h"
#include "nvsg/Group.h"

namespace nvsg
{
  /*! \brief Transform node. 
   *  \par Namespace: nvsg
   *  \remarks A Transform is a special Group node. In addition to the children held by a
   *  Transform like a Group, it contains a transformation that is appended to the current
   *  transformation. The children then are positioned relative to this transformation.
   *  \sa Group */
  class Transform : public Group
  {
    public:
      /*! \brief Default-constructs a Transform. 
       */
      NVSG_API Transform(void);

      /*! \brief Constructs a Transform from a Group. 
      * \param
      * rhs Group to construct the Transform from.
      * \remarks
      * This is kind of a partial copy constructor only copying the Group part from the source 
      * object.
      */
      NVSG_API Transform( const Group &rhs );

      /*! \brief Constructs a Transform as a copy of another Transform.
      */
      NVSG_API Transform( const Transform &rhs );

      /*! \brief Destructs a Transform.
       */
      NVSG_API virtual ~Transform(void);

      /*! \brief Get the transformation represented by this Transform.
       *  \return A reference to the constant nvmath::Trafo, representing the transformation.
       *  \sa nvmath::Trafo, setTrafo */
      NVSG_API const nvmath::Trafo & getTrafo( void ) const;

      /*! \brief Set the transformation for this Transform.
       *  \param trafo A reference to the constant nvmath::Trafo to use.
       *  \remarks The transformation \a trafo is copied to the internal one, and the bounding
       *  sphere of the Transform is invalidated.
       *  \sa nvmath::Trafo, getTrafo */
      NVSG_API virtual void setTrafo( const nvmath::Trafo & trafo );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Transform to copy from
       *  \return A reference to the assigned Transform
       *  \remarks The assignment operator calls the assignment operator of Group, and copies the
       *  transformation.
       *  \sa Group, nvmath::Trafo */
      NVSG_API Transform & operator=(const Transform & rhs);
      
      /*! \brief Test for equivalence with an other Transform. 
       *  \param p A reference to the constant Transform to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the Transform \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as a Group, they are equivalent if they have
       *  the same transformation.
       *  \sa Group, nvmath::Trafo */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
        /*! \brief Calculate the bounding sphere of this Transform.
       *  \return A nvmath::Sphere3f that contains the complete Transform.
       *  \remarks This function is called by the framework to determine a sphere that completely
       *  contains the Transform. First, the bounding sphere of the Group is determined, then
       *  this sphere is transformed according to the transformation. */
      NVSG_API virtual nvmath::Sphere3f calculateBoundingSphere( void ) const;

    private:
      nvmath::Trafo m_trafo;
  };

  inline const nvmath::Trafo & Transform::getTrafo( void ) const
  {
    return( m_trafo );
  }
}
