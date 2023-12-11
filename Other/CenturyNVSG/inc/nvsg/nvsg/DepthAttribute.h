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
#include "nvsg/StateAttribute.h"


namespace nvsg
{
  /*! \brief Enumeration to identify a depth function
   *  \par Namespace: nvsg
   *  \remarks On rendering, an incoming fragment passes the depth test if its z value has the
   *  specified relation to the value already stored in the depth buffer. The default is DF_LESS,
   *  which means that an incoming fragment passes the test if its z value is less than that
   *  already stored in the depth buffer. In this case, the z value represents the distance from
   *  the object to the viewpoint, and smaller values mean the corresponding objects are closer to
   *  the viewpoint.
   *  \sa DepthAttribute */
  typedef enum
  {
    DF_NEVER,
    DF_ALWAYS,
    DF_LESS,
    DF_LESS_OR_EQUAL,
    DF_EQUAL,
    DF_GREATER_OR_EQUAL,
    DF_GREATER,
    DF_NOT_EQUAL
  } DepthFunction;

  /*! \brief StateAttribute that determines the depth settings to use.
   *  \par Namespace: nvsg
   *  \remarks A DepthAttribute defines a nvsg::DepthFunction for the depth test and determines if
   *  the depth buffer is enabled for testing and/or writing.\n
   *  By default, the nvsg::DepthFunction is DF_LESS, the depth buffer is enabled for testing and
   *  writing.
   *  \note In the transparent part of the rendering pass, depth buffer writing is disabled, and
   *  can't be changed by a DepthAttribute.
   *  \sa BlendAttribute, DepthFunction, Material, StateAttribute  */
  class DepthAttribute : public StateAttribute
  {
    public:
      /*! \brief Default-constructs a DepthAttribute. 
       *  \remarks The defaults of a DepthAttribute is DF_LESS for the depth function, depth
       *  buffer write enabled, and a depth range from 0.0 to 1.0. */
      NVSG_API DepthAttribute();

      /*! \brief Constructs a DepthAttribute from a StateAttribute.
       *  \param rhs StateAttribute to construct the DepthAttribute from.
       *  \remarks The DepthAttribute will be initialized with DF_LESS for the depth function, 
       *  depth buffer write enabled, and a depth range from 0.0 to 1.0. */
      NVSG_API explicit DepthAttribute( const StateAttribute &rhs );

      /*! \brief Constructs a DepthAttribute as a copy from another DepthAttribute. */
      NVSG_API DepthAttribute( const DepthAttribute &rhs );

      /*! \brief Destructs a DepthAttribute. */ 
      NVSG_API virtual ~DepthAttribute();

      /*! \brief Get the nvsg::DepthFunction of this DepthAttribute.
       *  \return The nvsg::DepthFunction of this DepthAttribute.
       *  \sa nvsg::DepthFunction, setDepthFunction */
      NVSG_API nvsg::DepthFunction getDepthFunction() const;

      /*! \brief Set the nvsg::DepthFunction of this DepthAttribute.
       *  \param df The nvsg::DepthFunction to use.
       *  \sa nvsg::DepthFunction, getDepthFunction */
      NVSG_API void setDepthFunction( nvsg::DepthFunction df );

      /*! \brief Ask if depth buffer writing is enabled.
       *  \return \c true, if depth buffer writing is enabled, otherwise \c false.
       *  \sa isDepthTestingEnabled, setDepthWritingEnabled */
      NVSG_API bool isDepthWritingEnabled() const;

      /*! \brief Enable or disable depth buffer testing.
       *  \param enabled If \c true, depth buffer testing is enabled, otherwise it is disabled.
       *  \sa isDepthWritingEnabled, setDepthTestingEnabled */
      NVSG_API void setDepthWritingEnabled( bool enabled );

      /*! \brief Ask if depth buffer testing is enabled.
       *  \return \c true, if depth buffer testing is enabled, otherwise \c false.
       *  \sa isDepthWritingEnabled, setDepthTestingEnabled */
      NVSG_API bool isDepthTestingEnabled() const;

      /*! \brief Enable or disable depth buffer testing.
       *  \param enabled If \c true, depth buffer testing is enabled, otherwise it is disabled.
       *  \sa isDepthTestingEnabled, setDepthWritingEnabled */
      NVSG_API void setDepthTestingEnabled( bool enabled );

      /*! \brief Test for equivalence with another DepthAttribute.
       *  \param p            Pointer to the constant DepthAttribute to test for equivalence with.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c true
       *  \param deepCompare  Optional parameter to perform a deep comparsion; default is \c false
       *  \return \c true if the DepthAttribute \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as StateAttribute, they are considered to be
       *  equivalent if they the same nvsg::DepthFunction and the same state of depth buffer
       *  testing and writing.
       *  \sa StateAttribute, getDepthFunction, isDepthTestingEnabled, isDepthWritingEnabled */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant DepthAttribute to copy from
       *  \return A reference to the assigned DepthAttribute
       *  \remarks The assignment operator calls the assignment operator of StateAttribute and
       *  copies the DepthAttribute specific data then.
       *  \sa PrimitiveAttribute */
      NVSG_API DepthAttribute & operator=( const DepthAttribute & rhs );

    private:
      DepthFunction m_depthFunction;
      bool          m_depthTesting;
      bool          m_depthWriting;
  };

  inline DepthFunction DepthAttribute::getDepthFunction() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_depthFunction );
  }

  inline bool DepthAttribute::isDepthTestingEnabled() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_depthTesting );
  }

  inline bool DepthAttribute::isDepthWritingEnabled() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_depthWriting );
  }

} // namespace nvsg
