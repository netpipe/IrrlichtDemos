#include "Obstacle.hpp"

Obstacle *Obstacle::m_obstacle = NULL;
Obstacle Obstacle::m_default_obstacle_buffer;

Obstacle *Obstacle::Instance()
{
	if(!m_obstacle)
		m_obstacle = &m_default_obstacle_buffer;

	return m_obstacle;
}

void Obstacle::SetDefaultValues(void)
{
	obstacleExists = false;
}

void Obstacle::Add(ISceneNode *node)
{
	obstacleNode.push_back(node);

	obstacleExists = true;
}

void Obstacle::Clear(void)
{
	for(u32 i = 0; i < obstacleNode.size(); i++)
	{
		obstacleNode[i]->remove();
		obstacleNode.erase(i);
	}
	obstacleNode.clear();
}

void Obstacle::CheckCollisionWithPlayer(void)
{
	ISceneNode *playerNode = Player::Instance()->getPlayerNode();

	if(obstacleExists)
	{
        for(u32 i = 0; i < obstacleNode.size(); i++)
		{
			if(obstacleNode[i]->getTransformedBoundingBox().intersectsWithBox(
				playerNode->getTransformedBoundingBox()))
				{
					PlayerHelper::Instance()->DecreaseHealth(30);
				}
		//		if
        obstacleNode[i]->setPosition(vector3df( obstacleNode[i]->getPosition().X,
                                        obstacleNode[i]->getPosition().Y,
                                        obstacleNode[i]->getPosition().Z+1));

		}
	}
}
