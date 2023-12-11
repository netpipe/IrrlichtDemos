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
#include  "nvsg/LightSource.h"

namespace nvsg
{
  /*! \brief LightSource representing a point light.
    *  \par Namespace: nvsg
    *  \remarks Point lights have color and position within a scene, but no single direction. They
    *  give off light equally in all directions. A light bulb is a good example of a point light.
    *  Point lights are affected by attenuation and range, and illuminate a mesh on a
    *  vertex-by-vertex basis. During lighting, the point light's position in world space and the
    *  coordinates of the vertex being lit are used to derive a vector for the direction of the
    *  light and the distance that the light has traveled. Both are used, along with the vertex
    *  normal, to calculate the contribution of the light to the illumination of the surface.
    *  The defaults for a PointLight are as follows:\n
    *    - attenuation is off (constant = 1.0, linear = 0.0, quadratic = 0.0)
    *    - position is the origin (0.0,0.0,0.0)
    *  \sa LightSource, DirectedLight, SpotLight */
  class PointLight : public LightSource
  {
    public:
      /*! \brief Default-constructs a PointLight. 
        */
      NVSG_API PointLight();

      /*! \brief Constructs a PointLight from a LightSource. 
      * \param
      * rhs LightSource to construct the PointLight from.
      * \remarks
      * This is kind of a partial copy constructor only copying the LightSource part from the source 
      * object.
      */
      NVSG_API explicit PointLight( const LightSource &rhs );

      /*! \brief Constructs a PointLight as a copy of another PointLight.
      */
      NVSG_API PointLight( const PointLight &rhs );

      /*! \brief Destructs a PointLight
      */
      NVSG_API virtual ~PointLight();

      /*! \brief Get the position.
        *  \return A reference to the constant position of the PointLight.
        *  \remarks The default position is the origin (0.0,0.0,0.0).
        *  \sa setPosition */
      NVSG_API const nvmath::Vec3f & getPosition() const;

      /*! \brief Set the position
        *  \param position A reference to the constant position to set.
        *  \remarks The default position is the origin (0.0,0.0,0.0).
        *  \sa getPosition */
      NVSG_API void setPosition( const nvmath::Vec3f & position );

      /*! \brief Query if this PointLight is attenuated
        *  \return \c true if this light source is attenuated, otherwise \c false
        *  \remarks A PointLight is attenuated if the constant attenuation factor is different from
        *  one, or if the linear or quadratic attenuation factor is different from zero.\n
        *  By default, attenuation is off.
        *  \sa getAttenuation, setAttenuation */
      NVSG_API bool isAttenuated() const;

      /*! \brief Get the attenuation factors.
        *  \param c A reference to a float to get the constant attenuation factor.
        *  \param l A reference to a float to get the linear attenuation factor.
        *  \param q A reference to a float to get the quadratic attenuation factor.
        *  \remarks The attenuation factors are used to calculate the attenuation of this light at
        *  a lit point, using the equation
        *  \code atten = 1 / ( c + l * d + q * d * d ) \endcode
        *  where \c d is the distance between the light and the vertex.\n
        *  By default, attenuation is off.
        *  \sa isAttenuated, setAttenuation */
      NVSG_API void getAttenuation( float &c
                                  , float &l
                                  , float &q ) const;

      /*! \brief Get the attenuation factors as a nvmath::Vec3f.
        *  \return A reference to the constant nvmath::Vec3f with the attenuation factors.
        *  \remarks The attenuation factors are used to calculate the attenuation of this light at
        *  a lit point, using the equation
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
        *  a lit point, using the equation
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
        *  a lit point, using the equation\n
        *  \code atten = 1 / ( v[0] + v[1] * d + v[2] * d * d ) \endcode
        *  where \c v is the returned nvmath::Vec3f, and \c d is the distance between the light and
        *  the vertex.\n
        *  By default, attenuation is off.
        *  \sa isAttenuated, getAttenuation */
      NVSG_API void setAttenuation( const nvmath::Vec3f &attenuation );

      /*! \brief Assignment operator
        *  \param rhs A reference to the constant PointLight to copy from
        *  \return A reference to the assigned PointLight
        *  \remarks The assignment operator calls the assignment operator of LightSource, and
        *  copies the attenuation factors.
        *  \sa LightSource */
      NVSG_API PointLight & operator=( const PointLight & rhs );

      /*! \brief Test for equivalence with an other PointLight.
        *  \param p A pointer to the PointLight to test for equivalence with.
        *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c
        *  true.
        *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
        *  \return \c true if the PointLight \a p is equivalent to \c this, otherwise \c false.
        *  \remarks If \a p and \c this are equivalent as LightSource, they are equivalent if the
        *  attenuation factors are equal.
        *  \note The behavior is undefined if \a p is not a PointLight nor derived from one.
        *  \sa DirectedLight, LightSource, SpotLight */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    private:
      nvmath::Vec3f m_attenuation;
  };

  inline void PointLight::getAttenuation( float &c, float &l, float &q ) const
  {
    NVSG_TRACE();
    c = m_attenuation[0];
    l = m_attenuation[1];
    q = m_attenuation[2];
  }

  inline const nvmath::Vec3f& PointLight::getAttenuation() const
  {
    NVSG_TRACE();
    return m_attenuation;
  }

  inline bool PointLight::isAttenuated() const
  {
    NVSG_TRACE();
    return(   ( m_attenuation[0] != 1.0f )
          ||  ( m_attenuation[1] != 0.0f )
          ||  ( m_attenuation[2] != 0.0f )
          );
  }

  inline void PointLight::setAttenuation( float c, float l, float q )
  {
    NVSG_TRACE();
    m_attenuation[0] = c;
    m_attenuation[1] = l;
    m_attenuation[2] = q;
    increaseIncarnation();
  }

  inline void PointLight::setAttenuation( const nvmath::Vec3f & attenuation )
  {
    NVSG_TRACE();
    m_attenuation = attenuation;
    increaseIncarnation();
  }

  inline const nvmath::Vec3f & PointLight::getPosition() const
  {
    NVSG_TRACE();
    return m_trafo.getTranslation();
  }

  inline void PointLight::setPosition( const nvmath::Vec3f & position )
  {
    NVSG_TRACE();
    m_trafo.setTranslation(position);
    increaseIncarnation();
  }

} //  namespace nvsg
