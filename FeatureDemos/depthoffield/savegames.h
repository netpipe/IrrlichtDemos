#ifndef SAVEGAMES_H
#define SAVEGAMES_H
#include "joystick.h"
enum onoff {ON, OFF};
bool vsync_enabled;
bool fullscreen;
bool stencilbuffer;
bool DOF_enabled =true;
int resx;
int resy;
irr::IrrlichtDevice *device;

const char signature_menu[4]="MSF";
const char signature_game[4]="GSF";

class player; //defined in players.h

const char* dummy = "/dev/input/dum";
const char* dumrum = "/dev/input/dummy0";

int savemenu() {
   bool BOOLS[4] = { vsync_enabled, fullscreen, stencilbuffer, DOF_enabled};
   int INTS[3] = { track_count, resx, resy };
   float FLOATS [2] = { Music_Volume, Master_Volume };
   FILE *pFile = fopen ( "Menu.pns" , "wb" );
   if (pFile==NULL) {
#ifdef DEBUG
      std::cout << "Error 11: Could not open file for writing in binary mode " << pFile << std::endl;
#endif
      return 0;
   }
   bool success[4];
   success[0] = fwrite (signature_menu, sizeof(char) , 4, pFile);
   success[1] = fwrite (FLOATS , sizeof(float) , 2, pFile );
   success[2] = fwrite (INTS , sizeof(int), 3, pFile );
   success[3] = fwrite (BOOLS, sizeof(bool) , 4, pFile);
   fclose (pFile);
   if (success[0]&&success[1]&&success[2]&&success[3])
      return 1;
   else {
#ifdef DEBUG
      std::cout << "Error 8: Encountered a problem writing one or more arrays to the save file." << std::endl;
#endif
      return 0;
   }
}

int loadmenu (irrklang::ISound* sound=0, MusicEndedReceiver* musstop=0) {
   bool BOOLS[4];
   float FLOATS [2];
   int INTS[3];
   char signature[4];
   FILE * pFile = fopen ("Menu.pns" , "rb" );
   if (pFile==NULL) {
#ifdef DEBUG
      std::cout << "Error 9: Could not open file for reading in binary mode " << pFile << std::endl;
#endif
      if (!device) {
         res.x = 800;
         res.y = 600;
      }
      resx = res.x;
      resy = res.y;
      vsync_enabled=true;
      fullscreen=false;
      stencilbuffer=true;
      return 0;
   }
   fread (signature , sizeof(char), 4, pFile);
   fread (FLOATS , sizeof(float), 2, pFile);
   fread (INTS , sizeof(int), 3, pFile);
   fread (BOOLS , sizeof(bool), 4, pFile);
   if ((signature[0]==signature_menu[0])&&(signature[1]==signature_menu[1])&&(signature[2]==signature_menu[2])&&(signature[3]==signature_menu[3])) {
      Music_Volume = FLOATS[0];
      if (!device) {
         res.x = INTS[1];
         res.y = INTS[2];
      }
      resx = INTS[1];
      resy = INTS[2];
      vsync_enabled=BOOLS[0];
      fullscreen=BOOLS[1];
      stencilbuffer = BOOLS[2];
      Master_Volume = FLOATS[1];
      DOF_enabled = BOOLS[3];
      if (enginesound)
         enginesound->setSoundVolume(Master_Volume);
////////////////////////////////////////////////////////////SET DEVICE THINGIES
      if (sound) {
         sound->setVolume(Music_Volume);
         if ((sound)&&(track[track_count])&&(musstop)) {
            changing=true;
            enginesound->removeSoundSource(track[track_count]);
            std::ifstream inp;
            std::ofstream out;
            int trackc_prev = track_count;
            track_count = INTS[0];
            const char* myFileName = track[track_count];
            inp.open(myFileName, std::ifstream::in);
            inp.close();
            if(inp.fail()) {
               inp.clear(std::ios::failbit);
               sound = enginesound->play2D(track[trackc_prev], false, false, true);
            }
            else
               sound = enginesound->play2D(track[track_count], false, false, true);
         }
         sound->setSoundStopEventReceiver(musstop);
         changing=false;
      }
#ifdef DEBUG
      else {
         cout << "Error 5: irrklang::ISound, track array or MusicEndedReceiver are NULL" << endl;
      }
#endif
//////////////////////////////////////////////////////////////End of Device pointers
   }
   else {
#ifdef DEBUG
      cout << "Error 10: "<< pFile << " is corrupt!" << endl;
#endif
      if (!device) {
         res.x = 800;
         res.y = 600;
      }
      resx = res.x;
      resy = res.y;
      vsync_enabled=true;
      fullscreen=false;
      stencilbuffer=true;
      return 0;
   }
   fclose (pFile);
   return 1;
}
void setVolume(irrklang::ISound *sound, float volume) {
   Music_Volume = volume;
   savemenu();
   if (sound)
      sound->setVolume(Music_Volume);
}

