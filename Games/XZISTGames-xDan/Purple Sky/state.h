
#ifndef __STATE_H
#define __STATE_H

#include <irrlicht.h>

using namespace irr;

enum {IN_STARTSCREEN = 0, IN_GAME = 1, GAME_PAUSED = 2, GAME_OVER = 3};
extern int state;
extern int score_nutCount;

void pauseGame();
void continueGame();
void startGame();
void endGame();
void startScreen();
void gameOver();
void setScoreDisplayCount(int c);

#endif
