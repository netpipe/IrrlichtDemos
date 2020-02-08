// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_NAGSCREEN_H
#define GUI_MENU_NAGSCREEN_H

#include "../gui_dialog.h"
#include <string>

class GuiMenuNagscreen : public GuiDialog
{
public:
    GuiMenuNagscreen(const Config& config);
    ~GuiMenuNagscreen();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();
    virtual void OnUpdate(irr::u32 timeTick);

protected:
    void RemoveFunctors();
    bool OnButtonQuit(const irr::SEvent &event_);
    bool OnButtonBuy(const irr::SEvent &event_);
    const char* GetUrlBuy();    // will return a valid url or NULL (no empty strings)

private:
    irr::gui::IGUIButton* mButtonBuy;
    irr::gui::IGUIStaticText* mStaticNagPrice;
    irr::gui::IGUIStaticText* mStaticNagAdvertise;
	std::string mItemName;
	bool mBuyingRequested;
};

#endif // GUI_MENU_NAGSCREEN_H
