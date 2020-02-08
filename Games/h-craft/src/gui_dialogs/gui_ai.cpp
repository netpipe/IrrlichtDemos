// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_ai.h"

#ifdef HC1_ENABLE_EDITOR

#include "../gui_dialog.h"
#include "../gui.h"
#include "../config.h"
#include "../level.h"
#include "../recorder.h"
#include "../main.h"
#include "../game.h"
#include <assert.h>

using namespace irr;
using namespace gui;

GuiAi::GuiAi(const Config& config)
    : GuiDialog(config)
    , mWindow(NULL)
{
}

GuiAi::~GuiAi()
{
    RemoveFunctors();
}

bool GuiAi::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("cbShowTrack")), new EventFunctor<GuiAi>(this, &GuiAi::OnCheckBoxShowTrack) );
        AddGuiEventFunctor( GetIdForName(std::string("btnRecordToTrack")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonRecordToTrack) );
        AddGuiEventFunctor( GetIdForName(std::string("btnFixedSteps")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonFixedSteps) );
        AddGuiEventFunctor( GetIdForName(std::string("btnSmoothCenters")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonSmoothCenters) );
        AddGuiEventFunctor( GetIdForName(std::string("btnMakeBorders")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonMakeBorders) );
        AddGuiEventFunctor( GetIdForName(std::string("btnRealBorders")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonRealBorders) );
        AddGuiEventFunctor( GetIdForName(std::string("btnCenters")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonCenterCenters) );
        AddGuiEventFunctor( GetIdForName(std::string("btnSmoothAll")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonSmoothAll) );
        AddGuiEventFunctor( GetIdForName(std::string("btnSmoothLeft")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonSmoothLeft) );
        AddGuiEventFunctor( GetIdForName(std::string("btnSmoothRight")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonSmoothRight) );
        AddGuiEventFunctor( GetIdForName(std::string("btnRecordSpeed")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonRecordSpeed) );
        AddGuiEventFunctor( GetIdForName(std::string("btnRecordLine")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonRecordLine) );
        AddGuiEventFunctor( GetIdForName(std::string("btnSmoothIdealLine")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonSmoothIdealLine) );
        AddGuiEventFunctor( GetIdForName(std::string("btnFixTp")), new EventFunctor<GuiAi>(this, &GuiAi::OnButtonFixTp) );
        AddGuiEventFunctor( GetIdForName(std::string("cbAiTraining")), new EventFunctor<GuiAi>(this, &GuiAi::OnCheckBoxAiTraining) );
        AddGuiEventFunctor( GetIdForName(std::string("aidialog")), new EventFunctor<GuiAi>(this, &GuiAi::OnWindowEvent) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiAi::Load");

        mWindow = static_cast<IGUIWindow*>(GetElementByName(root, "aidialog", errorMsg));
        IGUICheckBox* cb = static_cast<IGUICheckBox*>(GetElementByName(root, "cbShowTrack", errorMsg));
        if ( cb )
        {
            cb->setChecked(APP.GetDrawAiTrack());
        }
    }
    return ok;
}

void GuiAi::RemoveFunctors()
{
    if ( !IsLoaded() )
    {
        return;
    }
    RemoveGuiEventFunctor( GetIdForName(std::string("cbShowTrack")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnRecordToTrack")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnFixedSteps")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnSmoothCenters")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnMakeBorders")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnRealBorders")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnCenters")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnSmoothAll")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnSmoothLeft")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnSmoothRight")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnRecordSpeed")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnRecordLine")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnSmoothIdealLine")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("btnFixTp")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("cbAiTraining")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("aidialog")) );
}

bool GuiAi::GetLevelRecord(Record& record_)
{
    int levelIndex = APP.GetLevelManager()->GetCurrentLevelIndex();
    if ( levelIndex < 0 )
        return false;
    const LevelSettings & levelSettings = APP.GetLevelManager()->GetLevel(levelIndex);

    std::string filenameLapRecord( APP.GetConfig()->MakeLapRecordName(levelSettings.mId, APP.GetStringTable()) );
    bool loaded = record_.Load(filenameLapRecord);
    if ( !loaded )
        return false;

    return true;
}

bool GuiAi::OnCheckBoxShowTrack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_CHECKBOX_CHANGED )
    {
        IGUICheckBox* cb = static_cast<IGUICheckBox*>(event_.GUIEvent.Caller);
        assert(cb);

        APP.SetDrawAiTrack(cb->isChecked());
    }
    return false;
}

bool GuiAi::OnButtonRecordToTrack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        Record record;
        if ( !GetLevelRecord(record) )
            return false;

        APP.GetLevelManager()->GetAiTrack().RecordCenters(record);

        return true;
    }
    return false;
}

bool GuiAi::OnButtonFixedSteps(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().MakeFixedStepSize();
        return true;
    }
    return false;
}

bool GuiAi::OnButtonSmoothCenters(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().SmoothTrack(false);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonMakeBorders(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().MakeBorders(100.f);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonRealBorders(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        float radius = APP.GetConfig()->GetHoverRadius();
        APP.GetLevelManager()->GetAiTrack().FindRealBorders(radius);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonCenterCenters(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().CalculateCenters();
        return true;
    }
    return false;
}

bool GuiAi::OnButtonSmoothAll(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().SmoothTrack(true);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonSmoothLeft(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().SmoothBorders(/*left_*/ true, /*right_*/ false);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonSmoothRight(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().SmoothBorders(/*left_*/ false, /*right_*/ true);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonRecordSpeed(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        Record record;
        if ( !GetLevelRecord(record) )
            return false;

        APP.GetLevelManager()->GetAiTrack().RecordSpeed(record);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonRecordLine(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        Record record;
        if ( !GetLevelRecord(record) )
            return false;

        APP.GetLevelManager()->GetAiTrack().RecordIdealLine(record);
        return true;
    }
    return false;
}

bool GuiAi::OnButtonSmoothIdealLine(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetLevelManager()->GetAiTrack().SmoothIdealLine();
        return true;
    }
    return false;
}

bool GuiAi::OnButtonFixTp(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        TrackMarker& tpTarget = APP.GetLevelManager()->GetTpSource();
        APP.GetLevelManager()->GetAiTrack().AddTrackBehindTp(&tpTarget);
        return true;
    }
    return false;
}

bool GuiAi::OnCheckBoxAiTraining(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_CHECKBOX_CHANGED )
    {
        IGUICheckBox* cb = static_cast<IGUICheckBox*>(event_.GUIEvent.Caller);
        assert(cb);

        APP.GetGame()->SetAiTraining(cb->isChecked());

        return true;
    }
    return false;
}

bool GuiAi::OnWindowEvent(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_CLOSED )
    {
        Hide();
        return true;
    }

    return false;
}

#endif // HC1_ENABLE_EDITOR
