
#ifndef __DEVICESETTINGS_H
#define __DEVICESETTINGS_H

#include <string>

struct DeviceSettings
{
    int driver;     // 0 == direct3d8, 1 == opengl, 2 == software...
    int width,height,bits;
    int fullscreen;
};

bool runApp(char *appName);
void deviceSettingsOpenWebpage(char *page);
void deviceSettingsErrorMsg(char *msg);
DeviceSettings getSettings();
std::string getSettingsFilePath();

#endif
