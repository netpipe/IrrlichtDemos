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

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//You can change this to any type of score you want.
typedef unsigned int ScoreVal;
typedef string ScoreName;
typedef unsigned short MaxScores;

class HighScore {
      public:
             HighScore(void);
             HighScore(string ScoreFile);
             ~HighScore();
             void Drop(void);
             
             void startRead(void);
             void endRead(void);
             
             //Returns false, if there was an error reading the file
             bool addScore(ScoreVal SubmitScore,ScoreName SubmitName);
             
             void setFile(string ScoreFile);
             void setMaxScores(MaxScores SetMaxScores = 10); 
             void setScorePosition(MaxScores mNewPosition);
             
             bool getScore(MaxScores skipScores = 0);

             ScoreName getCurrentName();
             ScoreVal  getCurrentScore();
             
             MaxScores getMaxScores();
             MaxScores getScorePosition();            
                   
      private:
             void initialise(string ScoreFile);
             bool mInit;
             ifstream *mInput;
             MaxScores mScorePosition;
             MaxScores mMaxScores;  
             string mScoreFile; 
             long mRecordLength;
             
             struct HighScore_Score
             {         
                 ScoreVal mScoreVal;
                 ScoreName mScoreName;       
             };
};
