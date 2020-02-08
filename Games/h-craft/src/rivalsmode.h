// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef RIVALSMODE_H
#define RIVALSMODE_H

#include "level.h"
#include "globals.h"
#include <vector>

struct GameSettings;

struct RivalsPlayer
{
    RivalsPlayer();

    void Clear();

    int mProfileId;             // -1 for invalid (not playing)
    unsigned int mPlacings[MAX_RIVALS];   // how often did the player get each placing. 0 for place 1, etc.
    unsigned int mSumTime;
    unsigned int mSumScore;
};

struct RivalsRace
{
    RivalsRace();

    unsigned int GetPlacingForPlayer(unsigned int idxPlayer_);   // [0 ; (MAX_RIVALS-1)]

    int mLevelIndex;
    unsigned int mTimeResults[MAX_RIVALS];   // results for each player
};

class RivalsMode
{
public:
    RivalsMode();

    void ClearAll();

    void CreateRaces(unsigned int numRaces_);
    bool RandomizeAllRaces(LevelDifficulty levelDifficulty_);
    int GetCurrentRace() const          { return mCurrentRace; }
    std::vector<RivalsRace>& GetRaces()  { return mRaces; }

    int GetCurrentPlayer() const        { return mCurrentPlayer; }
    RivalsPlayer* GetPlayer(unsigned int idx_);
    RivalsPlayer* GetWinnerPlayer();
    RivalsPlayer* GetBestTimeSumPlayer();

    void ResetIsNewLevel()                  { mIsNewLevel = false; }
    bool GetIsNewLevel() const              { return mIsNewLevel; }
    bool GetIsTournamentFinished() const    { return mTournamentFinished; }

    void GetGameSettingsForRace( unsigned int raceIdx_, GameSettings & settings_);

    void InfoGameGiveUp();
    void InfoGameFinished();
    void InfoContinue();

    static unsigned int GetNumRacesAvailable(LevelDifficulty levelDifficulty_);

protected:
    static bool IsLevelAvailable(int levelIdx_, LevelDifficulty levelDifficulty_);

private:
    int mCurrentPlayer;
    RivalsPlayer mPlayers[MAX_RIVALS];

    int mCurrentRace;
    std::vector<RivalsRace>  mRaces;

    bool mIsNewLevel;
    bool mTournamentFinished;
};

#endif // RIVALSMODE_H
