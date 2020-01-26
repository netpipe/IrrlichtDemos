#ifndef __SETTINGS_H__
#define __SETTINGS_H__

struct settings {
   bool fps;
   video::E_DRIVER_TYPE mode;
   int width;
   int height;
   bool depth;
   bool fullscreen;
   bool shaders;
   std::string playerName;
   unsigned gameType;
   AudioDevicePtr* Audio_Device;
};

#endif
