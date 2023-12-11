#ifndef _C_DIALOG_WINDOW
  #define _C_DIALOG_WINDOW

  #include <irrlicht.h>

using namespace irr;

/**
 * @class CDialogWindow
 * @author Christian Keimel / bulletbyte.de
 * This class is the parent of all dialog windows used in the application.
 * It provides some useful features like saving and loading the position
 * of the window.
 * @see CMaterialManager
 * @see CRoadDialog
 * @see CSegmentParameterWindow
 * @see CConectionParameterWindow
 * @see CTextureParameterWindow
 */
class CDialogWindow : public IEventReceiver {
  protected:
    gui::IGUIEnvironment *m_pGuiEnv;  /**<! the Irrlicht GUI environment */
    gui::IGUIWindow      *m_pWindow;  /**<! the actual window */

  public:
    /**
     * The constructor
     * @param pGuiEnv the Irrlicht GUI environment
     * @param cPos the position of the window
     * @param cDim the dimension of the window
     * @param bModal flag to make the window modal (or not)
     * @param sText title of the window
     */
    CDialogWindow(gui::IGUIEnvironment *pGuiEnv, const core::position2di &cPos, const core::dimension2du &cDim, bool bModal, const wchar_t *sText) {
      m_pGuiEnv=pGuiEnv;
      m_pWindow=m_pGuiEnv->addWindow(core::rect<s32>(cPos,cDim),bModal,sText);
    }
    
    /**
     * Set the visibility of the window
     * @param b the new value
     */
    virtual void setVisible(bool b) { m_pWindow->setVisible(b); }
    
    /**
     * Query the visibility of the window
     * @return the visibility of the window
     */
    virtual bool isVisible() { return m_pWindow->isVisible(); }
    
    /**
     * Irrlicht event receiver callback
     * @param event the event to be handled
     */
    virtual bool OnEvent(const SEvent &event) { return false; }
    
    /**
     * This method returns the string that's used by the main
     * routine to save the position of this window
     * @return the name of this window for the savefile
     */
    virtual const core::stringc &getSaveName()=0;
    
    /**
     * Load the window's position
     * @param in the input attributes
     */
    virtual void load(io::IAttributes *in) {
      core::position2di cPos;
      cPos.X=in->getAttributeAsInt("left");
      cPos.Y=in->getAttributeAsInt("top");

      m_pWindow->setRelativePosition(cPos);
    }
    
    /**
     * Save the window's position
     * @param out the output attributes
     */
    virtual void save(io::IAttributes *out) {
      core::position2di cPos=m_pWindow->getRelativePosition().UpperLeftCorner;
      out->addInt("left",cPos.X);
      out->addInt("top" ,cPos.Y);
    }
};

#endif
