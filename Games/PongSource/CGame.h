#pragma once

#include "CIrrObject.h"
#include "CEventReceiver.h"
#include "CPlayer.h"
#include "CPongBall.h"

class CGame : public CIrrObject, public CEventReceiver
{
public:
	CGame();
	~CGame();

	//virtual bool onEvent(SEvent event);
	void render();
private:
	void drawMap();


	CEventReceiver receiver;
	CPlayer *p1;
	CPlayer *p2;
	CPongBall *ball;
};
