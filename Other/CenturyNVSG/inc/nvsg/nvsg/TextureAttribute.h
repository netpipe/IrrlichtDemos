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

#include <map>    // uses std::map
#include <string> // uses std::string
#include "nvsgcommon.h"
#include "nvmath/Trafo.h" // uses Trafo
#include "nvmath/Vec4f.h" // uses Vec4f
#include "nvsg/StateAttribute.h" // base class definition


namespace nvsg
{

  //! Texture coordinate axis
  typedef enum
  {
    TCA_S = 0       //!< S axis
  , TCA_T           //!< T axis
  , TCA_R           //!< R axis
  , TCA_Q           //!< Q axis
  } TexCoordAxis;

  //! Texture environment mode
  typedef enum
  {
    TEM_REPLACE = 0  //!< C = C_t; A = A_t
  , TEM_MODULATE     //!< C = C_f*C_t; A = A_f*A_t
  , TEM_DECAL        //!< C = C_f*(1-A_t)+C_t*A_t; A = A_f
  , TEM_BLEND        //!< C = C_f*(1-C_t)+C_c*C_t; A = A_f*A_t
  , TEM_ADD          //!< C = min(1,C_f+C_t); A = A_f*A_t
  , TEM_ADD_SIGNED   //!< C = texUnitCur + texUnitPrev - 0.5; A = Ac*Ap
  , TEM_SUBTRACT     //!< C = texUnitCur - texUnitPrev; A = Ac*Ap
  , TEM_INTERPOLATE  //!< C = tUC * (tevColor) + tUP * (1-tevColor); A = Ac*Ap
  , TEM_DOT3_RGB     //!< C = texUnitCur.rgb dot texUnitPrev.rgb; A = N/A
  , TEM_DOT3_RGBA    //!< C = texUnitCur.rgba dot texUnitPrev.rgba
  } TextureEnvMode;

  //! Texture environment scale
  typedef enum
  {
    TES_1X = 1  //!< scale factor 1x or no scaling
  , TES_2X = 2  //!< scale factor 2x
  , TES_4X = 4  //!< scale factor 4x
  } TextureEnvScale;

  //! Texture coordinate generation mode
  typedef enum
  {
    TGM_OFF           = -1  //!< Off
  , TGM_OBJECT_LINEAR       //!< Object linear
  , TGM_EYE_LINEAR          //!< Eye linear
  , TGM_SPHERE_MAP          //!< Sphere map
  , TGM_REFLECTION_MAP      //!< Reflection map
  , TGM_NORMAL_MAP          //!< Normal map
  } TexGenMode;

  //! Texture generation plane
  typedef enum
  {
    TGLP_OBJECT = 0       //!< Object
  , TGLP_EYE              //!< Eye
  , TGLP_OBJECT_AND_EYE   //!< Object and eye
  } TexGenLinearPlane;

  //! Texture wrap coordinate axis
  typedef enum
  {
    TWCA_S = 0    //!< S axis
  , TWCA_T        //!< T axis
  , TWCA_R        //!< R axis
  } TexWrapCoordAxis;

  //! Texture wrap mode
  typedef enum
  {
    TWM_REPEAT = 0    //!< Corresponds to GL_REPEAT in OpenGL
  , TWM_CLAMP         //!< Corresponds to GL_CLAMP in OpenGL
  , TWM_MIRROR        //!< Corresponds to GL_MIRRORED_REPEAT_ARB in OpenGL
  } TextureWrapMode;

  //! Texture minification mode
  typedef enum
  {
    TFM_MIN_NEAREST = 0     //!< Corresponds to GL_NEAREST in OpenGL
  , TFM_MIN_BILINEAR        //!< Corresponds to GL_LINEAR in OpenGL
  , TFM_MIN_TRILINEAR       //!< Corresponds to GL_LINEAR_MIPMAP_LINEAR in OpenGL
  } TextureMinFilterMode;

  //! Texture magnification mode
  typedef enum
  {
    TFM_MAG_NEAREST = 0     //!< Corresponds to GL_NEAREST in OpenGL
  , TFM_MAG_BILINEAR        //!< Corresponds to GL_LINEAR in OpenGL
  } TextureMagFilterMode;

  class TextureAttributeItem;
  class TextureImage;

  //! Data of a texture attribute item that is worth sharing
  class TextureAttributeItemData : public nvutil::RCObject
  {
      friend class TextureAttributeItem;
    public:
      //! Copy constructor.
      /** Initialize a new copy from \a rhs */
      NVSG_API TextureAttributeItemData(const TextureAttributeItemData& rhs);

    private:
      // instantiation on stack is not permitted.
      TextureAttributeItemData();
      ~TextureAttributeItemData();
      // assignment also not permitted
      TextureAttributeItemData& operator=(const TextureAttributeItemData& rhs) {}

      nvmath::Vec4f         m_borderColor;
      nvmath::Vec4f         m_envColor;
      TextureEnvMode        m_envMode;            // enum environment mode to use with rasterization
      TextureEnvScale        m_envScale;          // enum environment scale to use with rasterization
      TextureMagFilterMode  m_magFilterMode;
      TextureMinFilterMode  m_minFilterMode;
      nvmath::Trafo         m_trafo;              // texture coordinate transformation
      TextureWrapMode       m_wrapMode[3];
      TexGenMode            m_texGenMode[4];
      nvmath::Vec4f         m_texGenPlane[2][4];
  };

