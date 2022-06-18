#ifndef _HIGHSCORE_HPP_
#define _HIGHSCORE_HPP_

#include "Globals.hpp"

class Highscore
{
  private:

    struct HIGHSCORE
    {
      char name[25];
      u32 score;
      u32 time;
    }; HIGHSCORE highScore[10];

    bool newHighScore;

    ofstream scoreFileOut;
    ifstream scoreFileIn;

  public:

    static Highscore *Instance();

    bool LoadHighScore(void);
    void SaveHighScore(void);
    void SortHighScore(void);
    void PrintHighScore(void);
    void CheckPlayerHighScore(u32 time, u32 totalAquired);
    stringc GetTopHighScoreAsString(s32 stage);

  protected:

    static Highscore *m_highscore;
    static Highscore m_default_highscore_buffer;

};

#endif
