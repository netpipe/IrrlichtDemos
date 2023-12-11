// Copyright NVIDIA Corporation 2002-2006
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
#include "nvmath/Mat44f.h"
#include "nvmath/Quatf.h"

namespace nvmath
{
  //! Transformation class.
  /** This class is used to ease transformation handling. It has an interface to rotate, scale,
    * and translate and can produce a \c Mat44f that combines them.  */
  class Trafo
  {
    public:
      //! Constructor: initialized to identity
      NVSG_API Trafo( void );

      //! Copy Constructor
      NVSG_API Trafo( const Trafo &rhs  //!< Trafo to copy
                    );

      //! Get the center of rotation of this transformation.
      /** \returns  \c Vec3f that describes the center or rotation. */
      NVSG_API const Vec3f & getCenter( void ) const;

      //! Get the rotational part of this transformation.
      /** \returns  \c Quatf that describes the rotational part */
      NVSG_API const Quatf & getOrientation( void ) const;

      //! Get the scale orientation part of this transform.
      /** \return \c Quatf that describes the scale orientational part */
      NVSG_API const Quatf & getScaleOrientation( void ) const;

      //! Get the scaling part of this transformation.
      /** \returns  \c Vec3f that describes the scaling part  */
      NVSG_API const Vec3f & getScaling( void ) const;

      //! Get the translational part of this transformation.
      /** \returns  \c Vec3f that describes the translational part  */
      NVSG_API const Vec3f & getTranslation( void ) const;

      /*! \brief Get the current transformation.
       *  \return The \c Mat44f that describes the transformation.
       *  \remarks The transformation is the concatenation of the center translation C, scale
       *  orientation SO, scaling S, rotation R, and translation T, by the following formula:
       *  \code
       *    M = -C * SO^-1 * S * SO * R * C * T
       *  \endcode
       *  \sa getInverse */
      NVSG_API Mat44f getMatrix( void ) const;

      /*! \brief Get the current inverse transformation.
       *  \return The \c Mat44f that describes the inverse transformation.
       *  \remarks The inverse transformation is the concatenation of the center translation C,
       *  scale orientation SO, scaling S, rotation R, and translation T, by the following
       *  formula:
       *  \code
       *    M = T^-1 * C^-1 * R^-1 * SO^-1 * S^-1 * SO * -C^-1
       *  \endcode
       *  \sa getMatrix */
      NVSG_API Mat44f getInverse( void ) const;

      //! Set the center of ration of the transformation.
      NVSG_API void setCenter( const Vec3f &center              //!<  center of rotation
                             );

      //! Set the \c Trafo to identity.
      NVSG_API void setIdentity( void );

      //! Set the rotational part of the transformation, using a quaternion.
      NVSG_API void setOrientation( const Quatf &orientation    //!<  rotational part of transformation
                                  );

      //! Set the scale orientational part of the transformation.
      NVSG_API void setScaleOrientation( const Quatf &scaleOrientation  //!< scale orientational part of transform
                                       );

      //! Set the scaling part of the transformation.
      NVSG_API void setScaling( const Vec3f &scaling            //!<  scaling part of transformation
                              );

      //! Set the translational part of the transformation.
      NVSG_API void setTranslation( const Vec3f &translation    //!<  translational part of transformation
                                  );

      //! Set the complete transformation by a Mat44f.
      /** The matrix is internally decomposed into a translation, rotation, scaleOrientation, and scaling. */
      NVSG_API void setMatrix( const Mat44f &matrix   //!< complete transformation
                             );

      //! Copy operator.
      NVSG_API Trafo & operator=( const Trafo &rhs //!< Trafo to copy
                       );

      //! Equality operator.
      /** \returns \c true if \c this is equal to \a t, otherwise \c false  */
      NVSG_API bool operator==( const Trafo &t    //!<  \c Trafo to compare with
                              ) const;

    private:
      Vec3f   m_center;             //!< center of rotation
      Quatf   m_orientation;        //!< orientational part of the transformation
      Quatf   m_scaleOrientation;   //!< scale orientation
      Vec3f   m_scaling;            //!< scaling part of the transformation
      Vec3f   m_translation;        //!< translational part of the transformation
  };

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // non-member functions
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  /*! \relates nvmath::Trafo
   *  This calculates the linear interpolation \code ( 1 - alpha ) * t0 + alpha * t1 \endcode */
  NVSG_API Trafo lerp( float alpha      //!<  interpolation parameter
                     , const Trafo &t0  //!<  starting value
                     , const Trafo &t1  //!<  ending value
                     );

  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
  inline const Vec3f & Trafo::getCenter( void ) const
  {
    NVSG_TRACE();
    return( m_center );
  }

  inline const Quatf & Trafo::getOrientation( void ) const
  {
    NVSG_TRACE();
    return( m_orientation );
  }

  inline const Quatf & Trafo::getScaleOrientation( void ) const
  {
    NVSG_TRACE();
    return( m_scaleOrientation );
  }

  inline const Vec3f & Trafo::getScaling( void ) const
  {
    NVSG_TRACE();
    return( m_scaling );
  }

  inline const Vec3f & Trafo::getTranslation( void ) const
  {
    NVSG_TRACE();
    return( m_translation );
  }

  inline void Trafo::setCenter( const Vec3f &center )
  {
    NVSG_TRACE();
    m_center = center;
  }

  inline void Trafo::setOrientation( const Quatf &orientation )
  {
    NVSG_TRACE();
    NVSG_ASSERT( isNormalized( orientation ) );
    m_orientation = orientation;
  }

  inline void Trafo::setScaleOrientation( const Quatf &scaleOrientation )
  {
    NVSG_TRACE();
    NVSG_ASSERT( isNormalized( scaleOrientation ) );
    m_scaleOrientation = scaleOrientation;
  }

  inline void Trafo::setScaling( const Vec3f &scaling )
  {
    NVSG_TRACE();
    m_scaling = scaling;
  }

  inline void Trafo::setTranslation( const Vec3f &translation )
  {
    NVSG_TRACE();
    m_translation = translation;
  }

}
