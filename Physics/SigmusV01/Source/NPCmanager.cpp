#include "NPCmanager.h"

CNPCManager::CNPCManager(CPlayer* Player, CPhysics* Physics, ISceneManager *smgr, IVideoDriver *driver, IGUIEnvironment *gui, ITimer* Timer)
{
	CNPCManager::smgr = smgr;
	CNPCManager::driver = driver;
	CNPCManager::gui = gui;
	CNPCManager::Timer = Timer;
	CNPCManager::Physics = Physics;
	CNPCManager::Player = Player;
};

CTurret* CNPCManager::AddTurret(const stringw Name, float TurnRate, IAnimatedMeshSceneNode *Node, vector3df Muzzle1, vector3df Muzzle2)
{
	CTurret* Turret = new CTurret(Name,TurnRate,Node,CNPCManager::smgr,CNPCManager::driver,CNPCManager::gui,CNPCManager::Timer,CNPCManager::Player, CNPCManager::Physics, Muzzle1, Muzzle2);
	CNPCManager::Turrets.push_back(Turret);
	return Turret;
};

void CNPCManager::Update()
{
	for(list<CTurret *>::Iterator Iterator = CNPCManager::Turrets.begin(); Iterator != CNPCManager::Turrets.end(); ++Iterator)
	{
		CNPCManager::_UpdateAll(*Iterator);
		//if(CNPCManager::Player->Projectiles.getSize()>0)
			//for(list<CPlayer::SProjectiles *>::Iterator IteratorPB = CNPCManager::Player->Projectiles.begin(); IteratorPB != CNPCManager::Player->Projectiles.end(); ++IteratorPB)
				//CNPCManager::_UpdateAllCollisions(*Iterator, *IteratorPB);
	}
};

void CNPCManager::_UpdateAllCollisions(CTurret* Turret, CPlayer::SProjectiles* Projectile)
{
	//Function not needed, needs to be removed
};

void CNPCManager::_UpdateAll(CTurret* Turret)
{
	Turret->UpdateTurret();
	Turret->UpdateVisuals();
};

void CNPCManager::remove()
{
	for(list<CTurret *>::Iterator Iterator = CNPCManager::Turrets.begin(); Iterator != CNPCManager::Turrets.end();)
	{
	    (*Iterator)->remove();
	    Iterator = CNPCManager::Turrets.erase(Iterator);
	}
    if(DEBUG_CONSOLE)
        cout<<"Cleaned NPC Manager"<<endl;
}
