#ifndef _GAME_H_
#define _GAME_H_

#include <irrlicht.h>
#include <vector>
#include <deque>

//#include <fmod.hpp>

#include "GameObject.h"
#include "EventReciever.h"
#include "GameState.h"

// Irrlicht
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using std::vector;
using std::deque;

class Game
{
private:
	IrrlichtDevice *device;
	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
	EventReciever *reciever;

	vector<GameObject*> *object;
	deque<IAnimatedMeshSceneNode*> *checkpoint;
	IAnimatedMeshSceneNode *first_checkpoint;
	vector<IAnimatedMeshSceneNode*> *arrow;

	IGUIStaticText *info;
	ITextSceneNode *countdown;

	u32 nr_of_laps;
	u32 max_laps;

	u32 last_time;

	void initSound();
	void addCheckPoint(const vector3df& p, const vector3df& r);
	void addArrow(const vector3df& p, const vector3df& r);
	GAME_STATE current_state;

public:
	Game();
	~Game();

	void init();
	void update();
	void run();
};

#endif