  //! Texture attribute item
  /** A TextureAttributeItem object fully describes a texture together with its environment and 
    * parameter settings to be used to render a textured geometry. 
    *
    * The use of a TextureAttributeItem object is meaningful only in conjunction with a 
    * TextureAttribute object, that serves as a kind of container for texture attribute items. */
  class TextureAttributeItem : public OwnedObject<TextureAttributeHandle>
  {
    friend class TextureAttribute;

  public:
    /*! \brief Default-constructs a TextureAttributeItem.
      * \remarks A TextureAttributeItem will be instantiated and default constructed 
      * by creating an associated TextureAttributeItemHandle through
      * CreateHandle(TextureAttributeItemHandle), which returns a pointer
      * to a TextureAttributeItemHandle as follows:
      * \code
      *     TextureAttributeItemHandle * hTexAttribItem = 
      *       CreateHandle(TextureAttributeItemHandle); 
      * \endcode
      * A default constructed TextureAttributeItem has no TextureImage assigned to it.
      * The texture border color initially is black, and the filtering initially is set
      * to use bilinear filtering for both, minification and magnification. Wrap modes
      * initially are set to repeat for all axes. The environment color initially is
      * black, and the environment mode is set to modulate by default. Scaling initially
      * is disabled. Texture coordinate generation also is off by default.
      * \n\n
      * To access the TextureAttributeItem API, you need to explicitely acquire 
      * access for either write or read-only operations on the object. 
      * The NVSG SDK provides the convenient helper templates WritableObject and
      * ReadableObject to serve this purpose. These two helper templates are defined
      * in the nvutil namespace. Instantiated objects of these template classes
      * can be used similar as an ordinary pointer:
      * \code
      *     // acquire write access to the TextureAttributeItem
      *     { // limit write access to this scope
      *       WritableObject<TextureAttributeItem> texAttribItem(hTexAttribItem);
      *       texAttribItem->setTextureImage(
      *         TextureImage::createFromFile("sunset.jpg", std::vector<std::string>()) );
      *       [...]
      *     } // write access will be released after leaving this scope
      * \endcode
      */ 
    NVSG_API TextureAttributeItem( void );
    
    /*! \brief Constructs a TextureAttributeItem from an Object.
      * \param rhs Specifies the source Object.
      * \remarks This is kind of a 'partial' copy constructor. 
      * Partial, because only the Object part of the new instantiated TextureAttributeItem
      * is copied from the \a rhs Object. The TextureAttributeItem specific data will be
      * default-initialized. 
      * \n\n
      * This partial copy constructor will be invoked by the CreateHandleFromBase macro. */
    NVSG_API TextureAttributeItem( const Object &rhs );
    
    /*! \brief Constructs a TextureAttributeItem as a copy from another TextureAttributeItem.
      * \param rhs Specifies the source TextureAttributeItem object.
      * \remarks The new instantiated TextureAttributeItem will be initialized with the same
      * texture parameter settings, texture environment settings, and texture-coordinate
      * generation settings as for the \a rhs TextureAttributeItem object.
      * \n\n
      * If the \a rhs TextureAttributeItem has a TextureImage assigned, the TextureImage
      * will not be deep-copied, but the same TextureImage will be assigned to the new
      * instantiated TextureAttributeItem, which in turn increments the reference counter 
      * for that TextureImage. 
      * \n\n 
      * This copy constructor will be invoked by the TextureAttributeItemHandle's clone 
      * member function. */
    NVSG_API TextureAttributeItem( const TextureAttributeItem& rhs );

    /*! \brief Destructs a TextureAttributeItem
      * \remarks This destructor is called when the associated TextureAttributeItemHandle gets 
      * destroyed, that is - when its reference count transfers from 1 to 0.
      * A previously attached TextureImage will be unreferenced, causing it to get
      * deleted if it is no longer referenced anywhere else.
      * \note
      * The calling thread needs to release previously acquired write or read-only accesses
      * to this TextureAttributeItem object prior to unreference the associated 
      * TextureAttributeItemHandle object. Otherwise, this results in undefined behavior. */
    NVSG_API virtual ~TextureAttributeItem();
    
    //! Returns whether the data of this object is shared.
    /** \returns \c true if the object data is shared, \c false otherwise. */
    NVSG_API virtual bool isDataShared( void ) const;

    //! Returns a 64-bit integer value, which uniquely identifies the object data. 
    /** \returns A DataID, that is - a 64-bit integer value, which uniquely identifies the Object data. */
    NVSG_API virtual DataID getDataID( void ) const;

    /*! \brief Returns the TextureImage assigned to the TextureAttributeItem.
      * \return 
      * A pointer to the TextureImageHandle specifying the TextureImage currently assigned to the
      * TextureAttributeItem. The function returns a null pointer if no TextureImage is currently
      * assigned.
      * \remarks
      * A TextureImage can be assigned to the TextureAttributeItem using setTextureImage.
      * \sa setTextureImage
      */
    NVSG_API TextureImageHandle * getTextureImage() const;

