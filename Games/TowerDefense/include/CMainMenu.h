#ifndef CMAINMENU_H
#define CMAINMENU_H

#include <CState.h>
#include <irrlicht.h>
class gui_catch;
class CMainMenu : public CState
{
public:
    /** Default constructor */
    CMainMenu(CStateManager* manager);
    /** Default destructor */
    virtual ~CMainMenu();


    void update(irr::f32 timediff);
protected:
    void becomeMainState(void);
    void becomeSlaveState(void);

    void press_exit(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender);
    void press_start(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender);
    void press_editor(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender);

    //MenuStuff
    gui_catch* catcher;
private:
};

#endif // CMAINMENU_H
