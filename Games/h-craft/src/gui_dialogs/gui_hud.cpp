// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_hud.h"
#include "gui_helper.h"
#include "gui_touch.h"

#include "../gui.h"
#include "../main.h"
#include "../string_table.h"
#include "../game.h"
#include "../config.h"

using namespace irr;
using namespace gui;

GuiHud::GuiHud(const Config& config)
    : GuiDialog(config)
    , mStaticTimeCurrentLap(0)
    , mStaticBestLap(0)
    , mStaticTrack(0)
    , mImageTrackRecord(0)
    , mStaticTrackRecord(0)
    , mStaticTimeTrackRecord(0)
    , mStaticLapRecord(0)
    , mStaticLapCount(0)
    , mStaticPosition(0)
    , mStaticSpecial1(0)
    , mStaticSpecial2(0)
    , mStaticGap(0)
    , mStaticCountdown(0)
    , mStaticCamera(0)
    , mStaticFps(0)
#if defined(HOVER_RELEASE) && !defined(_IRR_ANDROID_PLATFORM_)
    , mFpsVisible(false)
#else
    , mFpsVisible(true)
#endif
    , mImageSpeedBar(0)
    , mPosBackground(0)
    , mPosMarker1(0)
    , mPosMarker2(0)
    , mPosMarker3(0)
    , mPosMarkerVisible1(false)
    , mPosMarkerVisible2(false)
    , mPosMarkerVisible3(false)
    , mStaticGhostRecord(0)
    , mStaticGhostModeRecord(0)
    , mStaticGhostGapInfo(0)
    , mStaticGhostGap(0)
    , mImageGhostRecord(0)
    , mImageGhostGap(0)
    , mStaticSessionRecord(0)
	, mStaticSessionModeRecord(0)
	, mStaticSessionGapInfo(0)
	, mStaticSessionGap(0)
	, mStaticSessionActivePlayer(0)
	, mStaticSessionPlayerName(0)
    , mImageSessionRecord(0)
	, mImageSessionGap(0)
	, mImageSessionGap2(0)
    , mEndTimeSpecial1(0)
    , mEndTimeSpecial2(0)
    , mEndTimeGap(0)
    , mEndTimeCamera(0)
{
}

GuiHud::~GuiHud()
{
}