    /*! \brief Assigns a TextureImage to the TextureAttributeItem.
     * \param texImg Pointer to the TextureImageHandle specifying the TextureImage to assign.
     * \remarks A previously assigned TextureImage will be unassigned prior to assign the new
     * TextureImage specified by \a texImg. If the previously assigned TextureImage is not 
     * referenced by any other objects, it will be destroyed as it gets unassigned. */
    NVSG_API void setTextureImage(TextureImageHandle * texImg);
    
    /*! \brief Retrieves the file name associated with the attached TextureImage.
     * \return The file name associated with the attached TextureImage. 
     * If no TextureImage is assigned, an empty string will be returned. 
     * \sa setTextureImage, getTextureImage */
    NVSG_API const std::string& getTextureFileName() const;

    /*! \brief Returns the actual border color to be used with this TextureAttributeItem.
     * \returns A 4-component vector (RGBA) specifying the actual border color to be used. 
     * \sa setBorderColor */ 
    NVSG_API const nvmath::Vec4f& getBorderColor( void ) const;

    /*! \brief Sets the border color to be used with this TextureAttributeItem.
     * \param color Specifies the new border color. 
     * \remarks This allows you to alter the border color currently used.
     * The initial border color is black.
     * \sa getBorderColor */
    NVSG_API void setBorderColor( const nvmath::Vec4f &color );

    /*! \brief Returns the actual environment color of this TextureAttributeItem.
     * \return A 4-component vector (RGBA) specifying the current environment color.
     * \sa setEnvColor */
    NVSG_API const nvmath::Vec4f& getEnvColor( void ) const;

    /*! \brief Sets the environment color to be used with this TextureAttributeItem.
     * \param color Specifies the new environment color.
     * \remarks This allows you to alter the environment color currently used.
     * The initial environment color is black.
     * \n\n
     * The environment color is considered only if the TEM_BLEND environment mode
     * also was specified through setEnvMode. The luminance, or color value is
     * used somewhat like an alpha value to blend the fragment's color with the
     * environment color.
     * \sa getEnvColor, setEnvMode */
    NVSG_API void setEnvColor( const nvmath::Vec4f &color );

    /*! \brief Returns the actual used environment mode for this TextureAttributeItem.
     * \returns A TextureEnvMode enum specifying the actual environment mode. 
     * \sa setEnvMode */ 
    NVSG_API TextureEnvMode getEnvMode( void ) const;

    /*! \brief Sets the environment mode to be used with this TextureAttributeItem.
     * \param mode Specifies the new texture environment mode.
     * \remarks This allows you to alter the texture environment mode to use.
     * Accepted modes are TEM_REPLACE, TEM_MODULATE, TEM_DECAL, TEM_BLEND, TEM_ADD, 
     * TEM_ADD_SIGNED, TEM_SUBTRACT, TEM_INTERPOLATE, TEM_DOT3_RGB, and TEM_DOT3_RGBA.
     * The initial texture environment mode is TEM_MODULATE.
     * \sa getEnvMode */
    NVSG_API void setEnvMode( TextureEnvMode mode );

    /*! \brief Returns the RGB scale for this TextureAttributeItem.
     * \returns A TextureEnvScale enum specifying the environment scale. 
     * \sa setEnvScale */ 
    NVSG_API TextureEnvScale getEnvScale( void ) const;

    /*! \brief Returns the RGB scale for this TextureAttributeItem.  
     * \param scale Specifies the new environment scale.
     * \remarks This allows scaling the result of the texture environment operation.  
     * Currently, scaling only works with the following TextureEnv Modes: TEM_ADD_SIGNED, 
     * TEM_SUBTRACT, TEM_INTERPOLATE, TEM_DOT3_RGB, and TEM_DOT3_RGBA. 
     * Accepted values for \a scale are TES_1X, TES_2X, and TES_4X. 
     * The initial scale value is TES_1X, which means, scaling off.
     * \sa getEnvScale */
    NVSG_API void setEnvScale( TextureEnvScale scale );

    /*! \brief Returns the actual filter mode used with magnification.
     * \returns A TextureMagFilterMode enum specifying the actual magnifying filter mode. 
     * \sa setMagFilterMode */ 
    NVSG_API TextureMagFilterMode getMagFilterMode( void ) const;

    /*! \brief Sets the filter mode to use with magnification.
     * \param mode Specifies the new filter mode to use with magnification. 
     * \remarks This allows you to alter the filter mode to use with magnification.
     * Accepted filter modes used for magnification are TFM_MAG_NEAREST, and TFM_MAG_BILINEAR. 
     * The initial filter mode is TFM_MAG_BILINEAR. 
     * \sa getMagFilterMode */
    NVSG_API void setMagFilterMode( TextureMagFilterMode mode );

    /*! \brief Returns the actual filter mode used with minification.
     * \returns A TextureMinFilterMode enum specifying the actual filter mode used with minification. 
     * \sa setMinFilterMode */ 
    NVSG_API TextureMinFilterMode getMinFilterMode( void ) const;

    /*! \brief Sets the filter mode to use with minification.
    * \param mode Specifies the new filter mode to use with minification. 
    * \remarks This allows you to alter the filter mode to use with minification.
    * Accepted filter modes used for minification are TFM_MIN_NEAREST, TFM_MIN_BILINEAR,
    * and TFM_MIN_TRILINEAR. The TFM_MIN_TRILINEAR filter mode requires mipmaps available
    * for the assigned TextureImage. The initial filter mode is TFM_MIN_BILINEAR. 
    * \sa getMinFilterMode */
    NVSG_API void setMinFilterMode( TextureMinFilterMode mode );

