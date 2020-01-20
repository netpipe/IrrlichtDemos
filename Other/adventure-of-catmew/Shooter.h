#include "Game.h"
#include "Level.h"

typedef struct
{
gfx::ISprite* mewSprite;
bool mewDirection;
} mewmewShot;


class MewMewShooter
{
public:
	MewMewShooter(Game* mycore, gfx::ISprite* catguy, LevelManager* levelMan) 
	: levelManage(levelMan), core(mycore), catDude(catguy), switchy(false)
	{
	shoottex = core->getRoot()->getVideoDriver()->createTexture("media/misc/mewmewshot.png");
	};
	
	void update()
	{
		if(core->getRoot()->getKeyPressed(gfx::EKC_SPACE))
		{
			if(!switchy)
	 			shootMew();
			switchy = 1;
		}
		else
			switchy = 0;
	
		shootSpeed = 0.6f * core->getDeltaTime();
		for(int i = 0; i < shootArray.size(); ++i)
		{
			if(shootArray[i].mewDirection == RIGHT)
			shootArray[i].mewSprite->setPosition(shootArray[i].mewSprite->getPosition() + Vector(shootSpeed,0));
			else
			shootArray[i].mewSprite->setPosition(shootArray[i].mewSprite->getPosition() + Vector(-shootSpeed,0));

			if(shootArray[i].mewSprite->getPosition().x < core->wPos.x - 64 || shootArray[i].mewSprite->getPosition().x > core->wPos.x + core->getRoot()->getDimension().x + 64)
			{
				shootArray[i].mewSprite->remove();		
				shootArray.erase(shootArray.begin() + i,shootArray.begin() + i+1);
				--i;
				continue;
			}
			
			for(int z = 0;z < levelManage->getTileNumber();++z)
			{
				if(levelManage->getTile(z).isEnemy && levelManage->getTile(z).enemy->dead)
					continue;
				
				if(levelManage->getTile(z).sprite->getAbsoluteBoundingBox().isPointInside(shootArray[i].mewSprite->getPosition()))
				{
					if(levelManage->getTile(z).isEnemy)
						levelManage->getTile(z).enemy->HitPoints--;	
				
					shootArray[i].mewSprite->remove();		
					shootArray.erase(shootArray.begin() + i,shootArray.begin() + i+1);
					--i;
					break;
				}			
			}
		}
	};
	void shootMew()
	{
		mewmewShot thisShot;
		thisShot.mewSprite = core->getRoot()->getSceneManager()->createSprite(shoottex);
		thisShot.mewSprite->setLayer(1);
		thisShot.mewDirection = core->facing;
		if(core->facing == LEFT)
		thisShot.mewSprite->setRotation(180);

		thisShot.mewSprite->setPosition(catDude->getPosition() + Vector(-20 + (core->facing * 40),-25));
		shootArray.push_back(thisShot);
	};
private:
	std::vector<mewmewShot> shootArray;
	Game* core;
	gfx::ISprite* catDude;
	gfx::ITexture* shoottex;
	float shootSpeed;
	bool switchy;
	LevelManager* levelManage;
};
