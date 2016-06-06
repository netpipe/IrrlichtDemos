#pragma once
#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
#pragma comment(lib,"Irrlicht.lib")

class GameObject
{
public:
	GameObject();
	GameObject(IrrlichtDevice*, std::string, core::vector3df, core::vector3df);
	~GameObject(void);

	void setID(double);
	double getID();
	void SetType(std::string);
	std::string GetType();
	void change_state(int state);
	int get_state(void);
	int GetCurrHP();
	void SetCurrHP(int);
	int GetTotalHP();
	void SetTotalHP(int);
	double GetSpd();
	void SetSpd(double);
	std::string GetOwner();
	void SetOwner(std::string);
	void SetDelFlag(bool);
	bool GetDelFlag();

	virtual void TickAI(void);
	virtual void CheckCollide(GameObject *);
	virtual void UpdateSelf(void);
	
	IrrlichtDevice* device;
	scene::ISceneManager* smgr;
	video::IVideoDriver* driver;
	scene::IAnimatedMeshSceneNode* pNode;

	vector3df positionVec;
	vector3df lastPos;
	vector3df directionVec;
	GameObject* next;
	double lastFrame;
	double timePassed;
	std::string moda;
	std::string texa;
private:
	int c_hp;
	int t_hp;
	double move_spd;
	std::string owner;
	double uid;
	std::string type;
	bool del_flag;

	//statemachine
	int states; //
};
