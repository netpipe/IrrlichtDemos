// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "rivalsmode.h"
#include "profiles.h"
#include "random.h"
#include "game.h"
#include "main.h"
#include "app_restrictions.h"

using namespace irr;

// --------------------------------------------------------
RivalsPlayer::RivalsPlayer()
{
    Clear();
}

void RivalsPlayer::Clear()
{
    mProfileId = -1;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mPlacings[i] = 0;
    }
    mSumTime = 0;
    mSumScore = 0;
}

// --------------------------------------------------------
RivalsRace::RivalsRace()
{
    mLevelIndex = -1;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mTimeResults[i] = Game::TIME_UNUSED;
    }
}

unsigned int RivalsRace::GetPlacingForPlayer(unsigned int idxPlayer_)
{
    if( Game::TIME_UNUSED == (s32)mTimeResults[idxPlayer_] )
        return MAX_RIVALS-1;

    unsigned int placing = 0;
    for ( unsigned int i=0; i < MAX_RIVALS; ++i )
    {
        if ( i != idxPlayer_
            && mTimeResults[idxPlayer_] > mTimeResults[i] )
        {
            ++placing;
        }
    }
    return placing;
}

// --------------------------------------------------------
RivalsMode::RivalsMode()
{
    mCurrentPlayer = 0;
    mCurrentRace = 0;
    mIsNewLevel = false;
}

void RivalsMode::ClearAll()
{
    mIsNewLevel = true;
    mTournamentFinished = false;
    mCurrentRace = 0;
    mRaces.clear();
    mCurrentPlayer = 0;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        mPlayers[i].Clear();
    }

    if ( APP.GetGame() )
    {
        APP.GetGame()->ResetSessionRecord();
    }
}

void RivalsMode::CreateRaces(unsigned int numRaces_)
{
    for ( unsigned int i=0; i < numRaces_; ++i )
    {
        mRaces.push_back(RivalsRace());
    }
}

void RivalsMode::GetGameSettingsForRace( unsigned int raceIdx_, GameSettings & gameSettings_)
{
    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(mRaces[raceIdx_].mLevelIndex, LD_ALL);

    gameSettings_.mGameType = GT_RIVALS;
    gameSettings_.mLevelName = levelSettings.mName;
    gameSettings_.mNrOfBots = 0;
    gameSettings_.mNrOfLaps = levelSettings.mLaps;
    gameSettings_.mTargetTime = 0;
    gameSettings_.mBotDifficulty = BD_CHAMPIONSHIP;
}

bool RivalsMode::IsLevelAvailable(int levelIdx_, LevelDifficulty levelDifficulty_)
{
//    const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(levelIdx_, levelDifficulty_);
//    return APP.GetProfileManager()->IsLevelUnlocked( levelSettings.mId );

    if ( IS_DEMO_VERSION )
    {
        const LevelSettings& levelSettings = APP.GetLevelManager()->GetLevel(levelIdx_, levelDifficulty_);
        if (    levelSettings.mId != 28
            &&  levelSettings.mId != 34
            &&  levelSettings.mId != 26
            &&  levelSettings.mId != 18
    //        &&  levelSettings.mId != 15
            )
        {
            return false;
        }
    }

    return true;
}

unsigned int RivalsMode::GetNumRacesAvailable(LevelDifficulty levelDifficulty_)
{
    unsigned int available = 0;
    for ( int i = 0; i < APP.GetLevelManager()->GetNumLevels(levelDifficulty_); ++i )
    {
        if ( IsLevelAvailable(i, levelDifficulty_) )
        {
            ++available;
        }
    }
    return available;
}

