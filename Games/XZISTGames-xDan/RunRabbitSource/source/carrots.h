
#ifndef __CARROTS_H
#define __CARROTS_H

#include <irrlicht.h>
using namespace irr;

void resetCarrotsCollected();
void addCarrotDisplay();
void removeCarrotDisplay();

void addCarrots(scene::ISceneNode *platform);
void updateCarrots();
void rotateCarrotIcon();
void removeCarrots();

#endif