    /*! \brief Returns the actual wrap mode used for the specified texture coordinate.
     * \param target Specifies the texture coordinate to query the wrap mode for.
     * Accepted values are TWCA_S, TWCA_T, and TWCA_R.
     * \returns A TextureWrapMode enum specifying the actual wrap parameter. 
     * \sa setWrapMode */
    NVSG_API TextureWrapMode getWrapMode( TexWrapCoordAxis target ) const;

    /*! \brief Sets the wrap mode for the specified texture coordinate.
     * \param target Specifies the texture coordinate.
     * \param mode Specifies the new wrap mode.
     * \remarks This allows you to re-specify the wrap mode to use for the indicated 
     * texture coordinate. Accepted values for the texture coordinate are TWCA_S, TWCA_T,
     * and TWCA_R. Accepted values for the wrap mode are TWM_REPEAT, TWM_CLAMP, TWM_MIRROR.
     * The initial wrap mode is TWM_REPEAT for all texture coordinates. 
     * \sa getWrapMode */
    NVSG_API void setWrapMode( TexWrapCoordAxis target , TextureWrapMode mode);

    /*! \brief Returns the currently specified texture matrix.
     * \returns A 4x4 matrix specifying the actual texture matrix. 
     * \remarks The related texture coordinates are multiplied by the texture matrix
     * before any texture mapping occurs. The initial value for the texture matrix is 
     * the identity matrix. You are allowed to modify the texture matrix through 
     * setOrientation, setScaling, and setTranslation. 
     * \sa setOrientation, setScaling, setTranslation */
    NVSG_API nvmath::Mat44f getMatrix( void ) const;

    /*! \brief Returns the rotation part of the texture matrix.
      * \returns A quaternion specifying the rotation part of the currently specified texture matrix. 
      * \sa getMatrix, setOrientation */
    NVSG_API const nvmath::Quatf & getOrientation( void ) const;

    /*! \brief Sets the rotation part of the texture matrix.
     * \param orientation Specifies the new rotation. 
     * \remarks This allows you to re-specify the rotation to be applied to the related
     * texture coordinates before any texture mapping occurs. 
     * \sa getMatrix, getOrientation. */
    NVSG_API void setOrientation( const nvmath::Quatf & orientation );

    /*! \brief Returns the scaling part of the texture matrix.
     * \returns A 3-component vector specifying the scaling. 
     * \sa getMatrix, setScaling */
    NVSG_API const nvmath::Vec3f & getScaling( void ) const;

    /*! \brief Sets the scaling part of the texture matrix.
    * \param scale Specifies the new scaling. 
    * \remarks This allows you to re-specify the scaling to be applied to the related
    * texture coordinates before any texture mapping occurs. 
    * \sa getMatrix, getScaling. */
    NVSG_API void setScaling( const nvmath::Vec3f &scale );

    /*! \brief Returns the translation part of the texture matrix.
    * \returns A 3-component vector specifying the translation. 
    * \sa getMatrix, setTranslation */
    NVSG_API const nvmath::Vec3f & getTranslation( void ) const;

    /*! \brief Sets the translation part of the texture matrix.
    * \param translation Specifies the new translation. 
    * \remarks This allows you to re-specify the translation to be applied to the related
    * texture coordinates before any texture mapping occurs. 
    * \sa getMatrix, getTranslation. */
    NVSG_API void setTranslation( const nvmath::Vec3f &translation );

    /*! \brief Sets the texture-coordinate generation mode for the specified texture coordinate.
     * \param axis Specifies the texture coordinate for which to respecify the generation mode.
     * \param mode Specifies the texture-coordinate generation mode.
     * \remarks This allows you to re-specify the texture-coordinate generation mode for 
     * the indicated texture coordinate. Accepted values for the texture coordinate specified 
     * by \a axis are TCA_S, TCA_T, TCA_R, and TCA_Q. Accepted values for the texture-coordinate 
     * generation mode are TGM_OFF, TGM_OBJECT_LINEAR, TGM_EYE_LINEAR, TGM_SPHERE_MAP, 
     * TGM_REFLECTION_MAP, and TGM_NORMAL_MAP. The initial value for the texture-coordinate 
     * generation mode is TGM_OFF for all texture coordinates, which means, texture-coordinate 
     * generation off.
     * \n\n
     * If you specify a texture-coordinate generation mode other than TGM_OFF, automatic
     * texture-coordinate generation will be enabled for the particular texture coordinate.
     * \n\n
     * For the TGM_OBJECT_LINEAR and TGM_EYE_LINEAR generation mode, the linear plane, you 
     * are allowed to specify through setTexGenLinearPlane, will be taken into account for
     * automatic texture-coordinate generation.
     * \sa getTexGenMode, setTexGenLinearPlane*/
    NVSG_API void setTexGenMode( TexCoordAxis axis, TexGenMode mode );

    /*! \brief Returns the texture-coordinate generation mode for the specified coordinate. 
     * \returns A TexGenMode enum specifying the current texture-coordinate generation mode
     * used for the specified coordinate. Accepted values for \a axis are TCA_S, TCA_T,
     * TCA_R, and TCA_Q. 
     * \sa setTexGenMode */
    NVSG_API TexGenMode getTexGenMode( TexCoordAxis axis ) const;