bool RivalsMode::RandomizeAllRaces(LevelDifficulty levelDifficulty_)
{
    // get all available levels
    std::vector<int> availableLevelIndices;
    for ( int i = 0; i < APP.GetLevelManager()->GetNumLevels(levelDifficulty_); ++i )
    {
        if ( IsLevelAvailable(i, levelDifficulty_) )
        {
            int realIndex = APP.GetLevelManager()->GetRealIndex( i, levelDifficulty_);
            availableLevelIndices.push_back( realIndex );
        }
    }

    if ( !availableLevelIndices.size() )
        return false;

    std::vector<int> indicesLeft;
    indicesLeft = availableLevelIndices;

    RandomGenerator randGen;
    for ( unsigned int i=0; i < mRaces.size(); ++i )
    {
        int randNr = randGen.GetNumberInRange( 0, indicesLeft.size()-1);
        mRaces[i].mLevelIndex = indicesLeft[randNr];
        indicesLeft.erase( indicesLeft.begin() + randNr );
        if ( !indicesLeft.size() )
        {
            indicesLeft = availableLevelIndices;
        }
    }

    return true;
}

RivalsPlayer* RivalsMode::GetPlayer(unsigned int idx_)
{
    if ( idx_ >= MAX_RIVALS )
        return NULL;

    return &(mPlayers[idx_]);
}

RivalsPlayer* RivalsMode::GetWinnerPlayer()
{
    RivalsPlayer* best = NULL;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        if ( mPlayers[i].mProfileId >= 0 )
        {
            if (   !best
                ||  mPlayers[i].mSumScore > best->mSumScore
                ||  (mPlayers[i].mSumScore == best->mSumScore
                    && mPlayers[i].mSumTime < best->mSumTime ) )
            {
                best = &(mPlayers[i]);
            }
        }
    }
    if ( best->mSumScore > 0 )
        return best;

    return NULL;
}

RivalsPlayer* RivalsMode::GetBestTimeSumPlayer()
{
    RivalsPlayer* best = NULL;
    for ( int i=0; i < MAX_RIVALS; ++i )
    {
        if ( mPlayers[i].mProfileId >= 0 )
        {
            if (    !best
                ||  mPlayers[i].mSumTime < best->mSumTime )
            {
                best = &(mPlayers[i]);
            }
        }
    }

    if ( (s32)best->mSumTime != Game::TIME_UNUSED )
        return best;

    return NULL;
}

void RivalsMode::InfoGameGiveUp()
{
    mPlayers[mCurrentPlayer].mSumTime = Game::TIME_UNUSED;
    mRaces[mCurrentRace].mTimeResults[mCurrentPlayer] = Game::TIME_UNUSED;
}

void RivalsMode::InfoGameFinished()
{
    const GameResult& gameResult =  APP.GetGame()->GetResult();

    if ( !gameResult.mGameWasFinished )
        return;

    if ( (s32)mPlayers[mCurrentPlayer].mSumTime != Game::TIME_UNUSED )
    {
        mPlayers[mCurrentPlayer].mSumTime += gameResult.mLocalPlayerTrackTimeMs;
    }
    mRaces[mCurrentRace].mTimeResults[mCurrentPlayer] = gameResult.mLocalPlayerTrackTimeMs;
}

void RivalsMode::InfoContinue()
{
    ++mCurrentPlayer;
    while ( mCurrentPlayer < MAX_RIVALS && mPlayers[mCurrentPlayer].mProfileId < 0 )
    {
        ++mCurrentPlayer;
    }

    if ( mCurrentPlayer >= MAX_RIVALS )
    {
        // race finished
        mCurrentPlayer = 0;
        mIsNewLevel = true;

        if ( APP.GetGame() )
        {
            APP.GetGame()->ResetSessionRecord();
        }

        for ( int i=0; i < MAX_RIVALS; ++i )
        {
            unsigned int placing = mRaces[mCurrentRace].GetPlacingForPlayer(i);
            ++ mPlayers[i].mPlacings[placing];

            switch ( placing )
            {
                case 0:
                    mPlayers[i].mSumScore += 3;
                    break;
                case 1:
                    mPlayers[i].mSumScore += 2;
                    break;
                case 2:
                    mPlayers[i].mSumScore += 1;
                    break;
                default:
                    break;
            }
        }

        if ( mCurrentRace < (int)mRaces.size()-1 )
        {
            ++mCurrentRace;
        }
        else
        {
            mTournamentFinished = true;
        }
    }
}

