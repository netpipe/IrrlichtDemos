#ifndef _C_SETTINGS
  #define _C_SETTINGS

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class CSettings
 * This class provides the "select graphics options" dialog that is displayed at program start
 */
class CSettings : public IEventReceiver {
  private:
    /**
     * class CSettings::_VRES
     * This class is used to store the available screen modes
     */
    class _VRES {
      public:
        s32 iWidth;   /**< width of the mode */
        s32 iHeight;  /**< height of the mode */
        s32 iBpp;     /**< bit depth of the mode */
    };

    /**
     * @class CSettings::_DRV
     * In this class the name and the ID of the available drivers are stored
     */
    class _DRV {
      public:
        wchar_t sName[0xFF];    /**< name of the driver */
        E_DRIVER_TYPE iDriver;  /**< ID of the driver */
    };

    IGUIComboBox *m_pDrivers,     /**< combobox to pick the video driver */
                 *m_pResolution;  /**< combobox to pick the screen mode */
    IGUICheckBox *m_pFullscreen,  /**< checkbox to choose whether or not fullscreen is wanted */
                 *m_pShadows;     /**< checkbox to choose whether or not shadows are wanted */
    IGUIButton *m_pOk,            /**< the "OK" button */
               *m_pCancel;        /**< the "Cancel" button */
    SColor m_cBackground;         /**< the background color */

    c8 m_sSettingsFile[1024]; /**< the settings file */
    u32 m_iClose,             /**< is the "OK" or the "Cancel" button (or "ENTER" or "ESCAPE") pressed? */
        m_iResolution;        /**< the selected sceen mode */
    E_DRIVER_TYPE m_iDriver;  /**< the selected driver */
    bool m_bFullscreen,       /**< is fullscreen wanted? */
         m_bShadows,          /**< are shadows wanted? */
         m_bSettingsChanged;  /**< have the settings changed and need to be saved? */

    IrrlichtDevice  *m_pDevice;       /**< the irrlicht device to be used */
    IGUIEnvironment *m_pGuiEnv;       /**< the GUI environment */
    IVideoDriver    *m_pDriver;       /**< the driver */
    ISceneManager   *m_pSceneManager; /**< the scene manager */

    IVideoModeList *m_pVModes;    /**< list of the available video modes from the Irrlicht device */
    array<_VRES *> m_aVModes;     /**< list of the video modes that are available in the combobox */
    array<_DRV  *> m_aDrvs;       /**< list of the video drivers that are available in the combobox */

  public:
    CSettings(c8 *sScene, c8 *sSettingsFile, wchar_t *sTitle, SColor cBackground);
    ~CSettings();

    /**
     * run the dialog
     * @return "1" if the "OK" button was pressed, "2" for the "Cancel" button, "0" otherwise
     */
    u32 run();

    /**
     * Irrlicht event receiver
     * @param evt the event to be handled
     * @return was the event handled?
     */
    virtual bool OnEvent(const SEvent &evt);

    /**
     * create a new device from the user's settings
     * @return the newly created IrrlichtDevice instance
     */
    IrrlichtDevice *createDeviceFromSettings();
};

#endif