    /*! \brief Sets the plane to be used with linear texture-coordinate generation.
      * \param axis Specifies the texture coordinate for which to specify the plane.
      * \param target Specifies the target for which to specify the plane.
      * \param plane Specifies the plane to use with linear texture-coordinate generation. 
      * \remarks This function lets you specify distinct planes to be used with the 
      * linear texture-generation modes TGM_OBJECT_LINEAR, and TGM_EYE_LINEAR. 
      * For other texture-coordinate generation modes, these linear planes are of
      * no relevance. Different planes can be specifies for each available texture-coordinate.
      * Accepted values to specify the texture coordinate, indicated by \a axis,
      * are TCA_S, TCA_T, TCA_R, and TCA_Q. For \a target, TGLP_OBJECT, TGLP_EYE, or
      * TGLP_OBJECT_AND_EYE, are allowed values. If you specifying TGLP_OBJECT_AND_EYE,
      * the specified \a plane will be used for both, the TGM_OBJECT_LINEAR, and the
      * TGM_EYE_LINEAR texture-coordinate generation mode. 
      * \n\n
      * Initial \a plane values for the different texture coordinates, and for both 
      * linear generation modes are as follows:
      * \code
      *     TCA_S = (1, 0, 0, 0)
      *     TCA_T = (0, 1, 0, 0)
      *     TCA_R = (0, 0, 0, 0)
      *     TCA_Q = (0, 0, 0, 0)
      * \endcode
      * 
      * \note Other than setTexGenMode, the function does not automatically enable 
      * texture-coordinate generation. You need to explicitely call setTexGenMode
      * accordingly.
      * \sa getTexGenLinearPlane, setTexGenMode */
    NVSG_API void setTexGenLinearPlane( TexCoordAxis axis, TexGenLinearPlane target, const nvmath::Vec4f& plane );

    /*! \brief Returns the plane used with linear texture-coordinate generation.
     * \param axis Specifies the texture coordinate for which to query the plane. 
     * Accepted values are TCA_S, TCA_t, TCA_R, and TCA_Q.
     * \param plane Specifies the linear generation mode for which to query the plane.
     * Accepted values are TGLP_OBJECT and TGLP_EYE. 
     * \returns a 4-component vector specifying the plane. 
     * \note You must not specify TGLP_OBJECT_AND_EYE for \a plane! 
     * Doing so results in undefined behavior. 
     * \sa setTexGenLinearPlane */
    NVSG_API const nvmath::Vec4f &getTexGenLinearPlane( TexCoordAxis axis, TexGenLinearPlane plane ) const;


    /*! \brief Assigns values from another TextureAttributeItem object.
     * \param rhs Specifies the source TextureAttributeItem object.
     * \returns A reference to the left-hand-sided TextureAttributeItem object. 
     * \remarks This assignment operator will be invoked when assigning an
     * existing TextureAttributeItem object to another TextureAttributeItem object. 
     * \n\n
     * A TextureImage that previously was assigned to the left-hand-sided 
     * TextureAttributeItem, will be unreferenced, causing the TextureImage
     * to get deleted if it is not reference anywhere else. 
     * \n\n
     * If the \a rhs TextureAttributeItem has a TextureImage assigned, the TextureImage
     * will not be deep-copied, but the same TextureImage will be assigned to the 
     * left-hand-sided TextureAttributeItem, which in turn increments the reference counter 
     * for that TextureImage. */
    NVSG_API TextureAttributeItem & operator=(const TextureAttributeItem & rhs);

    /*! \brief Tests for equivalence with an other TextureAttributeItem.
     * \param p Pointer to the object to test for equivalence.
     * \param ignoreNames Object names will be ignored if \c true is specified here.
     * \param deepCompare A deep compare will be performed if \c true is specified here. 
     * This does not apply to a TextureAttributeItem, though, and therefore is ignored.
     * \return true if the TextureAttributeItem \a p is equivalent to this TextureAttributeItem. 
     * \remarks This function actually overrides Object::isEquivalent. This is the reason for
     * the passed object to be of type Object, rather than of type TextureAttributeItem.
     * For all objects of type other than TextureAttributeItem, this function returns \c false.
     * Only objects of type TextureAttributeItem will be considered for further comparison
     * on eqivalence. */
    NVSG_API virtual bool isEquivalent( const Object *p, bool ignoreNames, bool deepCompare ) const;

  protected:
    /*! \brief Invalidates the Incarnation.
      * \remarks This function is called from the framework if this object's 
      * Incarnation changes and a former Incarnation change already was 
      * evaluated by the framework. 
      * \n\n
      * The function iterates over all owners of this TextureAttributeItem and 
      * increases the Incarnation for each. */
    NVSG_API virtual void invalidateIncarnation( void );      // from bottom to top

private:
    void checkIncrementMipmapUseCount(); // conditional incrementation of mipmap use count
    void checkDecrementMipmapUseCount(); // conditional decrementation of mipmap use count

private:
    nvutil::RCPtr<TextureAttributeItemData>   m_sharedData; 
    TextureImageHandle * m_texImg; // texture image specification
  };

