#include "Stage.hpp"

Stage *Stage::m_stage = NULL;
Stage Stage::m_default_stage_buffer;

Stage *Stage::Instance()
{
  if(!m_stage)
    m_stage = &m_default_stage_buffer;

  return m_stage;
}

void Stage::SetDefaultValues(void)
{
  //! All of this should be dynamic.
  //! Currently there's no way to save/load unlocked status. (Could be added to score file)
  STAGE temp;

  temp.SceneFile = "Stage1";
  temp.ScoreFile = "Stage1/Stage1.sco";
  temp.Unlocked = true;
  Stages.push_back(temp);

  temp.SceneFile = "Stage2";
  temp.ScoreFile = "Stage2/Stage2.sco";
  temp.Unlocked = true;
  Stages.push_back(temp);

  temp.SceneFile = "Stage3";
  temp.ScoreFile = "Stage3/Stage3.sco";
  temp.Unlocked = true;
  Stages.push_back(temp);

  temp.SceneFile = "Stage4";
  temp.ScoreFile = "Stage4/Stage4.sco";
  temp.Unlocked = true;
  Stages.push_back(temp);
}

stringc Stage::GetSceneFile(u32 stage)
{
  currentStage = stage;
  return Stages[stage].SceneFile;
}

stringc Stage::GetScoreFile(void)
{
  return Stages[currentStage].ScoreFile;
}

bool Stage::GetUnlocked(u32 stage)
{
  return Stages[stage].Unlocked;
}
