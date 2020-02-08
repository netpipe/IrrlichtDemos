// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_MENU_GRAPHICS_H
#define GUI_MENU_GRAPHICS_H

#include "../gui_dialog.h"
#include <vector>

class GuiMenuGraphics : public GuiDialog
{
public:
    GuiMenuGraphics(const Config& config);
    ~GuiMenuGraphics();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();

protected:
    void RemoveFunctors();

    bool OnSliderResolution(const irr::SEvent &event_);
    bool OnSliderFilter(const irr::SEvent &event_);
    bool OnSliderAntialias(const irr::SEvent &event_);
    bool OnSliderLights(const irr::SEvent &event_);
    bool OnSliderTextureResolution(const irr::SEvent &event_);
    bool OnSliderParticles(const irr::SEvent &event_);
    bool OnSliderVsync(const irr::SEvent &event_);
    bool OnButtonAccept(const irr::SEvent &event_);
    bool OnButtonCancel(const irr::SEvent &event_);

	void AddNeedRestartCheck(irr::gui::IGUITextSlider* slider);
	bool CheckRestartNeeded(bool updateText = true);

private:
    irr::gui::IGUITextSlider* mSliderResolution;
    irr::gui::IGUITextSlider* mSliderFilter;
    irr::gui::IGUITextSlider* mSliderAntialias;
    irr::gui::IGUITextSlider* mSliderLights;
    irr::gui::IGUITextSlider* mSliderTextureResolution;
    irr::gui::IGUITextSlider* mSliderParticles;
    irr::gui::IGUITextSlider* mSliderVsync;
    irr::gui::IGUIStaticText* mActiveProfile;
    irr::gui::IGUIStaticText* mStaticInfo;
    irr::gui::IGUIStaticText* mStaticRestartInfo;

    struct NeedsRestart
    {
		irr::gui::IGUITextSlider* mTextSlider;
		irr::s32 mOldTextId;
    };
	std::vector<NeedsRestart> mNeedsRestart;
};

#endif // GUI_MENU_CREDITS_H