  //! Texture attribute
  /** This maintains bindings between abstract texture units and \c TextureAttributeItem objects.
    * For this purpose it provides an interface to bind a certain \c TextureAttributeItem to a 
    * specified texture unit or to unbind a \c TextureAttributeItem from a certain texture unit. 
    * It also provides an interface to iterate through all bounded \c TextureAttributeItem objects.
    * \par
    * As a \c StateAttribute, a \c TextureAttribute can be added to a \c StateSet of a certain 
    * \c GeoNode for texturing purposes. */ 
  class TextureAttribute : public StateAttribute
  {
    public:
      /*! \brief Specifies an invalid binding.
       * \remarks This particular identifier is used as end criteria when
       * iterating through all currently bound TextureAttributeItem objects:
       * \code
       * for ( unsigned int tu = pAttrib->getFirstItem()
       *     ; tu != TextureAttribute::INVALID_ITEM
       *     ; tu = pAttrib->getNextItem(tu))
       * {
       * }
       * \endcode 
       * \sa getFirstItem, getNextItem */
      enum { INVALID_ITEM = ~0 }; // note: used with unsigned comparison, so don't use -1

      /*! \brief Default-constructs a TextureAttribute.
       * \remarks A TextureAttribute will be instantiated and default constructed by creating 
       * an associated TextureAttributeHandle through CreateHandle(TextureAttributeHandle), 
       * which returns a pointer to a TextureAttributeHandle as follows:
       * \code
       *     TextureAttributeHandle * hTexAttrib = 
       *       CreateHandle(TextureAttributeHandle); 
       * \endcode
       * A default constructed TextureAttribute initially has no TextureAttributeItem bound.
       * \n\n
       * To access the TextureAttribute API, you need to explicitely acquire access 
       * for either write or read-only operations on the object. The NVSG SDK provides 
       * the convenient helper templates WritableObject and ReadableObject to serve 
       * this purpose. These two helper templates are defined in the nvutil namespace. 
       * Instantiated objects of these template classes can be used similar as an ordinary 
       * pointer:
       * \code
       *     // generate a TextureAttributeItem to be bound to the TextureAttribute below
       *     TextureAttributeItemHandle * hTexAttribItem =
       *       CreateHandle(TextureAttributeItemHandle);
       *
       *     // acquire write access to the TextureAttributeItem to assign a TextureImage
       *     { // limit write access to this scope
       *       WritableObject<TextureAttributeItem> texAttribItem(hTexAttribItem);
       *       texAttribItem->setTextureImage(
       *         TextureImage::createFromFile("sunset.jpg", std::vector<std::string>()) );
       *       [...]
       *     } // write access will be released after leaving this scope
       * 
       *     // acquire write access to the TextureAttribute to bind the 
       *     // TextureAttributeItem created before
       *     { // limit write access to this scope
       *       WritableObject<TextureAttribute> texAttrib(hTexAttrib);
       *       texAttrib->bindTextureAttributeItem( hTexAttribItem, 0 );
       *     } // write access will be released after leaving this scope
       * \endcode */
      NVSG_API TextureAttribute( void );

      /*! \brief Constructs a TextureAttribute from a StateAttribute.
       * \param rhs Specifies the source StateAttribute.
       * \remarks This is kind of a 'partial' copy constructor. 
       * Partial, because only the StateAttribute part of the new instantiated TextureAttribute
       * is copied from the \a rhs StateAttribute. The TextureAttribute specific data will be
       * default-initialized. 
       * \n\n
       * This partial copy constructor will be invoked by the CreateHandleFromBase macro. */
      NVSG_API TextureAttribute( const StateAttribute &rhs );
      
      /*! \brief Constructs a TextureAttribute as a copy from another TextureAttribute.
       * \param rhs Specifies the source TextureAttribute object.
       * \remarks This copy constructor will be invoked by the TextureAttributeHandle's 
       * clone member function. */
      NVSG_API TextureAttribute( const TextureAttribute& rhs );

      /*! \brief Destructs a TextureAttribute
       * \remarks This destructor is called when the associated TextureAttributeHandle gets 
       * destroyed, that is - when its reference count transfers from 1 to 0.
       * All bound TextureAttributeItems will be unbound, and hence, unreferenced, causing 
       * them to get deleted if they are no longer referenced anywhere else.
       * \note
       * The calling thread needs to release previously acquired write or read-only accesses
       * to this TextureAttribute object prior to unreference the associated TextureAttributeHandle 
       * object. Otherwise, this results in undefined behavior. */
      NVSG_API virtual ~TextureAttribute( void );

      /*! \brief Binds an existing TextureAttributeItem.
       * \param item Specifies the TextureAttributeItem object to be bound.
       * \param unit Specifies the zero-based texture unit, where to bind the TextureAttributeItem object.
       * \remarks This allows you to bind a TextureAttributeItem to a specified texture unit. 
       * If another TextureAttributeItem previously was bound to the same unit, it will be unbound,
       * and hence, unreferenced, causing it to get deleted if it is no longer referenced anywhere else.
       * \note
       * If \a unit exceeds the number of texture units provided by your current graphics system,
       * the bound TextureAttributeItem will not be processed by the renderer.
       * \sa unbindTextureAttributeItem */
      NVSG_API void bindTextureAttributeItem( TextureAttributeItemHandle* item, unsigned int unit );
      
