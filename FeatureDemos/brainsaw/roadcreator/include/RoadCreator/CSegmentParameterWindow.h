#ifndef _C_SEGMENT_PARAMETER_WINDOW
  #define _C_SEGMENT_PARAMETER_WINDOW

  #include <irrlicht.h>
  #include <INotification.h>
  #include <CDialogWindow.h>

using namespace irr;

class CSegment;
class CMaterialManager;
class CTextureParameterWindow;

/**
 * @class CSegmentParameterWindow
 * @author Christian Keimel / bulletbyte.de
 * This is the dialog for the segment parameters
 */
class CSegmentParameterWindow : public CDialogWindow, public ITextureModified {
  protected:
    gui::IGUIEditBox  *m_pName,         /**<! editfield for the (unique) name of the segment */
                      *m_pWidth,        /**<! editfield for the width of the segment */
                      *m_pLength,       /**<! editfield for the length of the segment */
                      *m_pPosition,     /**<! editfield for the position of the segment */
                      *m_pDirection,    /**<! editfield for the direction of the segment */
                      *m_pNormal;       /**<! editfield for the normal vector of the segment */
    gui::IGUIComboBox *m_pTexture;      /**<! combobox to select which texture to modify */
    gui::IGUIButton   *m_pEdTexture;    /**<! button to modify a texture */
    gui::IGUISpinBox  *m_pBaseOffset,   /**<! spinbox for the offset (i.e. height of the base) */
                      *m_pWallHeight,   /**<! height of the walls to create */
                      *m_pWallWidth;    /**<! width of the walls to create */
    gui::IGUICheckBox *m_pLevelBase,    /**<! checkbox to make sure all base vertices are in the same height */
                      *m_pNormalBase,   /**<! basement in normal direction of segment? */
                      *m_pNormalWall,   /**<! wall in normal direction of segment? */
                      *m_pWalls[4],     /**<! create walls on the sides of the segment? */
                      *m_pCorner[4];    /**<! create the cornerstones of the segment? */

    CSegment *m_pSegment;   /**<! currently selected segment */

    CTextureParameterWindow *m_pTextureParams;  /**<! "Texture Parameters" dialog */

  public:
    /** 
     * The constructor
     * @param pGuienv Irrlicht GUI environment
     * @param pTex the "
     */
    CSegmentParameterWindow(gui::IGUIEnvironment *pGuienv, CTextureParameterWindow *pTex);
    
    /**
     * Update the UI (Segment object --> UI)
     */
    void updateUI();
    
    /**
     * Update the segment object (UI --> Segment object)
     */
    void updateSegment();
    
    /**
     * Set the selected segment
     * @param p the new selected segment
     */
    void setSelectedSegment(CSegment *p) {
      m_pSegment=p;
      updateUI();
    }
    
    /**
     * The callback of the Irrlicht event receiver
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * The "texture modified" callback
     * @see ITextureModified
     */
    virtual void textureModified() {
      if (m_pSegment!=NULL) m_pSegment->update();
    }

    /**
     * Get the name of the dialog to load/save the size
     * @return the name of the dialog
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc s="SegmentParameterWindow";
      return s;
    }
};

#endif
