
#ifndef __PLAYER_H
#define __PLAYER_H

#include <irrlicht.h>

using namespace irr;

extern const f32 maxPlayerJump;

enum {CAMERA_PIVOT = 0, CAMERA_FOLLOW = 1};
extern int cameraType;

core::vector3df toVector3d(core::vector2df pos);
core::vector2df toVector2d(core::vector3df pos);

void turnCamera(int dir);
bool playerOnPlatform();
core::vector3df getPlayerPos();
void createPlayer();
void removePlayer();
void updatePlayer(double dt);
void playerPlatformJump();

#endif