      /*! \brief Unbinds a TextureAttributeItem.
       * \param unit Specifies the texture unit to unbind a currently bound TextureAttributeItem from.
       * \remarks This allows you to unbind the TextureAttributeItem that was last bound to the 
       * specified texture unit through bindTextureAttributeItem. The TextureAttributeItem then 
       * will be unreferenced, causing it to get deleted if no longer referenced anywhere else. 
       * Immediately after this call, no TextureAttributeItem is bound to that unit. 
       * \sa bindTextureAttributeItem */
      NVSG_API void unbindTextureAttributeItem( unsigned int unit );

      /*! \a Retrieves the first zero-based texture unit a TextureAttributeItem is bound to.
       * \returns The function returns a zero-based identifier indicating the first texture unit 
       * a TextureAttributeItem is bound to, or INVALID_ITEM, if currently no TextureAttributeItem 
       * is bound.
       * \remarks Use this to initiate the iteration of all bound TextureAttributeItems. This function
       * returns the first texture unit found with a valid binding. After that, use getNextItem
       * to iterate through all remaining bindings. 
       * \n\n
       * Use getTextureAttributeItem to receive the bound TextureAttributeItem from the texture unit
       * returned by getFirstItem, or getNextItem, respectively.
       * \code
       *    // assume hTexAttrib a pointer to a valid TextureAttributeItemHandle
       *
       *    // NOTE: 
       *    // we need to write-lock the TextureAttribute here, because the 
       *    // edit operations performed to the 'owned' TextureAttributeItem
       *    // objects below, will cause the incarnation count of the owner
       *    // to increase, which in turn requires write-access to the owner.
       *    // Using ReadableObjects instead, would result in a deadlock on
       *    // a multithreaded configuration!
       *    WritableObject<TextureAttribute> texAttrib(hTexAttrib);
       *
       *    for ( unsigned int tu = texAttrib->getFirstItem()
       *         ; tu != TextureAttribute::INVALID_ITEM
       *          ; tu = texAttrib->getNextItem(tu) )
       *    {
       *        WritableObject<TextureAttributeItem> theItem(texAttrib->getTextureAttributeItem(tu));
       *        theItem->setEnvMode(TEM_BLEND);
       *        theItem->setEnvColor(Vec4f(0.2f, 0.2f, 0.2f, 0.5f));
       *    }
       * \endcode
       * \sa getNextItem, INVALID_ITEM */
      NVSG_API unsigned int getFirstItem( void ) const;

      /*! \brief Retrieves the very next texture unit a TextureAttributeItem is bound to.
       * \param unit Specifies the texture unit returned by a previous getFirstItem, or getNextItem.
       * \returns The function returns a zero-based identifier indicating the next texture unit,
       * following \a unit, a TextureAttributeItem is bound to, or INVALID_ITEM, if no such 
       * unit is available.
       * \remarks Use this in a forward iteration loop after you established the very first
       * texture unit, a TextureAttributeItem is bound to, through getFirstItem. 
       * \sa getFirstItem */
      NVSG_API unsigned int getNextItem( unsigned int unit ) const;

      /*! \brief Retrieves the number of bound TextureAttributeItem objects.
       * \returns The number of TextureAttributeItem objects currently bound. 
       * \remarks You must not use this to forward iterate through the current bindings,
       * because there might be gaps in the chain of zero-based texture units. 
       * If for example two TextureAttributeItem objects are bound in total, and assume
       * the TextureAttributeItem objects are bound to texture unit 2 and 3, getNumberOfItems
       * will return 2, but getTextureAttributeItem(unit) will return NULL for unit < 2.
       * Instead you should use getFirstItem, getNextItem to forward iterate to all
       * valid bindings.
       * \sa getTextureAttributeItem, getFirstItem, getNextItem */
      NVSG_API size_t getNumberOfItems( void ) const;

      /*! \brief Retrieves the TextureAttributeItem bound to the specified texture unit. 
       * \params unit Specifies the texture unit to query.
       * \returns A pointer to the TextureAttributeItemHandle representing the TextureAttributeItem
       * bound to the specified texture unit, or NULL if no TextureAttributeItem is bound to that unit. */
      NVSG_API TextureAttributeItemHandle * getTextureAttributeItem( unsigned int ) const;

      /*! \brief Assigns values from another TextureAttribute object.
      * \param rhs Specifies the source TextureAttribute object.
      * \returns A reference to the left-hand-sided TextureAttribute object. 
      * \remarks This assignment operator will be invoked when assigning an 
      * existing TextureAttribute object to another TextureAttribute object. 
      * \n\n
      * All TextureAttributeItem objects currently bound to the left-hand-sided
      * TextureAttribute will be unbound prior to re-assigning the new bindings
      * from the right-hand-sided TextureAttribute. This will unreference all the
      * previously bound TextureAttributeItem objects, causing them to get deleted
      * if no longer referenced anywhere else. */
      NVSG_API TextureAttribute & operator=(const TextureAttribute & rhs);

