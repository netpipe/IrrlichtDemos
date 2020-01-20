#include "launcher.h"
#include "Logger.h"
using namespace irr;
bool LauncherEventReceiver::OnEvent(const SEvent& event){
  if(event.EventType == EET_GUI_EVENT){
    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType){
      case gui::EGET_BUTTON_CLICKED:
        switch(id){
          case GUI_ID_START_BUTTON:
          return stateElement[GUI_ID_START_BUTTON] = true;
        };
        break;
        default: return false;
    }
  }
  return false;
}
Launcher::Launcher(gui::IGUIEnvironment* env, IrrlichtDevice* device){
  this->gui = env;
  this->device = device;
  this->video = device->getVideoDriver();
  gui::IGUISkin* skin = env->getSkin();
  // skin->setColor(video::SColor())
  gui::IGUIFont* font = this->gui->getFont("./media/font/myfont.xml");
   if (font)
       skin->setFont(font);
  // this->gui->addStaticText(L"CASTLE BATTLE", core::rect<s32>(230, 50, 575, 100), false,false);
  gui::IGUIImage* image = this->gui->addImage(core::rect<s32>(0,0,800,600));
  image->setImage(video->getTexture("media/castlebattle.jpg"));

  this->players[0] = this->gui->addEditBox(L"Player 1", core::rect<s32>(10,300,250,330), false, 0, GUI_ID_PLAYER1_NAME);
  this->players[1] = this->gui->addEditBox(L"Player 2", core::rect<s32>(800-250,300,800-10,330), false, 0, GUI_ID_PLAYER2_NAME);
  this->players[0]->setMax(11);
  this->players[1]->setMax(11);
  this->start = this->gui->addButton(core::rect<s32>(300,500,500,550), 0, GUI_ID_START_BUTTON, L"START");
  this->listener = new LauncherEventReceiver();
  this->device->setEventReceiver(listener);
  log1("Initialized beginning screen");
}
LauncherEventReceiver::~LauncherEventReceiver(){
}
Launcher::~Launcher(){
}
void Launcher::endScreen(core::stringw* names, int winner){
    gui::IGUIImage* image = this->gui->addImage(core::rect<s32>(0,0,800,600));
    image->setImage(video->getTexture("media/castlebattle_end.jpg"));
    this->gui->addStaticText(names[winner].c_str(), core::rect<s32>(10,300,250,330), false,false)->setBackgroundColor(video::SColor(255,255,255,255));
    this->gui->addStaticText(names[!winner].c_str(), core::rect<s32>(800-250,300,800-10,330), false,false)->setBackgroundColor(video::SColor(255,255,255,255));
    //
    // this->players[winner] = this->gui->addEditBox(this->, core::rect<s32>(10,300,250,330), false, 0, GUI_ID_PLAYER1_NAME);
    // this->players[!winner] = this->gui->addEditBox(L"Player 2", core::rect<s32>(800-250,300,800-10,330), false, 0, GUI_ID_PLAYER2_NAME);
}
void Launcher::clear(){
  this->gui->clear();
  log1("Clearing GUI");
}
