#include "Enemy.h"

Enemy::Enemy(void)
{
}
Enemy::Enemy(IrrlichtDevice* device, std::string name, std::string owner, core::vector3df pos, core::vector3df dir, double spd, Waypoint *wp)
{
	moda = "../res/mod/";
	texa = "../res/tex/";
	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->positionVec = pos;
	this->directionVec = dir;
	this->next = new GameObject();
	this->setID(sin(rand()*123.456));
	this->SetDelFlag(false);
	eft = new Effects(device);

	this->SetCurrHP(15);
	this->SetTotalHP(15);
	this->SetType("enem");
	this->SetOwner(owner);
	lastFrame = device->getTimer()->getRealTime();
	timePassed = 0.0f; // Used as internal Cooldown Timer
	rofps = 1500;
	this->SetSpd(spd);
	//this->detectRad = detRad;
	isDead = false;
	this->dest=wp;

	this->moda.append(name);
	pNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh(this->moda.append(".md2").c_str()));
	this->texa.append(name);
	pNode->setMaterialTexture(0,driver->getTexture(this->texa.append(".jpg").c_str()));
	pNode->setMaterialFlag(video::EMF_LIGHTING, false);
	pNode->setScale(core::vector3df(1,1,1));
	pNode->setPosition(positionVec);
	pNode->setMD2Animation("walk");
	//pNode->setDebugDataVisible(scene::EDS_BBOX);

	/*texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood1.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood2.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood2.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood2.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood3.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood2.tga"));
	texture.push_back(device->getVideoDriver()->getTexture("../res/tex/blood1.tga"));*/
}

Enemy::~Enemy(void)
{
}

void Enemy::TickAI()
{
	timePassed -= (device->getTimer()->getRealTime()-lastFrame) * device->getTimer()->getSpeed();
	if(timePassed < 0)
	{
		timePassed = 0;
	}

	//if hp=0
	if(this->GetCurrHP()<=0)
	{
		if(!isDead)
		{
			isDead = true;
			timePassed = rofps;
			pNode->setMD2Animation("die");
			pNode->setMaterialTexture(0,driver->getTexture("../res/tex/botdead.jpg"));
			pNode->setLoopMode(false);
			pNode->addAnimator(smgr->createFlyStraightAnimator(this->positionVec,this->positionVec-this->directionVec*3,rofps*0.5,false));
		}else{
			if(timePassed == 0)
			{
				this->SetDelFlag(true);
			}
		}
	}/*else{
		if((this->GetCurrHP()/this->GetTotalHP())<0.1)
		{
			pNode->setMaterialTexture(0,driver->getTexture("../res/tex/botdying.jpg"));
		}
	}*/

	//movement
	if(!isDead)
	{
		if(this->positionVec.getDistanceFrom(dest->position)>10)
		{
			this->directionVec = (this->dest->position-this->positionVec).normalize();
			this->positionVec += this->directionVec * device->getTimer()->getSpeed() * this->GetSpd() *(device->getTimer()->getRealTime()-lastFrame)/10;
		}else{
			//this->positionVec = this->dest->position;
			this->dest = this->dest->GetRandNxt();
			
		}
	}
	lastFrame = device->getTimer()->getRealTime();
}

void Enemy::CheckCollide(GameObject * obj)
{
	if((obj->GetType().compare("enem") != 0)||(obj->GetType().compare("build") != 0))
	{
		if(this->pNode->getTransformedBoundingBox().intersectsWithBox(obj->pNode->getTransformedBoundingBox())==1)
		{
			if(!isDead)
			{
				if((obj->GetType()).compare("proj")==0)
				{
					if(this->GetOwner().compare(obj->GetOwner())!=0)
					{
						int amt = this->GetCurrHP() - ((Projectile*)obj)->dmg;
						if(amt < 0)
						{
							amt = 0;
						}
						this->SetCurrHP(amt);
						if(amt==0)
						{
							this->SpillBlood();
						}else{
							this->Pain();
						}
						obj->SetDelFlag(true);
					}
				}
			}
		}
	}
}

void Enemy::SpillBlood()
{
	/*ibb = device->getSceneManager()->addBillboardSceneNode(0,core::dimension2df(10,10),positionVec+directionVec*400,-1);
	ibb->addAnimator(device->getSceneManager()->createTextureAnimator(texture,40,false));
	ibb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	ibb->setMaterialFlag(video::EMF_LIGHTING,false);
	ibb->addAnimator(device->getSceneManager()->createDeleteAnimator(480));
	ibb->setDebugDataVisible(scene::EDS_MESH_WIRE_OVERLAY);*/
	/*ibb = device->getSceneManager()->addBillboardSceneNode(0,core::dimension2df(10,10),positionVec+directionVec*400,-1);
	ibb->setMaterialTexture(0,driver->getTexture("../res/tex/blood3.tga"));
	ibb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	ibb->setMaterialFlag(video::EMF_LIGHTING,false);
	ibb->addAnimator(smgr->createFlyStraightAnimator(this->positionVec+core::vector3df(0,4,0),this->positionVec+core::vector3df(0,6,0),200,false));
	ibb->addAnimator(device->getSceneManager()->createDeleteAnimator(200));*/
	scene::IParticleSystemSceneNode *ps = eft->particleSpray(device->getVideoDriver()->getTexture("../res/tex/bloodsplat.tga"), positionVec+core::vector3df(0,1,0),(directionVec+core::vector3df(0,1,0)).normalize()*.05,1000,250,800,2);
	ps->addAffector(ps->createGravityAffector());
}

void Enemy::Pain()
{
	ibb = device->getSceneManager()->addBillboardSceneNode(0,core::dimension2df(10,10),positionVec+directionVec*400,-1);
	ibb->setMaterialTexture(0,driver->getTexture("../res/tex/ah.tga"));
	ibb->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	ibb->setMaterialFlag(video::EMF_LIGHTING,false);
	ibb->addAnimator(smgr->createFlyStraightAnimator(this->positionVec+core::vector3df(0,5,0),this->positionVec+core::vector3df(0,12,0),600,false));
	ibb->addAnimator(device->getSceneManager()->createDeleteAnimator(700));
}