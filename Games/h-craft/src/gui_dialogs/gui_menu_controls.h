// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_CONTROLS_H
#define GUI_MENU_CONTROLS_H

#include "../gui_dialog.h"

class ControllerAxis;
class ControllerButton;

class GuiMenuControls : public GuiDialog
{
public:
    GuiMenuControls(const Config& config);
    ~GuiMenuControls();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);

    bool OnButtonAnalogSteer(const irr::SEvent &event_);
    bool OnButtonAnalogAccelerate(const irr::SEvent &event_);
    bool OnButtonAnalogBrake(const irr::SEvent &event_);
    bool OnButtonDigiLeft(const irr::SEvent &event_);
    bool OnButtonDigiRight(const irr::SEvent &event_);
    bool OnButtonDigiAccelerate(const irr::SEvent &event_);
    bool OnButtonDigiBrake(const irr::SEvent &event_);
    bool OnButtonPause(const irr::SEvent &event_);
    bool OnButtonCamera(const irr::SEvent &event_);

    void RequestAxisInputFromUser(ControllerAxis& axis_, irr::gui::IGUIElement* displayElement);
    void RequestButtonInputFromUser(ControllerButton& button_, irr::gui::IGUIElement* displayElement);

    void SetWaitingScreen(bool enable_);

private:
    irr::gui::IGUIButton*   mButtonAnalogSteer;
    irr::gui::IGUIButton*   mButtonAnalogAccelerate;
    irr::gui::IGUIButton*   mButtonAnalogBrake;
    irr::gui::IGUIButton*   mButtonDigiLeft;
    irr::gui::IGUIButton*   mButtonDigiRight;
    irr::gui::IGUIButton*   mButtonDigiAccelerate;
    irr::gui::IGUIButton*   mButtonDigiBrake;
    irr::gui::IGUIButton*   mButtonPause;
    irr::gui::IGUIButton*   mButtonCamera;
    irr::gui::IGUIStaticText* mActiveProfile;

    irr::gui::IGUIStaticText* mStaticWaiting;
    irr::gui::IGUIStaticText* mStaticAbort;
    irr::gui::IGUIImage*      mImageInsert;

    std::string             mSoundClick;
};


#endif // GUI_MENU_CONTROLS_H
