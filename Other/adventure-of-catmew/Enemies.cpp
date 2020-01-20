#include "Enemies.h"
#include <math.h>

Enemies::Enemies(Game* pCore) : core(pCore)
{
	animtime = 0;
	
	
	
	configMan = new conf::Configuration("media/enemies/enemies.cfg");
	
	for(int i = 0;i < configMan->getSectionCount();++i)
	{
		EnemyType eType;
		eType.Name = configMan->getSectionByID(i)->getName();
		eType.iden = conf::stripTextFormatting(configMan->getSectionByID(i)->getKeyData("Mark")).c_str()[0];
		
		eType.defHitPoints = conf::toNumber(configMan->getSectionByID(i)->getKeyData("Hitpoints"));
		std::vector<std::string> texStrArr = conf::toArray(configMan->getSectionByID(i)->getKeyData("Textures"));
		
		std::vector<ITexture*> ETextures;
		
		for(int z = 0;z < texStrArr.size();++z)
		{
			std::string texName = "media/enemies/";
			texName += conf::stripTextFormatting(texStrArr[z]);
						
			ETextures.push_back(core->getRoot()->getVideoDriver()->createTexture((char*)texName.c_str()));
		}
		
		eType.Textures = ETextures;
		
		EnemyTypes.push_back(eType);
	}
};

void Enemies::update()
{
	// Animate enemies
	if(animtime < core->getRoot()->getTime() - 200)
	{
			for(int i = 0;i < EnemyArray.size();++i)
			{
				if(!EnemyArray[i]->dead)
				{
				EnemyArray[i]->currAnim++;		
				if(EnemyArray[i]->currAnim >= EnemyArray[i]->type->Textures.size())
					EnemyArray[i]->currAnim = 0;
			
				EnemyArray[i]->sprite->getMaterial()->setTexture(0,EnemyArray[i]->type->Textures[EnemyArray[i]->currAnim]);
				}
			}
			animtime = core->getRoot()->getTime();
	}
	
	for(int i = 0;i < EnemyArray.size();++i)
	{
		if(!EnemyArray[i]->dead)
		{
			Vector EnPos = EnemyArray[i]->sprite->getPosition();
			EnemyArray[i]->sprite->setPosition(Vector((EnPos.x + (sin(EnemyArray[i]->posCycle) * 2.0f)),EnPos.y));
			
			if(EnemyArray[i]->HitPoints < 1)
			{
				EnemyArray[i]->dead = true;
				EnemyArray[i]->dieing = true;
			}
			
			EnemyArray[i]->posCycle += 0.01f;
		}
		
		if(EnemyArray[i]->dieing)
		{
			EnemyArray[i]->sprite->setRotation(EnemyArray[i]->dieProg * 5.0f);
			EnemyArray[i]->sprite->setScale(Vector(64,64) * (1.0f - (EnemyArray[i]->dieProg / 100.0f)));
			EnemyArray[i]->dieProg+=2;
			
			if(EnemyArray[i]->dieProg > 100)
			{
				EnemyArray[i]->sprite->remove();
				EnemyArray[i]->dieing = false;
			}
		}
	}	
};
