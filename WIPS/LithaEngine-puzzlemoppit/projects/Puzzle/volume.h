
#ifndef VOLUME_H
#define VOLUME_H

#include "Litha.h"

void set_wind_volume(s32 amount);
void set_sfx_volume(s32 amount);
void set_music_volume(s32 amount);

s32 get_wind_volume();
s32 get_sfx_volume();
s32 get_music_volume();

void set_volumes_from_settings(VariantMap settings);

#endif
