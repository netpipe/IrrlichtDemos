#pragma once
#include "GOList.h"
#include "GameObject.h"
#include "Tile.h"
#include "WPManager.h"

class AI
{
public:
	AI(IrrlichtDevice* device);
	~AI(void);

	void delObj(GameObject*);
	void addObj(GameObject*);
	void ClearDelQueue();
	void AddToDelQueue(GameObject*);
	void TickAIs();
	void UpdateSelfs();
	void CheckCollisions();
	void CheckDelFlag();
	void AIThink();
	void TickManager();
	int GetCurThreshold(){return cur_threshold;}

	IrrlichtDevice* device;
	irr::u32 lastFrame;
	irr::f32 timePassed;

	int level;
	int score;
	int no_of_buildings;
	int kill_count;
	int nxtLvl;

private:
	GOList* list;
	GOList* temp;
	irr::core::array<GameObject*> buildCreateQueue;
	int col_s;
	int cur_threshold;
	core::array<Tile*> placement;
	WPManager* waypoints;
};
