#include "stdafx.h"
#include "turret.h"

using namespace irr;
using namespace scene;


//turret object code
//controlled mostly by outside objects

turret::turret(irr::IrrlichtDevice *graphics,core::vector3df position, core::vector3df rotation, core::vector3df offset, int turret_number, turret_class type)
{
	this->offset = offset;
	this->turret_number = turret_number;
	this->type=type;
	t_model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/railgunturret.x"));
	if(type == TYPE_TURRET_RAIL)
	{
		rotate_speed = 20;
		reload_time = 2000;		//in milliseconds
		max_range = 6000;
	}
}

turret::~turret()
{
}

void turret::aimTurret(core::vector3df newrot, f32 frameDeltaTime)
{
	//This func slowly rotates the turret to face the desired rotation
	//Makes it more turretlike


	//t_model->setRotation(newrot);
	core::vector3df t_rot;
	t_rot = t_model->getRotation();

	t_rot.X = newrot.X;
	if(t_model->getRotation().Y<newrot.Y)
	{
		t_rot.Y+=rotate_speed*frameDeltaTime;
	}
	if(t_model->getRotation().Y>newrot.Y)
	{
		t_rot.Y-=rotate_speed*frameDeltaTime;
	}
	//t_rot.Z = 0;
	t_model->setRotation(t_rot);
}
void turret::setPos(core::vector3df newpos)
{
	t_model->setPosition(newpos);
}
core::vector3df turret::getRot() const
{
	return t_model->getRotation();
}
core::vector3df turret::getPos() const
{
	return t_model->getPosition();
}
core::vector3df turret::getOffset()
{
	return offset;
}
int turret::getTurretNum() const
{
	return turret_number;
}
