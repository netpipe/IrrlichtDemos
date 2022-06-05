#include "CMainMenu.h"
#include "CStateManager.h"
#include "gui_catch.h"

CMainMenu::CMainMenu(CStateManager* manager) : CState(manager)
{
    //ctor
    catcher = NULL;
    printf("create MainMenu\n");
}

CMainMenu::~CMainMenu()
{
    //dtor
    printf("delete MainMenu\n");
}
void CMainMenu::becomeSlaveState(void)
{
    catcher->remove();
}

void CMainMenu::becomeMainState(void)
{
    printf("MainMenu is now MainState\n");
    catcher = new gui_catch(Manager->getDevice()->getGUIEnvironment(), Manager->getDevice()->getGUIEnvironment()->getRootGUIElement());

    irr::gui::IGUIButton* button = Manager->getDevice()->getGUIEnvironment()->addButton(irr::core::rect<irr::s32>(0,0,1,1), catcher);
    button->setRelativePositionProportional(irr::core::rect<irr::f32>(0.35,0.3,0.65,0.4));
    button->setText(L"Start");
    catcher->registerElement(button, this, &CMainMenu::press_start);

    button = Manager->getDevice()->getGUIEnvironment()->addButton(irr::core::rect<irr::s32>(0,0,1,1), catcher);
    button->setRelativePositionProportional(irr::core::rect<irr::f32>(0.35,0.5,0.65,0.6));
    button->setText(L"Editor");
    catcher->registerElement(button, this, &CMainMenu::press_editor);

    button = Manager->getDevice()->getGUIEnvironment()->addButton(irr::core::rect<irr::s32>(0,0,1,1), catcher);
    button->setRelativePositionProportional(irr::core::rect<irr::f32>(0.35,0.7,0.65,0.8));
    button->setText(L"Exit");
    catcher->registerElement(button, this, &CMainMenu::press_exit);
}

void CMainMenu::update(irr::f32 timediff)
{

}

void CMainMenu::press_start(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender)
{
    if (eventType == irr::gui::EGET_BUTTON_CLICKED )
    {
        Manager->createState("Game");
        remove();
    }
}

void CMainMenu::press_editor(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender)
{
    if (eventType == irr::gui::EGET_BUTTON_CLICKED )
    {
        Manager->createState("Editor");
        remove();
    }
}

void CMainMenu::press_exit(irr::gui::EGUI_EVENT_TYPE eventType, irr::gui::IGUIElement* sender)
{
    if (eventType == irr::gui::EGET_BUTTON_CLICKED )
        remove();
}


