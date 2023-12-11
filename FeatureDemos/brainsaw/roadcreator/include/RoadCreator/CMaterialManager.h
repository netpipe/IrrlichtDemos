#ifndef _C_MATERIAL_MANAGER
  #define _C_MATERIAL_MANAGER

  #include <irrlicht.h>
  #include <CDialogWindow.h>

using namespace irr;

/**
 * @class CMaterialManager
 * @author Christian Keimel / bulletbyte.de
 * This class is the implementation of the dialog that
 * allows the user to pick a texture.
 * @see CDialogWindow
 */
class CMaterialManager : public CDialogWindow {
  protected:
    core::array<core::stringc> m_aMaterials;    /**<! list of textures */
    core::stringc m_sSelectedFile,              /**<! selected texture */
                  m_sTextureFolder;             /**<! folder to scan for textures */

    IrrlichtDevice       *m_pDevice;      /**<! the Irrlicht device */
    video::IVideoDriver  *m_pDrv;         /**<! the Irrlicht video driver */
    gui::IGUIEnvironment *m_pGuienv;      /**<! the Irrlicht GUI environment */
    gui::IGUIListBox     *m_pTextures;    /**<! GUI list of textures */
    gui::IGUIImage       *m_pTexture;     /**<! the image that shows the selected texture */
    gui::IGUIButton      *m_pSelect;      /**<! the "use this texture" button */

    void updateList();    /**<! update the list of textures */
    void updateTexture(); /**<! update the selected texture */

  public:
    /**
     * The constructor
     * @param pDevice the used Irrlicht device
     */
    CMaterialManager(IrrlichtDevice *pDevice);
    
    /**
     * Get the number of materials (textures)
     * @return the number of materials (textures)
     */
    u32 getMaterialCount();
    
    /**
     * Get a texture at a specific index
     * @param iIdx the index
     * @return the texture at the index
     */
    const core::stringc &getMaterial(u32 iIdx);
    
    /**
     * Irrlicht event receiver callback
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * Overloaded method to change visibility of the dialog
     * @param b the new visible flag
     */
    virtual void setVisible(bool b);
    
    /**
     * Query the selected texture
     * @return name of the selected texture
     */
    const core::stringc &getSelectedTexture();
    
    /**
     * Set the folder to scan for textures
     * @param sFolder the folder to scan for textures
     */
    void setTextureFolder(const core::stringc sFolder);
    
    /**
     * Set the selected texture
     * @param sTexture the selected texture
     */
    void setSelectedTexture(const core::stringc sTexture);
    
    /**
     * Get the name of the window for saving the position
     * @return the name of the window for saving the position
     */
    virtual const core::stringc &getSaveName();
};

#endif