int savejoy (joystick* joy1=NULL, joystick* joy2=NULL) {
   FILE *pFile = fopen ( "Joystick.pns" , "wb" );
   if (pFile==NULL) {
#ifdef DEBUG
      cout << "Error 11: Could not open file for writing in binary mode " << pFile << endl;
#endif
      return 0;
   }
   bool success[4];
   if ((joy1)&&(!joy2)) {
      success[0] = fwrite (joy1->getDeviceFile() , sizeof(char) , 15, pFile);
      success[1] = fwrite (joy1->getRumbleDeviceFile() , sizeof(char) , 18, pFile );
      success[2] = true;
      success[3] = true;
      fwrite (dummy , sizeof(char) , 15, pFile);
      fwrite (dumrum , sizeof(char) , 18, pFile );
   }
   else if ((!joy1)&&(joy2)) {
      success[0] = true;
      success[1] = true;
      fwrite (dummy , sizeof(char) , 15, pFile);
      fwrite (dumrum , sizeof(char) , 18, pFile );
      success[2] = fwrite (joy2->getDeviceFile() , sizeof(char) , 15, pFile);
      success[3] = fwrite (joy2->getRumbleDeviceFile() , sizeof(char) , 18, pFile );
   }
   else if ((joy1)&&(joy2)) {
      success[0] = fwrite (joy1->getDeviceFile() , sizeof(char) , 15, pFile);
      success[1] = fwrite (joy1->getRumbleDeviceFile() , sizeof(char) , 18, pFile );
      success[2] = fwrite (joy2->getDeviceFile() , sizeof(char) , 15, pFile);
      success[3] = fwrite (joy2->getRumbleDeviceFile() , sizeof(char) , 18, pFile );
   }
   else {
      success[0] = true;
      success[1] = true;
      success[2] = true;
      success[3] = true;
   }
   fclose (pFile);
   if (success[0]&&success[1]&&success[2]&&success[3])
      return 1;
   else {
#ifdef DEBUG
      cout << "Error 8: Encountered a problem writing one or more arrays to the save file." << endl;
#endif
      return 0;
   }
}

char jdf[15]="/dev/input/js0";
char rdf[18]="/dev/input/event0";
char jdf2[15]="/dev/input/js1";
char rdf2[18]="/dev/input/event1";

void loadjoy (int oneortwo=1) {
   FILE * pFile = fopen ("Joystick.pns" , "rb" );
   if (pFile==NULL) {
#ifdef DEBUG
      cout << "Error 9: Could not open file for reading in binary mode " << pFile << endl;
#endif
   }
   else {
      fread (jdf, sizeof(char), 15, pFile);
      fread (rdf, sizeof(char), 18, pFile);
      fread (jdf2, sizeof(char), 15, pFile);
      fread (rdf2, sizeof(char), 18, pFile);
   }
}

int savegame(FILE * pFile=NULL) {}
void loadgame(FILE * pFile=NULL) {
   return;
}

struct SAppContext {
   irr::s32 counter;
};

enum
{
	GUI_ID_QUIT_BUTTON,
	GUI_ID_LOAD_GAME_BUTTON,
	GUI_ID_OPTIONS_BUTTON,
        GUI_ID_RESUME_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR,
	GUI_ID_SINGLE_PLAYER_BUTTON,
	GUI_ID_SPLIT_SCREEN_BUTTON,
        GUI_ID_MAIN_MENU_BUTTON,
        GUI_ID_COUNT
};




class EventReceiver : public irr::IEventReceiver
{
public:
   EventReceiver(SAppContext & context) : Context(context) {
      for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
         KeyIsDown[i] = false;
      for (irr::u32 i=0; i<GUI_ID_COUNT; ++i)
         ButtonClicked[i] = false;
   }
   inline virtual bool OnEvent(const irr::SEvent &event) {
      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
      if (event.EventType == irr::EET_GUI_EVENT) {
         irr::s32 id = event.GUIEvent.Caller->getID();
         env = device->getGUIEnvironment();
         if (event.GUIEvent.EventType== irr::gui::EGET_SCROLL_BAR_CHANGED) {
            if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR) {
               irr::s32 pos = ((irr::gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
               for (irr::u32 i=0; i<irr::gui::EGDC_COUNT ; ++i) {
                  irr::video::SColor col = env->getSkin()->getColor((irr::gui::EGUI_DEFAULT_COLOR)i);
                  col.setAlpha(pos);                  env->getSkin()->setColor((irr::gui::EGUI_DEFAULT_COLOR)i, col);               }
            }
         }
         else if (event.GUIEvent.EventType== irr::gui::EGET_BUTTON_CLICKED)
            ButtonClicked[id]= true;
      }
      return false;
   }
   inline virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
      return KeyIsDown[keyCode];
   }
   inline virtual bool IsButtonClicked(int ButtonCode) {
      bool temp = ButtonClicked[ButtonCode];
      ButtonClicked[ButtonCode] = false;
      return temp;
   }
private:
   bool ButtonClicked[GUI_ID_COUNT];
   bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
   SAppContext & Context;
   irr::gui::IGUIEnvironment* env;
};
#endif
