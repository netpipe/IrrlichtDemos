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
#include "nvsg/LightSource.h"

namespace nvsg
{
  /*! \brief LightSource representing a spot light.
   *  \par Namespace: nvsg
   *  \remarks A SpotLight restricts the shape of the emitted light to a cone. To specify the
   *  angle between the axis of the cone and a ray along the edge of the cone, use
   *  setCutoffAngle() with the angle in radians. The angle of the cone at the apex is then twice
   *  this value. No light is emitted beyond the edges of the cone.\n
   *  In addition, there are two ways to control the intensity distribution of the light within
   *  the cone. First, you can set the attenuation factors with setAttenuation(). You can also set
   *  a falloff exponent using setFalloffExponent(), which by default is zero, to control how
   *  concentrated the light is. The light's intensity is highest in the center of the cone. It is
   *  attenuated toward the edges of the cone by the cosine of the angle between the direction of
   *  the light and the direction from the light to the vertex being lit, raised to the power of
   *  the falloff exponent. Thus, higher falloff exponents result in a more focused light source.\n
   *  The default for a SpotLight are as follows:\n
   *    - attenuation is off (constant = 1.0, linear = 0.0, quadratic = 0.0)
   *    - cut-off angle is 45°
   *    - direction is the negative z-axis (0.0,0.0,-1.0)
   *    - fall-off exponent is no fall-off (0.0)
   *    - position is the origin (0.0,0.0,0.0)
   *  \sa LightSource, DirectedLight, PointLight */
  class SpotLight : public LightSource
  {
    public:

      /*! \brief Default-constructs a SpotLight. 
       */
      NVSG_API SpotLight();

      /*! \brief Constructs a SpotLight from a LightSource. 
      * \param
      * rhs LightSource to construct the SpotLight from.
      * \remarks
      * This is kind of a partial copy constructor only copying the LightSource part from the source 
      * object.
      */
      NVSG_API SpotLight( const LightSource &rhs );

      /*! \brief Constructs a SpotLight as a copy of another SpotLight.
      */
      NVSG_API SpotLight( const SpotLight &rhs );

      /*! \brief Destructs a SpotLight.
       */
      NVSG_API virtual ~SpotLight();

      /*! \brief Get the position.
       *  \return A reference to the constant position of the SpotLight.
       *  \remarks The default position is the origin (0.0,0.0,0.0).
       *  \sa getDirection, getOrientation, setPosition */
      NVSG_API const nvmath::Vec3f & getPosition() const;

      /*! \brief Set the position
       *  \param position A reference to the constant position to set.
       *  \remarks The default position is the origin (0.0,0.0,0.0).
       *  \sa getPosition, setDirection, setOrientation */
      NVSG_API void setPosition( const nvmath::Vec3f & position );
      
      /*! \brief Get the direction.
       *  \return A reference to the constant direction of the SpotLight.
       *  \remarks The default direction is the negative z-axis (0.0,0.0,-1.0)
       *  \sa getPosition, setDirection, setOrientation */
      NVSG_API nvmath::Vec3f getDirection() const;

      /*! \brief Set the direction
       *  \param direction A reference to the constant direction to set.
       *  \remarks The default direction is the negative z-axis (0.0,0.0,-1.0)
       *  \note The direction is expected to be normalized.
       *  \sa getDirection, setOrientation, setPosition */
      NVSG_API void setDirection( const nvmath::Vec3f & direction );

      /*! \brief Get the orientation.
       *  \return A reference to the constant orientation of the SpotLight.
       *  \remarks The default direction is the negative z-axis (0.0,0.0,-1.0), thus the default
       *  orientation is (0.0,0.0,0.0,1.0).
       *  \sa getPosition, getDirection, setOrientation */
      NVSG_API const nvmath::Quatf & getOrientation() const;
      
      /*! \brief Set the orientation.
       *  \param orientation A reference to the constant orientation to set.
       *  \remarks The default direction is the negative z-axis (0.0,0.0,-1.0), thus the default
       *  orientation is (0.0,0.0,0.0,1.0).
       *  \sa getOrientation, setDirection, setPosition */
      NVSG_API void setOrientation( const nvmath::Quatf & orientation );

