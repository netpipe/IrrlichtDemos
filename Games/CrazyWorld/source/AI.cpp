#include "AI.h"
#include "Building.h"
#include "Enemy.h"

#define PER_BUILD_DIST_DIFF 60
#define MOD_HT_OFFSET 50
#define BUILD_COOLDOWN 10000
#define MAX_THRESHOLD 100

AI::AI(IrrlichtDevice* device)
{
	this->device = device;
	list = new GOList();
	this->cur_threshold = 0;
	this->no_of_buildings = 0;
	this->level = 1;
	kill_count = 0;
	//this->nxtLvl = 5; //according to kill count
	this->nxtLvl = 20; //according to score;
	score = 0;
	this->waypoints = new WPManager();
	col_s = 0;
	int row = 3;
	for(int i = 0; i <row; i++)
	{
		for(int j = 0; j <row; j++)
		{
			placement.push_back(new Tile(device,core::vector3df(i*PER_BUILD_DIST_DIFF,MOD_HT_OFFSET,j*PER_BUILD_DIST_DIFF),this->waypoints,PER_BUILD_DIST_DIFF/2,MOD_HT_OFFSET));
		}
	}
	
	scene::IAnimatedMeshSceneNode* pNode = device->getSceneManager()->addAnimatedMeshSceneNode(device->getSceneManager()->getMesh("../res/mod/plane.obj"));
	pNode->setMaterialTexture(0,device->getVideoDriver()->getTexture("../res/tex/terrain.jpg"));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(50,50,50));
	pNode->setPosition(placement[5]->position-core::vector3df(0,1,0));
}

AI::~AI(void)
{
}

void AI::delObj(GameObject* n)
{
	this->list->RemoveNode(n);
}

void AI::addObj(GameObject* n)
{
	this->list->InsertNode(n);
	//std::cout << "Size: " << list->GetSize() << " \nFPS: " << device->getVideoDriver()->getFPS() << std::endl;
}

void AI::TickAIs()
{
	for(int i = 1; i <= list->GetSize(); i++)
	{	
		list->GetNode(i)->TickAI();	
		if(list->GetNode(i)->GetType().compare("build")==0)
		{
			if(((Building*)list->GetNode(i))->createFlag == true)
			{
				buildCreateQueue.push_back(list->GetNode(i));
			}
		}
	}
	for(int i = 0; i < buildCreateQueue.size(); i++)
	{
		//std::cout << "Bot Created: " <<std::endl;
		if(this->cur_threshold < MAX_THRESHOLD)
		{
			this->addObj(new Enemy(this->device, "bot", "a", ((Building*)buildCreateQueue[i])->positionVec, ((Building*)buildCreateQueue[i])->directionVec, 0.2,((Building*)buildCreateQueue[i])->parent->waypoint[0]));
			this->cur_threshold++;
			((Building*)buildCreateQueue[i])->createFlag = false;
			((Building*)buildCreateQueue[i])->timePassed += ((Building*)buildCreateQueue[i])->rocps;
		}
	}
	buildCreateQueue.clear();
}

void AI::UpdateSelfs()
{
	for(int i = 1; i <=list->GetSize(); i++){
		(list->GetNode(i))->UpdateSelf();
	}
}

void AI::CheckDelFlag()
{
	temp = new GOList();
	for(int i = 1; i <=list->GetSize(); i++){
		if((list->GetNode(i))->GetDelFlag()==true)
		{
			if(list->GetNode(i)->GetType().compare("enem") == 0)
			{
				this->cur_threshold--;
				this->kill_count++;
				this->score += list->GetNode(i)->GetTotalHP();
			}
			if(list->GetNode(i)->GetType().compare("build") == 0)
			{
				this->no_of_buildings--;
				((Building*)list->GetNode(i))->parent->occupied = false;
				this->score += list->GetNode(i)->GetTotalHP();
			}
			AddToDelQueue(list->GetNode(i));
		}
	}
}

// Transfer GameObject to deletion queue
void AI::AddToDelQueue(GameObject * n)
{
	temp->InsertNode(n);
	this->delObj(n);
}

void AI::ClearDelQueue()
{
	if(temp->GetSize()>0)
	{
		for(int i = temp->GetSize(); i > 0; i--){
			delete (temp->GetNode(i));
		}
	}
}

void AI::CheckCollisions()
{
	if(col_s >0)
	{
		for(int i = 1; i <=list->GetSize()-1; i++){
			for(int j = i+1; j <= list->GetSize();j++)
			{
				(list->GetNode(i))->CheckCollide((list->GetNode(j)));
			}
		}
	}else{
		col_s++;
	}
}

void AI::AIThink()
{
	//Update Level
	//check if score reach nxtLvl
	if(this->score >= this->nxtLvl)
	{
		//Level++, nxtlvl*2
		level++;
		this->nxtLvl += this->nxtLvl*1.8;
	}

	//BuildingManagement
	//check if no_of_build < build limit
	if(this->no_of_buildings < this->level)
	{
		//check build cooldown == 0
		if(timePassed == 0)
		{
			//check if theres any spare space
			for(int i=0;i<placement.size();i++)
			{
				if(placement[i]->occupied == false)
				{
					//get spare space n create building, increase no_of_build,cooldown
					this->addObj(new Building(device,"hse","a",placement[i],1));
					no_of_buildings++;
					timePassed += BUILD_COOLDOWN;
					break;
				}
			}
		}
	}
}

void AI::TickManager()
{	
	timePassed -= (device->getTimer()->getRealTime()-lastFrame)  /** device->getTimer()->getSpeed()*/;
	if(timePassed < 0)
	{
		timePassed = 0;
	}
	TickAIs();
	AIThink();
	CheckCollisions();
	CheckDelFlag();
	ClearDelQueue();
	UpdateSelfs();
	
	lastFrame = device->getTimer()->getRealTime();
}