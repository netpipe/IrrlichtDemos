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
#include "nvsg/StateAttribute.h" // base class definition
#include "nvmath/Vec3f.h"


namespace nvsg
{
  class Material;

  /*! \brief ID to identify the front and back face properties of a Material.
   *  \par Namespace: nvsg
   *  \remarks All the properties of a Material can be independently set for the front and the    
   *  back faces.
   *  \sa Material */
  typedef enum
  {
    MF_FRONT = 0x01,                        //!< used for front material
    MF_BACK  = 0x02,                        //!< used for back material
    MF_FRONT_AND_BACK = MF_FRONT | MF_BACK  //!< used for both front and back material
  } MaterialFace;

//  ignore private member class on documentation
# if ! defined( DOXYGEN_IGNORE )
  class MaterialData : public nvutil::RCObject
  {
    friend class Material;

    public:
      //! Copy constructor
      /** Constructs a MaterialData object from another MaterialData object.
        * Public, because reference counting requires the copy constructor to be accessible. */
      NVSG_API MaterialData(const MaterialData& rhs);

    private:
      // instantiation on stack is not permitted
      MaterialData();
      ~MaterialData();
      // assignment also not permitted
      MaterialData& operator=(const MaterialData& rhs) {}

      nvmath::Vec3f m_backAmbientColor;
      nvmath::Vec3f m_backDiffuseColor;
      nvmath::Vec3f m_backEmissiveColor;
      float         m_backOpacity;
      nvmath::Vec3f m_backSpecularColor;
      float         m_backSpecularExponent;

      nvmath::Vec3f m_frontAmbientColor;
      nvmath::Vec3f m_frontDiffuseColor;
      nvmath::Vec3f m_frontEmissiveColor;
      float         m_frontOpacity;
      nvmath::Vec3f m_frontSpecularColor;
      float         m_frontSpecularExponent;
  };
#endif    //  DOXYGEN_IGNORE

  /*! \brief StateAttribute that describes material properties.
   *  \par Namespace: nvsg
   *  \remarks A Material describes the color properties of a geometry. The properties for front
   *  and back faces can be described independently. The following are the default values 
   *  that Material uses for both front and back faces:\n
   *    - ambient color is dark grey    (0.3,0.3,0.3)\n
   *    - diffuse color is light grey   (0.9,0.9,0.9)\n
   *    - emissive color is black       (0.0,0.0,0.0)\n
   *    - opacity is opaque             1.0\n
   *    - specular color is light grey  (0.9,0.9,0.9)\n
   *    - specular exponent is medium   10.0
   *  \sa StateAttribute */
  class Material : public StateAttribute
  {
    public:
      /*! \brief Default-constructs a Material. */
      NVSG_API Material();

      /*! \brief Constructs a Material from a StateAttribute. */
      NVSG_API explicit Material( const StateAttribute &rhs );

      /*! \brief Constructs a Material as a copy from another Material. */
      NVSG_API Material( const Material &rhs );

      /*! \brief Destructs a Material. */
      NVSG_API virtual ~Material();

      /*! \brief Query whether the data of this Material is shared with other Material objects.
       *  \return \c true, if the data of this Material is shared with other Material objects,
       *  otherwise \c false.
       *  \remarks A Material object shares its data with another Material object if it was
       *  either instantiated as a copy of the other, using Material::clone, or if it was assigned
       *  to by the other object using the assignment operator. In that case, the two objects have
       *  the same DataID.
       *  \sa clone, getDataID, operator=() */
      NVSG_API virtual bool isDataShared() const;

      /*! \brief Get the unique data identifier of this object.
       *  \return A 64-bit value that uniquely identifies the embedded data.
       *  \remarks A Material object shares its data with another Material object if it was
       *  either instantiated as a copy of the other, using Material::clone, or if it was assigned
       *  to by the other object using the assignment operator. In that case, the two objects have
       *  the same DataID.
       *  \sa clone, isDataShared, operator=() */
      NVSG_API virtual DataID getDataID() const;

