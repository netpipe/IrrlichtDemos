#ifndef _Stage_HPP_
#define _Stage_HPP_

#include "Globals.hpp"

class Stage
{
  private:

    struct STAGE
    {
      stringc SceneFile;
      stringc ScoreFile;
      bool Unlocked;
    };

    array<STAGE> Stages;

    s32 currentStage;

  public:

    static Stage *Instance();

    void SetDefaultValues(void);

    stringc GetSceneFile(u32 stage);
    stringc GetScoreFile(void);

    bool GetUnlocked(u32 stage);

  protected:

    static Stage *m_stage;
    static Stage m_default_stage_buffer;

};

#endif
