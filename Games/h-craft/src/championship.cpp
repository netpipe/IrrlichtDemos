// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "championship.h"
#include "config.h"
#include "level.h"
#include "main.h"
#include "game.h"
#include "logging.h"
#include "app_restrictions.h"
#include "tinyxml/tinyxml.h"

// --------------------------------------------------------
void CS_Requirement::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_)
        return;

    settings_->QueryIntAttribute("points", &mPoints );
}


// --------------------------------------------------------
void CS_Race::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    if ( TIXML_SUCCESS != settings_->QueryIntAttribute("level_id", &mLevelId )  )
    {
        LOG.Warn("missing level_id in node <entry>\n");
        return;
    }

    if ( TIXML_SUCCESS != settings_->QueryIntAttribute("laps", &mLaps )  )
    {
        const LevelSettings & levSettings = APP.GetLevelManager()->GetLevel( APP.GetLevelManager()->GetIndexForId(mLevelId) );
        mLaps = levSettings.mLaps;
    }

    settings_->QueryFloatAttribute("time1", &mTime1Seconds );
    settings_->QueryFloatAttribute("time2", &mTime2Seconds );
    settings_->QueryFloatAttribute("time3", &mTime3Seconds );
}


// --------------------------------------------------------
CS_RaceResult::CS_RaceResult()
{
    mIsPlayed = false;
    mIsOpen = false;
    mScore = 0;
}

void CS_RaceResult::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    int val=0;
    settings_->QueryIntAttribute("p", &val);
    mIsPlayed = val ? true : false;
    settings_->QueryIntAttribute("o", &val);
    mIsOpen = val ? true : false;
    settings_->QueryIntAttribute("s", &mScore);
}

void CS_RaceResult::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;
    settings_->SetAttribute("p", mIsPlayed ? 1 : 0 );
    settings_->SetAttribute("o", mIsOpen ? 1 : 0 );
    settings_->SetAttribute("s", mScore );
}



// --------------------------------------------------------
ChampionshipProgress::ChampionshipProgress()
{
    Reset();
}

void ChampionshipProgress::Reset()
{
    mNextRaceNeeded = 0;
    mTotalScore = 0;
    mHasDisplayedTutorial3 = false;
    mHasDisplayedWinner = false;
    mHasSeenChampionshipProgress = false;
    mRaceResults.clear();

    if ( !APP.GetChampionship() || !APP.GetChampionship()->GetNumRaces())
    {
        LOG.Warn("WARNING: ChampionshipProgress::Reset should only be called after Championship::LoadSettings()\n");
        return;
    }

    for ( unsigned int i=0; i < APP.GetChampionship()->GetNumRaces(); ++i )
    {
        CS_RaceResult raceResult;

        if ( i == 0 )
            raceResult.mIsOpen = true;
        else
            raceResult.mIsOpen = false;
        raceResult.mScore = 0;
        mRaceResults.push_back(raceResult);
    }
}

bool ChampionshipProgress::HasWon() const
{
    if ( mRaceResults.size() > 0 && mRaceResults[ mRaceResults.size()-1 ].mIsPlayed )
        return true;
    return false;
}

int ChampionshipProgress::GetScoreForQuarter(int quarter_) const
{
    if ( !IsQuarterOpen(quarter_) )
        return 0;

    const CS_Requirement &req = APP.GetChampionship()->GetRequirement(quarter_);
    int result = 0;
    for ( int i=req.mFromRace; i <= req.mToRace; ++i )
    {
        result += mRaceResults[i].mScore;
    }
    return result;
}

int ChampionshipProgress::GetScoreSumForQuarter(int quarter_) const
{
   if ( !IsQuarterOpen(quarter_) )
        return 0;

    const CS_Requirement &req = APP.GetChampionship()->GetRequirement(quarter_);
    int result = 0;
    for ( int i=0; i <= req.mToRace; ++i )
    {
        result += mRaceResults[i].mScore;
    }
    return result;
}

