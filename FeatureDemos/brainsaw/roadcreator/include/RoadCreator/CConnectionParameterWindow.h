#ifndef _C_CONNECTION_PARAMETER_WINDOW
  #define _C_CONNECTION_PARAMETER_WINDOW

  #include <irrlicht.h>

  #include <CDialogWindow.h>

using namespace irr;

class CTextureParameterWindow;
class CConnection;
class CSegment;

/**
 * @class CConnectionParameterWindow
 * @author Christian Keimel / bulletbyte.de
 * This class implements the window used to edit parameters of a connection.
 * @see CDialogWindow
 * @see ITextureModified
 */
class CConnectionParameterWindow : public CDialogWindow, public ITextureModified {
  protected:
    gui::IGUIButton     *m_pUseAsFirst,   /**<! the "use as first segment" button */
                        *m_pUseAsSecond,  /**<! the "use as second segment" button */
                        *m_pEdTexture;    /**<! "edit texture" button */
    gui::IGUIStaticText *m_pSegOne,       /**<! "First Segment" textfield */
                        *m_pSegTwo;       /**<! "Second Segment" textfield */
    gui::IGUISpinBox    *m_pBorder1,      /**<! Spinbox to chose border from first segment */
                        *m_pBorder2,      /**<! Spinbox to chose border from second segment */
                        *m_pSteps,        /**<! Spinbox for the number of steps to create */
                        *m_pOffset,       /**<! Spinbox to specify the base offset */
                        *m_pPointOff[4],  /**<! offset of the bezier points */
                        *m_pWallHeight,   /**<! Height of border wall */
                        *m_pWallWidth;    /**<! Width of the border wall */
    gui::IGUIComboBox   *m_pType,         /**<! Combobox for the connection type (Bezier1..3) */
                        *m_pTextures;     /**<! Combobox for the texture number (road, bottom, front, back, left, right */
    gui::IGUICheckBox   *m_pFlip,         /**<! "Flip connection" checkbox */
                        *m_pFlipVerts,    /**<! "Flip vertices" checkbox */
                        *m_pWalls[2];     /**<! add walls to the connection? */
    gui::IGUIEditBox    *m_pPoint[4];     /**<! Editboxes for the help points */

    CConnection *m_pSelectedCon;  /**<! the selected connection */
    CSegment    *m_pSelectedSeg;  /**<! the selected segment (can be used as first or second segment) */

    CTextureParameterWindow *m_pTextureParams;  /**<! the "Texture Parameters" window */

  public:
    /**
     * The constructor
     * @param pGuienv the Irrlicht GUI environment
     * @param pTex the "edit texture parameters" dialog
     */
    CConnectionParameterWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex);
    
    /**
     * Update the UI (copy changes from the actual connection object)
     */
    void updateUI();
    
    /**
     * Update the data (copy changes to the actual connection object). Please
     * note that in this case there are special updates done when an event from
     * a GUI element is processed, this method just updates the help points.
     */
    void updateData();
    
    /**
     * Set the selected connection
     * @param p the new selected connection
     */
    void setSelectedConnection(CConnection *p);
    
    /**
     * Set the selected segment
     * @param p the new selected segment
     */
    void setSelectedSegment(CSegment *p);
    
    /**
     * Irrlicht event callback
     * @param event the event from Irrlicht
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * A callback for notifications about texture modifications
     * @see CTextureParameterWindow::notify
     */
    virtual void textureModified();
    
    /**
     * Return the name of this window for saving the position
     * @return the name of this window for saving the position
     */
    virtual const core::stringc &getSaveName();
};

#endif
