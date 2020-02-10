
#ifndef __PLATFORMS_H
#define __PLATFORMS_H

#include <irrlicht.h>
using namespace irr;

void resetPlatforms();
void updatePlatformDifficulty(double dt);
void createPlatformUpdate(int time);
void freezePlatforms();
void updatePlatforms(double dt);
int getPlatformCollision(core::vector3df pos, core::aabbox3df *box);
f32 getClosestPlatformDistance2D(core::vector3df pos);
core::vector3df getLowestPlatformPoint();
void removePlatforms();

#endif