      /*! \brief Tests for equivalence with an other TextureAttribute.
      * \param p Pointer to the object to test for equivalence.
      * \param ignoreNames Object names will be ignored if \c true is specified here.
      * \param deepCompare A deep compare will be performed if \c true is specified here. 
      * \return true if the TextureAttribute \a p is equivalent to this TextureAttribute. 
      * \remarks This function actually overrides Object::isEquivalent. This is the reason for
      * the passed object to be of type Object, rather than of type TextureAttribute.
      * For all objects of type other than TextureAttribute, this function returns \c false.
      * Only objects of type TextureAttribute will be considered for further comparison
      * on eqivalence. 
      * \n\n
      * If \c true is specified for the \a deepCompare parameter, the function will iterate
      * through all valid bindings of both TextureAttribute objects, and in turn performs
      * an equivalence test on each pair of TextureAttributeItem objects. If instead \c false
      * is specified for the \a deepCompare parameter, the function just performs a fast
      * pointer-compare on the current bindings. 
      * \sa TextureAttributeItem::isEquivalent */
      NVSG_API virtual bool isEquivalent( const Object *p, bool ignoreNames, bool deepCompare ) const;

    protected:
      
      /*! \brief Validates the Incarnation state down the hierarchy.
       * \remarks This function is called from the framework to mark the Incarnation state as
       * valid down the hierarchy. This implementation in particular iterates over all
       * bound TextureAttributeItem objects and marks their Incarnation state as valid.
       * \n\n
       * If, for a certain object, the Incarnation state is marked as valid, the very next
       * increaseIncarnation will iterate over all owners of that object to increase the
       * Incarnation for each. 
       * \sa Object::increaseIncarnation */
      NVSG_API virtual void validateIncarnation( void ) const;  // from top to bottom

    private: // implementation helpers
      void _clearBindings();

    private:
      typedef std::map<unsigned int, TextureAttributeItemHandle*>::const_iterator ConstBindingIterator;
      typedef std::map<unsigned int, TextureAttributeItemHandle*>::iterator BindingIterator;
      std::map<unsigned int, TextureAttributeItemHandle*> m_bindings; // item bindings 
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - TextureAttribute
  inline unsigned int TextureAttribute::getFirstItem( void ) const
  {
    NVSG_TRACE();
    return !m_bindings.empty() ? m_bindings.begin()->first : INVALID_ITEM;
  }

  inline unsigned int TextureAttribute::getNextItem( unsigned int currItem ) const
  {
    NVSG_TRACE();
    if (currItem!=INVALID_ITEM)
    {
      ConstBindingIterator ci = m_bindings.upper_bound(currItem);
      return ci!=m_bindings.end() ? ci->first : INVALID_ITEM;
    }
    return INVALID_ITEM;
  }

  inline size_t TextureAttribute::getNumberOfItems( void ) const
  {
    NVSG_TRACE();
    return m_bindings.size();
  }

  inline TextureAttributeItemHandle* TextureAttribute::getTextureAttributeItem( unsigned int unit ) const
  {
    NVSG_TRACE();
    ConstBindingIterator ci = m_bindings.find(unit);
    return ci!=m_bindings.end() ? (*ci).second : NULL;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - TextureAttributeItem
  inline TextureImageHandle * TextureAttributeItem::getTextureImage() const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return m_texImg;
  }

  inline const nvmath::Vec4f& TextureAttributeItem::getBorderColor( void ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT_OBJECT_READ_LOCKED(this);
    return( m_sharedData->m_borderColor );
  }

  inline const nvmath::Vec4f & TextureAttributeItem::getEnvColor( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_envColor );
  }

  inline void TextureAttributeItem::setEnvColor( const nvmath::Vec4f & envColor )
  {
    NVSG_TRACE();
    nvutil::writeAccess(m_sharedData);
    m_sharedData->m_envColor = envColor;
    increaseIncarnation();
  }

  inline TextureEnvMode TextureAttributeItem::getEnvMode( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_envMode );
  }

  inline TextureEnvScale TextureAttributeItem::getEnvScale( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_envScale );
  }

  inline TextureWrapMode TextureAttributeItem::getWrapMode( TexWrapCoordAxis target ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(target==TWCA_S||target==TWCA_T||target==TWCA_R);
    return( m_sharedData->m_wrapMode[target] );
  }

  inline TextureMinFilterMode TextureAttributeItem::getMinFilterMode( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_minFilterMode );
  }

  inline TextureMagFilterMode TextureAttributeItem::getMagFilterMode( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_magFilterMode );
  }

  inline  nvmath::Mat44f  TextureAttributeItem::getMatrix( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_trafo.getMatrix() );
  }

  inline const nvmath::Quatf & TextureAttributeItem::getOrientation( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_trafo.getOrientation() );
  }

  inline const nvmath::Vec3f & TextureAttributeItem::getScaling( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_trafo.getScaling() );
  }

  inline const nvmath::Vec3f & TextureAttributeItem::getTranslation( void ) const
  {
    NVSG_TRACE();
    return( m_sharedData->m_trafo.getTranslation() );
  }

  inline TexGenMode TextureAttributeItem::getTexGenMode( TexCoordAxis axis ) const
  {
    return( m_sharedData->m_texGenMode[axis] );
  }

  inline const nvmath::Vec4f &TextureAttributeItem::getTexGenLinearPlane( TexCoordAxis axis, TexGenLinearPlane target ) const
  {
    NVSG_ASSERT(target==TGLP_OBJECT || target==TGLP_EYE);
    return( m_sharedData->m_texGenPlane[target][axis] );
  }
}
