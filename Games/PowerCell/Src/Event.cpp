#include "Event.hpp"

MyEventReceiver::MyEventReceiver()
{
  for(s32 i = 0; i < KEY_KEY_CODES_COUNT; i++)
  {
    keys[i] = false;
  }
}

bool MyEventReceiver::OnEvent(const SEvent &event)
{
  if(event.EventType == EET_KEY_INPUT_EVENT)
  {
    keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
  }

  if(event.EventType == EET_GUI_EVENT)
  {
    s32 id = event.GUIEvent.Caller->getID();
    IGUIEnvironment *env = irrDevice->getGUIEnvironment();


    switch(event.GUIEvent.EventType)
    {
      case EGET_BUTTON_CLICKED:
      {
        switch(id)
        {
          case BUTTON_MAIN_MENU_STAGE1:
          {
            Output::Instance()->w("Loading stage 1\n");
            env->clear();
            GameGUI::Instance()->Init();
            Scene::Instance()->LoadStage( Stage::Instance()->GetSceneFile(0) );
            Scene::Instance()->SetState(STATE_INGAME);
         //    Sound::Instance()->PlayBackgroundMusic(MUSIC_STAGE1);
          } break;
          case BUTTON_MAIN_MENU_STAGE2:
          {
            Output::Instance()->w("Loading stage 2\n");
            env->clear();
            GameGUI::Instance()->Init();
            Scene::Instance()->LoadStage( Stage::Instance()->GetSceneFile(1) );
            Scene::Instance()->SetState(STATE_INGAME);
        //    Sound::Instance()->PlayBackgroundMusic(MUSIC_STAGE2);
          } break;
          case BUTTON_MAIN_MENU_STAGE3:
          {
            Output::Instance()->w("Loading stage 3\n");
            env->clear();
            GameGUI::Instance()->Init();
            Scene::Instance()->LoadStage( Stage::Instance()->GetSceneFile(2) );
            Scene::Instance()->SetState(STATE_INGAME);
        //    Sound::Instance()->PlayBackgroundMusic(MUSIC_STAGE3);
          } break;
          case BUTTON_MAIN_MENU_STAGE4:
          {
            Output::Instance()->w("Loading stage 4\n");
            env->clear();
            GameGUI::Instance()->Init();
            Scene::Instance()->LoadStage( Stage::Instance()->GetSceneFile(3) );
            Scene::Instance()->SetState(STATE_INGAME);
          //  Sound::Instance()->PlayBackgroundMusic(MUSIC_STAGE4);
          } break;
        }
      } break;
    }
  }

  return false;
}
bool MyEventReceiver::getKeyState(EKEY_CODE key)
{
  return keys[key];
}