int ChampionshipProgress::GetMaxScoreForQuarter(int quarter_) const
{
    if ( !IsQuarterOpen(quarter_) )
        return 0;


    const CS_Requirement &req = APP.GetChampionship()->GetRequirement(quarter_);
    int result = quarter_*9;

    for ( int i=req.mFromRace; i<= req.mToRace; ++i )
    {
        if ( mRaceResults[i].mIsPlayed )
            result += 3;
    }

    return result;
}

bool ChampionshipProgress::IsQuarterOpen(int quarter_) const
{
    if ( quarter_ >= (int)APP.GetChampionship()->GetNumRequirements() )
        return false;

    const CS_Requirement &req = APP.GetChampionship()->GetRequirement(quarter_);
    if ( req.mFromRace < 0 || req.mFromRace >= (int)mRaceResults.size()
        || req.mToRace < 0 || req.mToRace >= (int)mRaceResults.size() )
    {
        return false;
    }

    for ( int i=req.mFromRace; i <= req.mToRace; ++i )
    {
        if ( mRaceResults[i].mIsOpen )
            return true;
    }
    return false;
}

bool ChampionshipProgress::IsQuarterPlayedComplete(int quarter_) const
{
    if ( !IsQuarterOpen(quarter_) )
        return false;

    const CS_Requirement &req = APP.GetChampionship()->GetRequirement(quarter_);
    for ( int i=req.mFromRace; i <= req.mToRace; ++i )
    {
        if ( !mRaceResults[i].mIsPlayed )
            return false;
    }
    return true;
}

bool ChampionshipProgress::IsItTimeToDisplayTutorial3() const
{
    if ( !mHasDisplayedTutorial3 && IsQuarterPlayedComplete(0) )
        return true;
    return false;
}

void ChampionshipProgress::SetTutorial3WasDisplayed()
{
    mHasDisplayedTutorial3 = true;
}

bool ChampionshipProgress::IsItTimeToDisplayWinner() const
{
    if( !mHasDisplayedWinner && HasWon() )
        return true;
    return false;
}

void ChampionshipProgress::SetWinnerWasDisplayed()
{
    mHasDisplayedWinner = true;
}

void ChampionshipProgress::CalcScores()
{
    mTotalScore = 0;

    for ( unsigned int i=0; i<mRaceResults.size(); ++i )
    {
        mTotalScore += mRaceResults[i].mScore;
    }
}

void ChampionshipProgress::CalcRaceScore( int idx_, GameResult& gameResult_) const
{
    gameResult_.mChampionshipScore = 0;

    if ( idx_ < 0 || idx_ > (int)mRaceResults.size() )
    {
        LOG.Warn("ChampionshipProgress::CalcRaceScore invalid index\n");
        return;
    }

    const CS_Race &race = APP.GetChampionship()->GetRace(idx_);
    if ( race.mType == CSRT_QUALIFICATION )
    {
        float resultTime = (float)gameResult_.mLocalPlayerTrackTimeMs/1000.f;
        if ( resultTime < race.mTime1Seconds )
            gameResult_.mChampionshipScore = 3;
        else if ( resultTime < race.mTime2Seconds )
            gameResult_.mChampionshipScore = 2;
        else if ( resultTime < race.mTime3Seconds )
            gameResult_.mChampionshipScore = 1;
    }
    else if ( race.mType == CSRD_RACE )
    {
        switch ( gameResult_.mLocalPlayerPlacing )
        {
            case 1:
                gameResult_.mChampionshipScore = 3;
                break;
            case 2:
                gameResult_.mChampionshipScore = 2;
                break;
            case 3:
                gameResult_.mChampionshipScore = 1;
                break;
            default:
                gameResult_.mChampionshipScore = 0;
        }
    }
    else
    {
        LOG.Warn("ChampionshipProgress::CalcRaceScore unknwn race type\n");
    }
}

