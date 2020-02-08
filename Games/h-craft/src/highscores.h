// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <string>
#include <vector>

#define MAX_HIGHSCORES 5

class IArchive;

enum EScoreTable
{
    EST_CHAMPIONSHIP,
    EST_TIMERACE,
    EST_BESTLAP,
};

struct HScoreEntry
{
    HScoreEntry();

    bool IsUserScore() const;

    std::wstring    mProfileName;
    unsigned int    mTime;
};

class Highscores
{
public:
    Highscores()    {}
    ~Highscores()   {}

    void Load(IArchive * archive_);
    void Save(IArchive * archive_);

    // return index where it score will be positioned, -1 for no highscore
    int CheckPosition(unsigned int time_) const;
    int AddEntry(const HScoreEntry & score_);

    const HScoreEntry& GetScore(int index_) const;

private:
    HScoreEntry mScores[MAX_HIGHSCORES];
};

class HighscoreManager
{
public:
    HighscoreManager();
    ~HighscoreManager();

    bool LoadOrCreate(); // create highscores if they can't be loaded
    bool Save();

    Highscores* GetHighscores(const EScoreTable & table_, int index_);

protected:
    Highscores* GetChampionshipScores(int idx_);
    Highscores* GetTimeraceScores(int idx_);
    Highscores* GetLapScores(int idx_);

    bool Load();
    bool Create();
    void ClearAllScores();

private:
    std::vector<Highscores*> mScoresChampionship;
    std::vector<Highscores*> mScoresTimerace;
    std::vector<Highscores*> mScoresLap;
};

#endif // HIGHSCORES_H
