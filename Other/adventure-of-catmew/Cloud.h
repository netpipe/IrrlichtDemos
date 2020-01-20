#include "Game.h"

typedef struct
{
gfx::ISprite* cloudSprite;
bool cloudDirection;
float Speed;
float apparPos;
float height;
} cloudStruct;

class CloudManager
{
public:
	CloudManager(Game* mycore)
	{
		core = mycore;
		cloudtex = core->getRoot()->getVideoDriver()->createTexture("media/misc/cloud.png");
	};
	
	void update()
	{
		srand(core->getDeltaTime() * 100);
		cloudSpeed = 0.01f * core->getDeltaTime();

		for(int i = 0; i < cloudArray.size(); ++i)
		{
			if(cloudArray[i].cloudDirection == RIGHT)
				cloudArray[i].apparPos += cloudSpeed * cloudArray[i].Speed;			
			else
				cloudArray[i].apparPos -= cloudSpeed * cloudArray[i].Speed;
			
			cloudArray[i].cloudSprite->setPosition(Vector(core->wPos.x + cloudArray[i].apparPos,cloudArray[i].height));
			
			if(cloudArray[i].cloudSprite->getPosition().x < core->wPos.x - 512 || cloudArray[i].cloudSprite->getPosition().x > core->wPos.x +  core->getRoot()->getDimension().x + 512)
			{
				cloudArray[i].cloudSprite->remove();		
				cloudArray.erase(cloudArray.begin() + i,cloudArray.begin() + i+1);
				--i;
				//printf("\n Array Size: %d", cloudArray.size());
			}
		}

		if(cloudArray.size() < 10)
		{
			cloudStruct myCloud;
			
			float randSize = 50 + rand()%300;
			Vector cloudSize = Vector(randSize,randSize / 2);
			myCloud.cloudSprite = core->getRoot()->getSceneManager()->createSprite(cloudtex,cloudSize);
			myCloud.cloudSprite->setLayer(95);
			//printf("CloudSize = X:%f Y:%f",cloudSize.x,cloudSize.y);		
			
			myCloud.cloudDirection = rand()%3 > 1 ? LEFT : RIGHT;
			if(myCloud.cloudDirection == LEFT)
			{
				myCloud.apparPos = core->getRoot()->getDimension().x + 500.0f;
				myCloud.height = 450 + rand()%150;
			}
			else
			{
				myCloud.apparPos = -500.0f;
				myCloud.height = 450 + rand()%150;
			}
			
			myCloud.cloudSprite->setPosition(Vector(core->wPos.x + myCloud.apparPos,myCloud.height));
			
			myCloud.Speed = rand()%10* 0.5f;
			cloudArray.push_back(myCloud);
		}
	};
private:
	Game* core;
	std::vector<cloudStruct> cloudArray;
	gfx::ITexture* cloudtex;
	float cloudSpeed;
};

