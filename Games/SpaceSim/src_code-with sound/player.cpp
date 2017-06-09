#include "stdafx.h"
#include "player.h"





using namespace irr;


//Player object code
//Although it uses alot of the same stuff as the ship object, theres too many differences between the player
//and the ship to base it off the ship class

Player::Player(irr::IrrlichtDevice *graphics,irrklang::ISoundEngine *sound, KeyListener *receiver, const core::vector3df& ship_position)
{
	//store variables
	this->graphics = graphics;
	this->sound = sound;

	last_time = 0;

	ship=graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/cruiser.x"));
	ship->setPosition(ship_position);	//sets the position
	if(ship)
	{
		//setup player values
		ship->setScale(core::vector3df(0.3,0.3,0.3));
		ship->setRotation(core::vector3df(0,0,0));
		ship->setMaterialFlag(video::EMF_LIGHTING,true);
		ship->getMaterial(0).Lighting =true;
		ship->getMaterial(0).NormalizeNormals=true;
		ship->getMaterial(0).Shininess=128;
	}
	//setup player variables
	//TODO: inherit variables from ship classes
	this->hullPoints = 1000;
	this->armorPoints = 1000;
	this->shieldPoints = 2000;
	this->maxVelocity = 80;
	this->maxTurn = 0.2;
	this->velocity = 0;

	this->maxShieldPoints = shieldPoints;

	turn.X = ship->getRotation().X;
	turn.Y = ship->getRotation().Y;
	turn.Z = ship->getRotation().Z;

	exhaust_01 = ship->getJointNode("exhaust1");
	exhaust_02 = ship->getJointNode("exhaust2");
	exhaust_03 = ship->getJointNode("exhaust3");


	//setup particle effects
	scene::IParticleSystemSceneNode *exhaust_ps1=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_01);
	scene::IParticleSystemSceneNode *exhaust_ps2=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_02);
	scene::IParticleSystemSceneNode *exhaust_ps3=graphics->getSceneManager()->addParticleSystemSceneNode(false,exhaust_03);
	scene::IParticleEmitter *em = exhaust_ps1->createBoxEmitter(core::aabbox3d<f32>(-15,0,-15,15,1,15), core::vector3df(0.0f,0.0f,0.0f),50,100,video::SColor(0,0,0,0),
                video::SColor(0,255,255,255),       // brightest color
                300,600,0,                         // min and max age, angle
                core::dimension2df(10.f,10.f),         // min size
                core::dimension2df(25.f,25.f));        // max size
	exhaust_ps1->setEmitter(em);
	exhaust_ps2->setEmitter(em);
	exhaust_ps3->setEmitter(em);

	em->drop();

	//attach particle effects
	exhaust_ps1->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps1->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps2->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	//setup turret positioning
	//reminder = use an array next time
	turret *turret1 = new turret(graphics,getPos(),core::vector3df(0,0,0),core::vector3df(0,25,0),1,TYPE_TURRET_RAIL);
	turret *turret2 = new turret(graphics,getPos(),core::vector3df(0,0,0),core::vector3df(0,25,0),2,TYPE_TURRET_RAIL);
	turret *turret3 = new turret(graphics,getPos(),core::vector3df(0,0,0),core::vector3df(0,25,0),3,TYPE_TURRET_RAIL);
	turret *turret4 = new turret(graphics,getPos(),core::vector3df(0,0,0),core::vector3df(0,25,0),4,TYPE_TURRET_RAIL);

	turret_manager.push_back(turret1);
	turret_manager.push_back(turret2);
	turret_manager.push_back(turret3);
	turret_manager.push_back(turret4);
	turret_node[1] = ship->getJointNode("turret_01");
	turret_node[2] = ship->getJointNode("turret_02");
	turret_node[3] = ship->getJointNode("turret_03");
	turret_node[4] = ship->getJointNode("turret_04");



	cannonFired.primary_time = graphics->getTimer()->getTime();
	cannonFired.primary=false;
	engineSound = sound->play3D("res/sounds/engine.ogg",getPos(),true,false,true);


}

Player::~Player()
{
}


void Player::manageTurrets(CShip* player_target, f32 frameDeltaTime)
{


	//update turrets position
	for(int i = 0; i < turret_manager.size();i++)
	{
		turret_manager[i]->setPos(turret_node[turret_manager[i]->getTurretNum()]->getAbsolutePosition());
	}
	//calculate angle to target using arctan
	if(player_target!=0)
	{
		if(player_target->getHullPoints()>0)
		{
			//go through all the turrets and aim them

			for(int i = 0;i< turret_manager.size();i++)
			{
				//This code uses the atan2 func to get the angle between the turret and the target ship
				//to aim properly
				const float toTargetX(player_target->getPos().X - turret_manager[i]->getPos().X);
				const float toTargetZ(player_target->getPos().Z - turret_manager[i]->getPos().Z);
				const float toTargetY(player_target->getPos().Y - turret_manager[i]->getPos().Y);
				float angleY = atan2(toTargetX,toTargetZ)*180/3.14159265;


				float tmp = sqrt(toTargetX*toTargetX + toTargetZ*toTargetZ);
				float angleX = atan2(tmp,toTargetY)*180/3.14159265;
				angleX-=270;

				if(angleX<0)
					angleX+=360;
				if(angleX>360)
					angleX-=360;

				angleX = angleX+180;
				turret_manager[i]->aimTurret(core::vector3df(angleX,angleY,0), frameDeltaTime);
			}
		}
	}
	else
	{
		for(int i = 0;i< turret_manager.size();i++)
		{
			turret_manager[i]->aimTurret(getRot(),frameDeltaTime);
		}
	}
}

