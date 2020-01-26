/* High score
highscore.h

HighScore scr();
scr.setFile("scores.scr");
scr.setMaxScores(5);
scr.open("scores.scr");
scr.addScore(34,"bob");
scr.addScore(12,"tom");
if(scr.addScore(44,"Me");
   //Then we made the high score list
scr.startRead();

for(int i = 0;i < scr.getMaxScores();i++)
{
   if(scr.getScore(i)){
       cout << i+1 << ": " << scr.getCurrentName();
       cout << " " << scr.getCurrentScore(); << endl;
   }
   else
      cout << i+1 << ":" << endl;
      //Or simply 'Break;'  out
}
scr.endRead();
*/

#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include <string>
#include <iostream>
#include <fstream>

#define MAX_SCORES 10

using namespace std;

//You can change this to any type of score you want.
typedef unsigned int ScoreVal;
typedef std::string ScoreName;

struct HighScore_Score
{         
    ScoreVal mScoreVal;
    ScoreName mScoreName;      
};  

class HighScore {
      public:
             HighScore(void);
             HighScore(std::string ScoreFile);
             ~HighScore();
             void Drop(void);
             
             bool readFile();
             bool writeFile();
             
             //Returns false, if there was an error reading the file
             bool addScore(ScoreVal SubmitScore,ScoreName SubmitName);
             int getNumScores();
             HighScore_Score getScore(int getScore);                 
      private:
             void initialise(std::string ScoreFile);
             bool mInit;
             std::string mScoreFile; 
             int mLowScore;
             int mNumOfEntrys;
             HighScore_Score mScoreArr[MAX_SCORES];
};

#endif
