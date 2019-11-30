#ifndef _C_ROAD_DIALOG
  #define _C_ROAD_DIALOG

  #include <irrlicht.h>

  #include <CDialogWindow.h>

using namespace irr;

/**
 * @class CRoadDialog
 * @author Christian Keimel / bulletbyte.de
 * This is the dialog to load/save a road
 */
class CRoadDialog : public CDialogWindow {
  protected:
    bool m_bLoad;                 /**<! is it a "load" dialog? */
    gui::IGUIListBox *m_pList;    /**<! list of found files */
    gui::IGUIButton  *m_pOk,      /**<! the "OK" button */
                     *m_pCancel;  /**<! the "Cancel" button */
    gui::IGUIEditBox *m_pEdName;  /**<! the "filename" edit field */

    IrrlichtDevice *m_pDevice;  /**<! the Irrlicht device */

    core::stringc m_sSelectedFile;  /**<! the selected file */

  public:
    /**
     * The constructor
     * @param pDevice the Irrlicht device
     */
    CRoadDialog(IrrlichtDevice *pDevice);
    
    /**
     * Show the dialog
     * @param bLoad make it a "Load" or "Save as" dialog
     * @param sHeadline headline of the dialog
     */
    void showDialog(bool bLoad, const core::stringc sHeadline);
    
    /**
     * Callback method for the Irrlicht event receiver
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * Get the selected file
     * @return the selected file
     */
    const core::stringc &getSelectedFile() {
      return m_sSelectedFile;
    }
    
    /**
     * Set the folder to search for road files
     * @param sFolder the new folder
     */
    void setFolder(const core::stringc &sFolder);

    /**
     * Get the name of the dialog to load/save the size
     * @return the name of the dialog
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc s="RoadDialog";
      return s;
    }
};

#endif
