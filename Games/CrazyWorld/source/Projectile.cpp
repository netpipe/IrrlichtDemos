#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile(IrrlichtDevice* device, std::string mod, std::string owner, core::vector3df dir, core::vector3df pos, double spd, irr::u32 age, irr::u32 dmg):GameObject(device, mod, pos, dir)
{
	this->max_age = age;
	this->SetSpd(spd);
	lastFrame = device->getTimer()->getRealTime();
	timePassed = 0;
	this->SetType("proj");
	this->SetOwner(owner);
	this->dmg = dmg;
}

Projectile::~Projectile(void)
{
	
}

void Projectile::TickAI()
{
	timePassed += (device->getTimer()->getRealTime()-lastFrame) * device->getTimer()->getSpeed();
	if(this->timePassed < this->max_age)
	{
		if(this->GetDelFlag() == false)
		{
			this->positionVec = this->positionVec + this->directionVec* this->GetSpd() * device->getTimer()->getSpeed() * (device->getTimer()->getRealTime()-lastFrame)/10;
			lastFrame = device->getTimer()->getRealTime();
		}
	}else
	{
		if(this->GetDelFlag() != true)
			this->SetDelFlag(true);
	}
}

void Projectile::CheckCollide(GameObject *obj)
{	
	if(obj->GetType().compare("proj") != 0)
	{
		if(this->pNode->getTransformedBoundingBox().intersectsWithBox(obj->pNode->getTransformedBoundingBox())==1)
		{
			if((obj->GetType()).compare("enem")==0)
			{
				if(this->GetOwner().compare(obj->GetOwner())!=0)
				{
					int amt = obj->GetCurrHP()- dmg;
					if(amt < 0)
					{
						amt = 0;
					}
					((Enemy*)obj)->SetCurrHP(amt);
					this->SetDelFlag(true);
				}
			}else{
				if((obj->GetType()).compare("build")==0)
				{
					if(this->GetOwner().compare(obj->GetOwner())!=0)
					{
						int amt = obj->GetCurrHP()- dmg;
						if(amt < 0)
						{
							amt = 0;
						}
						((Enemy*)obj)->SetCurrHP(amt);
						this->SetDelFlag(true);
					}
				}
			}
		}
	}
}