void ChampionshipProgress::UpdateRace(int idx_, const GameResult& gameResult_)
{
    if ( idx_ < 0 || idx_ > (int)mRaceResults.size() )
    {
        LOG.Warn("ChampionshipProgress::UpdateRace invalid index\n");
        return;
    }

    if ( gameResult_.mChampionshipScore > mRaceResults[idx_].mScore )
    {
        mRaceResults[idx_].mScore = gameResult_.mChampionshipScore;
        CalcScores();
    }
    mRaceResults[idx_].mIsPlayed = true;
    CalcOpenRaces();
}

void ChampionshipProgress::CalcOpenRaces()
{
    if ( !mRaceResults[mNextRaceNeeded].mIsPlayed )
        return;

    int checkRace = mNextRaceNeeded+1;
    if ( CanOpen( checkRace ) )
    {
        mRaceResults[checkRace].mIsOpen = true;
        mNextRaceNeeded = checkRace;
    }

    ReCheckNextRace();  // the key could be added while the game is running
}

bool ChampionshipProgress::CanOpen( int checkRace_) const
{
    for ( unsigned int i=0; i < APP.GetChampionship()->GetNumRequirements(); ++i )
    {
        const CS_Requirement &requirement = APP.GetChampionship()->GetRequirement(i);

        if (    requirement.mFromRace <= checkRace_
            &&  requirement.mToRace >= checkRace_ )
        {
            if ( mTotalScore >= requirement.mPoints )
            {
                return true;
            }
        }
    }
    return false;
}

// After unlocking the demo we might have to change the next race,
// as for the demo one race in the middle of the championship can be played.
// Also we have special rules for the demo itself.
void ChampionshipProgress::ReCheckNextRace()
{
    if ( !IS_DEMO_VERSION && mNextRaceNeeded > 0 && !mRaceResults[mNextRaceNeeded-1].mIsOpen )
    {
        mRaceResults[mNextRaceNeeded].mIsOpen = false;
        mRaceResults[mNextRaceNeeded].mIsPlayed = false;
        mRaceResults[mNextRaceNeeded].mScore = 0;
        mNextRaceNeeded = 0;
        while( mRaceResults[mNextRaceNeeded+1].mIsOpen )
        {
            ++mNextRaceNeeded;
        }
        if ( CanOpen( mNextRaceNeeded+1 ) )
        {
            mRaceResults[mNextRaceNeeded+1].mIsOpen = true;
            ++mNextRaceNeeded;
        }
        CalcScores();
    }
    if ( IS_DEMO_VERSION )
    {
        if ( mNextRaceNeeded >= 3 )
        {
            mRaceResults[3].mIsOpen = false;
            mRaceResults[18].mIsOpen = true;
            mNextRaceNeeded = 18;
        }
    }
}

void ChampionshipProgress::ReadFromXml(const TiXmlElement * settings_)
{
    if ( !settings_ )
        return;

    settings_->QueryIntAttribute("next", &mNextRaceNeeded);
    int tut3=0;
    settings_->QueryIntAttribute("tut3", &tut3);
    mHasDisplayedTutorial3 = tut3;
    int win=0;
    settings_->QueryIntAttribute("win", &win);
    mHasDisplayedWinner = win;
    int seenProgress = 0;
    settings_->QueryIntAttribute("prog", &seenProgress);
    mHasSeenChampionshipProgress = seenProgress;

    mRaceResults.clear();

    const TiXmlNode* nodeRaceResult = settings_->IterateChildren("res", NULL);
    while ( nodeRaceResult )
    {
        const TiXmlElement* eleRaceResult = nodeRaceResult->ToElement();
        if ( eleRaceResult )
        {
            CS_RaceResult raceResult;
            raceResult.ReadFromXml(eleRaceResult);
            mRaceResults.push_back(raceResult);
        }

        nodeRaceResult = settings_->IterateChildren("res", nodeRaceResult);
    }

    CalcScores();
    ReCheckNextRace();
}

