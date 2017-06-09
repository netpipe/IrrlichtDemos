#pragma once

#include <iostream>
using namespace std;

#include "CEventReceiver.h"
#include "CIrrObject.h"
#include "CSprite.h"
#include "CTimer.h"
using namespace video;

struct Controls
{
	EKEY_CODE down;
	EKEY_CODE up;
};


class CPlayer : public CSprite
{
public:
	CPlayer(s32 x, s32 y, u32 width, u32 height, Controls &controls, CEventReceiver *receiver);
	~CPlayer();

	s32 getScore() const;
	s32 getPlayerId() const;

	void draw();
	void update();

	f32 getSpeed() const;

	void setScore(int score);
	void setPlayerId(s32 playerId);

private:
	CEventReceiver *receiver;
	Controls controls;
	f32 speed;
	s32 score;
	s32 playerId;
	CTimer *timer;
	f32 y;
};
