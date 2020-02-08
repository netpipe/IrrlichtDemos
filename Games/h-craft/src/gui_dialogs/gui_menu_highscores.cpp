// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_highscores.h"
#include "gui_menu_main.h"
#include "../main.h"
#include "../gui.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../level.h"

using namespace irr;
using namespace gui;


GuiMenuHighscores::GuiMenuHighscores(const Config& config)
    : GuiDialog(config)
    , mActiveProfile(0)
    , mSliderScoreMode(0)
    , mSliderDifficulty(0)
    , mSliderTrack(0)
    , mLevelIndex(0)
    , mTrackIndex(0)
    , mDifficultyIndex(0)
    , mSeasonIndex(0)
{
    SetSuppressSceneRendering(true);
    for ( int i=0; i < MAX_HIGHSCORES; ++i )
        mStaticScore[i] = 0;
}

GuiMenuHighscores::~GuiMenuHighscores()
{
    RemoveFunctors();
}

bool GuiMenuHighscores::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("id_back")), new EventFunctor<GuiMenuHighscores>(this, &GuiMenuHighscores::OnButtonBack) );

        AddGuiEventFunctor( GetIdForName(std::string("scoretype")), new EventFunctor<GuiMenuHighscores>(this, &GuiMenuHighscores::OnSliderRacemode) );
        AddGuiEventFunctor( GetIdForName(std::string("trackdifficulty")), new EventFunctor<GuiMenuHighscores>(this, &GuiMenuHighscores::OnSliderDifficulty) );
        AddGuiEventFunctor( GetIdForName(std::string("track")), new EventFunctor<GuiMenuHighscores>(this, &GuiMenuHighscores::OnSliderTrack) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuArcade::Load");

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mSliderScoreMode = static_cast<IGUITextSlider*>(GetElementByName(root, "scoretype", errorMsg));
        mSliderDifficulty = static_cast<IGUITextSlider*>(GetElementByName(root, "trackdifficulty", errorMsg));
        mSliderTrack = static_cast<IGUITextSlider*>(GetElementByName(root, "track", errorMsg));
        mStaticScore[0] = static_cast<IGUIStaticText*>(GetElementByName(root, "hs1", errorMsg));
        mStaticScore[1] = static_cast<IGUIStaticText*>(GetElementByName(root, "hs2", errorMsg));
        mStaticScore[2] = static_cast<IGUIStaticText*>(GetElementByName(root, "hs3", errorMsg));
        mStaticScore[3] = static_cast<IGUIStaticText*>(GetElementByName(root, "hs4", errorMsg));
        mStaticScore[4] = static_cast<IGUIStaticText*>(GetElementByName(root, "hs5", errorMsg));

        if ( mSliderScoreMode )
        {
            mSliderScoreMode->clearTexts();
            mSliderScoreMode->addText( APP.GetStringTable()->Get("id_score_championship").c_str() );
            mSliderScoreMode->addText( APP.GetStringTable()->Get("id_score_timerace").c_str() );
            mSliderScoreMode->addText( APP.GetStringTable()->Get("id_score_bestlap").c_str() );
            ChangedMode();
        }
    }
    return ok;
}

bool GuiMenuHighscores::IsChampionshipMode() const
{
    if ( !mSliderScoreMode)
        return true;
    if ( 0 == mSliderScoreMode->getCurrentTextId() )
        return true;

    return false;
}

void GuiMenuHighscores::ChangedMode()
{
    if ( mSliderScoreMode && mSliderDifficulty && mSliderTrack)
    {
        if ( IsChampionshipMode() )
        {
            mSliderDifficulty->clearTexts();
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_season_all").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_season_1").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_season_2").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_season_3").c_str() );
            mSliderDifficulty->setCurrentTextId(mSeasonIndex);

            GuiHelper::FillTextSliderWithLevelNames(mSliderTrack, GetSeason());
            if ( mLevelIndex >= 0 )
                mSliderTrack->setCurrentTextId(mLevelIndex);
        }
        else
        {
            mSliderDifficulty->clearTexts();
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_all").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_easy").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_medium").c_str() );
            mSliderDifficulty->addText( APP.GetStringTable()->Get("id_dif_hard").c_str() );
            mSliderDifficulty->setCurrentTextId(mDifficultyIndex);

            GuiHelper::FillTextSliderWithTrackNames(mSliderTrack, GetDifficulty(), false);
            if ( mTrackIndex >= 0 )
                mSliderTrack->setCurrentTextId(mTrackIndex);
        }
    }
}

