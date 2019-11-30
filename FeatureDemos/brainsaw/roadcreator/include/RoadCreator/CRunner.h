#ifndef _C_RUNNER_DEFINED
  #define _C_RUNNER_DEFINED

  #include <irrlicht.h>
  #include <CTextureParameters.h>

  #include <CSegment.h>
  #include <CSurface.h>
  #include <CConnection.h>

class CProjects;
class IRoadPart;
class CRoadDialog;
class CRoadLoader;
class CAlignDialog;
class INotification;
class CDialogWindow;
class CSurfaceWindow;
class CMaterialManager;
class CTextureParameterWindow;
class CSegmentParameterWindow;
class CConnectionParameterWindow;

using namespace irr;

/**
 * @class CRunner
 * @author Christian Keimel / bulletbyte.de
 * This is the main class. It's constructed in the main function
 * and started.
 */
class CRunner : public IEventReceiver {
  protected:
    /**
     * Enumeration of button IDs
     */
    enum eButtonIds {
      eBtnProjects,       /**<! projects ID */
      eBtnResetCamera,    /**<! reset camera ID */
      eBtnNew,            /**<! new ID */
      eBtnLoad,           /**<! load ID */
      eBtnSave,           /**<! save ID */
      eBtnSaveAs,         /**<! save as ID */
      eBtnAddSegment,     /**<! add a segment ID */
      eBtnDelSegment,     /**<! delete a segment ID */
      eBtnAddConnection,  /**<! add a connection ID */
      eBtnDelConnection,  /**<! delete a connection ID */
      eBtnCreateNode,     /**<! create a scene node out of the road */
      eBtnSurface,        /**<! "Surface" Button */
      eBtnLoadScene,      /**<! "load scene" button */
      eBtnAlignSegment,   /**<! "align segment to node" button */
      eBtnCloneSegment    /**<! "clone segment" button */
    };

    core::stringc m_sBaseDir,     /**<! the application's working directory */
                  m_sSettings;    /**<! the settings file */

    IrrlichtDevice *m_pDevice;          /**<! the Irrlicht device */
    video::IVideoDriver *m_pDriver;     /**<! the Irrlicht video driver */
    scene::ISceneManager *m_pSmgr;      /**<! the Irrlicht scene manager */
    gui::IGUIEnvironment *m_pGuienv;    /**<! the Irrlicht gui environment */
    gui::IGUICheckBox    *m_pShrink,    /**<! "shrink node" checkbox */
                         *m_pSingle;    /**<! "display single node" checkbox */
    gui::IGUIEditBox     *m_pOffset;    /**<! offset of the shrinked node */
    gui::IGUIComboBox    *m_pSingleBuf; /**<! render single buffer only */

    scene::ICameraSceneNode *m_pCam;  /**<! the camera scene node */
    scene::ISceneNode *m_pNode;       /**<! scene node of the road */
    core::vector3df m_vCamLookAt;     /**<! the camera target */

    core::list<IEventReceiver *> m_lReceivers;  /**<! list of event receivers */
    core::list<CDialogWindow *> m_lDialogs;     /**<! list of dialogs */

    gui::IGUITab *m_pGuiRoot,   /**<! the root gui element */
                 *m_pSegTab,    /**<! the tab with the segment list */
                 *m_pJntTab;    /**<! the tab with the connection list */

    gui::IGUIListBox *m_pLstSeg,  /**<! the segment list */
                     *m_pLstCon;  /**<! the connection list */

    gui::IGUIFileOpenDialog *m_pFileOpen;

    CAlignDialog *m_pDlgAlign;

    scene::ISceneNode *m_pArrowBase,  /**<! the (empty) root node for the arrow */
                      *m_pSelected;   /**<! the selected scene node */

    f32 m_fAngleH,    /**<! horizontal angle of the camera rotation */
        m_fAngleV;    /**<! vertical angle of the camera rotation */
    s32 m_iMouseX,    /**<! X-Coordinate of the mouse cursor position */
        m_iMouseY;    /**<! Y-Coordinate of the mouse cursor position */
    u32 m_iState;     /**<! Render state */

    bool m_bInitOk,   /**<! "Initialization OK" flag */
         m_bLeftBtn,  /**<! "Left Button pressed" flag */
         m_bHover,    /**<! "Hover" flag (for window draggin) */
         m_bSegment;  /**<! was the last selection a segment? */

    CProjects                  *m_pProjects;  /**<! "select project" dialog */
    CRoadDialog                *m_pRoad;      /**<! "load/save road" dialog */
    CSegmentParameterWindow    *m_pWinSeg;    /**<! "segment parameters" dialog */
    CConnectionParameterWindow *m_pWinCon;    /**<! "connection parameters" dialog */
    CTextureParameterWindow    *m_pWinTex;    /**<! "texture parameters" dialog */
    CMaterialManager           *m_pMaterials; /**<! "texture selection" dialog */
    CSurfaceWindow             *m_pSurface;   /**<! "Surface parameters" dialog */

    CSegment    *m_pSelectedSeg;    /**<! the currently selected segment */
    CConnection *m_pSelectedCon;    /**<! the currently selected connection */
    CSurface    *m_pTheSurface;

    CRoadLoader *m_pLoader;

    CTextureParameters m_aDefaultSeg[_SEGMENT_NUMBER_OF_BUFFERS   ],
                       m_aDefaultCon[_CONNECTION_NUMBER_OF_BUFFERS],
                       m_aDefaultSrf[_SURFACE_NUMBER_OF_BUFFERS   ];

    /**
     * "Load Settings" method.
     * @see m_sSettings
     * @see CDialogWindow::load
     * @see CProjects::loadProject
     */
    void load();

    /**
     * Save the settings
     * @see m_sSettings
     * @see CDialogWindow::save
     * @see CProjects::saveProject
     */
    void save();

    /**
     * This method is used to make sure a segment name is unique within a road definition
     * @param s the input name
     * @return unique version of the name (maybe with an appended "_<number>" string
     */
    core::stringc &unifySegmentName(core::stringc &s);

    /** Update the list of segments */
    void updateSegmentList();

    /** Update the list of connections */
    void updateConnectionList();

    /**
     * This method loads a road from a file
     * @param sName the file to load
     * @see CSegment::load
     * @see CConnection::load
     * @see CTextureParameters::load
     */
    void loadRoad(const core::stringc sName);

    /**
     * Remove a road from memory
     */
    void destroyRoad();

    void updateSingleBufferBox(IRoadPart *p);

  public:
    /**
     * The constructor
     * @param eDriver the driver type
     * @param cDim size of the window
     * @param bFullscreen fullscreen or not
     * @param bStencilBuffer use stencil buffer shadows?
     * @param bVSync use synchronisation with the monitor
     */
    CRunner(video::E_DRIVER_TYPE eDriver,core::dimension2d<u32> cDim, u32 iBits=16, bool  bFullscrenn=false, bool bStencilBuffer=false, bool bVSync=false);

    /**
     * The destructor
     */
    virtual ~CRunner();

    /**
     * Update the tabs on the right side after the window was resized
     * @param cNewSize the new size of the window
     */
    void updateTabs(core::dimension2du cNewSize) ;

    /**
     * The main method. This is called on startup and
     * the program ends when this method is finished.
     */
    virtual void run();

    /**
     * The Irrlicht event receiver callback
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);

    /**
     * Add an additional event receiver
     * @param p event receiver to add
     */
    virtual void addEventReceiver(IEventReceiver *p);

    /**
     * Remove an event receiver
     * @param p event receiver to remove
     */
    virtual void removeEventReceiver(IEventReceiver *p);
};

#endif
