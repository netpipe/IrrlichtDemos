#pragma once

#include <iostream>
//#include <irrKlang.h>
#include "CSprite.h"
#include "CPlayer.h"
#include "CTimer.h"

//using namespace irrklang;

using namespace std;


class CPongBall : public CSprite
{
public:
	CPongBall(s32 x, s32 y, u32 width, u32 height, CPlayer *p1, CPlayer *p2);
	~CPongBall();

	void update();
	void draw();
	void checkCollision(const CPlayer *player1, const CPlayer *player2);

private:
	bool c;
	f32 x;
	f32 y;
	double speed;
	double angle;
	CPlayer *p1;
	CPlayer *p2;
//	ISoundEngine *soundEngine;
	CTimer *timer;
};