      /*! \brief Query if this SpotLight is attenuated
       *  \return \c true if this light source is attenuated, otherwise \c false
       *  \remarks A SpotLight is attenuated if the constant attenuation factor is different from
       *  one, or if the linear or quadratic attenuation factor is different from zero.\n
       *  By default, attenuation is off.
       *  \sa getAttenuation, setAttenuation */
      NVSG_API bool isAttenuated() const;

      /*! \brief Get the attenuation factors.
       *  \param c A reference to a float to get the constant attenuation factor.
       *  \param l A reference to a float to get the linear attenuation factor.
       *  \param q A reference to a float to get the quadratic attenuation factor.
       *  \remarks The attenuation factors are used to calculate the attenuation of this light at
       *  a lit point by the equation
       *  \code atten = 1 / ( c + l * d + q * d * d ) \endcode
       *  where \c d is the distance between the light and the vertex.\n
       *  By default, attenuation is off.
       *  \sa isAttenuated, setAttenuation */
      NVSG_API void  getAttenuation( float &c
                                   , float &l
                                   , float &q ) const;
 
      /*! \brief Get the attenuation factors as a nvmath::Vec3f.
       *  \return A reference to the constant nvmath::Vec3f with the attenuation factors.
       *  \remarks The attenuation factors are used to calculate the attenuation of this light at
       *  a lit point by the equation
       *  \code atten = 1 / ( v[0] + v[1] * d + v[2] * d * d ) \endcode
       *  where \c v is the returned nvmath::Vec3f, and \c d is the distance between the light and
       *  the vertex.\n
       *  By default, attenuation is off.
       *  \sa isAttenuated, setAttenuation */
      NVSG_API const nvmath::Vec3f& getAttenuation() const;

      /*! \brief Set the attenuation factors.
       *  \param c The constant attenuation factor.
       *  \param l The linear attenuation factor.
       *  \param q The quadratic attenuation factor.
       *  \remarks The attenuation factors are used to calculate the attenuation of this light at
       *  a lit point by the equation
       *  \code atten = 1 / ( c + l * d + q * d * d ) \endcode
       *  where \c d is the distance between the light and the vertex.\n
       *  By default, attenuation is off.
       *  \sa isAttenuated, getAttenuation */
      NVSG_API void setAttenuation( float c
                                  , float l
                                  , float q );
 
      /*! \brief Set the attenuation factors by a nvmath::Vec3f.
       *  \param attenuation A reference to the constant attenuation factors.
       *  \remarks The attenuation factors are used to calculate the attenuation of this light at
       *  a lit point by the equation\n
       *  \code atten = 1 / ( v[0] + v[1] * d + v[2] * d * d ) \endcode
       *  where \c v is the returned nvmath::Vec3f, and \c d is the distance between the light and
       *  the vertex.\n
       *  By default, attenuation is off.
       *  \sa isAttenuated, getAttenuation */
      NVSG_API void setAttenuation( const nvmath::Vec3f &attenuation );

      /*! \brief Get the cut-off angle.
       *  \return The cut-off angle in degrees.
       *  \remarks The light's cone is cut off at this angle (in radian). Points beyond this cone
       *  are not lit. The default is pi/4 (or 45°).
       *  \sa setCutoffAngle */
      NVSG_API float getCutoffAngle() const;

      /*! \brief Set the cut-off angle.
       *  \param ca The cut-off angle in degrees.
       *  \remarks The light's cone is cut off at this angle (in radian). Points beyond this cone
       *  are not lit. The default is pi/4 (or 45°).
       *  \sa getCutoffAngle */
      NVSG_API void setCutoffAngle( float ca );

      /*! \brief Get the fall-off exponent.
       *  \return The fall-off exponent.
       *  \remarks The falloff exponent controls the sharpness of the spot light. The higher the
       *  exponent, the more focused the light source. The default is 0.0
       *  \sa setFalloffExponent */
      NVSG_API float getFalloffExponent() const;

