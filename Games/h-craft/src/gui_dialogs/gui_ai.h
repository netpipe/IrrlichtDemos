// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_AI_H
#define GUI_AI_H

#include "../compile_config.h"

#ifdef HC1_ENABLE_EDITOR

#include "../gui_dialog.h"

class Record;

class GuiAi : public GuiDialog
{
public:
    GuiAi(const Config& config);
    ~GuiAi();

    virtual bool Load(const char* filename_, bool reloadLast_=false);

    irr::gui::IGUIWindow* GetWindow()   { return mWindow; }

protected:

    void RemoveFunctors();

    bool OnCheckBoxShowTrack(const irr::SEvent &event_);
    bool OnButtonRecordToTrack(const irr::SEvent &event_);
    bool OnButtonFixedSteps(const irr::SEvent &event_);
    bool OnButtonSmoothCenters(const irr::SEvent &event_);
    bool OnButtonMakeBorders(const irr::SEvent &event_);
    bool OnButtonRealBorders(const irr::SEvent &event_);
    bool OnButtonCenterCenters(const irr::SEvent &event_);
    bool OnButtonSmoothAll(const irr::SEvent &event_);
    bool OnButtonSmoothLeft(const irr::SEvent &event_);
    bool OnButtonSmoothRight(const irr::SEvent &event_);
    bool OnButtonRecordSpeed(const irr::SEvent &event_);
    bool OnButtonRecordLine(const irr::SEvent &event_);
    bool OnButtonSmoothIdealLine(const irr::SEvent &event_);
    bool OnButtonFixTp(const irr::SEvent &event_);
    bool OnCheckBoxAiTraining(const irr::SEvent &event_);
    bool OnWindowEvent(const irr::SEvent &event_);

    bool GetLevelRecord(Record& record_);

private:
    irr::gui::IGUIWindow* mWindow;
};

#endif // HC1_ENABLE_EDITOR

#endif // GUI_AI_H
