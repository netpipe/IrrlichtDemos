#ifndef _IRR_CONFIG_CONTROLLER
  #define _IRR_CONFIG_CONTROLLER

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class CIrrCC
 * @brief The central class of IrrConfigController
 * This class is used to provide the use configurable Irrlicht controller. Controller items can be
 * defined, and the user will be able to change the controls using a GUI provided by this class. Controls
 * can be grouped into several sets, but this does only influence the creation of the GUI.
 * @author Christian Keimel / dustbin::games
 */
class CIrrCC : public IEventReceiver {
  public:
    enum eControllerType {
      eCtrlAxis,            //the controller is an axis
      eCtrlToggleButton,    //the controller is a toggle button (state changes when button is pushed, e.g. drop a bomb
      eCtrlButton,          //the controller is a normal button (state remains the same, e.g. boost
      eCtrlFader
    };

  private:
    /**
     * The supported controller types
     * @see CIrrCC::CIrrCCItem::m_iType
     */
    enum IrrConfigControlTypes {
      CC_TYPE_NUL,  /**< NULL type ... not used */
      CC_TYPE_KEY,  /**< keyboard type */
      CC_TYPE_JOY,  /**< joystick axis type */
      CC_TYPE_MOU,  /**< mouse movement type */
      CC_TYPE_MBT,  /**< mouse button type */
      CC_TYPE_JBT,  /**< joystick button type */
      CC_TYPE_POV   /**< joystick POV type */
    };

    /**
     * @class CIrrCC::CIrrCCItem
     * @brief this class represents the controllers the user can configure
     * @see CIrrCC::m_aItems
     */
    class CIrrCCItem {
      private:
        stringw keyCodeToString(EKEY_CODE e); /**< helper function to convert a keycode to a string */
        IrrlichtDevice  *m_pDevice;           /**< the Irrlicht device */
        IGUIEnvironment *m_pGuiEnv;           /**< the Irrlicht gui environment */
        ICursorControl  *m_pCrsCtrl;          /**< the cursor control */
        IGUIStaticText  *m_pCtrlText;         /**< the static text that displays the control */
        IGUITab         *m_pGuiElement;       /**< the GUI element that contains the control */

        void updateMouse();

      public:
        IrrConfigControlTypes m_iType;    /**< type of control, e.g. key or joystick */

        stringw m_sName;                  /**< name of the control */
        EKEY_CODE m_eKey;                 /**< the key code of the control */
        eControllerType m_eType;          /**< type of control */

        u32 m_iSet,                 /**< The set of the control. */
            m_iIdx,                 /**< The index of the control in CIrrCC::m_aItems */
            m_iMouseAxis,           /**< This control's mouse axis */
            m_iMouseBtn,            /**< This control's mouse button */
            m_iJoyAxis,             /**< This control's joystick axis */
            m_iJoyButton,           /**< This control's joystick button */
            m_iFaderStep,           /**< time step for fader */
            m_iFaderLastStep;       /**< the last step time of the fader */
        u8  m_iJoyId;               /**< This control's joystick .. irrlicht joystick ID */
        u16 m_iPov;                 /**< This control's POV position */
        s32 m_iMouseDirection,      /**< This control's mouse direction */
            m_iJoyDirection;        /**< This control's joystick direction */
        f32 m_fValue,               /**< This control's value. Should be queried using CIrrCCItem::get() */
            m_fDetectedValue,       /**< The last detected value. CIrrCCItem::set does not influence this */
            m_fMouseSensitivity,    /**< This control's mouse sensitivity */
            m_fFaderDiff,           /**< increase / decrease per time step for fader */
            m_fFaderValue;          /**< the value of the fader */

        CIrrCCItem *m_pBrother;     /**< the other CCItem with forms an axis with this one */

        bool m_bConflict;           /**< does this control have a conflict with another control? */
        bool isButton();            /**< is this controller a button? */

        /**
         * The item's constructor.
         * @param sName name of the item
         * @param pDevice irrlicht device for the item
         * @param iSet set of the item
         */
        CIrrCCItem(stringw sName, IrrlichtDevice *pDevice, u32 iSet);

        /**
         * The item's destructor
         */
        ~CIrrCCItem();

