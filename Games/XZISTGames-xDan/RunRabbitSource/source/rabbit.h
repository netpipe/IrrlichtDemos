
#ifndef __RABBIT_H
#define __RABBIT_H

#include <irrlicht.h>
using namespace irr;

void createRabbit();
void updateRabbit(double dt);
void jumpRabbit();
void removeRabbit();
bool rabbitHitNode(scene::ISceneNode *otherNode);
bool rabbitDied();

#endif