      /*! \brief Get the ambient color.
       *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
       *  ambient color. The default is MF_FRONT.
       *  \return A reference to the constant ambient color.
       *  \remarks Ambient reflection, like ambient light, is nondirectional. Ambient reflection
       *  has a lesser impact on the apparent color of a rendered object, but it does affect the
       *  overall color and is most noticeable when little or no diffuse light reflects off the
       *  material. A material's ambient reflection is affected by the ambient light set for a
       *  scene. The default is dark grey (0.3,0.3,0.3).
       *  \note The behaviour is undefined, if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
       *  \sa getDiffuseColor, getEmissiveColor, getSpecularColor, setAmbientColor */
      NVSG_API const nvmath::Vec3f& getAmbientColor( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the ambient color
       *  \param ac A reference to the constant color to set as ambient.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks Ambient reflection, like ambient light, is nondirectional. Ambient reflection
       *  has a lesser impact on the apparent color of a rendered object, but it does affect the
       *  overall color and is most noticeable when little or no diffuse light reflects off the
       *  material. A material's ambient reflection is affected by the ambient light set for a
       *  scene. The default is dark grey (0.3,0.3,0.3).
       *  \sa getAmbientColor, setDiffuseColor, setEmissiveColor, setSpecularColor */
      NVSG_API void setAmbientColor( const nvmath::Vec3f &ac
                                   , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Get the diffuse color.
      *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
      *  diffuse color. The default is MF_FRONT.
      *  \return A reference to the constant diffuse color.
      *  \remarks The diffuse and ambient colors of a material describe how a material reflects
      *  the ambient and diffuse light in a scene. Because most scenes contain much more diffuse
      *  light than ambient light, diffuse reflection plays the largest role in determining
      *  color. Additionally, because diffuse light is directional, the angle of incidence for
      *  diffuse light affects the overall intensity of the reflection. Diffuse reflection is
      *  greatest when the light strikes a vertex parallel to the vertex normal. As the angle
      *  increases, the effect of diffuse reflection diminishes. The amount of light reflected is
      *  the cosine of the angle between the incoming light and the vertex normal. The default is
      *  light grey (0.9,0.9,0.9).
      *  \note The behavior is undefined if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
      *  \sa getAmbientColor, getEmissiveColor, getSpecularColor, setDiffuseColor */
      NVSG_API const nvmath::Vec3f& getDiffuseColor( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the diffuse color
       *  \param dc A reference to the constant color to set as diffuse.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks The diffuse and ambient colors of a material describe how a material reflects
       *  the ambient and diffuse light in a scene. Because most scenes contain much more diffuse
       *  light than ambient light, diffuse reflection plays the largest role in determining
       *  color. Additionally, because diffuse light is directional, the angle of incidence for
       *  diffuse light affects the overall intensity of the reflection. Diffuse reflection is
       *  greatest when the light strikes a vertex parallel to the vertex normal. As the angle
       *  increases, the effect of diffuse reflection diminishes. The amount of light reflected is
       *  the cosine of the angle between the incoming light and the vertex normal. The default is
       *  light grey (0.9,0.9,0.9).
       *  \sa getDiffuseColor, setAmbientColor, setEmissiveColor, setSpecularColor */
      NVSG_API void setDiffuseColor( const nvmath::Vec3f &dc
                                   , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Get the emissive color.
       *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
       *  emissive color. The default is MF_FRONT.
       *  \return A reference to the constant emissive color.
       *  \remarks Materials can be used to make a rendered object appear to be self-luminous. The
       *  emissive color of a material is used to describe the color of the emitted light.
       *  Emission affects an object's color and can, for example, make a dark material brighter
       *  and take on part of the emitted color. You can use a material's emissive color to add
       *  the illusion that an object is emitting light, without incurring the computational
       *  overhead of adding a light to the scene. Remember, materials with emissive color don't
       *  emit light that can be reflected by other objects in a scene. To achieve this reflected
       *  light, you need to place an additional light within the scene. The default is black
       *  (0.0,0.0,0.0).
       *  \note The behavior is undefined if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
       *  \sa getAmbientColor, getDiffuseColor, getSpecularColor, setEmissiveColor */
      NVSG_API const nvmath::Vec3f& getEmissiveColor( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the emissive color
       *  \param ec A reference to the constant color to set as emissive.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks Materials can be used to make a rendered object appear to be self-luminous. The
       *  emissive color of a material is used to describe the color of the emitted light.
       *  Emission affects an object's color, and can, for example, make a dark material brighter
       *  and take on part of the emitted color. You can use a material's emissive color to add
       *  the illusion that an object is emitting light, without incurring the computational
       *  overhead of adding a light to the scene. Remember, materials with emissive color don't
       *  emit light that can be reflected by other objects in a scene. To achieve this reflected
       *  light, you need to place an additional light within the scene. The default is black
       *  (0.0,0.0,0.0).
       *  \sa getEmissiveColor, setAmbientColor, setDiffuseColor, setSpecularColor */
      NVSG_API void setEmissiveColor( const nvmath::Vec3f &ec
                                    , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Get the opacity.
       *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
       *  opacity. The default is MF_FRONT.
       *  \return The opacity.
       *  \remarks The opacity of a material controls how much light is reflected and how much
       *  light goes through. An opacity of 1.0 means the material is completely opaque - all
       *  light is reflected and no objects behind shine through. An opacity of 0.0 means the
       *  material is completely transparent - no light is reflected and objects behind are seen
       *  undisturbed. The default is opaque 1.0.
       *  \note The behavior is undefined if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
       *  \sa setOpacity */
      NVSG_API float getOpacity( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the opacity.
       *  \param o The opacity to set.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks The opacity of a material controls how much light is reflected and how much
       *  light goes through. An opacity of 1.0 means the material is completely opaque - all
       *  light is reflected and no objects behind shine through. An opacity of 0.0 means the
       *  material is completely transparent - no light is reflected and objects behind are seen
       *  undisturbed. The default opacity is 1.0 (completely opaque).
       *  \sa getOpacity */
      NVSG_API void setOpacity( float o
                              , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Get the specular color.
       *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
       *  specular color. The default is MF_FRONT.
       *  \return A reference to the constant specular color.
       *  \remarks Specular reflection creates highlights on objects, making them appear shiny.
       *  The material contains two members that describe the specular highlight color as well as
       *  the material's overall shininess. You establish the color of the specular highlights by
       *  setting the specular color to the desired RGBA color — the most common colors are white
       *  or light gray. The values you set in the specular exponent control how sharp the
       *  specular effects are. The default of the specular color is dark grey (0.9,0.9,0.9).
       *  \note The behavior is undefined if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
       *  \sa getAmbientColor, getDiffuseColor, getEmissiveColor, getSpecularExponent, setSpecularColor */
      NVSG_API const nvmath::Vec3f& getSpecularColor( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the specular color
       *  \param sc A reference to the constant color to set as specular.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks Specular reflection creates highlights on objects, making them appear shiny.
       *  The material contains two members that describe the specular highlight color as well as
       *  the material's overall shininess. You establish the color of the specular highlights by
       *  setting the specular color to the desired RGBA color — the most common colors are white
       *  or light gray. The values you set in the specular exponent control how sharp the
       *  specular effects are. The default of the specular color is dark grey (0.9,0.9,0.9).
       *  \sa getSpecularColor, setAmbientColor, setDiffuseColor, setEmissiveColor, setSpecularExponent */
      NVSG_API void setSpecularColor( const nvmath::Vec3f &sc
                                    , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Get the specular exponent.
       *  \param mf Optional identifier to select front face (MF_FRONT) or back face (MF_BACK)
       *  opacity. The default is MF_FRONT.
       *  \return The specular exponent.
       *  \remarks Specular reflection creates highlights on objects, making them appear shiny.
       *  The material contains two members that describe the specular highlight color as well as
       *  the material's overall shininess. You establish the color of the specular highlights by
       *  setting the specular color to the desired RGBA color — the most common colors are white
       *  or light gray. The values you set in the specular exponent control how sharp the
       *  specular effects are. The default of the specular exponent is 10.0.
       *  \note The behavior is undefined if MF_FRONT_AND_BACK is used as the MaterialFace \a mf.
       *  \sa getSpecularColor, setSpecularExponent */
      NVSG_API float getSpecularExponent( MaterialFace mf = MF_FRONT ) const;

      /*! \brief Set the specular exponent.
       *  \param se The specular exponent to set.
       *  \param mf Optional identifier to select front face (MF_FRONT), back face (FM_BACK) or
       *  both (MF_FRONT_AND_BACK) to set. The default is MF_FRONT_AND_BACK.
       *  \remarks Specular reflection creates highlights on objects, making them appear shiny.
       *  The material contains two members that describe the specular highlight color as well as
       *  the material's overall shininess. You establish the color of the specular highlights by
       *  setting the specular color to the desired RGBA color — the most common colors are white
       *  or light gray. The values you set in the specular exponent control how sharp the
       *  specular effects are. The default of the specular exponent is 10.0.
       *  \sa getSpecularExponent, setSpecularColor */
      NVSG_API void setSpecularExponent( float se
                                       , MaterialFace mf = MF_FRONT_AND_BACK );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Material to copy from.
       *  \return A reference to the assigned Material.
       *  \remarks The assignment operator calls the assignment operator of StateAttribute and
       *  copies all the material properties. */
      NVSG_API Material & operator=( const Material & rhs );

      /*! \brief Test for equivalence with an other Material. 
       *  \param p A reference to the constant Material to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the Material \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as StateAttribute, they are equivalent
       *  if all their material properties are equal.
       *  \sa StateAttribute */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    
  private:
      // helper performing the actual assignment
      // called inside setOpacity for different faces
      void assignOpacity(float& lhs, float rhs);

  private:
      nvutil::RCPtr<MaterialData> m_material; // shareable material data
  };

  inline const nvmath::Vec3f& Material::getAmbientColor( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontAmbientColor : m_material->m_backAmbientColor );
  }

  inline const nvmath::Vec3f& Material::getDiffuseColor( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontDiffuseColor : m_material->m_backDiffuseColor );
  }

  inline const nvmath::Vec3f& Material::getEmissiveColor( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontEmissiveColor : m_material->m_backEmissiveColor );
  }

  inline float Material::getOpacity( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontOpacity : m_material->m_backOpacity );
  }

  inline const nvmath::Vec3f& Material::getSpecularColor( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontSpecularColor : m_material->m_backSpecularColor );
  }

  inline float Material::getSpecularExponent( MaterialFace mf ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( mf != MF_FRONT_AND_BACK );
    return( ( mf & MF_FRONT ) ? m_material->m_frontSpecularExponent : m_material->m_backSpecularExponent );
  }

} //  namespace nvsg