        /**
         * This method constructs a GUI element for changing this control item
         * @param pParent parent GUI element
         * @param cPos position of the item's element
         * @param cEditPos position of the item's static text that can change
         */
        IGUITab *getGuiElement(IGUIElement *pParent, position2di cPos, position2di cEditPos);

        /**
         * Set the item to "CC_TYPE_KEY" using the given key
         * @param e the key for this item
         */
        void setKey(EKEY_CODE e);

        /**
         * Set the item to "CC_TYPE_MOU" using the given mouse axis in the given direction
         * @param iAxis the mouse axis
         * @param iDirection the direction of this item
         */
        void setMouseMove(u32 iAxis, s32 iDirection);

        /**
         * Set the item to "CC_TYPE_MBT" using the given button number
         * @param iBtn button number to use (0=left, 1=right, 2=middle)
         */
        void setMouseButton(u32 iBtn);

        /**
         * Set the item to "CC_TYPE_JOY" using the given joystick, axis and direction
         * @param id the irrlicht joystick id
         * @param iAxis the axis for this item
         * @param iDirection the direction for this item
         */
        void setJoyAxis(u8 id, u32 iAxis, s32 iDirection);

        /**
         * Set the item to "CC_TYPE_JBT" using the given joystick and joystick button
         * @param id the irrlicht joystick id
         * @param iBtn the joystick button to use
         */
        void setJoyButton(u8 id, u32 iBtn);

        /**
         * Set the item to "CC_TYPE_POV" using the given joystick and POV position
         * @param id the irrlicht joystick id
         * @param iPov the POV position for this item
         */
        void setPov(u8 id, u16 iPov);

        /**
         * Update the item's text. This method is called after the control has changed
         */
        void updateCtrlText();

        /**
         * Get the item's text
         * @return the item's text
         */
        IGUIStaticText *getCtrlText();

        /**
         * Does this item have a conflict with another item?
         * @return Does this item have a conflict with another item?
         */
        bool hasConflict();

        /**
         * Set the item's conflict flag
         * @param b "true" if a conflict was found, "false" otherwise
         */
        void setConflict(bool b);

        /**
         * Check whether or not this item is in conflict with another one
         * @param pOther the other item
         * @return "true" if both of the items use the same control, "false" otherwise
         */
        bool conflicts(CIrrCCItem *pOther);

        /**
         * Set the item's mouse sensitivity. This is use for items that use mouse movement as control
         * @param f the item's mouse sensitivity
         */
        void setMouseSensitivity(f32 f);

        /**
         * Get the item's current position. You could also just take the item's public "m_fValue" property,
         * but mouse movement items calculate this value in this method
         */
        f32 get();

        f32 getRaw();

        /**
         * Set the item's value. Can be called from outside to reset e.g. a button state
         * @param f the new value
         */
        void set(f32 f);

        /**
         * Set the item's value. Is called by CIrrCC::OnEvent
         * @param f the new value
         */
        void setWithDetected(f32 f);
    };

    /**
     * @class CJoyState
     * @brief This class is used for detecting joystick movement when changing an item's control
     */
    class CJoyState {
      public:
        u16 iAxis[6];   /**< the axis positions of the joystick */
        u32 iButtons;   /**< the button states of the joystick  */
        u16 iPov;       /**< the POV position of the joystick   */

        /**
         * Constructor
         */
        CJoyState() {
          for (u32 i=0; i<6; i++) iAxis[i]=0;
          iButtons=0;
          iPov=0;
        }

        /**
         * Check whether or not a button was pressed
         * @param iBtn the button number
         * @return "true" if the button was pressed, "false" otherwise
         */
        bool buttonPressed(u32 iBtn) {
          u32 i=1<<iBtn;
          return (iButtons&i)!=0;
        }
    };

