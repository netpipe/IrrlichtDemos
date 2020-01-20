#include "Player.h"

Player::Player(Game* pCore) : core(pCore)
{
 
catDude = core->getRoot()->getSceneManager()->createSprite(core->getRoot()->getVideoDriver()->createTexture("media/player/cdstand.png"));
catDude->setPosition(Vector(400,400));

catstand = core->getRoot()->getVideoDriver()->createTexture("media/player/cdstand.png");

catwalkL[0] = core->getRoot()->getVideoDriver()->createTexture("media/player/cdwalkL1.png");
catwalkL[1] = core->getRoot()->getVideoDriver()->createTexture("media/player/cdwalkL2.png");
	
catwalkR[0] = core->getRoot()->getVideoDriver()->createTexture("media/player/cdwalkR1.png");
catwalkR[1] = core->getRoot()->getVideoDriver()->createTexture("media/player/cdwalkR2.png");
core->facing = RIGHT;
animbool = 0;
animtime = 0;
jumpSwitch = false;
wallJumpState = 1;
velocity = Vector(0,0);
LastGroundPlace = Vector(0,0);
}

void Player::update()
{
		if(catDude->getPosition().y < -64)
		{
			die();
		}
		const float Speed = 0.25f * core->getDeltaTime();
		WorldPos = core->getRoot()->getSceneManager()->getWorldPosition();


		if(core->getRoot()->getKeyPressed(gfx::EKC_LEFT_CONTROL))
		{
			if(!jumpSwitch)
			{
				velocity.y = 2.2f;
				jumpSwitch = true;
			}
			else if(core->getRoot()->getKeyPressed(gfx::EKC_LEFT) && wallJumpState == 3)
			{
				velocity.x -= 2.0f;
				velocity.y = 1.0f;
				wallJumpState = 2;
			}
			else if(core->getRoot()->getKeyPressed(gfx::EKC_RIGHT) && wallJumpState == 4)
			{
				velocity.x += 2.0f;
				velocity.y = 1.0f;
				wallJumpState = 2;
			}
		}	

		// Walking to the left
		if(core->getRoot()->getKeyPressed(gfx::EKC_LEFT))
		{
			core->facing = LEFT;
	 		catDude->getMaterial()->setTexture(0,catwalkL[animbool]);
	 		
	 		velocity.x -= 0.3f;

			
		}

		// Walking to the right
		else if(core->getRoot()->getKeyPressed(gfx::EKC_RIGHT))
		{
			core->facing = RIGHT;
	 		catDude->getMaterial()->setTexture(0,catwalkR[animbool]);
			
			velocity.x += 0.3f;
		}
		
		// Lookin' at the screen
		else if(core->getRoot()->getKeyPressed(gfx::EKC_DOWN))
		{
	 		catDude->getMaterial()->setTexture(0,catstand);
		}

		else if(core->facing == LEFT)
		{
			catDude->getMaterial()->setTexture(0,catwalkL[1]);	
		}

		else if(core->facing == RIGHT)
		{
			catDude->getMaterial()->setTexture(0,catwalkR[1]);	
		}

		
		catDude->setPosition(catDude->getPosition() + Vector(Speed * velocity.x,Speed * velocity.y));
		
		if(catDude->getPosition().x > (WorldPos.x + core->getRoot()->getDimension().x * 0.6f) && velocity.x > 0)
				core->getRoot()->getSceneManager()->setWorldPosition(WorldPos + Vector(Speed * velocity.x,0));
		
		if(catDude->getPosition().x < (WorldPos.x + core->getRoot()->getDimension().x * 0.4f) && velocity.x < 0 && WorldPos.x > (Speed * 5.0f))
				core->getRoot()->getSceneManager()->setWorldPosition(WorldPos + Vector(Speed * velocity.x,0));
				
		if(WorldPos.x < 0)
			core->getRoot()->getSceneManager()->setWorldPosition(Vector(0,WorldPos.y));

		velocity.x *= 0.75f;
		
		// Attempt at making the decrease in velocity frame-inpendant...failed :P
		//velocity.x *= (0.75f * core->getDeltaTime()) / 40.0f;

		
		if(animtime < core->getRoot()->getTime() - 200)
		{
			animtime = core->getRoot()->getTime();
			animbool = !animbool;
		}
};
