#ifndef _EVENTRECIEVER_H_
#define _EVENTRECIEVER_H_

#include <irrlicht.h>
#include <vector>
#include <utility>
//#include <fmod.hpp>
#include "GameState.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;
using namespace scene;
using namespace io;

using std::vector;
using std::pair;

class EventReciever : public IEventReceiver
{
private:
	ISceneNode *node;
	vector3df wheels;
	vector3df velocity;
	float speed;
	GAME_STATE current_state;
	GAME_STATE next_state;

	vector< pair<EKEY_CODE, bool> *> *key;

	// Sound
//	FMOD::System *system;
//	FMOD::Channel *channel;
//    FMOD::DSP *dsp;

public:
	EventReciever();
	virtual ~EventReciever() {}

	virtual bool OnEvent(const SEvent &event);
	void setSteer(ISceneNode *n);
	void dispatch();
	void setCurrentState(GAME_STATE state);
	GAME_STATE getNextState();
};

#endif