    IrrlichtDevice *m_pDevice;            /**< the Irrlicht device */
    u32 m_iGuiItemHeight;                 /**< the height of the items using the current font */
    s32 m_iFocusedIndex;                  /**< index of the currently focused GUI element */
    f32 m_fMouseSensitivity;              /**< the mouse sensitivity */
    IGUIStaticText *m_pFocused;           /**< the focused text element */
    ICursorControl *m_pCrsCtrl;           /**< the cursor control object */
    position2di m_cMousePos;              /**< the position of the mouse */
    array<CIrrCCItem *> m_aItems;         /**< the array with the control items */
    array<stringw> m_aMaxLenText;         /**< the texts with the maximum length of all sets */
    array<SJoystickInfo> m_aJoysticks;    /**< the joystick info array */
    array<CJoyState *> m_aJoyState;       /**< array with the current joystick states */
    bool m_bAllowMouse,                   /**< is mouse movement allowed for items? */
         m_bAllowFKeys,                   /**< are "Fxx" keys allowed for items? */
         m_bAllowJoystick,                /**< is joystick allows for items? */
         m_bLoaded,                       /**< is the control set already loaded from the XML file? */
         m_bSetsCanConflict;              /**< do same controls of different sets cause a conflict? */

  public:
    /**
     * The constructor
     * @param pDevice the irrlicht device
     */
    CIrrCC(IrrlichtDevice *pDevice);

    /**
     * destructor
     */
    virtual ~CIrrCC();

    /**
     * add a new control item
     * @param iSet set of the item
     * @param sName name of the item
     * @param eDefault default control key of the item
     * @param bIsButton is this a button or part of an axis?
     * @return index of the item
     */
    u32 addItem(u32 iSet, stringw sName, EKEY_CODE eDefault, eControllerType eType);

    /**
     * Create a gui to configure the control items
     * @param iSet set of items
     * @param pParent parent GUI element
     * @param cPos position of the newly created gui
     */
    void addGui(u32 iSet, IGUIElement *pParent, position2di cPos);

    /**
     * Set the "allow mouse" flag
     * @param b new value for the "allow mouse" flag
     */
    void setAllowMouse(bool b);

    /**
     * Set the "allow F-Keys" flag
     * @param b new value for the "allow F-Keys" flag
     */
    void setAllowFKeys(bool b);

    /**
     * Set the "allow joystick" flag
     * @param b new value for the "allow joystick" flag
     */
    void setAllowJoystick(bool b);

    /**
     * Set the mouse sensitivity
     * @param f new value for the mouse sensitivity
     */
    void setMouseSensitivity(f32 f);

    /**
     * Get the status of a specific item. This value is always between "0" and "1" (except for mouse movement)
     * @param idx index of the item
     * @return status of the item
     */
    f32 get(u32 idx);

    void set(u32 idx, f32 f);

    /**
     * This method must be called from the GUI's event receiver. It handles configuration of the control items
     * @param event the event to be handled
     * @return "true" if the event was processed, "false" otherwise
     */
    bool ConfigEvent(const SEvent &event);

    void reset();

    /**
     * This is the event handler to update the item's values. You can either set the instance of the class
     * as irrlicht event receiver or call this method from your own event receiving code.
     * @param event the event to be handled
     * @return "true" if the event was processed, "false" otherwise
     */
    virtual bool OnEvent (const SEvent &event);

    /**
     * decide whether or not the various defined sets cause conflicts or not
     * @param b true if the sets can cause conflicts, false otherwise
     */
    void setSetsCanConflict(bool b) { m_bSetsCanConflict=b; }

    /**
     * Can the various defined sets cause conflicts?
     * @return Can the various defined sets cause conflicts?
     */
    bool setsCanConflict() { return m_bSetsCanConflict; }

    /**
     * Create an axis of two controls defined as eCtrlAxis. If you query the value of the first
     * control item you will get <position of idx1>-<position of idx2>, if you query the second
     * item you'll get <position of idx2>-<position of idx1>.
     * @param idx1 the first index of the axis (value returned by CIrrCC::addItem)
     * @param idx2 the second index of the axis (value returned by CIrrCC::addItem)
     * @see eControllerType
     */
    void createAxis(u32 idx1, u32 idx2);

    void createFader(u32 idx1, u32 idx2, u32 iStep, f32 iInc);

    /**
     * Save the controls to an XML file
     * @param sFileName name of the XML file
     */
    void save(c8 *sFileName);

    /**
     * Load the controls from an XML file
     * @param sFileName name of the XML file
     */
    void load(c8 *sFileName);
};

#endif
