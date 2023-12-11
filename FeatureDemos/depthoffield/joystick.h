#ifndef JOYSTICK_H
#define JOYSTICK_H
#include "engine_sound.h"

enum joy_type { Gamepad, Wii };
class joystick {
public:
   joystick (char* jdf, joy_type type1, char* rdf);
   ~joystick () {}
   bool check_exist();
   void check_rumble();
   char* getDeviceFile () {return joystick_device_file;}
   char* getRumbleDeviceFile () {return rumble_device_file;}
   void setType (joy_type j) {type=j;}
   void rumble (int length, int strength);
   joy_type getType () {return type;}
private:
   bool works;
   int fd;
   bool rumbleb;
   char* joystick_device_file;
   joy_type type;
   char* rumble_device_file;
};

void joystick::check_rumble() {
   std::ifstream inp;
   if ((((rumble_device_file[0]=='n')||(rumble_device_file[0]=='N'))&&((rumble_device_file[1]=='o')||(rumble_device_file[1]=='O')))||(!rumble_device_file))
      rumbleb=false;
   else {
      inp.open(rumble_device_file, std::ifstream::in);
      inp.close();
      if (inp.fail()) {
         inp.clear(std::ios::failbit);
         rumbleb=false;
      }
      else {
         long features;
         fd = open(rumble_device_file, O_RDWR);
         if (fd == -1) {
            std::cout << "Error 13: Could not open rumble device file" << std::endl;
            rumbleb=false;
         }
         else {
            if (!ioctl(fd, EVIOCGBIT(EV_FF, sizeof(long)), features)) {
               std::cout << "Error 14: Ioctl query failed" << std::endl;
               rumbleb = false;
            }
            else rumbleb=true;
         }
      }
   }
}

bool joystick::check_exist() {
   std::ifstream inp;
   inp.open(joystick_device_file, std::ifstream::in);
   inp.close();
   if(inp.fail()) {
      inp.clear(std::ios::failbit);
      return 0;
   }
   else
      return 1;
   return 0;
}

void joystick::rumble (int length, int strength) {
   if (rumbleb) {
      input_event play;
      ff_effect rumble;
      rumble.type = FF_RUMBLE;
      rumble.id = -1;
      rumble.u.rumble.strong_magnitude = strength;
      rumble.replay.length = length;
      rumble.replay.delay = 0;
      if (ioctl(fd, EVIOCSFF, &rumble) == -1) {
         std::cout << "Error 15: Cannot upload force feedback event FF_RUMBLE" << std::endl;
      }
      play.type = EV_FF;
      play.code = rumble.id;
      play.value = 1;
      write(fd, &play, sizeof(play));
   }
}

joystick::joystick (char* jdf, joy_type type1, char* rdf) {
   if (jdf) {
      joystick_device_file=jdf;
      if (check_exist()) {
         type=type1;
         if (rdf) {
            rumble_device_file=rdf;
            check_rumble();
         }
         else {
            rumbleb = false;
            works = false;
         }
      }
      else {
         rumbleb = false;
         works = false;
      }
   }
   else {
      rumbleb = false;
      works = false;
   }
}
#endif