bool GuiHud::Load(const char* filename_, bool reloadLast_)
{
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        irr::gui::IGUIEnvironment * env = GetEnvironment();
        if ( !env )
            return false;

        IGUIElement * root = env->getRootGUIElement();
        if ( !root )
            return false;

        std::string errorMsg("GuiHud::Load");

        mStaticTimeCurrentLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeCurLap", errorMsg));
        mStaticBestLap = static_cast<IGUIStaticText*>(GetElementByName(root, "timeBestLap", errorMsg));
        mStaticTrack = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrack", errorMsg));
        mImageTrackRecord = static_cast<IGUIImage*>(GetElementByName(root, "bg_trackrecord", errorMsg));
        mStaticTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "trackrecord", errorMsg));
        mStaticTimeTrackRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeTrackRecord", errorMsg));
        mStaticLapRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "timeLapRecord", errorMsg));
        mStaticLapCount = static_cast<IGUIStaticText*>(GetElementByName(root, "lapcount", errorMsg));
        mStaticPosition = static_cast<IGUIStaticText*>(GetElementByName(root, "position", errorMsg));
        mStaticSpecial1 = static_cast<IGUIStaticText*>(GetElementByName(root, "special01", errorMsg));
        mStaticSpecial2 = static_cast<IGUIStaticText*>(GetElementByName(root, "special02", errorMsg));
        mStaticGap = static_cast<IGUIStaticText*>(GetElementByName(root, "gap", errorMsg));
        mStaticCamera = static_cast<IGUIStaticText*>(GetElementByName(root, "camera", errorMsg));
        mStaticFps = static_cast<IGUIStaticText*>(GetElementByName(root, "fps", errorMsg));
        mStaticCountdown = static_cast<IGUIStaticText*>(GetElementByName(root, "countdown", errorMsg));
        mImageSpeedBar = static_cast<IGUIImage*>(GetElementByName(root, "speed_bar", errorMsg));
        mPosBackground = static_cast<IGUIImage*>(GetElementByName(root, "pos_background", errorMsg));
        mPosMarker1 = static_cast<IGUIImage*>(GetElementByName(root, "posmarker1", errorMsg));
        mPosMarker2 = static_cast<IGUIImage*>(GetElementByName(root, "posmarker2", errorMsg));
        mPosMarker3 = static_cast<IGUIImage*>(GetElementByName(root, "posmarker3", errorMsg));
        mStaticGhostRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_record", errorMsg));
        mStaticGhostModeRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_ModeRecord", errorMsg));
        mStaticGhostGapInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_gapinfo", errorMsg));
        mStaticGhostGap = static_cast<IGUIStaticText*>(GetElementByName(root, "ghost_gap", errorMsg));
        mImageGhostRecord = static_cast<IGUIImage*>(GetElementByName(root, "bg_ghostrecord", errorMsg));
        mImageGhostGap = static_cast<IGUIImage*>(GetElementByName(root, "bg_ghostgap", errorMsg));
        mStaticSessionRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "session_record", errorMsg));
        mStaticSessionModeRecord = static_cast<IGUIStaticText*>(GetElementByName(root, "session_ModeRecord", errorMsg));
        mStaticSessionGapInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "session_gapinfo", errorMsg));
        mStaticSessionGap = static_cast<IGUIStaticText*>(GetElementByName(root, "session_gap", errorMsg));
        mStaticSessionActivePlayer = static_cast<IGUIStaticText*>(GetElementByName(root, "active_player", errorMsg));
        mStaticSessionPlayerName = static_cast<IGUIStaticText*>(GetElementByName(root, "playername", errorMsg));
        mImageSessionRecord = static_cast<IGUIImage*>(GetElementByName(root, "bg_sessionrecord", errorMsg));
        mImageSessionGap = static_cast<IGUIImage*>(GetElementByName(root, "bg_sessiongap", errorMsg));
        mImageSessionGap2 = static_cast<IGUIImage*>(GetElementByName(root, "bg_sessiongap2", errorMsg));

#if (IRR_SVN >= 4734)
	mImageSpeedBar->setDrawBounds(core::rect<f32>(0.f, 0.f, 1.f, 1.f));
#endif
    }
    return ok;
}

