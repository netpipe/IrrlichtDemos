#ifndef _C_MENU
  #define _C_MENU

  #include "IState.h"

/**
 * @class CMenu
 * This class implements the main menu of the game. It is a subclass of "IState" and therefore a state
 * of the finite state machine that makes up the states of the application
 * @see IState
 */
class CMenu : public IState, public IEventReceiver {
  private:
    bool m_bSelect;     /**< was a menu item selected? */
    s32 m_iMenuMove;    /**< move to another menu item */
    u32 m_iLastTime,    /**< the last time of the "update" method */
        m_iActiveItem,  /**< number of the active item */
        m_iItemCount;   /**< the number of menu items */
    f32 m_fItemHeight;  /**< the height of the menu items */

    IAnimatedMeshSceneNode *m_pMenu;  /**< the menu ... a scene node that includes all menu items */

  public:
    CMenu(IrrlichtDevice *pDevice); /**< the state's contructor */
    virtual ~CMenu();               /**< the state's destructor */

    virtual void activate();    /**< the implementation of the "activate" method */
    virtual void deactivate();  /**< the implementation of the "deactivate" method */
    virtual int update();       /**< the implementation of the "update" method */

    /**
     * the callback for Irrlicht events
     * @param evt the event to be handled
     * @return was the event handled by this handler?
     */
    virtual bool OnEvent(const SEvent &evt);
};

#endif
