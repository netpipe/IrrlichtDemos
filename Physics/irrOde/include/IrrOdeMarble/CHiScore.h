#ifndef _C_HI_SCORE
  #define _C_HI_SCORE

  #include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
 * @class HiScore
 * This is a class that manages and displays the highscore table
 */
class HiScore : public IEventReceiver {
  private:
    /**
     * @class HiScore::_ENTRY
     * This class holds the entries of the highscore table
     */
    class _ENTRY {
      public:
        u32 iTime;
        stringw sName;
    };

    list<_ENTRY *> m_lList;     /**< list of highscore entries */
    IrrlichtDevice *m_pDevice;  /**< irrlicht device */
    IGUIEditBox *m_pEdit;       /**< edit box for the user to enter name */
    s32 m_iRet;                 /**< the return value */
    _ENTRY *m_pEntry;           /**< highscore entry of the user */

  public:
    /**
     * The constructor
     * @param pDevice Irrlicht device for the XML reader and writer
     */
    HiScore(IrrlichtDevice *pDevice);

    /**
     * The destructor
     */
    ~HiScore();

    /**
     * @brief show the highscore table
     * This function is used to show the highscore table
     * @param iTime a new time value to be entered to the list. If this param is 0 the highscore list will just be shown, no entry will be added
     */
    void show(u32 iTime);

    /**
     * @brief save the highscore table
     * This method saves the highscore table
     */
    void save();

    /**
     * @brief update the screen
     * This function is called every frame and it just returns a value not equal to "0" if a name was entered or
     * the "ESCAPE" key was pressed. In that case the highscore table will not be saved
     */
    int update();

    /**
     * Event receiver for the Irrlicht event pipeline
     * @param evt the event that wants to be handled
     */
    virtual bool OnEvent(const SEvent &evt);
};

#endif