void ChampionshipProgress::WriteToXml(TiXmlElement * settings_) const
{
    if ( !settings_ )
        return;

    settings_->Clear();
    settings_->SetAttribute("next", mNextRaceNeeded);

    int tut3= mHasDisplayedTutorial3 ? 1: 0;
    settings_->SetAttribute("tut3", tut3);

    int win = mHasDisplayedWinner ? 1 : 0;
    settings_->SetAttribute("win", win);

    int seenProgress = mHasSeenChampionshipProgress ? 1 : 0;
    settings_->SetAttribute("prog", seenProgress);

    for ( unsigned int i=0; i < mRaceResults.size(); ++i )
    {
        TiXmlElement ele("res");
        mRaceResults[i].WriteToXml(&ele);
        settings_->InsertEndChild( ele );
    }
}


// --------------------------------------------------------
Championship::Championship()
: mRaceSelected(-1)
{
}

int Championship::GetMaxScore() const
{
    return mRaces.size() * 3;
}

void Championship::LoadSettings()
{
    mRequirements.clear();
    mRaces.clear();

    if ( !APP.GetLevelManager()->GetNumLevels() )
    {
        LOG.Warn("WARNING: LevelManager should have levels before Championship::LoadSettings()\n");
    }

    std::string xmlName ( APP.GetConfig()->GetPathLevels() + "championship.xml" );
    TiXmlDocument xmlDoc(xmlName.c_str());
    xmlDoc.SetIrrFs(APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem(), TiXmlDocument::E_ON_READ_FAIL_ANDROID);

    if ( !xmlDoc.LoadFile() )
    {
        LOG.WarnLn("Championship::LoadSettings: could not load xml ", xmlName.c_str());
        LOG.DebugLn("TiXmlDocument::ErrorDesc: ", xmlDoc.ErrorDesc());
        return;
    }

    TiXmlNode * championshipNode = xmlDoc.FirstChild("championship");
	if (!championshipNode)
	{
	    LOG.WarnLn("Championship::LoadSettings: missing node championship in file ", xmlName.c_str() );
		return;
	}

    TiXmlNode* nodeEntry = championshipNode->IterateChildren("entry", NULL);
    while ( nodeEntry )
    {
        const TiXmlElement* eleEntry = nodeEntry->ToElement();
        if ( eleEntry )
        {
            const char * valType = eleEntry->Attribute("type");
            if ( valType && strlen(valType) )
            {
                bool isRace = false;
                if ( 0 == strcmp(valType, "REQ") )
                {
                    CS_Requirement req;
                    req.mFromRace = mRaces.size();
                    req.mToRace = req.mFromRace-1;
                    req.ReadFromXml( eleEntry );
                    mRequirements.push_back(req);
                }
                else if ( 0 == strcmp(valType, "QUAL") )
                {
                    isRace = true;
                    CS_Race race;
                    race.mType = CSRT_QUALIFICATION;
                    race.ReadFromXml( eleEntry );
                    mRaces.push_back( race );
                }
                else if ( 0 == strcmp(valType, "RACE") )
                {
                    isRace = true;
                    CS_Race race;
                    race.mType = CSRD_RACE;
                    race.ReadFromXml( eleEntry );
                    mRaces.push_back( race );
                }

                if ( isRace && mRequirements.size() )
                {
                    mRequirements[ mRequirements.size()-1 ].mToRace = mRaces.size()-1;
                }
            }
        }

        nodeEntry = championshipNode->IterateChildren("entry", nodeEntry);
    }

    if ( mRaces.size() )
    {
        mRaceSelected = 0;
    }
}

unsigned int Championship::GetNumRaces() const
{
    return mRaces.size();
}

const CS_Race&  Championship::GetRace(int idx_) const
{
    return mRaces[idx_];
}
