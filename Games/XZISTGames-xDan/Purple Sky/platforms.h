
#ifndef __PLATFORMS_H
#define __PLATFORMS_H

#include <irrlicht.h>

using namespace irr;

extern const f32 minPlatformSeparation;
extern const f32 platformStep;

void createPlatforms();
void removePlatforms();
void updatePlatforms(double dt);
bool getClosestPlatformExcluding(core::vector2df pos, core::vector2df ex, core::vector2df &dest);
void setTouchedPlatform(core::vector2df platPos);
bool onPlatform(core::vector2df platPos);
void setSelected(core::vector2df platPos);

#endif
