/* High Score Library
highscore.cpp
*/

#include "HighScore.h"

HighScore::HighScore()
{
     initialise("HighScores.sco");
}

HighScore::HighScore(string ScoreFile)
{
     initialise(ScoreFile);
}

void HighScore::initialise(string ScoreFile)
{
     mInit = true;
     readFile();
     mScoreFile = ScoreFile;   
}

HighScore::~HighScore()
{
}
void HighScore::Drop(void)
{
     return;
}

bool HighScore::readFile()
{
    ifstream lScoreFile(mScoreFile.c_str(),ios::in);    
    if(lScoreFile.is_open())
    {
         int i = 0;
         mNumOfEntrys = 0;
         while ( ! lScoreFile.eof() && i < MAX_SCORES)
         {
               lScoreFile >> mScoreArr[i].mScoreName;
               lScoreFile >> mScoreArr[i].mScoreVal;
               mLowScore = mScoreArr[i].mScoreVal;
               i++;
         }
         mNumOfEntrys = i;
    }else
         return true;   
    lScoreFile.close();
    return true;  
}

bool HighScore::writeFile()
{
    ofstream lScoreFile(mScoreFile.c_str(),ios::out);     
    if(lScoreFile.is_open())
    {  
        for(unsigned i = 0;i < mNumOfEntrys;i++)
        {
           if(i < MAX_SCORES){           
               lScoreFile << mScoreArr[i].mScoreName << endl;
               lScoreFile << mScoreArr[i].mScoreVal << endl;
           }
        }
     }
     lScoreFile.close();
    return true;
}

int HighScore::getNumScores()
{
    if(mNumOfEntrys > MAX_SCORES)
       return MAX_SCORES;
    return mNumOfEntrys;
}

HighScore_Score HighScore::getScore(int getScore)
{
     return mScoreArr[getScore];
}

bool HighScore::addScore(ScoreVal SubmitScore,ScoreName SubmitName)
{
    if(readFile())
    {
        if(mLowScore < SubmitScore){
                int lPlaceHere = -1;
                for(unsigned i = 0;i < mNumOfEntrys;i++){
                    if(mScoreArr[i].mScoreVal < SubmitScore){
                         if(lPlaceHere == -1)
                            lPlaceHere = i;
                         if(i+1 < MAX_SCORES)
                         {
                            mScoreArr[i+1].mScoreName = mScoreArr[i].mScoreName;                             
                            mScoreArr[i+1].mScoreVal = mScoreArr[i].mScoreVal;  
                         }
                    }
                }
                mScoreArr[lPlaceHere].mScoreName = SubmitName;                             
                mScoreArr[lPlaceHere].mScoreVal = SubmitScore;     
        }
    }
    else
       return false;
    if(mNumOfEntrys < MAX_SCORES)
       mNumOfEntrys++;
    writeFile();
    return true;
}
