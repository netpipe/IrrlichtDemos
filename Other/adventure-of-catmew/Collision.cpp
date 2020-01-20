#include "Collision.h"
#include <math.h>
#include <iostream>

CollisionManager::CollisionManager(Game* pCore, LevelManager* levelMan, Player* catguy) : core(pCore), levelManage(levelMan), catMewMew(catguy)
{
};

bool CollisionManager::update()
{
	bool didIntersect = false;
	
	Vector Position = catMewMew->getCatDude()->getPosition();
	Vector tilePos;			
	/*
	Box catBox = 
	Box
	(
	Vector(Position + Vector(-32,32)),
	Vector(Position + Vector(32,32)),
	Vector(Position + Vector(32,-32)),
	Vector(Position + Vector(-32,-32))
	);
	*/
	
	bool Points[5] = {};
	
	Vector cPoints[10] = {};
	cPoints[0] = Vector(Position + Vector(-12,20));
	cPoints[1] = Vector(Position + Vector( 12,20));
	cPoints[2] = Vector(Position + Vector(-12,-48));
	cPoints[3] = Vector(Position + Vector( 12,-48));
	cPoints[4] = Vector(Position + Vector( 24,6));
	cPoints[5] = Vector(Position + Vector( 24,-32));
	cPoints[6] = Vector(Position + Vector( -24, 6));
	cPoints[7] = Vector(Position + Vector( -24, -32));
	cPoints[8] = Vector(Position + Vector( -12, 48));
	cPoints[9] = Vector(Position + Vector( 12, 48));
	
	
	float TopLimit = 0;
	float BottomLimit = 0;
	float RightLimit = 0;
	float LeftLimit = 0;
	Box tileBox;
	

	for(int i = 0;i < levelManage->getTileNumber();++i)
	{
			// If this tile is an enemy and the enemy is dead then skip this round
			if(levelManage->getTile(i).isEnemy && levelManage->getTile(i).enemy->dead)
				continue;
				
			if(levelManage->getTile(i).sprite->getOnScreen() && levelManage->getTile(i).type > ETT_COL)
			{
	
			tilePos = levelManage->getTile(i).sprite->getPosition();
				
			if(	tilePos.x < Position.x + 128 && tilePos.x > Position.x - 128 &&
				tilePos.y < Position.y + 128 && tilePos.y > Position.y - 128)
			{
				tileBox = 
				
				Box
				(
				Vector(tilePos + Vector(-32,32)),
				Vector(tilePos + Vector(32,32)),
				Vector(tilePos + Vector(32,-32)),
				Vector(tilePos + Vector(-32,-32))
				);
				
				/*
				core->getRoot()->getVideoDriver()->renderLine(*tileBox.TopRight,*tileBox.TopLeft,Color(1, 0, 0, 1));
				core->getRoot()->getVideoDriver()->renderLine(*tileBox.BottomLeft,*tileBox.TopLeft,Color(1, 0, 0, 1));
				core->getRoot()->getVideoDriver()->renderLine(*tileBox.TopRight,*tileBox.BottomRight,Color(1, 0, 0, 1));
				core->getRoot()->getVideoDriver()->renderLine(*tileBox.BottomRight,*tileBox.BottomLeft,Color(1, 0, 0, 1));
				*/
				
				if(tileBox.isPointInside(cPoints[0]) || tileBox.isPointInside(cPoints[1]))
				{
					Points[0] = true;
					BottomLimit = tileBox.BottomLeft->y;
					didIntersect = true;
					if(levelManage->getTile(i).isEnemy)
					{
						catMewMew->die();
						//catMewMew->getCatDude()->setPosition(Vector(0,-512.0f));
					}
				}
				
				if(tileBox.isPointInside(cPoints[2]) || tileBox.isPointInside(cPoints[3]))
				{
					Points[1] = true;
					TopLimit = tileBox.TopRight->y;
					didIntersect = true;
					
					
					if(levelManage->getTile(i).isEnemy)
					{
						catMewMew->velocity.y = 2.0f;
						levelManage->getTile(i).enemy->HitPoints--;

					}
				}
				
				if(tileBox.isPointInside(cPoints[4]) || tileBox.isPointInside(cPoints[5]))
				{
					Points[2] = true;
					LeftLimit = tileBox.TopLeft->x;
					didIntersect = true;
					if(levelManage->getTile(i).isEnemy)
					{
						catMewMew->die();
						//catMewMew->getCatDude()->setPosition(Vector(0,-512.0f));
					}
				}
				
				if(tileBox.isPointInside(cPoints[6]) || tileBox.isPointInside(cPoints[7]))
				{
					Points[3] = true;
					RightLimit = tileBox.TopRight->x;
					didIntersect = true;
					if(levelManage->getTile(i).isEnemy)
					{
						catMewMew->die();
						//catMewMew->getCatDude()->setPosition(Vector(0,-512.0f));
					}
				}
				
				if(tileBox.isPointInside(cPoints[8]) || tileBox.isPointInside(cPoints[9]))
				{
					Points[4] = true;
				}
			}
			}
	}	

	

	if(Points[1])
	{
		//core->getRoot()->getVideoDriver()->renderLine(cPoints[2],cPoints[3],Color(1, 0, 0, 1));
		if(!Points[4])
		{
			Position.y = TopLimit + 48.0f;
			catMewMew->jumpSwitch = false;
		}
		else
			catMewMew->jumpSwitch = true;
			
		catMewMew->wallJumpState = 1;
		catMewMew->LastGroundPlace = Position;	
		if(catMewMew->velocity.y < 0)
			catMewMew->velocity.y = 0;
	}
	else
	{
		catMewMew->velocity.y -= 0.005f * core->getDeltaTime();
	}
	
	if(Points[0])
	{
		//core->getRoot()->getVideoDriver()->renderLine(cPoints[0],cPoints[1],Color(1, 0, 0, 1));
		Position.y = BottomLimit - 20.0f;
		catMewMew->velocity.y = 0;
	}
	
	if(catMewMew->wallJumpState != 2)
		catMewMew->wallJumpState = 1;
	
	if(Points[2] && !Points[3])
	{
		//core->getRoot()->getVideoDriver()->renderLine(cPoints[4],cPoints[5],Color(1, 0, 0, 1));
		Position.x = LeftLimit - 24.0f;
		if(!Points[1] && catMewMew->wallJumpState == 1)
		catMewMew->wallJumpState = 3;
	}
	
	if(Points[3] && !Points[2])
	{
		//core->getRoot()->getVideoDriver()->renderLine(cPoints[6],cPoints[7],Color(1, 0, 0, 1));
		Position.x = RightLimit + 24.0f;
		if(!Points[1] && catMewMew->wallJumpState == 1)
		catMewMew->wallJumpState = 4;
	}
	
		
	
	if(didIntersect)
		catMewMew->getCatDude()->setPosition(Position);
	
	
	
	return didIntersect;
};