void GuiHud::Show()
{
    GuiDialog::Show();

    if ( mStaticFps )
        mStaticFps->setVisible(mFpsVisible);

    bool showTrackRecord =      GT_TIMERACE == APP.GetGame()->GetSettings().mGameType
                            ||  GT_CHAMPIONSHIP == APP.GetGame()->GetSettings().mGameType
                            ||  GT_RIVALS == APP.GetGame()->GetSettings().mGameType
                            ;
    if ( mStaticTrackRecord )
        mStaticTrackRecord->setVisible(showTrackRecord);
    if ( mStaticTimeTrackRecord )
        mStaticTimeTrackRecord->setVisible(showTrackRecord);
    if ( mImageTrackRecord )
        mImageTrackRecord->setVisible(showTrackRecord);

    if ( mPosMarker1 )
        mPosMarker1->setVisible( mPosMarkerVisible1 );
    if ( mPosMarker2 )
        mPosMarker2->setVisible( mPosMarkerVisible2 );
    if ( mPosMarker3 )
        mPosMarker3->setVisible( mPosMarkerVisible3 );

	if ( mPosBackground )
		mPosBackground->setVisible( mPosMarkerVisible1 || mPosMarkerVisible2 || mPosMarkerVisible3 );

    bool showGhostRecord = GHT_NONE != APP.GetGame()->GetSettings().mGhostType;

    if ( mStaticGhostRecord )
        mStaticGhostRecord->setVisible( showGhostRecord );
    if ( mStaticGhostModeRecord )
        mStaticGhostModeRecord->setVisible( showGhostRecord );
    if ( mStaticGhostGapInfo )
        mStaticGhostGapInfo->setVisible( showGhostRecord );
    if ( mStaticGhostGap )
        mStaticGhostGap->setVisible( showGhostRecord );
    if ( mImageGhostRecord )
        mImageGhostRecord->setVisible( showGhostRecord );
    if ( mImageGhostGap )
        mImageGhostGap->setVisible( showGhostRecord );

    bool showSession = GT_RIVALS == APP.GetGame()->GetSettings().mGameType;
    if ( mStaticSessionRecord )
        mStaticSessionRecord->setVisible(showSession);
	if ( mStaticSessionModeRecord )
        mStaticSessionModeRecord->setVisible(showSession);
	if ( mStaticSessionGapInfo )
        mStaticSessionGapInfo->setVisible(showSession);
	if ( mStaticSessionGap )
        mStaticSessionGap->setVisible(showSession);
	if ( mStaticSessionActivePlayer )
        mStaticSessionActivePlayer->setVisible(showSession);
	if ( mStaticSessionPlayerName )
        mStaticSessionPlayerName->setVisible(showSession);
    if ( mImageSessionRecord )
        mImageSessionRecord->setVisible(showSession);
	if ( mImageSessionGap )
        mImageSessionGap->setVisible(showSession);
	if ( mImageSessionGap2 )
        mImageSessionGap2->setVisible(showSession);
    if ( mStaticCamera )
        mStaticCamera->setText(L"");

	if ( GetConfig().GetUseTouchInput() != ETI_NO_TOUCH && APP.GetGui()->GetGuiTouch() )
	{
		APP.GetGui()->GetGuiTouch()->Show();
		APP.GetGui()->GetGuiTouch()->SetDialogState(ETDS_PLAY);
	}
}

void GuiHud::Hide()
{
    if ( APP.GetGui()->GetGuiTouch() )
		APP.GetGui()->GetGuiTouch()->Hide();

    GuiDialog::Hide();
}


void GuiHud::Update(u32 time_)
{
    if ( mEndTimeSpecial1 && time_ > mEndTimeSpecial1 )
        mStaticSpecial1->setText(L"");
    if ( mEndTimeSpecial2 && time_ > mEndTimeSpecial2 )
        mStaticSpecial2->setText(L"");
    if ( mEndTimeGap && time_ > mEndTimeGap )
        mStaticGap->setText(L"");
    if ( mEndTimeCamera && time_ > mEndTimeCamera )
        mStaticCamera->setText(L"");
}

void GuiHud::SetPosMarkerVisible(int index_, bool visible_)
{
    switch ( index_ )
    {
        case 0:
            mPosMarkerVisible1 = visible_;
			if ( mPosMarker1 )
				mPosMarker1->setVisible( mPosMarkerVisible1 );
            break;
        case 1:
            mPosMarkerVisible2 = visible_;
            if ( mPosMarker2 )
				mPosMarker2->setVisible( mPosMarkerVisible2 );
            break;
        case 2:
            mPosMarkerVisible3 = visible_;
            if ( mPosMarker3 )
				mPosMarker3->setVisible( mPosMarkerVisible3 );
            break;
    }
}

void GuiHud::SetPosMarker(int index_, float relativePos_)
{
    if ( !mPosBackground )
        return;

    irr::gui::IGUIImage * marker = NULL;

    switch ( index_ )
    {
        case 0:
            marker = mPosMarker1;
            break;
        case 1:
            marker = mPosMarker2;
            break;
        case 2:
            marker = mPosMarker3;
            break;
    }

    if ( !marker )
        return;

    if ( relativePos_ < -1.f )
        relativePos_ = -1.f;
    else if ( relativePos_ > 1.f )
        relativePos_ = 1.f;
    relativePos_ += 1.f;
    relativePos_ *= 0.5;

    core::rect<s32> rectMax = mPosBackground->getRelativePosition();
    core::rect<s32> rect = marker->getRelativePosition();

    int maxWidth = rectMax.LowerRightCorner.X - rectMax.UpperLeftCorner.X;
    ++maxWidth;

    int oldDist = rect.LowerRightCorner.X - rect.UpperLeftCorner.X;
    rect.UpperLeftCorner.X = rectMax.UpperLeftCorner.X + (int)(relativePos_ * (float)maxWidth);
    rect.UpperLeftCorner.X -= (int)(oldDist* 0.5f);
    rect.LowerRightCorner.X = rect.UpperLeftCorner.X + oldDist;

	marker->setRelativePosition(rect);
}

