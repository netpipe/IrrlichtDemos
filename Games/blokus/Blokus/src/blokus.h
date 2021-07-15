#ifndef _BLOKUS_H_
#define _BLOKUS_H_

#include "common.h"
#include "draw.h"

extern s32 BlockNo;
extern s32 TURN;

void rotate(s32 mode, s32 num);
bool ptCheck(s32 mousex, s32 mousey, s32 num, s32 color);

void game_init();
void game_start();
void game_board();
void game_peace(s32 mousex, s32 mousey, s32 num, s32 turn);
void game_block(s32 color);
void game_giveup(s32 turn);
s32  game_click(s32 mousex, s32 mousey, s32 num, s32 col);
void game_state(s32 alpha, s32 state);

#endif // _BLOKUS_H_

