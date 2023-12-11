#ifndef _C_TEXTURE_PARAMETERS
  #define _C_TEXTURE_PARAMETERS

  #include <irrlicht.h>

using namespace irr;

/**
 * @class CTextureParameters
 * @author Christian Keimel / bulletbyte.de
 * This class is used to store all necessary texture parameters
 */
class CTextureParameters {
  protected:
    core::stringc m_sTexture;   /**<! the texture */
    u32 m_iRotate;              /**<! the texture rotation (0=0 degrees, 1=90 degrees, 2=180 degrees, 3=270 degrees) */
    f32 m_fOffsetX,             /**<! X-offset of the texture */
        m_fOffsetY,             /**<! Y-offset of the texture */
        m_fScaleX,              /**<! X-scale of the texture */
        m_fScaleY;              /**<! Y-scale of the texture */
    bool m_bStretch;            /**<! flag to stretch the texture to fit */

  public:
    /**
     *  The constructor
     */
    CTextureParameters() {
      m_sTexture="";
      m_iRotate=0;
      m_fOffsetX=0.0f;
      m_fOffsetY=0.0f;
      m_fScaleX=1.0f;
      m_fScaleY=1.0f;
      m_bStretch=false;
    }

    /**
     * Set the texture file
     * @param sTexture the texture
     */
    void setTexture(const core::stringc &sTexture) {
      m_sTexture=sTexture;
    }

    /**
     * Set the texture rotation
     * @param i the texture rotation (0=0 degrees, 1=90 degrees, 2=180 degrees, 3=270 degrees)
     */
    void setRotate(u32 i) {
      m_iRotate=i;
    }

    /**
     * Set the offset of the texture
     * @param f the offset of the texture
     */
    void setOffsetX(f32 f) {
      m_fOffsetX=f;
    }

    void setOffsetY(f32 f) {
      m_fOffsetY=f;
    }

    /**
     * Set the scale of the texture
     * @param f the new scale of the texture
     */
    void setScaleX(f32 f) {
      m_fScaleX=f;
    }

    void setScaleY(f32 f) {
      m_fScaleY=f;
    }

    /**
     * Set the stretch flag of the texture
     * @param b the new value for the stretch flag
     */
    void setStretch(bool b) {
      m_bStretch=b;
    }

    /**
     * Get the file path of the texture
     * @return the file path of the texture
     */
    const core::stringc &getTexture() {
      return m_sTexture;
    }

    /**
     * Get the rotation of the texture
     * @return the rotation of the texture (0=0 degrees, 1=90 degrees, 2=180 degrees, 3=270 degrees)
     */
    u32 getRotate() {
      return m_iRotate;
    }

    /**
     * Get the offset of the texture
     * @return the offset of the texture
     */
    f32 getOffsetX() {
      return m_fOffsetX;
    }

    f32 getOffsetY() {
      return m_fOffsetY;
    }

    /**
     * Get the scale of the texture
     * @return the scale of the texture
     */
    f32 getScaleX() {
      return m_fScaleX;
    }

    f32 getScaleY() {
      return m_fScaleY;
    }

    /**
     * Get the stretch flag of the texture
     * @return the stretch flag of the texture
     */
    bool getStretch() {
      return m_bStretch;
    }

    /**
     * Save the texture parameters to an attribute object
     * @param out the attribute object
     */
    void save(io::IAttributes *out) {
      out->addString("Texture",m_sTexture.c_str());

      out->addInt("Rotate",m_iRotate);

      out->addFloat("OffsetX",m_fOffsetX);
      out->addFloat("OffsetY",m_fOffsetY);
      out->addFloat("ScaleX" ,m_fScaleX );
      out->addFloat("ScaleY" ,m_fScaleY );

      out->addBool("Stretch",m_bStretch);
    }

    /**
     * Load the texture parameters from an attribute object
     * @param out the attribute object
     */
    void load(io::IAttributes *in) {
      m_sTexture=in->getAttributeAsString("Texture");

      m_iRotate=in->getAttributeAsInt("Rotate");

      if (in->existsAttribute("OffsetX")) m_fOffsetX=in->getAttributeAsFloat("OffsetX"); else m_fOffsetX=1.0f;
      if (in->existsAttribute("OffsetY")) m_fOffsetY=in->getAttributeAsFloat("OffsetY"); else m_fOffsetY=1.0f;

      if (in->existsAttribute("ScaleX"))  m_fScaleX=in->getAttributeAsFloat("ScaleX" ); else m_fScaleX=1.0f;
      if (in->existsAttribute("ScaleY"))  m_fScaleY=in->getAttributeAsFloat("ScaleY" ); else m_fScaleY=1.0f;

      m_bStretch=in->getAttributeAsBool("Stretch");
    }

    void copyTo(CTextureParameters *pDest) {
      pDest->setTexture(m_sTexture);
      pDest->setRotate (m_iRotate );
      pDest->setOffsetX(m_fOffsetX);
      pDest->setOffsetY(m_fOffsetY);
      pDest->setScaleX (m_fScaleX );
      pDest->setScaleY (m_fScaleY );
      pDest->setStretch(m_bStretch);
    }
};

#endif
