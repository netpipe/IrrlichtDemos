#ifndef _C_SURFACE_WINDOW
  #define _C_SURFACE_WINDOW
  
  #include <irrlicht.h>
  #include <INotification.h>
  #include <CDialogWindow.h>
  
using namespace irr;

class CSurface;
class CTextureParameterWindow;
  
class CSurfaceWindow : public CDialogWindow, public ITextureModified {
  protected:
    gui::IGUIEditBox  *m_pOffset,
                      *m_pStart_X,
                      *m_pStart_Y,
                      *m_pEnd_X,
                      *m_pEnd_Y,
                      *m_pFences,
                      *m_pTexR_X,
                      *m_pTexR_Y;
    gui::IGUIButton   *m_pCreate,
                      *m_pRemove,
                      *m_pTexture;
    gui::IGUIComboBox *m_pTextureSelect;
    
    CSurface *m_pSurface;
        
    void updateSurface();
    CTextureParameterWindow *m_pTextureParams;
    
  public:
    CSurfaceWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex);
    
    /**
     * Irrlicht event receiver callback
     * @param event the event to be handled
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * This method returns the string that's used by the main
     * routine to save the position of this window
     * @return the name of this window for the savefile
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc sSaveName="SurfaceWindow";
      return sSaveName;
    }
    
    void setSurface(CSurface *p);
    
    CSurface *getSurface() { return m_pSurface; }
    
    virtual void textureModified() { updateSurface(); }
};

#endif
