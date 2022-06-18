#include "Highscore.hpp"

Highscore *Highscore::m_highscore = NULL;
Highscore Highscore::m_default_highscore_buffer;

Highscore *Highscore::Instance()
{
  if(!m_highscore)
    m_highscore = &m_default_highscore_buffer;

  return m_highscore;
}

bool Highscore::LoadHighScore(void)
{
  Output::Instance()->w("Loading high score.\n");

  scoreFileIn.open( Stage::Instance()->GetScoreFile().c_str() );

  if(!scoreFileIn)
  {
  	/*
    strcpy(highScore[0].name, "Insane");
    highScore[0].score = 46;
    highScore[0].time = uberHighScore;

    strcpy(highScore[1].name, "Expert");
    highScore[1].score = 46;
    highScore[1].time = uberHighScore+10;

    strcpy(highScore[2].name, "Average");
    highScore[2].score = 46;
    highScore[2].time = uberHighScore+20;

    strcpy(highScore[3].name, "Newbie");
    highScore[3].score = 46;
    highScore[3].time = uberHighScore+30;
		*/
    for(u32 i = 0; i < 10; i++)
    {
      strcpy(highScore[i].name, "-");
      highScore[i].score = 0;
      highScore[i].time = 999;
    }
    return false;
  }
  else
  {
    scoreFileIn.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    scoreFileIn.close();
  }
  return true;
}

void Highscore::SaveHighScore(void)
{
  Output::Instance()->w("Saving high score.\n");

  scoreFileOut.open( Stage::Instance()->GetScoreFile().c_str() );
  scoreFileOut.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));
  scoreFileOut.close();
}

void Highscore::SortHighScore(void)
{
  Output::Instance()->w("Sorting high score.\n");

  u32 i, j, flag = 1;
  HIGHSCORE temp;
  u32 len = 10;

  for(i = 0; (i <= len) && flag; i++)
	{
		flag = 0;
		for(j = 0; j < (len-1); j++)
		{
			if(highScore[j+1].time < highScore[j].time)
			{
				temp = highScore[j];
				highScore[j] = highScore[j+1];
				highScore[j+1] = temp;
				flag = 1;
			}
		}
	}
  for(i = 0; (i <= len) && flag; i++)
	{
		flag = 0;
		for(j = 0; j < (len-1); j++)
		{
			if(highScore[j+1].score > highScore[j].score)
			{
				temp = highScore[j];
				highScore[j] = highScore[j+1];
				highScore[j+1] = temp;
				flag = 1;
			}
		}
	}

}

void Highscore::PrintHighScore(void)
{
  printf("Highscore List:\n");
  printf("Name           | Time     | Cells    |\n");
  for(u32 i = 0; i < 10; i++)
  {
    printf("%15s|", highScore[i].name);
    printf("%10d|", highScore[i].time);
    printf("%10d|\n", highScore[i].score);
  }
}

void Highscore::CheckPlayerHighScore(u32 time, u32 totalAquired)
{
  LoadHighScore();
  Output::Instance()->w("Checking high score.\n");

  //! Check if our score is higher than the lowest on the the list.
  if(highScore[9].score < totalAquired)
  {
    newHighScore = true;
  }
  //! If our score is equal, check if our time is better.
  else if((highScore[9].score == totalAquired) && (highScore[9].time > time))
  {
    newHighScore = true;
  }

  if(newHighScore)
  {
    printf("Congratulations!\nYou made it to the high score list!\n");

    strcpy(highScore[9].name, "* Player *");
    highScore[9].time = time;
    highScore[9].score = totalAquired;
  }
  else
    printf("Sorry, you didn't qualify for the high score list.\n");

  SortHighScore();
  PrintHighScore();
  SaveHighScore();
}

stringc Highscore::GetTopHighScoreAsString(s32 stage)
{
	Stage::Instance()->GetSceneFile(stage);

	stringc str = L"Stage ";

	str += stage+1;
	str += L": ";

	if(!LoadHighScore())
	{
		str += L"No highscore!\n";
		return str;
	}

	str += highScore[0].name;
	str += L" got ";
	str += highScore[0].score;
	str += L" cells in ";
	str += highScore[0].time;
	str += L" seconds, ";
	str += L"\n";

	return str;
}