void GuiHud::SetTimeLap(u32 time_)
{
    if ( !mStaticTimeCurrentLap )
        return;

    mStaticTimeCurrentLap->setText(GuiHelper::MakeTimeString(time_).c_str());
}

void GuiHud::SetTimeBestLap(u32 time_)
{
    if ( !mStaticBestLap )
        return;

    mStaticBestLap->setText(GuiHelper::MakeTimeString(time_).c_str());
}

u32 GuiHud::GetTimeBestLap()
{
    if ( !mStaticBestLap )
        return 0;

    std::wstring wstr(mStaticBestLap->getText());
    return GuiHelper::GetTimeFromTimeString(wstr);
}

void GuiHud::SetTimeTrack(u32 time_)
{
    if ( !mStaticTrack )
        return;

    mStaticTrack->setText(GuiHelper::MakeTimeString(time_).c_str());
}

void GuiHud::SetTimeTrackRecord(u32 time_)
{
    if ( !mStaticTimeTrackRecord )
        return;

    mStaticTimeTrackRecord->setText(GuiHelper::MakeTimeString(time_).c_str());
}

void GuiHud::SetTimeLapRecord(u32 time_)
{
    if ( !mStaticLapRecord )
        return;

    mStaticLapRecord->setText(GuiHelper::MakeTimeString(time_).c_str());
}

void GuiHud::SetSpeed(f32 speedScaled_)
{
    if ( ! mImageSpeedBar )
        return;

#if (IRR_SVN >= 4734)
	mImageSpeedBar->setDrawBounds(core::rect<f32>(0.f, 0.f, speedScaled_, 1.f));
#endif
}

void GuiHud::SetLapCount(u32 lap_, u32 maxLaps_)
{
    if ( !mStaticLapCount )
        return;
    core::stringw text;

    if ( maxLaps_ > 0 )
    {
        if ( lap_ > maxLaps_ )
            lap_ = maxLaps_;
		text = APP.GetStringTable()->Get("id_hdlapcount");
		text += L' ';
		text += core::stringw(lap_);
		text += L" / ";
		text += core::stringw(maxLaps_);
    }
    else
    {
		text = APP.GetStringTable()->Get("id_hdlapcount");
		text += L' ';
		text += core::stringw(lap_);
		text += L" / -";
    }

	mStaticLapCount->setText(text.c_str());
}

void GuiHud::SetPosition(u32 pos_, u32 players_)
{
    if ( !mStaticPosition )
        return;

	core::stringw text(APP.GetStringTable()->Get("id_hdposition"));
	text += L' ';
	text += core::stringw(pos_);
	text += L" / ";
	text += core::stringw(players_);

    mStaticPosition->setText(text.c_str());
}

void GuiHud::SetReverseTime(s32 time_)
{
   if ( !mStaticPosition )
        return;
    std::wstring strGapTime( APP.GetStringTable()->Get("id_gap").c_str() );
    strGapTime += GuiHelper::MakeTimeString(time_, true);
    mStaticPosition->setText( strGapTime.c_str() );
}

void GuiHud::SetSpecialText1(const wchar_t * text_, u32 timeRemove_)
{
    if ( !mStaticSpecial1 )
        return;

    mStaticSpecial1->setText(text_);
    mEndTimeSpecial1 = timeRemove_;
}

void GuiHud::SetSpecialText2(const wchar_t * text_, u32 timeRemove_)
{
    if ( !mStaticSpecial2 )
        return;

    mStaticSpecial2->setText(text_);
    mEndTimeSpecial2 = timeRemove_;
}

