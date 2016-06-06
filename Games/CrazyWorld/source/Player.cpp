#include "Player.h"
#include "Projectile.h"
#include <math.h>

Player::Player(void)
{

}

Player::Player(IrrlichtDevice* device, AI* ai, std::string owner, core::vector3df pos, core::vector3df dir, int dmg)
{
	lastFrame = device->getTimer()->getRealTime();
	timePassed = 0.0f; // Used as internal Cooldown Timer
	rofps = 300;
	score = 0;
	this->dmg = dmg;
	this->dmg_multiplier = 1.5;
	this->ai = ai;
	isFiring=false;
	o=owner;

	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->positionVec = pos;
	this->lastPositionVec = pos;
	this->directionVec = dir;
}

Player::~Player(void)
{

}

void Player::Attack()
{
	if(timePassed == 0){
		ai->addObj(new Projectile(device,"bullet",o,this->directionVec,this->positionVec+this->directionVec*10,10.0,2000,dmg*dmg_multiplier));
		timePassed += rofps;
	}
}

void Player::TickAI()
{
	timePassed -= (device->getTimer()->getRealTime()-lastFrame) * device->getTimer()->getSpeed();
	if(timePassed < 0)
	{
		timePassed = 0;
	}
	
	/*if(irr::core::vector3df(0,0,0).getDistanceFrom(camera->getPosition())>150)
	{
		camera->setPosition(this->lastPositionVec);
	}*/

	if(isFiring)
	{
		Attack();
	}
	lastFrame = device->getTimer()->getRealTime();
}

void Player::Update()
{
	if(irr::core::vector3df(0,0,0).getDistanceFrom(this->positionVec)<=600)
	{
		this->lastPositionVec = this->positionVec;
	}
	this->positionVec = camera->getPosition();
	this->directionVec = (camera->getTarget()-camera->getPosition());
}