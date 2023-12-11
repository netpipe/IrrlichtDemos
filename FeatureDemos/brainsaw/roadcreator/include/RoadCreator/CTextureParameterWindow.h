#ifndef _C_TEXTURE_PARAMETER_WINDOW
  #define _C_TEXTURE_PARAMETER_WINDOW

  #include <irrlicht.h>
  #include <CDialogWindow.h>

using namespace irr;

class CTextureParameters;
class CMaterialManager;
class ITextureModified;

/**
 * @class CTextureParameterWindow
 * @author Christian Keimel / bulletbyte.de
 * This class provides a dialog to modify the texture parameters
 */
class CTextureParameterWindow : public CDialogWindow {
  protected:
    u32 m_iTextureSelectState;    /**<! state that indicates whether or not a texture is currently being selected */

    gui::IGUIEnvironment *m_pGuienv;  /**<! the Irrlicht GUI environment */

    gui::IGUIButton     *m_pTextureSelect;  /**<! the "select texture" button */
    gui::IGUIStaticText *m_pTextureName;    /**<! the "texture name" text */
    gui::IGUIComboBox   *m_pTextureRotate;  /**<! the "texture rotation" combobox */
    gui::IGUICheckBox   *m_pTextureStretch; /**<! the "stretch texture" checkbox */
    gui::IGUISpinBox    *m_pTextureOffsetX, /**<! the "texture offset X" spinbox */
                        *m_pTextureOffsetY, /**<! the "texture offset Y" spinbox */
                        *m_pTextureScaleX,  /**<! the "X texture scale" spinbox */
                        *m_pTextureScaleY;  /**<! the "Y texture scale" spinbox */

    CTextureParameters *m_pParameters;    /**<! the current texture parameters object */

    core::list<ITextureModified *> m_lNotify; /**<! list of registered notification listeners */
    CMaterialManager *m_pMaterials;           /**<! the "select texture" dialog */

    void updateUI();    /**<! update the UI */
    void notify();      /**<! notify all listeners of a change */

  public:
    /**
     * The constructor
     * @param pGuienv the Irrlicht GUI environment
     * @param pMaterials the instance of the "select texture" dialog
     * @see CMaterialManager
     */
    CTextureParameterWindow(gui::IGUIEnvironment *pGuienv, CMaterialManager *pMaterials);
    
    /**
     * Get the currently assigned Texture Parameters object
     * @return the currently assigned texture parameters object
     * @see CTextureParameters
     */
    CTextureParameters *getParameters() { return m_pParameters; }

    /**
     * Set the currently assigned Texture Parameters object
     * @param p the new texture parameters object
     * @see CTextureParameters
     */
    void setParameters(CTextureParameters *p) {
      m_pParameters=p;
      updateUI();
    }
    
    /**
     * Add a notification listener
     * @param p notification listener to add
     */
    void addNotify(ITextureModified *p);

    /**
     * Remove a notification listener
     * @param p notification listener to remove
     */
    void delNotify(ITextureModified *p);
    
    /**
     * Irrlicht event receiver callback
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * Set the visibility of the dialog
     * @b the new flag
     */
    virtual void setVisible(bool b) {
      m_iTextureSelectState=0;
      m_pWindow->setVisible(b);
    }
    
    /**
     * Get the name of the dialog to load/save the size
     * @return the name of the dialog
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc s="TextureParameterWindow";
      return s;
    }
};

#endif