void GuiMenuHighscores::Show()
{
    if ( mActiveProfile )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            mActiveProfile->setText( profile->mName.c_str() );
        }
        else
        {
            mActiveProfile->setText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
        }
    }

    if ( mSliderScoreMode && mSliderDifficulty && mSliderTrack)
    {
        int trackIndex = mSliderTrack->getCurrentTextId();

        EScoreTable scoreTable = EST_CHAMPIONSHIP;
        switch ( mSliderScoreMode->getCurrentTextId() )
        {
            case 0:
            {
                scoreTable = EST_CHAMPIONSHIP;
                int numRacesPerSeason = APP.GetChampionship()->GetNumRaces() / 3;
                switch ( GetSeason() )
                {
                    case SEASON_2:
                        trackIndex += numRacesPerSeason;
                    break;
                    case SEASON_3:
                        trackIndex += 2 * numRacesPerSeason;
                    break;
                    default:
                    break;
                }
                break;
            }
            case 1:
            {
                scoreTable = EST_TIMERACE;
                trackIndex = APP.GetLevelManager()->GetIndexForName( mSliderTrack->getCurrentText() );
                break;
            }
            case 2:
            {
                scoreTable = EST_BESTLAP;
                trackIndex = APP.GetLevelManager()->GetIndexForName( mSliderTrack->getCurrentText() );
                break;
            }
            default:
                break;
        }

        Highscores * hs = APP.GetHighscoreManager()->GetHighscores(scoreTable, trackIndex);

        for ( int i=0; i < MAX_HIGHSCORES; ++i )
        {
            if ( !mStaticScore[i] )
                continue;

            HScoreEntry entry;
            if ( hs )
            {
                entry = hs->GetScore(i);
            }
            std::wstring wstr( entry.mProfileName );
            wstr += L" ";
            wstr += GuiHelper::MakeTimeString(entry.mTime);

            mStaticScore[i]->setText(wstr.c_str());
        }
    }

    GuiDialog::Show();
}

void GuiMenuHighscores::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;
    RemoveGuiEventFunctor( GetIdForName(std::string("id_back")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("scoretype")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("trackdifficulty")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("track")) );
}

bool GuiMenuHighscores::OnButtonBack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuMain() );
    }

    return false;
}

bool GuiMenuHighscores::OnSliderRacemode(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
        return false;

    ChangedMode();
    Show();
    return false;
}

bool GuiMenuHighscores::OnSliderDifficulty(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
        return false;

    if ( IsChampionshipMode() )
    {
        mLevelIndex = 0;
        mSeasonIndex = mSliderDifficulty->getCurrentTextId();
    }
    else
    {
        mTrackIndex = 0;
        mDifficultyIndex = mSliderDifficulty->getCurrentTextId();;
    }
    ChangedMode();
    Show();
    return false;
}

bool GuiMenuHighscores::OnSliderTrack(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType != (gui::EGUI_EVENT_TYPE)EGET_TEXTSLIDER_CHANGED )
        return false;

    if ( IsChampionshipMode() )
    {
        mLevelIndex = mSliderTrack->getCurrentTextId();
    }
    else
    {
        mTrackIndex = mSliderTrack->getCurrentTextId();
    }

    Show();
    return false;
}

CS_Season GuiMenuHighscores::GetSeason() const
{
    if ( !mSliderDifficulty )
    {
        return SEASON_ALL;
    }
    switch ( mSliderDifficulty->getCurrentTextId() )
    {
        case 0:
            return SEASON_ALL;
        case 1:
            return SEASON_1;
        case 2:
            return SEASON_2;
        case 3:
            return SEASON_3;
        default:
            break;
    }
    return SEASON_3;
}


LevelDifficulty GuiMenuHighscores::GetDifficulty() const
{
    if ( !mSliderDifficulty )
    {
        return LD_ALL;
    }
    switch ( mSliderDifficulty->getCurrentTextId() )
    {
        case 0:
            return LD_ALL;
        case 1:
            return LD_EASY;
        case 2:
            return LD_MEDIUM;
        case 3:
            return LD_HARD;
        default:
            break;
    }
    return LD_HARD;
}