//main player loop, called from gameloop
void Player::playerRun(f32 frameDeltaTime)
{
	if(getHull()>0)
	{
		engineSound->setPosition(getPos());
		//Shields regenerate 1 point every 0.1 seconds
		if(shieldPoints<maxShieldPoints)
		{
			if(last_time<graphics->getTimer()->getTime())
			{
				shieldPoints+=1;
				last_time = graphics->getTimer()->getTime()+100;	//milliseconds
			}
		}
		//player rotation code
		//slowly rotate player toward a point
		if(ship->getRotation().Y<turn.Y)	//ship wants to rotate right
		{
			pRot = ship->getRotation();
			rotSlow.Y=getMaxTurn()*(abs(getRot().Y-turn.Y));	//simulate inertia
			if(rotSlow.Y>15)
				rotSlow.Y=15;
			pRot.Z=0;
			pRot.Y+=rotSlow.Y*frameDeltaTime;
			setRot(pRot);
		}
		if(ship->getRotation().Y>turn.Y)	//ship wants to rotate left
		{
			pRot = ship->getRotation();
			rotSlow.Y=getMaxTurn()*(abs(getRot().Y-turn.Y));	//simulate inertia
			if(rotSlow.Y>15)
				rotSlow.Y=15;
			pRot.Y-=rotSlow.Y*frameDeltaTime;
			pRot.Z=0;
			setRot(pRot);
		}
		if(ship->getRotation().X>turn.X)	//turn up
		{
			pRot=ship->getRotation();
			rotSlow.X=getMaxTurn()*(abs(ship->getRotation().X-turn.X));
			if(rotSlow.X>15)
				rotSlow.X=15;
			pRot.X-=rotSlow.X*frameDeltaTime;
			pRot.Z=0;
			ship->setRotation(pRot);
		}
		if(ship->getRotation().X<turn.X)	//turn down
		{
			pRot=ship->getRotation();
			rotSlow.X=getMaxTurn()*(abs(ship->getRotation().X-turn.X));
			if(rotSlow.X>15)
				rotSlow.X=15;
			pRot.X+=rotSlow.X*frameDeltaTime;
			pRot.Z=0;
			ship->setRotation(pRot);
		}



		//if player has velocity move player in direction
		if(getVelocity()!=0)
		{
			pPos=getPos();
			float i = getRot().Y;
			float z = -(getRot().X);	//if i dont do this the ship doesnt rotate right

			//use trig to determine where the player should move to
			pPos.Y=frameDeltaTime*getVelocity()*(sin(z * 3.14/180));
			pPos.Y+=getPos().Y;

			pPos.X=frameDeltaTime*(getVelocity())*(sin(i * 3.14/180));
			pPos.X+=getPos().X;

			pPos.Z=frameDeltaTime*getVelocity()*(cos(i * 3.14/180));
			pPos.Z+=getPos().Z;


			setPos(pPos);
		}
	}

}



void Player::damagePlayer(int damage)
{
	//if shield exists, damage shield first
	if(shieldPoints>0)
	{
		shieldPoints-=damage;
	}
	else
	{
		//if armor exists, damage armor with resistance
		if(armorPoints>0)
		{
			armorPoints-=damage/1.25;
		}
		else
		{
			if(hullPoints>0)
			{
				hullPoints-=damage;
			}
		}
	}
}


//The shooting works the same way as the other ships
//The gamemanager constantly checks the variable if its true
//then creates the projectile in the gamemanager obj
void Player::shoot()
{
	if(cannonFired.primary_time<graphics->getTimer()->getTime())
	{
		cannonFired.primary_time=graphics->getTimer()->getTime()+turret_manager[0]->reload_time;
		cannonFired.primary=true;
	}
	else
	{
		cannonFired.primary=false;
	}
}

//should probably use a template for this
//painful to read
//but its alot of the same stuff
core::vector3df Player::getRot() const{return ship->getRotation();}
core::vector3df Player::getPos() const{return ship->getPosition();}
void Player::setPos(const core::vector3df& pPos){ship->setPosition(pPos);}
void Player::setRot(const core::vector3df& pRot){ship->setRotation(pRot);}
int Player::getMaxSpeed(){return this->maxVelocity;}
float Player::getMaxTurn() {return this->maxTurn;}
void Player::setVelocity(float& velocity) {this->velocity = velocity;}
float Player::getVelocity() const {return this->velocity;}
int Player::getHull() const { return this->hullPoints; }
int Player::getArmor() const { return this->armorPoints;}
int Player::getShield() const { return this->shieldPoints;}