void GuiHud::SetGap(const wchar_t * text_, u32 timeRemove_)
{
    if ( !mStaticGap )
        return;

    mStaticGap->setText(text_);
    mEndTimeGap = timeRemove_;
}

void GuiHud::SetCountdown(HUD_COUNTDOWN count_)
{
    if ( !mStaticCountdown)
        return;

    switch ( count_ )
    {
        case HUD_COUNT_3:
            mStaticCountdown->setText( APP.GetStringTable()->Get("id_coundown3").c_str() );
            break;
        case HUD_COUNT_2:
            mStaticCountdown->setText( APP.GetStringTable()->Get("id_coundown2").c_str() );
            break;
        case HUD_COUNT_1:
            mStaticCountdown->setText( APP.GetStringTable()->Get("id_coundown1").c_str() );
            break;
        case HUD_COUNT_GO:
            mStaticCountdown->setText( APP.GetStringTable()->Get("id_coundown_go").c_str() );
            break;
        case HUD_COUNT_NONE:
        default:
            mStaticCountdown->setText( L"" );
            break;
    }
}

void GuiHud::SetCameraName(const wchar_t * text_, u32 timeRemove_)
{
    if ( !mStaticCamera )
        return;
    mStaticCamera->setText(text_);
    mEndTimeCamera = timeRemove_;
}

void GuiHud::SetFpsVisible(bool enable_)
{
    mFpsVisible = enable_;
    Show();
}

void GuiHud::SetFps(int fps_)
{
    if ( !mStaticFps )
        return;

	core::stringw text(L"FPS ");
	text += core::stringw(fps_);
    mStaticFps->setText(text.c_str());
}

void GuiHud::SetTimeGhostRecord(u32 time_)
{
    if ( !mStaticGhostModeRecord )
        return;

    std::wstring strTime( GuiHelper::MakeTimeString(time_, false) );
    strTime += L" (";

    if ( GHT_EXPORTED_LAP == APP.GetGame()->GetSettings().mGhostType
        ||  GHT_LAP == APP.GetGame()->GetSettings().mGhostType
        )
    {
        strTime += APP.GetStringTable()->Get("id_score_lap").c_str();
    }
    else if ( GHT_EPORTED_TRACK == APP.GetGame()->GetSettings().mGhostType
            || GHT_TRACK == APP.GetGame()->GetSettings().mGhostType
            )
    {
        strTime += APP.GetStringTable()->Get("id_score_track").c_str();
    }
    else
    {
        return;
    }
    strTime += L")";

    mStaticGhostModeRecord->setText( strTime.c_str() );
}

void GuiHud::SetTimeGhostGap(s32 time_)
{
    if ( !mStaticGhostGap )
        return;

    std::wstring strTime( GuiHelper::MakeTimeString(time_, true, 1) );
    mStaticGhostGap->setText( strTime.c_str() );
}

void GuiHud::SetTimeSessionRecord(u32 time_)
{
    std::wstring strTime( GuiHelper::MakeTimeString(time_, false) );
    strTime += L" (";
    strTime += APP.GetStringTable()->Get("id_score_track").c_str();
    strTime += L")";
	mStaticSessionModeRecord->setText( strTime.c_str() );
}

void GuiHud::SetSessionPlayerName(const wchar_t * text_)
{
    if ( !mStaticSessionPlayerName )
        return;
	mStaticSessionPlayerName->setText(text_);
}

void GuiHud::SetTimeSessionGap(s32 time_)
{
    if ( !mStaticSessionGap )
        return;

    std::wstring strTime( GuiHelper::MakeTimeString(time_, true, 1) );
    mStaticSessionGap->setText( strTime.c_str() );
}

void GuiHud::OnMouseClick(s32 x, s32 y)
{
	if ( mStaticFps && mStaticFps->isPointInside(core::position2d<s32>(x,y)) )
	{
		SetFpsVisible(!GetFpsVisible());
	}
}