      /*! \brief Set the fall-off exponent.
       *  \param fe The fall-off exponent.
       *  \remarks The falloff exponent controls the sharpness of the spot light. The higher the
       *  exponent, the more focused the light source. The default is 0.0
       *  \sa getFalloffExponent */
      NVSG_API void setFalloffExponent( float fe );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant SpotLight to copy from
       *  \return A reference to the assigned SpotLight
       *  \remarks The assignment operator calls the assignment operator of LightSource, and
       *  copies the attenuation factors, the cut-off angle and the fall-off exponent.
       *  \sa LightSource */
      NVSG_API SpotLight & operator=(const SpotLight & rhs);
      
      /*! \brief Test for equivalence with an other SpotLight.
       *  \param p A pointer to the SpotLight to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
       *  true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the SpotLight \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as LightSource, they are equivalent, if the
       *  attenuation factors, the cut-off angle, and the fall-off exponent are equal.
       *  \note The behaviour is undefined if \a p is not a SpotLight or derived from one.
       *  \sa DirectedLight, LightSource, PointLight */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    private:
      nvmath::Vec3f m_attenuation;
      float         m_cutoffAngle;
      float         m_falloffExponent;
  };

  inline  const nvmath::Vec3f&  SpotLight::getAttenuation()  const
  {
    NVSG_TRACE();
    return m_attenuation;
  }

  inline  void  SpotLight::getAttenuation( float &c, float &l, float &q )  const
  {
    NVSG_TRACE();
    c = m_attenuation[0];
    l = m_attenuation[1];
    q = m_attenuation[2];
  }

  inline  float SpotLight::getCutoffAngle()  const
  {
    NVSG_TRACE();
    return( m_cutoffAngle );
  }

  inline  float SpotLight::getFalloffExponent()  const
  {
    NVSG_TRACE();
    return( m_falloffExponent );
  }

  inline  bool  SpotLight::isAttenuated()  const
  {
    NVSG_TRACE();
    return(   ( m_attenuation[0] != 1.0f )
          ||  ( m_attenuation[1] != 0.0f )
          ||  ( m_attenuation[2] != 0.0f )
          );
  }

  inline  void  SpotLight::setAttenuation( float c, float l, float q )
  {
    NVSG_TRACE();
    m_attenuation[0] = c;
    m_attenuation[1] = l;
    m_attenuation[2] = q;
    increaseIncarnation();
  }

  inline void SpotLight::setAttenuation( const nvmath::Vec3f & attenuation )
  {
    NVSG_TRACE();
    m_attenuation = attenuation;
    increaseIncarnation();
  }

  inline  void  SpotLight::setCutoffAngle( float ca )
  {
    NVSG_TRACE();
    m_cutoffAngle = ca;
    increaseIncarnation();
  }

  inline  void  SpotLight::setFalloffExponent( float fe )
  {
    NVSG_TRACE();
    m_falloffExponent = fe;
    increaseIncarnation();
  }

  inline nvmath::Vec3f SpotLight::getDirection() const
  {
    NVSG_TRACE();
    return nvmath::Vec3f (0.f, 0.f, -1.f) * m_trafo.getOrientation();
  }
  
  inline  const nvmath::Vec3f & SpotLight::getPosition() const
  {
    NVSG_TRACE();
    return m_trafo.getTranslation();
  }

  inline  void  SpotLight::setPosition( const nvmath::Vec3f & position )
  {
    NVSG_TRACE();
    m_trafo.setTranslation(position);
    increaseIncarnation();
  }

  inline const nvmath::Quatf & SpotLight::getOrientation() const
  {
    NVSG_TRACE();
    return m_trafo.getOrientation();
  }

  inline void SpotLight::setOrientation( const nvmath::Quatf & orientation )
  {
    NVSG_TRACE();
    m_trafo.setOrientation(orientation);
    increaseIncarnation();
  }
} //  namespace nvsg
