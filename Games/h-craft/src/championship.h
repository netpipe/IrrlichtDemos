// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include <vector>

class TiXmlElement;
struct GameResult;

struct CS_Requirement
{
    void ReadFromXml(const TiXmlElement * settings_);

    int mPoints;
    int mFromRace;
    int mToRace;
};

enum CS_RaceType
{
    CSRT_QUALIFICATION,
    CSRD_RACE,
};

struct CS_Race
{
    void ReadFromXml(const TiXmlElement * settings_);

    CS_RaceType mType;
    int         mLevelId;
    int         mLaps;
    float       mTime1Seconds;
    float       mTime2Seconds;
    float       mTime3Seconds;
};

struct CS_RaceResult
{
    CS_RaceResult();

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

    bool    mIsPlayed;  // race was finished regularly in the current championship
    bool    mIsOpen;    // race can be played in the current championship (requirements are met, but still not necessarily selectable)
    int     mScore;
};

class ChampionshipProgress
{
public:
    ChampionshipProgress();
    void Reset();

    unsigned int GetNumRaceResults() const              { return mRaceResults.size(); }
    const CS_RaceResult& GetRaceResult(int idx_) const  { return mRaceResults[idx_]; }

    // TODO: stupid name - something like highest race allowed
    int  GetNextRaceNeeded() const                      { return mNextRaceNeeded; }
    bool HasWon() const;

    int GetTotalScore() const   { return mTotalScore; }
    int GetScoreForQuarter(int quarter_) const;
    int GetScoreSumForQuarter(int quarter_) const;
    int GetMaxScoreForQuarter(int quarter_) const;
    bool IsQuarterOpen(int quarter_) const;
    bool IsQuarterPlayedComplete(int quarter_) const;
    bool IsItTimeToDisplayTutorial3() const;
    void SetTutorial3WasDisplayed();
    bool IsItTimeToDisplayWinner() const;
    void SetWinnerWasDisplayed();
    bool HasSeenChampionshipProgress() const            { return mHasSeenChampionshipProgress || mNextRaceNeeded > 0; }
    void SetHasSeenChampionshipProgress()               { mHasSeenChampionshipProgress = true; }

    void UpdateRace(int idx_, const GameResult& gameResult_);
    void CalcRaceScore( int idxRace_, GameResult& gameResult_) const;

    void ReadFromXml(const TiXmlElement * settings_);
    void WriteToXml(TiXmlElement * settings_) const;

protected:
    void CalcScores();
    void CalcOpenRaces();
    void ReCheckNextRace();
    bool CanOpen(int checkRace_) const;

private:
    int                         mNextRaceNeeded;
    int                         mTotalScore;
    bool                        mHasDisplayedTutorial3;
    bool                        mHasDisplayedWinner;
    bool                        mHasSeenChampionshipProgress;
    std::vector<CS_RaceResult>  mRaceResults;
};

class Championship
{
public:
    Championship();
    void LoadSettings();

    unsigned int GetNumRaces() const;
    const CS_Race&  GetRace(int idx_) const;
    unsigned int GetNumRequirements() const                 { return mRequirements.size(); }
    const CS_Requirement& GetRequirement(int idx_) const    { return mRequirements[idx_]; }

    int GetMaxScore() const;

    void SetRaceSelected(int selected_)                     { mRaceSelected = selected_; }
    int GetRaceSelected() const                             { return mRaceSelected; }

private:
    int                             mRaceSelected;
    std::vector<CS_Requirement>     mRequirements;
    std::vector<CS_Race>            mRaces;
};

#endif // CHAMPIONSHIP_H
