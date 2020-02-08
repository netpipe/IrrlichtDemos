// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "highscores.h"
#include "main.h"
#include "game.h"
#include "level.h"
#include "championship.h"
#include "config.h"
#include "streaming.h"
#include "helper_str.h"
#include "logging.h"
#include <assert.h>

HScoreEntry::HScoreEntry()
{
    mTime = Game::TIME_UNUSED;
    mProfileName = L"----";
}

bool HScoreEntry::IsUserScore() const
{
    return (irr::s32)mTime != Game::TIME_UNUSED;
}


void Highscores::Load(IArchive * archive_)
{
    assert(archive_);

    for ( int i=0; i<MAX_HIGHSCORES; ++i )
    {
        char * str = NULL;
        if ( archive_->ReadString(&str)  )
        {
            std::string dummyName(str);
            mScores[i].mProfileName = ExtStr::FromUtf8(dummyName);
            delete[] str;
        }
        archive_->Read(mScores[i].mTime);
    }
}

void Highscores::Save(IArchive * archive_)
{
    assert(archive_);

    for ( int i=0; i<MAX_HIGHSCORES; ++i )
    {
        std::string dummyName( ExtStr::ToUtf8(mScores[i].mProfileName) );
        archive_->WriteString( dummyName.c_str() );
        archive_->Write(mScores[i].mTime);
    }
}

// return index where it score will be positioned, -1 for no highscore
int Highscores::CheckPosition(unsigned int time_) const
{
    for ( int i=0; i < MAX_HIGHSCORES; ++i )
    {
        if ( time_ < GetScore(i).mTime )
            return i;
    }
    return -1;
}

int Highscores::AddEntry(const HScoreEntry & score_)
{
    int pos = CheckPosition(score_.mTime);
    if ( pos < 0 )
        return pos;
    for ( int i = MAX_HIGHSCORES-1; i > pos; --i )
    {
        mScores[i] = mScores[i-1];
    }
    mScores[pos] = score_;
    return pos;
}

const HScoreEntry& Highscores::GetScore(int index_) const
{
    if ( index_ < 0 || index_ >= MAX_HIGHSCORES )
    {
        assert(!"Highscores::GetScore invalid index");
        return mScores[0];
    }

    return mScores[index_];
}

// --------------------------------------------------------
HighscoreManager::HighscoreManager()
{
}

HighscoreManager::~HighscoreManager()
{
    ClearAllScores();
}

void HighscoreManager::ClearAllScores()
{
    for ( unsigned int i=0; i < mScoresChampionship.size(); ++i )
        delete mScoresChampionship[i];
    for ( unsigned int i=0; i < mScoresTimerace.size(); ++i )
        delete mScoresTimerace[i];
    for ( unsigned int i=0; i < mScoresLap.size(); ++i )
        delete mScoresLap[i];

    mScoresChampionship.clear();
    mScoresTimerace.clear();
    mScoresLap.clear();
}

bool HighscoreManager::LoadOrCreate()
{
    if ( Load() )
        return true;
    return Create();
}

bool HighscoreManager::Create()
{
    ClearAllScores();

    Championship* cs = APP.GetChampionship();
    if ( cs )
    {
        for ( unsigned int i=0; i < cs->GetNumRaces(); ++i )
        {
            mScoresChampionship.push_back( new Highscores() );
        }
    }

    LevelManager* lm = APP.GetLevelManager();
    if ( lm )
    {
        for ( int i=0; i < lm->GetNumLevels(); ++i )
        {
            mScoresTimerace.push_back( new Highscores() );
            mScoresLap.push_back( new Highscores() );
        }
    }

    return false;
}

bool HighscoreManager::Load()
{
    ClearAllScores();

    FileArchive archive;

    std::string scoreFile( APP.GetConfig()->MakeFilenameUserData("scores") );

    if (!archive.Open(scoreFile, "rb") )
        return false;

    unsigned int size = 0;
    archive.Read(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        Highscores * hs = new Highscores();
        hs->Load(&archive);
        mScoresChampionship.push_back(hs);
    }
    archive.Read(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        Highscores * hs = new Highscores();
        hs->Load(&archive);
        mScoresTimerace.push_back(hs);
    }
    archive.Read(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        Highscores * hs = new Highscores();
        hs->Load(&archive);
        mScoresLap.push_back(hs);
    }

    return true;
}

bool HighscoreManager::Save()
{
    FileArchive archive;

    std::string scoreFile( APP.GetConfig()->MakeFilenameUserData("scores") );

    if (!archive.Open(scoreFile, "wb") )
    {
		LOG.WarnLn("Warning: Can't save highscores to: ",  scoreFile.c_str());
        return false;
	}

    unsigned int size = mScoresChampionship.size();
    archive.Write(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        mScoresChampionship[i]->Save(&archive);
    }
    size = mScoresTimerace.size();
    archive.Write(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        mScoresTimerace[i]->Save(&archive);
    }
    size = mScoresLap.size();
    archive.Write(size);
    for ( unsigned int i=0; i < size; ++i )
    {
        mScoresLap[i]->Save(&archive);
    }

    return true;
}

Highscores* HighscoreManager::GetHighscores(const EScoreTable & table_, int index_)
{
    switch ( table_ )
    {
        case EST_CHAMPIONSHIP:
            return GetChampionshipScores(index_);
        case EST_TIMERACE:
            return GetTimeraceScores(index_);
        case EST_BESTLAP:
            return GetLapScores(index_);
    }

    assert(!"HighscoreManager::GetHighscores unknown scoretable");
    return GetChampionshipScores(index_);
}

Highscores* HighscoreManager::GetChampionshipScores(int idx_)
{
    if ( idx_ < 0 || idx_ >= (int)mScoresChampionship.size() )
        return NULL;
    return mScoresChampionship[idx_];
}

Highscores* HighscoreManager::GetTimeraceScores(int idx_)
{
    if ( idx_ < 0 || idx_ >= (int)mScoresTimerace.size() )
        return NULL;
    return mScoresTimerace[idx_];
}

Highscores* HighscoreManager::GetLapScores(int idx_)
{
    if ( idx_ < 0 || idx_ >= (int)mScoresLap.size() )
        return NULL;
    return mScoresLap[idx_];
}
