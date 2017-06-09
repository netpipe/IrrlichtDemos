#include "stdafx.h"
#include "ship.h"


using namespace irr;
using namespace scene;
using namespace core;


//main ship code
//player object does not inheriet from ship code

CShip::CShip(irr::IrrlichtDevice *graphics,  const core::vector3df& ship_position,ship_class s_class,ship_faction faction, ship_types type, const wchar_t *name)
{
	//initialize variables for ship
	this->graphics = graphics;
	//this->sound = sound;
	this->hullPoints = 1000;
	this->shieldPoints = 2000;
	this->armorPoints = 1000;
	this->maxTurn = 0.2;

	this->maxShieldPoints = 2000;

	this->s_class = s_class;
	this->faction = faction;
	this->type = type;


	//temporary until I implement faction warfare
	//so I guess its actually permanent
	if(faction==FACTION_PROVIAN_CONSORTIUM)
		hostile = true;
	if(faction==FACTION_TERRAN_ALLIANCE)
		hostile = false;

	//definately need to adjust this later
	//don't want ships to constantly move at full speed
	velocity=80;

	//adjust later
	ship=graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/vulture.x"));
	ship->setPosition(ship_position);
	ship->setScale(core::vector3df(0.3,0.3,0.3));

	ship->setMaterialFlag(video::EMF_LIGHTING,true);
	ship->getMaterial(0).NormalizeNormals=true;

	last_time = graphics->getTimer()->getTime();
	cannon.primary = graphics->getTimer()->getTime();
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
	scene::IParticleEmitter *em = exhaust_ps1->createBoxEmitter(core::aabbox3d<f32>(-15,0,-15,15,1,15), // emitter size
                core::vector3df(0.0f,0.0f,0.0f),   // initial direction
                100,200,                             // emit rate
                video::SColor(0,255,255,255),       // darkest color
                video::SColor(0,255,255,255),       // brightest color
                300,600,0,                         // min and max age, angle
                core::dimension2df(10.f,10.f),         // min size
                core::dimension2df(25.f,25.f));        // max size
	exhaust_ps1->setEmitter(em);
	exhaust_ps2->setEmitter(em);
	exhaust_ps3->setEmitter(em);

	em->drop();
	exhaust_ps1->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps1->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps2->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);
	exhaust_ps3->setMaterialTexture(0,graphics->getVideoDriver()->getTexture("res/rail_shot.pcx"));
	exhaust_ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA);

	//Create 2d picture over ship
	//to indicate that it is a target
	array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
	if(hostile==true)
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/target_array_enemy.png"),array_pos);
	else
		target_array = graphics->getGUIEnvironment()->addImage(graphics->getVideoDriver()->getTexture("res/target_array.png"),array_pos);
}

CShip::~CShip()
{
	ship->setVisible(false);
	velocity=0;
	target_array->remove();
}

void CShip::AIRun(f32 frameDeltaTime)
{
	if(hullPoints>0)
	{
		//update 2d picture position
		array_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(getPos(),graphics->getSceneManager()->getActiveCamera());
		target_array->setRelativePosition(vector2d<s32>(array_pos.X-32,array_pos.Y-32));
		movement(frameDeltaTime);

		//Shields regenerate 1 point every 0.1 seconds
		if(shieldPoints<maxShieldPoints)
		{
			if(last_time<graphics->getTimer()->getTime())
			{
				shieldPoints+=1;
				last_time = graphics->getTimer()->getTime()+100;	//milliseconds
			}
		}
	}
}

