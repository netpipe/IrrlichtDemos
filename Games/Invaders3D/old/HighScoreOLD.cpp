/* High Score Library
highscore.cpp
*/
#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include "HighScore.h"

HighScore::HighScore(void)
{
     initialise("HighScores.scr");
}

HighScore::HighScore(string ScoreFile)
{
     initialise(ScoreFile);
}

void HighScore::initialise(string ScoreFile)
{
     mScorePosition = 0;
     mInit = true;
     mRecordLength = sizeof(HighScore_Score);
     mScoreFile = ScoreFile;   
}

HighScore::~HighScore()
{
}



void HighScore::Drop(void)
{
     return;
}

bool HighScore::addScore(ScoreVal SubmitScore,ScoreName SubmitName)
{
     //Find the lowest score
    // fstream lScoreFile(mScoreFile,ios::in | ios::out | ios::binary );
    fstream lScoreFile(mScoreFile.c_str(),ios::in | ios::out | ios::binary );
    // ifstream lScoreFileIn(mScoreFile,ios::in | ios::binary );
    // ofstream lScoreFileOut(mScoreFile,ios::out | ios::binary );
     if(lScoreFile.is_open())
     {
         //Tmp Read Record
         HighScore_Score s;
         //Get File Size
         long lBegin,lEnd,lRecordCount;
         lBegin = lScoreFile.tellg();
         lScoreFile.seekg(0,ios::end);
         lEnd = lScoreFile.tellg();
         lScoreFile.seekg(0,ios::beg);
         lRecordCount = (lEnd-lBegin);
         
         MaxScores lPlace = 0;
         if(lRecordCount != 0) //Non Empty File
         {
             lRecordCount /= mRecordLength;
             if(lRecordCount > mMaxScores)
                 lRecordCount = mMaxScores;
             for(MaxScores i=lRecordCount;i>0;i--)
             {
                  lScoreFile.read((char*)&s,mRecordLength);
                  if(s.mScoreVal < SubmitScore){ //We found a weak record
                       lPlace = i;
                       //Shuffle Records
                       //if(lPlace
                  }
                  else // No more offending records exist.
                       break;
             }
             //Sorry,  you just sucked :(
             if(lPlace == 0) return true;
             lPlace -= 1;
             //Shuffle Values
             for(MaxScores i=lPlace;i<=lRecordCount;i++){
                  lScoreFile.seekg(lPlace*mRecordLength,ios::beg);
//                  lScoreFile                 
             }
         }
             //Place our new score in
             s.mScoreVal = SubmitScore;
             s.mScoreName = SubmitName;
             lScoreFile.seekg(lPlace*mRecordLength,ios::beg);
             lScoreFile.write((char*)&s,mRecordLength);
         
             
     }
     else
         return false;
         
     //Output to the lowest score
     /*ofstream lOutput(mScoreFile);
     if(lOutput.is_open())
     {
        
     }*/
             
}
             
void HighScore::setMaxScores(MaxScores SetMaxScores)
{
     return;
}
void HighScore::setScorePosition(MaxScores mNewPosition)
{
     return;
}
             
/*bool HighScore::getScore(MaxScores skipScores);
{
     
     mScorePosition++;
     return true;
     //if(
}*/
bool HighScore::getScore(MaxScores skipScores)
{
     return false;
}
MaxScores HighScore::getMaxScores()
{
    return 1;
}

MaxScores HighScore::getScorePosition()
{
    return 1;
} 


#endif
