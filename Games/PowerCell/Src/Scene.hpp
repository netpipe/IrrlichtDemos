#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "Globals.hpp"

class Scene
{
  private:

    ISceneNode *messageCleared;
    ICameraSceneNode *endCamera;

    IrrlichtDevice *irrDevice;

    int STATE_CURRENT;

    bool gameOver;

  public:

    static Scene *Instance();

    void SetDefaultValues(void);
    void EndGame(void); // Aquire playerNode from Player class.
    void UpdateEndCamera(vector3df pos);

    bool GameOver(void);
    void GameOver(bool val);

    void SetState(int state);
    int GetState(void);

    void LoadStage(stringc Stage);

    __inline void setDevice(IrrlichtDevice *device)
    {
      irrDevice = device;
    }
        vector3df startPoS;

  protected:

    static Scene *m_scene;
    static Scene m_default_scene_buffer;

};

#endif