void CShip::movement(f32 frameDeltaTime)
{
	if(ship->getRotation().Y<turn.Y)	//ship wants to rotate right
	{
		sRot = ship->getRotation();
		rotSlow.Y=maxTurn*(abs(ship->getRotation().Y-turn.Y));	//simulate inertia
		if(rotSlow.Y>15)
			rotSlow.Y=15;
		sRot.Z=0;
		sRot.Y+=rotSlow.Y*frameDeltaTime;
		ship->setRotation(sRot);
	}

	if(ship->getRotation().Y>turn.Y)	//ship wants to rotate right
	{
		sRot = ship->getRotation();
		rotSlow.Y=maxTurn*(abs(ship->getRotation().Y-turn.Y));	//simulate inertia
		if(rotSlow.Y>15)
			rotSlow.Y=15;
		sRot.Z=0;
		sRot.Y-=rotSlow.Y*frameDeltaTime;
		ship->setRotation(sRot);
	}
	if(ship->getRotation().X<turn.X)	//ship wants to rotate up
	{
		sRot = ship->getRotation();
		rotSlow.X=maxTurn*(abs(ship->getRotation().X-turn.X));	//simulate inertia
		if(rotSlow.X>15)
			rotSlow.X=15;
		sRot.Z=0;
		sRot.X+=rotSlow.X*frameDeltaTime;
		ship->setRotation(sRot);
	}
	//only move ship if ship has velocity
	if(velocity!=0)
	{
		sPos=getPos();
		float i = ship->getRotation().Y;
		float z = -(ship->getRotation().X);	//if i dont do this the ship doesnt rotate right


		sPos.Y=frameDeltaTime*velocity*(sin(z * 3.14/180));
		sPos.Y+=getPos().Y;

		sPos.X=frameDeltaTime*velocity*(sin(i * 3.14/180));
		sPos.X+=getPos().X;

		sPos.Z=frameDeltaTime*velocity*(cos(i * 3.14/180));
		sPos.Z+=getPos().Z;


		ship->setPosition(sPos);
	}
}
void CShip::damageShip(int damage)
{
	//Damage shields first, then armor (which has resistance) then hull.
	if(shieldPoints>0)
	{
		shieldPoints-=damage;
	}
	else
	{
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

//rotates the ship to point
void CShip::rotateToPoint(core::vector3df point)
{
	const float x = (point.X - getPos().X);
	const float y = (point.Y - getPos().Y);
	const float z = (point.Z - getPos().Z);
	float angleY = atan2(x,z)*180/3.145296;


	float angleX = atan2(y,z)*180/3.145296;

	turn.Y = angleY;

}
//rotate away from the taret
void CShip::rotateAwayFromPoint(core::vector3df& point)
{
	const float x = (point.X - getPos().X);
	const float y = (point.Y - getPos().Y);
	const float z = (point.Z - getPos().Z);
	float angleY = atan2(x,z)*180/3.145296;


	float angleX = atan2(y,z)*180/3.145296;

	turn.Y = -angleY;

}
//aim turrets at point
void CShip::shootAtPoint(core::vector3df point)
{
	const float x = (point.X - getPos().X);
	const float y = (point.Y - getPos().Y);
	const float z = (point.Z - getPos().Z);
	float angleY = atan2(x,z)*180/3.145296;

	float tmp  = sqrt(x*x+z*z);
	float angleX = atan2(tmp,y)*180/3.14159265;
	angleX-=270;
	fireRot.X = -angleX;
	fireRot.Z = 0;
	fireRot.Y = angleY;
}

//Creates a shot
//then the gameManager picks up the shot to add to the projectile_manager
projectile *CShip::addShot(core::vector3df pos)
{
	////sound->play3D("res/sounds/photon.wav",getPos(),false,false,true);
	projectile *p = new photonCannonShot(graphics, pos, core::vector3df(fireRot.X,fireRot.Y,fireRot.Z),ship);
	return p;
}

//The gameManager uses this to see if the ship wants to shoot
//if the ship wants to shoot, it returns true
//which then the gameManager creates a bullet
void CShip::ableToShoot()
{
	if(cannon.primary<graphics->getTimer()->getTime())
	{
		cannon.primary = graphics->getTimer()->getTime()+2000;
		cannon.primary_shoot=true;
	}
	else
	{
		cannon.primary_shoot=false;
	}
}

//player engagement code
//although, it can be easily used to engage other ships
void CShip::engagePlayer(core::vector3df playerpos)
{
	shootAtPoint(playerpos);
	ableToShoot();
	dodgeFire();
	if(getDistToPoint(playerpos)>5000) //outside effective range....
	{
		//rotateToPoint(playerpos);
	}
	if(getDistToPoint(playerpos)<3000) //too close...
	{
		//rotateAwayFromPoint(playerpos);
	}

}

//returns distance to point, no biggie
float CShip::getDistToPoint(core::vector3df& point)
{
	float x = getPos().X - point.X;
	float y = getPos().Y - point.Y;
	float z = getPos().Z - point.Z;

	float dist = sqrt(x*x+y*y+z*z);
	return dist;
}

//Makes ships slightly less retarded
void CShip::dodgeFire()
{
	if(last_time<graphics->getTimer()->getTime())
	{
		turn.Y += rand()%360 - 180;
		turn.X += rand()%20-10;
		last_time = graphics->getTimer()->getTime() + rand()%1000;
	}
}






int CShip::getHullPoints() const { return this->hullPoints; }
int CShip::getArmorPoints() const { return this->armorPoints; }
int CShip::getShieldPoints() const { return this->shieldPoints; }
bool CShip::getHostileToPlayer() const
{
	return this->hostile;
}
void CShip::drop()
{
	delete this;
}
core::vector3df CShip::getPos() const
{
	return ship->getPosition();
}
core::vector3df CShip::getRot() const
{
	return ship->getRotation();
}

ship_class CShip::getShipClass()
{
	return s_class;
}
ship_faction CShip::getShipFaction()
{
	return faction;
}
float CShip::getVelocity()
{
	return velocity;
}
