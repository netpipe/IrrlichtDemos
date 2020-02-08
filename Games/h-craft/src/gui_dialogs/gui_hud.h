// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_HUD
#define GUI_HUD

#include "../compile_config.h"
#include "../gui_dialog.h"
#include <string>

enum HUD_COUNTDOWN
{
    HUD_COUNT_3,
    HUD_COUNT_2,
    HUD_COUNT_1,
    HUD_COUNT_GO,
    HUD_COUNT_NONE,
};

class GuiHud : public GuiDialog
{
public:
    GuiHud(const Config& config);
    ~GuiHud();

    virtual bool Load(const char* filename_, bool reloadLast_=false);
    virtual void Show();
    virtual void Hide();

    void Update(irr::u32 time_);

    void SetTimeLap(irr::u32 time_);
    void SetTimeBestLap(irr::u32 time_);
    irr::u32 GetTimeBestLap();
    void SetTimeTrack(irr::u32 time_);
    void SetTimeTrackRecord(irr::u32 time_);
    void SetTimeLapRecord(irr::u32 time_);

    void SetSpeed(irr::f32 speedScaled_);    // between 0 and 1

    void SetLapCount(irr::u32 lap_, irr::u32 maxLaps_);
    void SetPosition(irr::u32 pos_, irr::u32 players_);
    void SetReverseTime(irr::s32 time_);

    void SetSpecialText1(const wchar_t * text_, irr::u32 timeRemove_ = 0);  // timeRemove_ = 0 for infinite time
    void SetSpecialText2(const wchar_t * text_, irr::u32 timeRemove_ = 0);
    void SetGap(const wchar_t * text_, irr::u32 timeRemove_=0);
    void SetCountdown(HUD_COUNTDOWN count_);
    void SetCameraName(const wchar_t * text_, irr::u32 timeRemove_=0);

    // index_ can be 0, 1, 2
    void SetPosMarkerVisible(int index_, bool visible_);
    // relativePos_ in range [-1; 1]
    void SetPosMarker(int index_, float relativePos_);

    void SetFps(int fps_);
    void SetFpsVisible(bool enable_);
    bool GetFpsVisible() const  { return mFpsVisible; }

    void SetTimeGhostRecord(irr::u32 time_);
    void SetTimeGhostGap(irr::s32 time_);

    void SetTimeSessionRecord(irr::u32 time_);
    void SetSessionPlayerName(const wchar_t * text_);
    void SetTimeSessionGap(irr::s32 time_);

    void OnMouseClick(irr::s32 x, irr::s32 y);

private:
    irr::gui::IGUIStaticText *  mStaticTimeCurrentLap;
    irr::gui::IGUIStaticText *  mStaticBestLap;
    irr::gui::IGUIStaticText *  mStaticTrack;
    irr::gui::IGUIImage *       mImageTrackRecord;
    irr::gui::IGUIStaticText *  mStaticTrackRecord;
    irr::gui::IGUIStaticText *  mStaticTimeTrackRecord;
    irr::gui::IGUIStaticText *  mStaticLapRecord;
    irr::gui::IGUIStaticText *  mStaticLapCount;
    irr::gui::IGUIStaticText *  mStaticPosition;
    irr::gui::IGUIStaticText *  mStaticSpecial1;
    irr::gui::IGUIStaticText *  mStaticSpecial2;
    irr::gui::IGUIStaticText *  mStaticGap;
    irr::gui::IGUIStaticText *  mStaticCountdown;
    irr::gui::IGUIStaticText *  mStaticCamera;
    irr::gui::IGUIStaticText *  mStaticFps;
    bool                        mFpsVisible;
    irr::gui::IGUIImage *       mImageSpeedBar;
    irr::gui::IGUIImage *       mPosBackground;
    irr::gui::IGUIImage *       mPosMarker1;
    irr::gui::IGUIImage *       mPosMarker2;
    irr::gui::IGUIImage *       mPosMarker3;
    bool                        mPosMarkerVisible1;
    bool                        mPosMarkerVisible2;
    bool                        mPosMarkerVisible3;

    irr::gui::IGUIStaticText *  mStaticGhostRecord;
    irr::gui::IGUIStaticText *  mStaticGhostModeRecord;
    irr::gui::IGUIStaticText *  mStaticGhostGapInfo;
    irr::gui::IGUIStaticText *  mStaticGhostGap;
    irr::gui::IGUIImage *       mImageGhostRecord;
    irr::gui::IGUIImage *       mImageGhostGap;

	irr::gui::IGUIStaticText *  mStaticSessionRecord;
	irr::gui::IGUIStaticText *  mStaticSessionModeRecord;
	irr::gui::IGUIStaticText *  mStaticSessionGapInfo;
	irr::gui::IGUIStaticText *  mStaticSessionGap;
	irr::gui::IGUIStaticText *  mStaticSessionActivePlayer;
	irr::gui::IGUIStaticText *  mStaticSessionPlayerName;
    irr::gui::IGUIImage *       mImageSessionRecord;
	irr::gui::IGUIImage *       mImageSessionGap;
	irr::gui::IGUIImage *       mImageSessionGap2;

    irr::u32 mEndTimeSpecial1;
    irr::u32 mEndTimeSpecial2;
    irr::u32 mEndTimeGap;
    irr::u32 mEndTimeCamera;
};

#endif // GUI_HUD
